#include <executor.hpp>

namespace Terreate::Core {
void Executor::Worker() {
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

Executor::TaskHandle Executor::Enqueue(Task &&task) {
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

Executor::Executor(Uint const &numWorkers) {
  if (numWorkers == 0) {
    throw Exceptions::ExecutorError(
        "Number of workers must be greater than 0.");
  }

  for (Uint i = 0; i < numWorkers; ++i) {
    mWorkers.emplace_back(&Executor::Worker, this);
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

Vec<Exceptions::ExecutorError> Executor::GetExceptions() const {
  Vec<Exceptions::ExecutorError> exceptions;
  for (auto &handle : mHandles) {
    if (handle.valid()) {
      try {
        handle.get();
      } catch (std::exception const &e) {
        exceptions.push_back(Exceptions::ExecutorError(e.what()));
      }
    }
  }
  return exceptions;
}

Executor::TaskHandle Executor::Schedule(Task const &target,
                                        Vec<TaskHandle> const &dependencies) {
  Task wrapper = ([&target, &dependencies]() {
    for (auto const &dependency : dependencies) {
      if (dependency.valid()) {
        dependency.wait();
      }
    }
    target();
  });
  return this->Enqueue(std::move(wrapper));
}

} // namespace Terreate::Core
