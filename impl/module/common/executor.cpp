#include "../../../include/module/common/executor.hpp"

namespace Terreate {
void Executor::worker() {
  while (true) {
    std::packaged_task<void()> task;
    {
      std::unique_lock<std::mutex> lock(mQueueMutex);
      mCV.wait(lock, [this] { return !mTaskQueue.empty() || mStop; });

      if (mStop && mTaskQueue.empty()) {
        return;
      }

      task = std::move(mTaskQueue.front());
      mTaskQueue.pop();
    }

    task();

    if (mNumJobs.fetch_sub(1) == 1) {
      mComplete.store(true);
      mComplete.notify_all();
    }
  }
}

Executor::TaskHandle Executor::enqueue(Task &&task) {
  std::packaged_task<void()> wrapper(std::move(task));
  TaskHandle future = wrapper.get_future().share();
  mHandles.push_back(future);

  {
    std::lock_guard<std::mutex> lock(mQueueMutex);
    mTaskQueue.push(std::move(wrapper));
    mNumJobs.fetch_add(1);
    mComplete.store(false);
  }
  mCV.notify_one();

  return future;
}

Executor::Executor(u32 const &numWorkers) {
  if (numWorkers == 0) {
    throw Exception::ExecutorError("Number of workers must be greater than 0.");
  }

  for (u32 i = 0; i < numWorkers; ++i) {
    mWorkers.emplace_back(&Executor::worker, this);
  }
}

Executor::~Executor() {
  {
    std::lock_guard<std::mutex> lock(mQueueMutex);
    mStop.store(true);
  }
  mCV.notify_all();

  for (auto &worker : mWorkers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

vec<Exception::ExecutorError> Executor::getExceptions() const {
  vec<Exception::ExecutorError> exceptions;
  for (auto &handle : mHandles) {
    if (handle.valid()) {
      try {
        handle.get();
      } catch (std::exception const &e) {
        exceptions.push_back(Exception::ExecutorError(e.what()));
      }
    }
  }
  return exceptions;
}

Executor::TaskHandle Executor::schedule(Task const &target,
                                        vec<TaskHandle> const &dependencies) {
  Task wrapper = ([&target, &dependencies]() {
    for (auto const &dependency : dependencies) {
      if (dependency.valid()) {
        dependency.wait();
      }
    }
    target();
  });
  return this->enqueue(std::move(wrapper));
}

} // namespace Terreate
