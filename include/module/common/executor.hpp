#pragma once

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

#include "exception.hpp"
#include "type.hpp"

namespace Terreate {
typedef function<void()> Task;

template <typename T>
concept Runnable = requires(T t) {
  { t() } -> std::same_as<void>;
};

class Executor {
public:
  typedef std::shared_future<void> TaskHandle;

private:
  vec<TaskHandle> mHandles;
  std::queue<std::packaged_task<void()>> mTaskQueue;
  std::mutex mQueueMutex;

  vec<std::thread> mWorkers;
  std::condition_variable mCV;

  std::atomic<u32> mNumJobs = 0u;
  std::atomic<bool> mComplete = false;
  std::atomic<bool> mStop = false;

private:
  void worker();
  TaskHandle enqueue(Task &&task);

public:
  explicit Executor(
      u32 const &numWorkers = std::thread::hardware_concurrency());
  ~Executor();

  vec<Exception::ExecutorError> getExceptions() const;

  TaskHandle schedule(Task const &target) {
    return this->enqueue([target] { target(); });
  }
  TaskHandle schedule(Task const &target, vec<TaskHandle> const &dependencies);
  template <Runnable T> TaskHandle schedule(T &target) {
    return this->enqueue([target] { target(); });
  }

  void waitForAll() const { mComplete.wait(false); }
};

} // namespace Terreate
