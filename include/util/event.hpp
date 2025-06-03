#pragma once
#include "uuid.hpp"

#include "../common/type.hpp"

#include <mutex>

namespace Terreate::Util {
template <typename... Args> class Subscriber {
public:
  using Callback = Type::function<void(Args...)>;
  Callback mCallback;
  UUID mUUID;

public:
  Subscriber() : mCallback(nullptr), mUUID(UUID::empty()) {}
  Subscriber(Callback callback) : mCallback(callback) {}
  Subscriber(Subscriber const &subscriber) {
    mCallback = subscriber.mCallback;
    mUUID = subscriber.mUUID;
  }
  virtual ~Subscriber() = default;

  virtual UUID const getUUID() const { return mUUID; }
  virtual void onEvent(Args... args) {
    if (mCallback != nullptr) {
      mCallback(std::forward<Args>(args)...);
    }
  }
};

template <typename... EventArgs> class Event final {
private:
  using Callback = Subscriber<EventArgs...>;
  typedef std::recursive_mutex mutex;

private:
  std::mutex mEventMutex;
  Type::umap<UUID, Callback> mCallbacks;
  Type::umap<UUID, Callback> mTriggers;

public:
  Event() = default;
  ~Event() = default;

  /*
   * @brief: Subscribe to the event
   * @param: subscriber: Callback function to be called when the event is
   */
  void subscribe(Callback const &subscriber) {
    std::lock_guard<std::mutex> lock(mEventMutex);
    mCallbacks[subscriber.getUUID()] = subscriber;
  }
  /*
   * @brief: Subscribe to the event as a trigger
   * @param: subscriber: Callback function to be called when the event is
   */
  void trigger(Callback const &subscriber) {
    std::lock_guard<std::mutex> lock(mEventMutex);
    mTriggers[subscriber.getUUID()] = subscriber;
  }
  /*
   * @brief: Unsubscribe from the event
   * @param: callback: Callback function to be removed from the event
   */
  void unsubscribe(Callback const &callback) {
    std::unique_lock<std::mutex> lock(mEventMutex);
    for (auto it = mCallbacks.begin(); it != mCallbacks.end(); ++it) {
      if (it->first == callback.getUUID()) {
        mCallbacks.erase(it);
        break;
      }
    }
  }

  /*
   * @brief: Publish the event
   * @param: args: Arguments to be passed to the callback functions
   */
  void publish(EventArgs... args) {
    std::unique_lock<std::mutex> lock(mEventMutex);
    for (auto &[_, callback] : mCallbacks) {
      callback.onEvent(std::forward<EventArgs>(args)...);
    }
    for (auto &[_, callback] : mTriggers) {
      callback.onEvent(std::forward<EventArgs>(args)...);
    }
    mTriggers.clear();
  }

  Event &operator+=(Callback const &callback) {
    this->subscribe(callback);
    return *this;
  }
  Event &operator-=(Callback const &callback) {
    this->unsubscribe(callback);
    return *this;
  }
};

} // namespace Terreate::Util
