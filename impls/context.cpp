#include <context.hpp>

namespace Terreate {
void Context::QuitContext() {
  mRunning.store(false);
  mRunning.notify_all();
}

void Context::QuitApp() {
  this->QuitContext();
  mHandler->Quit();
}

void Context::Run(FrameFunction const &frameFunction) {
  mContextThread = std::thread([this, frameFunction]() {
    mNumContexts->fetch_add(1);

    onStart.Publish(this);
    while (!this->window->IsClosed() && mQuit->load() && mRunning.load()) {
      window->SetCurrentContext();
      if (!frameFunction(this)) {
        break;
      }
    }
    onEnd.Publish(this);

    this->QuitContext();
    window->Hide();
    window->Destroy();

    if (mNumContexts->fetch_sub(1) == 1) {
      mQuit->store(false);
    }
  });
  mHandler->RegisterThread(&mContextThread);
}

void ContextHandler::RegisterThread(std::thread *thread) {
  std::unique_lock<std::mutex> lock(mThreadMutex);
  mContextThreads.push_back(thread);
}

ContextHandler::ContextHandler() {
  Graphic::InitializeGLFW();
  WindowSettings setting;
  setting.visible = false;
  mMasterWindow.Assign(1, 1, "Master Window", setting);
  Graphic::InitializeGLAD();
  glfwMakeContextCurrent(nullptr);
}

ContextHandler::~ContextHandler() {
  this->Quit();
  for (auto &thread : mContextThreads) {
    if (thread->joinable()) {
      thread->join();
    }
  }

  mMasterWindow->Destroy();
  Graphic::Terminate();
}

Resource<Context>
ContextHandler::CreateContext(Uint const &width, Uint const &height,
                              Str const &title,
                              WindowSettings const &settings) {
  std::unique_lock<std::mutex> lock(mContextMutex);
  auto context =
      Resource<Context>::Create(width, height, title, settings, mMasterWindow,
                                &mRunning, &mNumContexts, this);
  mContexts.push_back(context); // TODO: [Windows] Error -> No viable
                                // overloaded. / replace with emplace_back()?
  glfwMakeContextCurrent(nullptr);
  return context;
}

void ContextHandler::Quit() {
  mRunning.store(false);
  mRunning.notify_all();
  for (auto &context : mContexts) {
    context->QuitContext();
  }
}

} // namespace Terreate
