#pragma once

#include "common/type.hpp"
#include "common/uuid.hpp"

namespace Terreate {
template <typename... EventArgs> class Event {
private:
  using Callback = function<void(EventArgs...)>;

private:
  std::mutex mEventMutex;
  umap<UUID, Callback> mCallbacks;
  umap<UUID, Callback> mTriggers;

public:
  Event() = default;
  ~Event() = default;

  UUID subscribe(Callback const &subscriber);
  UUID trigger(Callback const &subscriber);
  void unsubscribe(UUID const &uuid);
  void publish(EventArgs... args);
};

} // namespace Terreate

// ====================
// Implementation
// ====================
namespace Terreate {

template <typename... EventArgs>
UUID Event<EventArgs...>::subscribe(
    Event<EventArgs...>::Callback const &subscriber) {
  guard<std::mutex> lock(mEventMutex);
  UUID uuid = UUID();
  mCallbacks[uuid] = subscriber;
  return uuid;
}

template <typename... EventArgs>
UUID Event<EventArgs...>::trigger(
    Event<EventArgs...>::Callback const &subscriber) {
  guard<mutex> lock(mEventMutex);
  UUID uuid = UUID();
  mTriggers[uuid] = subscriber;
  return uuid;
}

template <typename... EventArgs>
void Event<EventArgs...>::unsubscribe(UUID const &uuid) {
  ulock<mutex> lock(mEventMutex);
  for (auto it = mCallbacks.begin(); it != mCallbacks.end(); ++it) {
    if (it->first == uuid) {
      mCallbacks.erase(it);
      break;
    }
  }
}

template <typename... EventArgs>
void Event<EventArgs...>::publish(EventArgs... args) {
  std::unique_lock<std::mutex> lock(mEventMutex);
  for (auto &[_, callback] : mCallbacks) {
    callback(std::forward<EventArgs>(args)...);
  }
  for (auto &[_, callback] : mTriggers) {
    callback(std::forward<EventArgs>(args)...);
  }
  mTriggers.clear();
}
} // namespace Terreate
