#ifndef __TERREATEIO_CONTAINERS_MATERIAL_HPP__
#define __TERREATEIO_CONTAINERS_MATERIAL_HPP__

#include <core/math.hpp>
#include <core/uuid.hpp>
#include <io/image.hpp>
#include <types.hpp>

namespace Terreate::IO {
using namespace Terreate::Types;
using namespace Terreate::Math;

typedef vec4 Color;

enum class MaterialConstant {
  ALPHA,
  EMITTANCE,
  DISCARD,
  OCCULSION,
  METALLIC,
  ROUGHNESS,
  REFRACTION
};

enum class MaterialColor { AMBIENT, DIFFUSE, SPECULAR, EMISSION, OCCLUSION };

enum class MaterialTexture {
  UNDEFINED,
  AMBIENT,
  DIFFUSE,
  SPECULAR,
  NORMAL,
  EMISSION,
  OCCLUSION,
  METALLIC_ROUGHNESS,
  REFLECTION
};

struct Mapping {
  Uint UVIndex = 0u;
  Bool enabled = false;
  MaterialTexture type = MaterialTexture::UNDEFINED;
  Uint layer = 0u;
};

class Material {
private:
  Map<MaterialConstant, Float> mConstants;
  Map<MaterialColor, Color> mColors;
  Map<MaterialTexture, Texture> mTextures;
  Core::UUID mMaterialID;

public:
  /*
   * @brief: Material data container class.
   */
  Material() = default;
  ~Material() = default;

  /*
   * @brief: Get material UUID.
   * @return: Material UUID.
   */
  Core::UUID const &GetUUID() const { return mMaterialID; }
  /*
   * @brief: Get material constant data.
   * @return: Material constant data.
   */
  Map<MaterialConstant, Float> const &GetConstants() const {
    return mConstants;
  }
  /*
   * @brief: Get material color data.
   * @return: Material color data.
   */
  Map<MaterialColor, Color> const &GetColors() const { return mColors; }
  /*
   * @brief: Get material texture data.
   * @return: Material texture data.
   */
  Map<MaterialTexture, Texture> const &GetTextures() const { return mTextures; }
  /*
   * @brief: Get material constant data by type.
   * @param: type: Material constant type.
   * @return: Material constant data.
   */
  Float GetConstant(MaterialConstant type) const { return mConstants.at(type); }
  /*
   * @brief: Get material color data by type.
   * @param: type: Material color type.
   * @return: Material color data.
   */
  Color GetColor(MaterialColor type) const { return mColors.at(type); }
  /*
   * @brief: Get texture data by type.
   * @param: type: Texture type.
   * @return: Texture data.
   */
  Texture const &GetTexture(MaterialTexture type) const {
    return mTextures.at(type);
  }
};
} // namespace Terreate::IO

#endif // __TERREATEIO_CONTAINERS_MATERIAL_HPP__
