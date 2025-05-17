#ifndef __TERREATE_GRAPHICS_BUFFER_HPP__
#define __TERREATE_GRAPHICS_BUFFER_HPP__

#include <core/exception.hpp>
#include <core/object.hpp>
#include <core/type.hpp>
#include <graphic/compute.hpp>
#include <graphic/shader.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

struct AttributeData {
  u64 vboIndex;
  u64 index;
  u64 size;
  u64 stride;
  u64 offset;
};

class BufferDataConstructor {
private:
  Vec<Vec<Vec<float>>> mVertexDataComponents;
  Vec<Vec<u32>> mVertexIndices;
  Vec<Str> mAttributeNames;
  Map<Str, AttributeData> mAttributes;
  Vec<float> mVertexData;
  bool mConstructed = false;
  bool mUpdated = false;
  u32 mOffset = 0u;

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
  Vec<Str> const &getAttributeNames() const { return mAttributeNames; }
  /*
   * @brief: Get attributes
   * @return: Attributes
   */
  Map<Str, AttributeData> const &getAttributes() const { return mAttributes; }
  /*
   * @brief: Get vertex data
   * @return: Vertex data
   */
  Vec<float> const &getVertexData() const;

  /*
   * @brief: Set vertex indices
   * @detail: Indices are used to construct each vertex data from vertex
   * component data
   * @param: indices: Indices to set
   */
  void setVertexIndices(Vec<Vec<u32>> const &indices) {
    mVertexIndices = indices;
  }

  /*
   * @brief: Add a vertex component to the buffer data constructor
   * @param: name: Name of the vertex component
   * @param: data: Data of the vertex component
   */
  void addVertexComponent(Str const &name, Vec<Vec<float>> const &data);
  /*
   * @brief: Reload a vertex component in the buffer data constructor
   * @param: name: Name of the vertex component
   * @param: data: Data of the vertex component
   */
  void reloadVertexComponent(Str const &name, Vec<Vec<float>> const &data);
  /*
   * @brief: Construct the buffer data
   */
  void construct();
};

class Buffer {
private:
  Core::Object mVAO = Core::Object();
  Core::Object mIBO = Core::Object();
  u64 mIndexCount = 0u;
  bool mLoadedIndices = false;
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
  void setAttributeDivisor(AttributeData const &attribute,
                           u32 const &divisor) const;
  /*
   * @brief: Set vertex attribute divisor
   * @param: name: Name of the attribute to set divisor
   * @param: divisor: Divisor value
   */
  void setAttributeDivisor(Str const &name, u32 const &divisor) const {
    this->setAttributeDivisor(mAttributes.at(name), divisor);
  }

  /*
   * @brief: Load data into the buffer
   * @param: raw: Raw data to load
   * @param: attrs: Attributes to load
   * @param: locations: Locations to load
   * @param: usage: Buffer usage
   */
  void loadData(Vec<float> const &raw, Map<Str, AttributeData> const &attrs,
                Map<Str, u32> const &locations,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  /*
   * @brief: Load data into the buffer
   * @param: bdc: Buffer data constructor object
   * @param: locations: Locations to load
   * @param: usage: Buffer usage
   */
  void loadData(BufferDataConstructor const &bdc,
                Map<Str, u32> const &locations,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  /*
   * @brief: Reload data into the buffer
   * @param: target: Target buffer
   * @param: bdc: Buffer data constructor object
   */
  void reloadData(AttributeData const &target,
                  BufferDataConstructor const &bdc);
  /*
   * @brief: Reload data into the buffer
   * @param: name: Name of the attribute to reload
   * @param: bdc: Buffer data constructor object
   */
  void reloadData(Str const &name, BufferDataConstructor const &bdc) {
    this->reloadData(mAttributes.at(name), bdc);
  }
  /*
   * @brief: Reload data into the buffer
   * @param: target: Target buffer
   * @param: bdc: Buffer data constructor object
   * @param: offset: Offset to reload data
   */
  void reloadData(AttributeData const &target, BufferDataConstructor const &bdc,
                  u32 const &offset);
  /*
   * @brief: Reload data into the buffer
   * @param: name: Name of the attribute to reload
   * @param: bdc: Buffer data constructor object
   * @param: offset: Offset to reload data
   */
  void reloadData(Str const &name, BufferDataConstructor const &bdc,
                  u32 const &offset) {
    this->reloadData(mAttributes.at(name), bdc, offset);
  }
  /*
   * @brief: Load indices into the buffer
   * @param: indices: Indices to load
   */
  void loadIndices(Vec<u32> const &indices);
  /*
   * @brief: Load indices into the buffer
   * @param: indices: Indices to load
   */
  void loadIndices(Vec<Vec<u32>> const &indices) {
    this->loadIndices(Buffer::flatten(indices));
  }
  /*
   * @brief: Reload indices into the buffer
   * @param: indices: Indices to reload
   */
  void reloadIndices(Vec<u32> const &indices);
  /*
   * @brief: Reload indices into the buffer
   * @param: indices: Indices to reload
   */
  void reloadIndices(Vec<Vec<u32>> const &indices) {
    this->reloadIndices(Buffer::flatten(indices));
  }
  /*
   * @brief: Bind the buffer
   */
  void bind() const { glBindVertexArray(mVAO); }
  /*
   * @brief: Unbind the buffer
   */
  void unbind() const { glBindVertexArray(0); }
  /*
   * @brief: Draw the buffer
   * @param: mode: Mode to draw
   */
  void draw(DrawMode const &mode) const;
  /*
   * @brief: Draw the buffer
   * @param: mode: Mode to draw
   * @param: count: Number of instances to draw
   */
  void draw(DrawMode const &mode, u64 const &count) const;

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
  template <typename T> static Vec<T> flatten(Vec<Vec<T>> const &data) {
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
  void bind() const { glBindBuffer(GL_UNIFORM_BUFFER, mUBO); }
  void unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

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
  void loadData(T const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &data, (GLenum)usage);
    this->unbind();
  }
  /*
   * @brief: Load data array into the new uniform buffer
   * @note: If you want to update the data, use ReloadData() instead
   * @param: data: Data array to load
   * @param: usage: Buffer usage
   */
  template <typename T>
  void loadData(Vec<T> const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_UNIFORM_BUFFER, data.size() * sizeof(T), data.data(),
                 (GLenum)usage);
    this->unbind();
  }
  /*
   * @brief: Reload data into the uniform buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data to reload
   * @param: offset: Offset to reload data
   */
  template <typename T> void reloadData(T const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &data);
    this->unbind();
  }
  /*
   * @brief: Reload data array into the uniform buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data array to reload
   * @param: offset: Offset to reload data
   */
  template <typename T>
  void reloadData(Vec<T> const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, data.size() * sizeof(T),
                    data.data());
    this->unbind();
  }
  /*
   * @brief: Allocate the uniform buffer
   * @param: size: Size to allocate
   * @param: usage: Buffer usage
   */
  void allocate(u64 const &size,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);

  /*
   * @brief: Bind the uniform buffer
   * @param: shader: Shader to bind
   * @param: name: Name of the uniform buffer
   */
  void bind(Shader const &shader, Str const &name) const;
};

