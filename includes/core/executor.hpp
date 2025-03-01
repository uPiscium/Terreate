#ifndef __TERREATECORE_EXECUTOR_HPP__
#define __TERREATECORE_EXECUTOR_HPP__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

#include <exceptions.hpp>
#include <types.hpp>

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

  std::atomic<Uint> mNumJobs = 0u;
  std::atomic<Bool> mComplete = false;
  std::atomic<Bool> mStop = false;

private:
  void Worker();
  TaskHandle Enqueue(Task &&task);

public:
  explicit Executor(
      Uint const &numWorkers = std::thread::hardware_concurrency());
  ~Executor();

  Vec<Exceptions::ExecutorError> GetExceptions() const;

  TaskHandle Schedule(Task const &target) {
    return this->Enqueue([target] { target(); });
  }
  TaskHandle Schedule(Task const &target, Vec<TaskHandle> const &dependencies);
  template <Runnable T> TaskHandle Schedule(T &target) {
    return this->Enqueue([target] { target(); });
  }

  void WaitForAll() const { mComplete.wait(false); }
};

} // namespace Terreate::Core

#endif // __TERREATECORE_EXECUTOR_HPP__
