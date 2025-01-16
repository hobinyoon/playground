#include <gflags/gflags.h>
#include <glog/logging.h>

#include <memory>
#include <vector>

struct A {
  virtual void Print() { LOG(INFO); }
};

struct B : public A {
  void Print() { LOG(INFO); }
};

void Func1(A* a) {
  LOG(INFO) << typeid(a).name() << "\n";
  LOG(INFO) << typeid(*a).name() << "\n";
  // Interesting! *a's type is A until the Print() functions are added. Maybe
  // the compiler is smart enough to know that there's no need to tell them
  // apart.

  a->Print();
  // Hitting B::Print()
}

// TODO: shared_ptr later
void Func2(std::unique_ptr<A> a) {
  LOG(INFO) << typeid(a).name() << "\n";
  LOG(INFO) << typeid(*a).name() << "\n";
  // Interesting! *a's type is A until the Print() functions are added. Maybe
  // the compiler is smart enough to know that there's no need to tell them
  // apart.

  a->Print();
  // Hitting B::Print()
}

void Pass_Pointer_Child() {
  auto b = std::make_unique<B>();
  LOG(INFO) << typeid(b).name() << "\n";
  b->Print();
  Func1(b.get());
}

void Pass_UniqPointer_Child() {
  auto b = std::make_unique<B>();
  LOG(INFO) << typeid(b).name() << "\n";
  b->Print();
  Func2(std::move(b));
}

void Func3(std::vector<std::unique_ptr<A>> vec_a) {
  LOG(INFO) << typeid(vec_a).name() << "\n";
}

void Pass_Vector_UniqPointer_Child() {
  std::vector<std::unique_ptr<B>> vec_b;
  vec_b.push_back(std::make_unique<B>());
  vec_b.push_back(std::make_unique<B>());

  LOG(INFO) << typeid(vec_b).name() << "\n";

  // Pass it to a function that takes a vector of unique_ptr of type A
  // Func3(vec_b);
  // hello_world.cc:64:14: error: could not convert 'vec_b' from
  // 'std::vector<std::unique_ptr<B, std::default_delete<B> > >' to
  // 'std::vector<std::unique_ptr<A> >'
}

void Func4(std::vector<std::shared_ptr<A>> vec_a) {
  LOG(INFO) << typeid(vec_a).name() << "\n";
}

void Pass_Vector_SharedPointer_Child() {
  std::vector<std::shared_ptr<B>> vec_b;
  vec_b.push_back(std::make_shared<B>());
  vec_b.push_back(std::make_shared<B>());

  LOG(INFO) << typeid(vec_b).name() << "\n";

  // Pass it to a function that takes a vector of unique_ptr of type A
  Func4(vec_b);
  // hello_world.cc:82:14: error: could not convert 'vec_b' from
  // 'std::vector<std::shared_ptr<B> >' to 'std::vector<std::shared_ptr<A> >'
}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_logtostdout = true;
  FLAGS_colorlogtostdout = true;

  // Pass_Pointer_Child();
  // Pass_UniqPointer_Child();
  // Pass_Vector_UniqPointer_Child();
  Pass_Vector_SharedPointer_Child();

  return 0;
}