class ShaderStorageBuffer {
private:
  Core::Object mSSBO = Core::Object();
  u64 mSize = 0u;

private:
  void bind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, mSSBO); }
  void unbind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); }

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
  u64 const &getSize() const { return mSize; }
  /*
   * @brief: Retrieve data from the buffer
   * @param: data: Data to retrieve
   * @param: offset: Offset to retrieve data
   */
  template <typename T> void getData(T &data, u64 const &offset = 0u) const {
    if (mSize == 0u) {
      return;
    }

    if (mSize > sizeof(T)) {
      throw Exceptions::BufferError("Data size is too small.");
    }

    this->bind();
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, mSize, &data);
    this->unbind();
  }
  /*
   * @brief: Retrieve data array from the buffer
   * @param: data: Data array to retrieve
   * @param: offset: Offset to retrieve data
   */
  template <typename T>
  void getData(Vec<T> &data, u64 const &offset = 0u) const {
    if (mSize == 0u) {
      return;
    }

    if (mSize % sizeof(T) != 0) {
      throw Exceptions::BufferError("Data size is not a multiple of T.");
    }

    data.resize(mSize / sizeof(T));
    this->bind();
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, mSize, data.data());
    this->unbind();
  }

  /*
   * @brief: Load data into the new shader storage buffer
   * @note: If you want to update the data, use ReloadData() instead
   * @param: data: Data to load
   * @param: usage: Buffer usage
   */
  template <typename T>
  void loadData(T const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &data, (GLenum)usage);
    mSize = sizeof(T);
    this->unbind();
  }
  /*
   * @brief: Load data array into the new shader storage buffer
   * @note: If you want to update the data, use ReloadData() instead
   * @param: data: Data array to load
   * @param: usage: Buffer usage
   */
  template <typename T>
  void loadData(Vec<T> const &data,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW) {
    this->bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(),
                 (GLenum)usage);
    mSize = data.size() * sizeof(T);
    this->unbind();
  }
  /*
   * @brief: Reload data into the shader storage buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data to reload
   * @param: offset: Offset to reload data
   */
  template <typename T> void reloadData(T const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T), &data);
    mSize = sizeof(T);
    this->unbind();
  }
  /*
   * @brief: Reload data array into the shader storage buffer
   * @note: If this is first time to load the data, use LoadData() instead
   * @param: data: Data array to reload
   * @param: offset: Offset to reload data
   */
  template <typename T>
  void reloadData(Vec<T> const &data, u64 const &offset = 0u) {
    this->bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, data.size() * sizeof(T),
                    data.data());
    mSize = data.size() * sizeof(T);
    this->unbind();
  }

  /*
   * @brief: Allocate the shader storage buffer
   * @param: size: Size to allocate
   * @param: usage: Buffer usage
   */
  void allocate(u64 const &size,
                BufferUsage const &usage = BufferUsage::STATIC_DRAW);
  /*
   * @brief: Bind the shader storage buffer
   * @param: shader: Shader to bind
   * @param: name: Name of the shader storage buffer
   */
  void bind(Shader &shader, Str const &name) const;
  /*
   * @brief: Bind the shader storage buffer
   * @param: kernel: Compute kernel to bind
   * @param: name: Name of the shader storage buffer
   */
  void bind(Compute::ComputeKernel &kernel, Str const &name) const;
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_BUFFER_HPP__
