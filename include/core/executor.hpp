#ifndef __TERREATECORE_EXECUTOR_HPP__
#define __TERREATECORE_EXECUTOR_HPP__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

#include <core/exception.hpp>
#include <core/type.hpp>

namespace Terreate::Core {
using namespace Terreate::Types;

typedef Function<void()> Task;

template <typename T>
concept Runnable = requires(T t) {
  { t() } -> std::same_as<void>;
};

class Executor {
public:
  typedef std::shared_future<void> TaskHandle;

private:
  Vec<TaskHandle> mHandles;
  std::queue<std::packaged_task<void()>> mTaskQueue;
  std::mutex mQueueMutex;

  Vec<std::thread> mWorkers;
  std::condition_variable mCV;

  std::atomic<i32> mNumJobs = 0u;
  std::atomic<bool> mComplete = false;
  std::atomic<bool> mStop = false;

private:
  void worker();
  TaskHandle enqueue(Task &&task);

public:
  explicit Executor(
      u32 const &numWorkers = std::thread::hardware_concurrency());
  ~Executor();

  Vec<Exceptions::ExecutorError> GetExceptions() const;

  TaskHandle schedule(Task const &target) {
    return this->enqueue([target] { target(); });
  }
  TaskHandle schedule(Task const &target, Vec<TaskHandle> const &dependencies);
  template <Runnable T> TaskHandle Schedule(T &target) {
    return this->enqueue([target] { target(); });
  }

  void waitForAll() const { mComplete.wait(false); }
};

} // namespace Terreate::Core

#endif // __TERREATECORE_EXECUTOR_HPP__
