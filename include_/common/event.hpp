#pragma once

#include "type.hpp"
#include "uuid.hpp"

#include <mutex>

namespace Terreate {
template <typename... EventArgs> class Event final {
private:
  using Callback = function<void(EventArgs...)>;

private:
  std::mutex mEventMutex;
  umap<UUID, Callback> mCallbacks;
  umap<UUID, Callback> mTriggers;

public:
  Event() = default;
  ~Event() = default;

  UUID subscribe(Callback const &subscriber) {
    std::lock_guard<std::mutex> lock(mEventMutex);
    UUID uuid = UUID();
    mCallbacks[uuid] = subscriber;
    return uuid;
  }
  UUID trigger(Callback const &subscriber) {
    std::lock_guard<std::mutex> lock(mEventMutex);
    UUID uuid = UUID();
    mTriggers[uuid] = subscriber;
    return uuid;
  }
  void unsubscribe(UUID const &uuid) {
    std::unique_lock<std::mutex> lock(mEventMutex);
    for (auto it = mCallbacks.begin(); it != mCallbacks.end(); ++it) {
      if (it->first == uuid) {
        mCallbacks.erase(it);
        break;
      }
    }
  }
  void publish(EventArgs... args) {
    std::unique_lock<std::mutex> lock(mEventMutex);
    for (auto &[_, callback] : mCallbacks) {
      callback(std::forward<EventArgs>(args)...);
    }
    for (auto &[_, callback] : mTriggers) {
      callback(std::forward<EventArgs>(args)...);
    }
    mTriggers.clear();
  }
};

} // namespace Terreate
