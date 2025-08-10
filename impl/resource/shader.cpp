#include <resource/exception.hpp>
#include <resource/shader.hpp>

namespace Terreate::Resource {
Shader::Shader(str const &path, ShaderType type) : mPath(path), mType(type) {
  ifstream file;
  file.open(path.c_str());

  if (!file.is_open()) {
    throw Exception::ShaderResourceError("Failed to open file: " + path);
  }

  std::stringstream stream;
  stream << file.rdbuf();
  file.close();

  mSource = stream.str();
}

UUID const &ShaderManager::getIDFor(str const &path) const {
  if (mPathIDMap.contains(path)) {
    return mPathIDMap.at(path);
  }
  throw Exception::ShaderResourceError("Shader not found for path: " + path);
}

shared<Shader> ShaderManager::get(UUID const &id) const {
  if (mRegistry && mRegistry->has<Shader>(id)) {
    return mRegistry->get<Shader>(id);
  }
  throw Exception::ShaderResourceError("Shader not found for ID: " +
                                       id.toString());
}

shared<Shader> ShaderManager::create(str const &path, ShaderType type) {
  if (mPathIDMap.contains(path)) {
    UUID id = mPathIDMap.at(path);
    if (mRegistry && mRegistry->has<Shader>(id)) {
      return mRegistry->get<Shader>(id);
    } else {
      return nullptr; // Resource not found in registry
    }
  }

  auto shader = std::make_shared<Shader>(path, type);
  if (mRegistry) {
    mRegistry->load<Shader>(shader);
    mPathIDMap[path] = shader->getID();
  } else {
    throw Exception::ShaderResourceError("Resource registry is not injected.");
  }

  return shader;
}

void ShaderManager::destroy(UUID const &id) {
  if (!mRegistry) {
    return; // No action needed if registry is not set
  }
  if (!mRegistry->has<Shader>(id)) {
    return; // No action needed if shader is not found
  }

  auto shader = mRegistry->get<Shader>(id);
  mPathIDMap.erase(shader->getPath());
  mRegistry->unload<Shader>(id);
}

void ShaderManager::destroy(str const &path) {
  if (!mRegistry || path.empty()) {
    return; // No action needed for empty path
  }

  if (!mPathIDMap.contains(path)) {
    throw Exception::ShaderResourceError("Resource registry is not injected.");
  }

  UUID id = mPathIDMap.at(path);
  if (!mRegistry->has<Shader>(id)) {
    return; // No action needed if shader is not found
  }

  auto shader = mRegistry->get<Shader>(id);
  mRegistry->unload<Shader>(id);
  mPathIDMap.erase(path);
}
} // namespace Terreate::Resource
