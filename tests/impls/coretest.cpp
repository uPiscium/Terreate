#include <coretest.hpp>
#include <exceptions.hpp>
#include <iostream>

namespace Terreate::Test::Core {

// Event
void TestEvent() {
  std::cout << ">>>>>>>>>>>>>>>>>>>> TestEvent\n";
  Event<int, int> event;
  Subscriber<int, int> add([](int a, int b) {
    std::cout << "Published [a + b]:" << a + b << std::endl;
  });
  Subscriber<int, int> mul([](int a, int b) {
    std::cout << "Published [a * b]:" << a * b << std::endl;
  });
  event += add;
  std::cout << "Subscribed: [a + b]" << std::endl;
  event += mul;
  std::cout << "Subscribed: [a * b]" << std::endl;
  event.Publish(1, 2);
  event.Publish(3, 4);
  event -= add;
  std::cout << "Unsubscribed: [a + b]" << std::endl;
  event.Publish(5, 6);
  std::cout << "<<Event test done>>" << std::endl;
}

// Executor
void Task(int n, int i) {
  for (int j = 0; j < n; ++j) {
    std::this_thread::sleep_for(std::chrono::milliseconds(i * 300));
    std::cout << "Task " << i << std::endl;
  }
}

void ErrorTask() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  throw Exceptions::ExecutorError("This is expected error.");
}

struct Callable {
  void operator()() { std::cout << "Callable" << std::endl; }
};

void TestExecutor() {
  std::cout << ">>>>>>>>>>>>>>>>>>>> TestExecutor\n";
  Executor executor;
  for (int i = 0; i < 3; ++i) {
    std::cout << "Schedule Task " << i << std::endl;
    executor.Schedule([i]() { Task(3, i); });
  }
  std::cout << "Schedule ErrorTask" << std::endl;
  executor.Schedule(ErrorTask);
  std::cout << "Schedule Callable" << std::endl;
  executor.Schedule(Callable());

  executor.WaitForAll();
  std::cout << "Done" << std::endl;

  std::cout << "Exceptions:" << std::endl;
  for (auto &e : executor.GetExceptions()) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  std::cout << "<<Executor test done>>" << std::endl;
}

// Object
void TestObject() {
  std::cout << ">>>>>>>>>>>>>>>>>>>> TestObject\n";
  Object obj;
  std::cout << "obj: " << obj.Ref() << std::endl;
  Object ref1 = obj;
  ref1 = 1;
  std::cout << "ref1 = obj; ref1 = 1;" << std::endl;
  std::cout << "obj: " << obj.Ref() << std::endl;
  Object ref2 = obj;
  ref2 = 2;
  std::cout << "ref2 = obj; ref2 = 2;" << std::endl;
  std::cout << "obj: " << obj.Ref() << std::endl;
  std::cout << "ref1: " << ref1.Ref() << std::endl;
  std::cout << "ref2: " << ref2.Ref() << std::endl;
  std::cout << "Ref count: " << obj.Count() << std::endl;

  std::cout << "Delete obj" << std::endl;
  obj.Delete();
  try {
    std::cout << obj.Ref() << std::endl;
  } catch (Exceptions::NullObjectException &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  std::cout << "<<Object test done>>" << std::endl;
}

// Nullable
void TestNullable() {
  std::cout << ">>>>>>>>>>>>>>>>>>>> TestNullable\n";
  Nullable<int> a = 1;
  Nullable<int> b = 2;
  Nullable<int> c = a + b;
  std::cout << "a = " << a.Value() << std::endl;
  std::cout << "b = " << b.Value() << std::endl;
  std::cout << "c = a + b = " << c.Value() << std::endl;

  Nullable<int> d;
  std::cout << "Validate d: " << d.Valid() << std::endl;

  try {
    std::cout << d.Value() << std::endl;
  } catch (Exceptions::NullReferenceException &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  std::cout << "<<Nullable test done>>" << std::endl;
}

// Property
void TestProperty() {
  std::cout << ">>>>>>>>>>>>>>>>>>>> TestProperty\n";
  int a = 1;
  Property<int> prop(&a);
  prop.getter = [&a]() -> int & {
    std::cout << "Getter" << std::endl;
    return a;
  };
  prop.setter = [&a](int const &value) {
    std::cout << "Setter" << std::endl;
    a = value;
  };
  std::cout << "a = " << a << std::endl;
  int b = prop;
  std::cout << "b = " << b << std::endl;
  std::cout << "<<Property test done>>" << std::endl;
}

// UUID
void TestUUID() {
  std::cout << ">>>>>>>>>>>>>>>>>>>> TestUUID\n";
  UUID uuid;
  std::cout << "Generate UUID" << std::endl;
  std::cout << uuid.ToString() << std::endl;
  std::cout << "<<UUID test done>>" << std::endl;
}

} // namespace Terreate::Test::Core
