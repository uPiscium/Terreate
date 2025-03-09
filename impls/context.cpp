#include <context.hpp>

namespace Terreate {
Context::Context(Uint const &width, Uint const &height, Str const &title,
                 WindowSettings const &settings, Display &shared,
                 std::atomic<Bool> *quit, std::atomic<Ubyte> *numContexts,
                 ContextHandler *handler)
    : mQuit(quit), mNumContexts(numContexts) {
  this->window = Display::Create(width, height, title, settings, shared.Get());
  this->handler = handler;
}

void Context::QuitApp() {
  this->QuitContext();
  this->handler->Quit();
}

void Context::Run(FrameFunction const &frameFunction) {
  mContextThread = std::thread([this, frameFunction]() {
    mNumContexts->fetch_add(1);

    onStart.Publish(this);
    while (mQuit->load() && !this->window->IsClosed()) {
      window->SetCurrent();
      if (!frameFunction(this)) {
        break;
      }
    }
    onEnd.Publish(this);

    window->Hide();
    window->Destroy();

    if (mNumContexts->fetch_sub(1) == 1) {
      mQuit->store(false);
    }
  });
  this->handler->RegisterThread(&mContextThread);
}

void ContextHandler::RegisterThread(std::thread *thread) {
  std::unique_lock<std::mutex> lock(mThreadMutex);
  mContextThreads.push_back(thread);
}

ContextHandler::ContextHandler() {
  Graphic::InitializeGLFW();
  WindowSettings setting;
  setting.visible = false;
  mMasterWindow = Display::Create(1, 1, "Master Window", setting);
  Graphic::InitializeGLAD();
  glfwMakeContextCurrent(nullptr);
}

ContextHandler::~ContextHandler() {
  this->Quit();
  for (auto &thread : mContextThreads) {
    if ((thread != nullptr) && thread->joinable()) {
      thread->join();
    }
  }

  for (auto &context : mContexts) {
    context.Delete();
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
  mContexts.push_back(context);
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
