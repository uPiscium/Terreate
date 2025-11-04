#pragma once

#include "common/type.hpp"

namespace Terreate::SDL {

class Icon {
private:
  PROHIBIT_COPY_AND_ASSIGN(Icon);

private:
  SDL_Surface *mIcon = nullptr;

public:
  Icon() = default;
  ~Icon();

  void setImage(pair<u32> const &size, vec<u8> const &pixels);

  operator SDL_Surface *() const;
};

} // namespace Terreate::SDL
