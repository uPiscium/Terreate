#ifndef __TERREATE_CONTEXT_HPP__
#define __TERREATE_CONTEXT_HPP__

#include <core.hpp>
#include <graphic.hpp>
#include <resource.hpp>

namespace Terreate {
using namespace Terreate::Types;
using namespace Terreate::Graphic::Subscribers;

typedef Resource<Graphic::Window> Display;
typedef Graphic::WindowSettings WindowSettings;

using Core::Event;

class ContextHandler;

class Context {
private:
  ContextHandler *mHandler;
  std::atomic<Bool> mRunning = true;
  std::thread mContextThread;
  std::atomic<Bool> *mQuit;
  std::atomic<Ubyte> *mNumContexts;

public:
  typedef Function<Bool(Context *)> FrameFunction;

public:
  Display window;
  Event<Context *> onStart;
  Event<Context *> onEnd;

public:
  Context(Uint const &width, Uint const &height, Str const &title,
          WindowSettings const &settings, Display &shared,
          std::atomic<Bool> *quit, std::atomic<Ubyte> *numContexts,
          ContextHandler *handler)
      : mQuit(quit), mNumContexts(numContexts), mHandler(handler) {
    window = Display(width, height, title, settings, shared.Get());
  }
  ~Context() { this->QuitContext(); }

  void QuitContext();
  void QuitApp();

  void Run(FrameFunction const &frameFunction);
};

typedef Core::Subscriber<Context *> ContextSubscriber;

class ContextHandler {
private:
  std::atomic<Bool> mRunning = true;
  std::atomic<Ubyte> mNumContexts = 0u;
  Vec<Resource<Context>> mContexts;
  Vec<std::thread *> mContextThreads;
  Display mMasterWindow;
  std::mutex mContextMutex;
  std::mutex mThreadMutex;

private:
  friend class Context;
  void RegisterThread(std::thread *thread);

public:
  ContextHandler();
  ~ContextHandler();

  Display &GetMasterDisplay() { return mMasterWindow; }

  Bool IsRunning() { return mRunning.load(); }

  Resource<Context> CreateContext(Uint const &width, Uint const &height,
                                  Str const &title) {
    return this->CreateContext(width, height, title, WindowSettings());
  }
  Resource<Context> CreateContext(Uint const &width, Uint const &height,
                                  Str const &title,
                                  WindowSettings const &settings);

  void Quit();
};
} // namespace Terreate

#endif // __TERREATE_CONTEXT_HPP__
