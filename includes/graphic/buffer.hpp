#ifndef __TERREATE_GRAPHICS_BUFFER_HPP__
#define __TERREATE_GRAPHICS_BUFFER_HPP__

#include <core/object.hpp>
#include <exceptions.hpp>
#include <graphic/compute.hpp>
#include <graphic/shader.hpp>
#include <types.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

struct AttributeData {
  Ulong vboIndex;
  Ulong index;
  Ulong size;
  Ulong stride;
  Ulong offset;
};

class BufferDataConstructor {
private:
  Vec<Vec<Vec<Float>>> mVertexDataComponents;
  Vec<Vec<Uint>> mVertexIndices;
  Vec<Str> mAttributeNames;
  Map<Str, AttributeData> mAttributes;
  Vec<Float> mVertexData;
  Bool mConstructed = false;
  Bool mUpdated = false;
  Uint mOffset = 0u;

public:
  /*
   * @brief: Construct a new BufferDataConstructor object. BufferDataConstructor
   * is used to construct data for a new buffer object
   */
  BufferDataConstructor() {}
  ~BufferDataConstructor() {}

  /*
   * @brief: Get attribute names
   * @return: Attribute names
   */
  Vec<Str> const &GetAttributeNames() const { return mAttributeNames; }
  /*
   * @brief: Get attributes
   * @return: Attributes
   */
  Map<Str, AttributeData> const &GetAttributes() const { return mAttributes; }
  /*
   * @brief: Get vertex data
   * @return: Vertex data
   */
  Vec<Float> const &GetVertexData() const;

  /*
   * @brief: Set vertex indices
   * @detail: Indices are used to construct each vertex data from vertex
   * component data
   * @param: indices: Indices to set
   */
  void SetVertexIndices(Vec<Vec<Uint>> const &indices) {
    mVertexIndices = indices;
  }

  /*
   * @brief: Add a vertex component to the buffer data constructor
   * @param: name: Name of the vertex component
   * @param: data: Data of the vertex component
   */
  void AddVertexComponent(Str const &name, Vec<Vec<Float>> const &data);
  /*
   * @brief: Reload a vertex component in the buffer data constructor
   * @param: name: Name of the vertex component
   * @param: data: Data of the vertex component
   */
  void ReloadVertexComponent(Str const &name, Vec<Vec<Float>> const &data);
  /*
   * @brief: Construct the buffer data
   */
  void Construct();
};

class Buffer {
private:
  Core::Object mVAO = Core::Object();
  Core::Object mIBO = Core::Object();
  Ulong mIndexCount = 0u;
  Bool mLoadedIndices = false;
  Vec<Core::Object> mBuffers;
  Map<Str, AttributeData> mAttributes;

public:
  /*
   * @brief: Construct a new Buffer object
   */
  Buffer() { glGenVertexArrays(1, mVAO); }
  ~Buffer();

  /*
   * @brief: Set vertex attribute divisor
   * @param: attribute: Attribute to set divisor
   * @param: divisor: Divisor value
   */
  void SetAttributeDivisor(AttributeData const &attribute,
                           Uint const &divisor) const;
  /*
   * @brief: Set vertex attribute divisor
   * @param: name: Name of the attribute to set divisor
   * @param: divisor: Divisor value
   */
  void SetAttributeDivisor(Str const &name, Uint const &divisor) const {
    this->SetAttributeDivisor(mAttributes.at(name), divisor);
  }

