#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

#include "compute.hpp"
#include "exception.hpp"
#include "shader.hpp"

namespace Terreate::OpenGL {
struct AttributeData {
  u64 vboIndex;
  u64 index;
  u64 size;
  u64 stride;
  u64 offset;
};

class BufferDataConstructor {
private:
  vec<vec<vec<float>>> mVertexDataComponents;
  vec<vec<u32>> mVertexIndices;
  vec<str> mAttributeNames;
  umap<str, AttributeData> mAttributes;
  vec<float> mVertexData;
  bool mConstructed = false;
  bool mUpdated = false;
  u32 mOffset = 0u;

public:
  BufferDataConstructor() {}
  ~BufferDataConstructor() {}

  vec<str> const &GetAttributeNames() const { return mAttributeNames; }
  umap<str, AttributeData> const &GetAttributes() const { return mAttributes; }
  vec<float> const &GetVertexData() const;

  void SetVertexIndices(vec<vec<u32>> const &indices) {
    mVertexIndices = indices;
  }

  void AddVertexComponent(str const &name, vec<vec<float>> const &data);
  void ReloadVertexComponent(str const &name, vec<vec<float>> const &data);
  void Construct();
};

class Buffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(Buffer);

private:
  GLObject mVAO = 0;
  GLObject mIBO = 0;
  u64 mIndexCount = 0u;
  bool mLoadedIndices = false;
  vec<GLObject> mBuffers;
  umap<str, AttributeData> mAttributes;

public:
  Buffer() { glGenVertexArrays(1, &mVAO); }
  ~Buffer();

  void setAttributeDivisor(AttributeData const &attribute,
                           u32 const &divisor) const;
  void setAttributeDivisor(str const &name, u32 const &divisor) const {
    this->setAttributeDivisor(mAttributes.at(name), divisor);
  }

  void loadData(vec<float> const &raw, umap<str, AttributeData> const &attrs,
                umap<str, u32> const &locations,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  void loadData(BufferDataConstructor const &bdc,
                umap<str, u32> const &locations,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  void reloadData(AttributeData const &target,
                  BufferDataConstructor const &bdc);
  void reloadData(str const &name, BufferDataConstructor const &bdc) {
    this->reloadData(mAttributes.at(name), bdc);
  }
  void reloadData(AttributeData const &target, BufferDataConstructor const &bdc,
                  u32 const &offset);
  void reloadData(str const &name, BufferDataConstructor const &bdc,
                  u32 const &offset) {
    this->reloadData(mAttributes.at(name), bdc, offset);
  }
  void loadIndices(vec<u32> const &indices);
  void loadIndices(vec<vec<u32>> const &indices) {
    this->loadIndices(Buffer::flatten(indices));
  }
  void reloadIndices(vec<u32> const &indices);
  void reloadIndices(vec<vec<u32>> const &indices) {
    this->reloadIndices(Buffer::flatten(indices));
  }
  void bind() const { glBindVertexArray(mVAO); }
  void unbind() const { glBindVertexArray(0); }
  void draw(DrawMode const &mode) const;
  void draw(DrawMode const &mode, u64 const &count) const;

  AttributeData &operator[](str const &name) { return mAttributes[name]; }
  AttributeData &operator[](char const *name) { return mAttributes[name]; }
  AttributeData const &operator[](str const &name) const {
    return mAttributes.at(name);
  }
  AttributeData const &operator[](char const *name) const {
    return mAttributes.at(name);
  }

public:
  template <typename T> static vec<T> flatten(vec<vec<T>> const &data) {
    vec<T> flat;
    for (auto const &v : data) {
      flat.insert(flat.end(), v.begin(), v.end());
    }
    return flat;
  }
};

class UniformBuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(UniformBuffer);

private:
  GLObject mUBO = 0;

private:
  void bind() const { glBindBuffer(GL_UNIFORM_BUFFER, mUBO); }
  void unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

public:
  UniformBuffer() { glGenBuffers(1, &mUBO); }
  ~UniformBuffer();

  template <typename T>
  void loadData(T const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &data, (GLenum)usage);
    this->unbind();
  }
  template <typename T>
  void loadData(vec<T> const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_UNIFORM_BUFFER, data.size() * sizeof(T), data.data(),
                 (GLenum)usage);
    this->unbind();
  }
  template <typename T> void reloadData(T const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &data);
    this->unbind();
  }
  template <typename T>
  void reloadData(vec<T> const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, data.size() * sizeof(T),
                    data.data());
    this->unbind();
  }
  void allocate(u64 const &size,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);

  void bind(Shader const &shader, str const &name) const;
};

class ShaderStorageBuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(ShaderStorageBuffer);

private:
  GLObject mSSBO = 0;
  u64 mSize = 0u;

private:
  void bind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, mSSBO); }
  void unbind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); }

public:
  ShaderStorageBuffer() { glGenBuffers(1, &mSSBO); }
  ~ShaderStorageBuffer();

  u64 const &getSize() const { return mSize; }
  template <typename T> void GetData(T &data, u64 const &offset = 0u) const {
    if (mSize == 0u) {
      return;
    }

    if (mSize > sizeof(T)) {
      throw Exception::BufferError("Data size is too small.");
    }

    this->bind();
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, mSize, &data);
    this->unbind();
  }
  template <typename T>
  void getData(vec<T> &data, u64 const &offset = 0u) const {
    if (mSize == 0u) {
      return;
    }

    if (mSize % sizeof(T) != 0) {
      throw Exception::BufferError("Data size is not a multiple of T.");
    }

    data.resize(mSize / sizeof(T));
    this->bind();
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, mSize, data.data());
    this->unbind();
  }

  template <typename T>
  void loadData(T const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &data, (GLenum)usage);
    mSize = sizeof(T);
    this->unbind();
  }
  template <typename T>
  void loadData(vec<T> const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(),
                 (GLenum)usage);
    mSize = data.size() * sizeof(T);
    this->unbind();
  }
  template <typename T> void reloadData(T const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T), &data);
    mSize = sizeof(T);
    this->unbind();
  }
  template <typename T>
  void reloadData(vec<T> const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, data.size() * sizeof(T),
                    data.data());
    mSize = data.size() * sizeof(T);
    this->unbind();
  }

  void allocate(u64 const &size,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  void bind(Shader &shader, str const &name) const;
  void bind(ComputeKernel &kernel, str const &name) const;
};
} // namespace Terreate::OpenGL
