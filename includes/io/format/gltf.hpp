#ifndef __TERREATE_IO_FORMAT_GLTF_HPP__
#define __TERREATE_IO_FORMAT_GLTF_HPP__

#include <types.hpp>

namespace Terreate::IO::GLTF {
using namespace Terreate::Types;

class GLTF {
private:
  Shared<std::ifstream> mStream;

public:
  GLTF();
  ~GLTF();
};
} // namespace Terreate::IO::GLTF

#endif // __TERREATE_IO_FORMAT_GLTF_HPP__
