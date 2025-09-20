// unique ptr is a non-copyable, moveable smart pointer raii class
#include <iostream>

template<typename T>
class UniquePointer {
private:
  T* m_pointer = nullptr;

public:
  UniquePointer(T* pointer) : m_pointer{pointer} {}
  ~UniquePointer() {
    delete m_pointer;
  }

  UniquePointer(const UniquePointer&) = delete;
  UniquePointer& operator=(const UniquePointer&) = delete;
  
  UniquePointer(UniquePointer&& other) {
    m_pointer = other.m_pointer;
    other.m_pointer = nullptr;
  }

  UniquePointer& operator=(UniquePointer&& other) {
    if (*this == other) return *this;

    delete m_pointer;
    m_pointer = other.m_pointer;
    other.m_pointer = nullptr;

    return *this;
  }

  T* release() {
    T* to_release = m_pointer;
    m_pointer = nullptr;
    return to_release;
  }

  T* get() const {
    return m_pointer;
  }

  explicit operator bool() const {
    return m_pointer != nullptr;
  }
};

template<typename T, typename... Args>
UniquePointer<T> MakeUnique(Args&&... args) {
  return UniquePointer<T>(new T(std::forward<Args>(args)...));
}

class Box {
public:
  Box() {
    std::cout << "Box constructor called" << std::endl;
  }

  ~Box() {
    std::cout << "Box destructor called" << std::endl;
  }

  Box(const Box& other) {
    std::cout << "Box Copy constructor called" << std::endl;
  }

  Box& operator=(const Box& other) {
    std::cout << "Box Copy operator called" << std::endl;
  }

  Box(const Box&& other) {
    std::cout << "Box move constructor called" << std::endl;
  }

  Box&& operator=(const Box&& other) {
    std::cout << "Box move operator called" << std::endl;
  }
};

int main() {
  UniquePointer<Box> unique_pointer = MakeUnique<Box>();
  std::cout << "Expected: Box constructor called" << std::endl;

  // CE should not work
  // UniquePointer<Box> copy = unique_pointer;
  std::cout << "unique_pointer.get(): Got: " << unique_pointer.get() << " Expected: pointer addr" << std::endl;
  
  UniquePointer<Box> move = std::move(unique_pointer);
  std::cout << "Expected: Box move constructor called" << std::endl;

  std::cout << "move.get(): Got: " << move.get() << " Expected: address" << std::endl;
  std::cout << "unique_pointer.get(): Got: " << unique_pointer.get() << " Expected: nullptr" << std::endl;

  UniquePointer<Box> unique_pointer2 = MakeUnique<Box>();
  Box* box_pointer = unique_pointer2.release();
  std::cout << "get(): Got: " << unique_pointer2.get() << " Expected: nullptr" << std::endl;
  delete box_pointer;
}
