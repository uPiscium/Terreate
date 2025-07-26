#include <component/controller.hpp>

namespace Terreate::Component {
void ComponentController::update(float const &delta) {
  for (auto &pair : mSystems) {
    auto system = std::dynamic_pointer_cast<IComponentSystem>(pair.second);
    if (system) {
      system->update(delta);
    }
  }
}
} // namespace Terreate::Component
