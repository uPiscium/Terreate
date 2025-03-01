#include <graphic/buffer.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

Vec<Float> const &BufferDataConstructor::GetVertexData() const {
  if (!mConstructed) {
    throw Exceptions::BufferError("Data not constructed.");
  }
  return mVertexData;
}

void BufferDataConstructor::AddVertexComponent(Str const &name,
                                               Vec<Vec<Float>> const &data) {
  mUpdated = true;
  mVertexDataComponents.push_back(data);
  mAttributeNames.push_back(name);
  mAttributes.insert(
      {name,
       {0, mVertexDataComponents.size() - 1, data[0].size(), 0, mOffset}});
  mOffset += data[0].size() * sizeof(Float);
}

void BufferDataConstructor::ReloadVertexComponent(Str const &name,
                                                  Vec<Vec<Float>> const &data) {
  mUpdated = true;
  if (mAttributes.find(name) == mAttributes.end()) {
    this->AddVertexComponent(name, data);
    return;
  }

  Uint index = mAttributes.at(name).index;
  mVertexDataComponents[index] = data;
}

void BufferDataConstructor::Construct() {
  if (mVertexDataComponents.size() == 0) {
    throw Exceptions::BufferError("No data components added.");
  }

  if (mVertexIndices.size() == 0) {
    throw Exceptions::BufferError("No indices added.");
  }

  if (mVertexIndices[0].size() != mVertexDataComponents.size()) {
    throw Exceptions::BufferError("Indices and data components mismatch.");
  }

  for (auto &attr : mAttributes) {
    attr.second.stride = mOffset;
  }

  mVertexData.clear();
  for (auto &indices : mVertexIndices) {
    for (Uint i = 0; i < indices.size(); ++i) {
      auto &data = mVertexDataComponents[i];
      Uint &index = indices[i];
      mVertexData.insert(mVertexData.end(), data[index].begin(),
                         data[index].end());
    }
  }
  mConstructed = true;
}

Buffer::~Buffer() {
  if (mVAO.Count() <= 1) {
    glDeleteVertexArrays(1, mVAO);
    mVAO.Delete();
  }
  if (mIBO.Count() <= 1) {
    glDeleteBuffers(1, mIBO);
    mIBO.Delete();
  }
  for (auto &buffer : mBuffers) {
    if (buffer.Count() <= 1) {
      glDeleteBuffers(1, buffer);
      buffer.Delete();
    }
  }
}

void Buffer::SetAttributeDivisor(AttributeData const &attribute,
                                 Uint const &divisor) const {
  this->Bind();
  glVertexAttribDivisor(attribute.index, divisor);
  this->Unbind();
}

void Buffer::LoadData(Vec<Float> const &raw,
                      Map<Str, AttributeData> const &attrs,
                      Map<Str, Uint> const &locations,
                      BufferUsage const &usage) {
  Ulong size = raw.size() * sizeof(Float);
  this->Bind();
  Core::Object buffer = Core::Object();
  glGenBuffers(1, buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, raw.data(), (GLenum)usage);

  for (auto &[name, attr] : attrs) {
    if (locations.find(name) == locations.end()) {
      throw Exceptions::BufferError("Attribute location not found.");
    }
    Uint index = locations.at(name);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, attr.size, GL_FLOAT, GL_FALSE, attr.stride,
                          reinterpret_cast<void const *>(attr.offset));
    mAttributes.insert(
        {name, {mBuffers.size(), index, attr.size, attr.stride, attr.offset}});
  }
  this->Unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  mBuffers.push_back(buffer);
}