  /*
   * @brief: Load data into the buffer
   * @param: raw: Raw data to load
   * @param: attrs: Attributes to load
   * @param: locations: Locations to load
   * @param: usage: Buffer usage
   */
  void LoadData(Vec<Float> const &raw, Map<Str, AttributeData> const &attrs,
                Map<Str, Uint> const &locations,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  /*
   * @brief: Load data into the buffer
   * @param: bdc: Buffer data constructor object
   * @param: locations: Locations to load
   * @param: usage: Buffer usage
   */
  void LoadData(BufferDataConstructor const &bdc,
                Map<Str, Uint> const &locations,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  /*
   * @brief: Reload data into the buffer
   * @param: target: Target buffer
   * @param: bdc: Buffer data constructor object
   */
  void ReloadData(AttributeData const &target,
                  BufferDataConstructor const &bdc);
  /*
   * @brief: Reload data into the buffer
   * @param: name: Name of the attribute to reload
   * @param: bdc: Buffer data constructor object
   */
  void ReloadData(Str const &name, BufferDataConstructor const &bdc) {
    this->ReloadData(mAttributes.at(name), bdc);
  }
  /*
   * @brief: Reload data into the buffer
   * @param: target: Target buffer
   * @param: bdc: Buffer data constructor object
   * @param: offset: Offset to reload data
   */
  void ReloadData(AttributeData const &target, BufferDataConstructor const &bdc,
                  Uint const &offset);
  /*
   * @brief: Reload data into the buffer
   * @param: name: Name of the attribute to reload
   * @param: bdc: Buffer data constructor object
   * @param: offset: Offset to reload data
   */
  void ReloadData(Str const &name, BufferDataConstructor const &bdc,
                  Uint const &offset) {
    this->ReloadData(mAttributes.at(name), bdc, offset);
  }
  /*
   * @brief: Load indices into the buffer
   * @param: indices: Indices to load
   */
  void LoadIndices(Vec<Uint> const &indices);
  /*
   * @brief: Load indices into the buffer
   * @param: indices: Indices to load
   */
  void LoadIndices(Vec<Vec<Uint>> const &indices) {
    this->LoadIndices(Buffer::Flatten(indices));
  }
  /*
   * @brief: Reload indices into the buffer
   * @param: indices: Indices to reload
   */
  void ReloadIndices(Vec<Uint> const &indices);
  /*
   * @brief: Reload indices into the buffer
   * @param: indices: Indices to reload
   */
  void ReloadIndices(Vec<Vec<Uint>> const &indices) {
    this->ReloadIndices(Buffer::Flatten(indices));
  }
  /*
   * @brief: Bind the buffer
   */
  void Bind() const { glBindVertexArray(mVAO); }
  /*
   * @brief: Unbind the buffer
   */
  void Unbind() const { glBindVertexArray(0); }
  /*
   * @brief: Draw the buffer
   * @param: mode: Mode to draw
   */
  void Draw(DrawMode const &mode) const;
  /*
   * @brief: Draw the buffer
   * @param: mode: Mode to draw
   * @param: count: Number of instances to draw
   */
  void Draw(DrawMode const &mode, Ulong const &count) const;

  AttributeData &operator[](Str const &name) { return mAttributes[name]; }
  AttributeData &operator[](char const *name) { return mAttributes[name]; }
  AttributeData const &operator[](Str const &name) const {
    return mAttributes.at(name);
  }
  AttributeData const &operator[](char const *name) const {
    return mAttributes.at(name);
  }

public:
  /*
   * @brief: Flatten a 2D vector into a 1D vector
   * @param: data: Data to flatten
   * @return: Flattened data
   */
  template <typename T> static Vec<T> Flatten(Vec<Vec<T>> const &data) {
    Vec<T> flat;
    for (auto const &v : data) {
      flat.insert(flat.end(), v.begin(), v.end());
    }
    return flat;
  }
};

class UniformBuffer {
private:
  Core::Object mUBO = Core::Object();

private:
  void Bind() const { glBindBuffer(GL_UNIFORM_BUFFER, mUBO); }
  void Unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

public:
  /*
   * @brief: Construct a new UniformBuffer object
   */
  UniformBuffer() { glGenBuffers(1, mUBO); }
  ~UniformBuffer();

  /*
   * @brief: Load data into the new uniform buffer
   * @note: If you want to update the data, use ReloadData() instead
   * @param: data: Data to load
   * @param: usage: Buffer usage
   */
  template <typename T>
  void LoadData(T const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->Bind();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &data, (GLenum)usage);
    this->Unbind();
  }
  /*
   * @brief: Load data array into the new uniform buffer
   * @note: If you want to update the data, use ReloadData() instead
   * @param: data: Data array to load
   * @param: usage: Buffer usage
   */
  template <typename T>
  void LoadData(Vec<T> const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->Bind();
    glBufferData(GL_UNIFORM_BUFFER, data.size() * sizeof(T), data.data(),
                 (GLenum)usage);
    this->Unbind();
  }
  /*
   * @brief: Reload data into the uniform buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data to reload
   * @param: offset: Offset to reload data
   */
  template <typename T>
  void ReloadData(T const &data, Ulong const &offset = 0u) {
    this->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &data);
    this->Unbind();
  }
  /*
   * @brief: Reload data array into the uniform buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data array to reload
   * @param: offset: Offset to reload data
   */
  template <typename T>
  void ReloadData(Vec<T> const &data, Ulong const &offset = 0u) {
    this->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, data.size() * sizeof(T),
                    data.data());
    this->Unbind();
  }
  /*
   * @brief: Allocate the uniform buffer
   * @param: size: Size to allocate
   * @param: usage: Buffer usage
   */
  void Allocate(Ulong const &size,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);

  /*
   * @brief: Bind the uniform buffer
   * @param: shader: Shader to bind
   * @param: name: Name of the uniform buffer
   */
  void Bind(Shader const &shader, Str const &name) const;
};