void Buffer::LoadData(BufferDataConstructor const &bdc,
                      Map<Str, Uint> const &locations,
                      BufferUsage const &usage) {
  Map<Str, AttributeData> const &attributes = bdc.GetAttributes();
  Vec<Float> data = bdc.GetVertexData();
  Ulong size = data.size() * sizeof(Float);
  this->Bind();
  Core::Object buffer = Core::Object();
  glGenBuffers(1, buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, data.data(), (GLenum)usage);

  for (auto &name : bdc.GetAttributeNames()) {
    if (locations.find(name) == locations.end()) {
      throw Exceptions::BufferError("Attribute location not found.");
    }
    AttributeData const &attr = attributes.at(name);
    Uint index = locations.at(name);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, attr.size, GL_FLOAT, GL_FALSE, attr.stride,
                          reinterpret_cast<void const *>(attr.offset));
    mAttributes.insert(
        {name, {mBuffers.size(), index, attr.size, attr.stride, attr.offset}});
  }
  this->Unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  mBuffers.push_back(buffer);
}

void Buffer::ReloadData(AttributeData const &target,
                        BufferDataConstructor const &bdc) {
  Vec<Float> data = bdc.GetVertexData();
  Ulong size = data.size() * sizeof(Float);
  this->Bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[target.vboIndex]);
  glBufferSubData(GL_ARRAY_BUFFER, target.offset, size, data.data());
  this->Unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::ReloadData(AttributeData const &target,
                        BufferDataConstructor const &bdc, Uint const &offset) {
  Vec<Float> data = bdc.GetVertexData();
  Ulong size = data.size() * sizeof(Float);
  this->Bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[target.vboIndex]);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data.data());
  this->Unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::LoadIndices(Vec<Uint> const &indices) {
  mLoadedIndices = true;
  mIndexCount = indices.size();
  this->Bind();
  glGenBuffers(1, mIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Uint),
               indices.data(), GL_STATIC_DRAW);
  this->Unbind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Buffer::ReloadIndices(Vec<Uint> const &indices) {
  mIndexCount = indices.size();
  this->Bind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(Uint),
                  indices.data());
  this->Unbind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Buffer::Draw(DrawMode const &mode) const {
  if (!mLoadedIndices) {
    throw Exceptions::BufferError("No indices loaded into buffer.");
  }

  if (mBuffers.size() == 0) {
    throw Exceptions::BufferError("No buffers attached to buffer.");
  }

  this->Bind();
  glDrawElements((GLenum)mode, mIndexCount, GL_UNSIGNED_INT, 0);
  this->Unbind();
}

void Buffer::Draw(DrawMode const &mode, Ulong const &count) const {
  if (!mLoadedIndices) {
    throw Exceptions::BufferError("No indices loaded into buffer.");
  }

  if (mBuffers.size() == 0) {
    throw Exceptions::BufferError("No buffers attached to buffer.");
  }

  this->Bind();
  glDrawElementsInstanced((GLenum)mode, mIndexCount, GL_UNSIGNED_INT, nullptr,
                          count);
  this->Unbind();
}

UniformBuffer::~UniformBuffer() {
  if (mUBO.Count() <= 1) {
    glDeleteBuffers(1, mUBO);
    mUBO.Delete();
  }
}

void UniformBuffer::Allocate(Ulong const &size, BufferUsage const &usage) {
  this->Bind();
  glBufferData(GL_UNIFORM_BUFFER, size, nullptr, (GLenum)usage);
  this->Unbind();
}

void UniformBuffer::Bind(Shader const &shader, Str const &name) const {
  shader.BindUniformBlock(name, (Uint)mUBO);
  glBindBufferBase(GL_UNIFORM_BUFFER, (Uint)mUBO, mUBO);
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
  if (mSSBO.Count() <= 1) {
    glDeleteBuffers(1, mSSBO);
    mSSBO.Delete();
  }
}

void ShaderStorageBuffer::Allocate(Ulong const &size,
                                   BufferUsage const &usage) {
  this->Bind();
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, (GLenum)usage);
  mSize = size;
  this->Unbind();
}

void ShaderStorageBuffer::Bind(Shader &shader, Str const &name) const {
  shader.BindStorageBlock(name, (Uint)mSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (Uint)mSSBO, mSSBO);
}

void ShaderStorageBuffer::Bind(Compute::ComputeKernel &shader,
                               Str const &name) const {
  shader.BindStorageBlock(name, (Uint)mSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (Uint)mSSBO, mSSBO);
}
} // namespace Terreate::Graphic