class ShaderStorageBuffer {
private:
  Core::Object mSSBO = Core::Object();
  Size mSize = 0u;

private:
  void Bind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, mSSBO); }
  void Unbind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); }

public:
  /*
   * @brief: Construct a new ShaderStorageBuffer object
   */
  ShaderStorageBuffer() { glGenBuffers(1, mSSBO); }
  ~ShaderStorageBuffer();

  /*
   * @brief: Get the size of the buffer
   * @return: Size of the buffer
   */
  Size const &GetSize() const { return mSize; }
  /*
   * @brief: Retrieve data from the buffer
   * @param: data: Data to retrieve
   * @param: offset: Offset to retrieve data
   */
  template <typename T> void GetData(T &data, Ulong const &offset = 0u) const {
    if (mSize == 0u) {
      return;
    }

    if (mSize > sizeof(T)) {
      throw Exceptions::BufferError("Data size is too small.");
    }

    this->Bind();
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, mSize, &data);
    this->Unbind();
  }
  /*
   * @brief: Retrieve data array from the buffer
   * @param: data: Data array to retrieve
   * @param: offset: Offset to retrieve data
   */
  template <typename T>
  void GetData(Vec<T> &data, Ulong const &offset = 0u) const {
    if (mSize == 0u) {
      return;
    }

    if (mSize % sizeof(T) != 0) {
      throw Exceptions::BufferError("Data size is not a multiple of T.");
    }

    data.resize(mSize / sizeof(T));
    this->Bind();
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, mSize, data.data());
    this->Unbind();
  }

  /*
   * @brief: Load data into the new shader storage buffer
   * @note: If you want to update the data, use ReloadData() instead
   * @param: data: Data to load
   * @param: usage: Buffer usage
   */
  template <typename T>
  void LoadData(T const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->Bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &data, (GLenum)usage);
    mSize = sizeof(T);
    this->Unbind();
  }
  /*
   * @brief: Load data array into the new shader storage buffer
   * @note: If you want to update the data, use ReloadData() instead
   * @param: data: Data array to load
   * @param: usage: Buffer usage
   */
  template <typename T>
  void LoadData(Vec<T> const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->Bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(),
                 (GLenum)usage);
    mSize = data.size() * sizeof(T);
    this->Unbind();
  }
  /*
   * @brief: Reload data into the shader storage buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data to reload
   * @param: offset: Offset to reload data
   */
  template <typename T>
  void ReloadData(T const &data, Ulong const &offset = 0u) {
    this->Bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T), &data);
    mSize = sizeof(T);
    this->Unbind();
  }
  /*
   * @brief: Reload data array into the shader storage buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data array to reload
   * @param: offset: Offset to reload data
   */
  template <typename T>
  void ReloadData(Vec<T> const &data, Ulong const &offset = 0u) {
    this->Bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, data.size() * sizeof(T),
                    data.data());
    mSize = data.size() * sizeof(T);
    this->Unbind();
  }

  /*
   * @brief: Allocate the shader storage buffer
   * @param: size: Size to allocate
   * @param: usage: Buffer usage
   */
  void Allocate(Ulong const &size,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  /*
   * @brief: Bind the shader storage buffer
   * @param: shader: Shader to bind
   * @param: name: Name of the shader storage buffer
   */
  void Bind(Shader &shader, Str const &name) const;
  /*
   * @brief: Bind the shader storage buffer
   * @param: kernel: Compute kernel to bind
   * @param: name: Name of the shader storage buffer
   */
  void Bind(Compute::ComputeKernel &kernel, Str const &name) const;
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_BUFFER_HPP__
