#include <cstring>
#include <iostream>
#include <new>
#include <cstdlib>

void* operator new(std::size_t size) {
    std::cout << "[Global new] Allocating " << size << " bytes\n";
    if (void* p = std::malloc(size))
        return p;
    throw std::bad_alloc{};
}

void operator delete(void* p) noexcept {
    std::cout << "[Global delete] Freeing memory\n";
    std::free(p);
}

class String {
private:
  size_t m_size;
  size_t m_capacity;

  union {
    struct {
      char* m_ptr;
    } heap;
    char buffer[sizeof(heap)];
  };

public:
  String() {};
  explicit String(const char* c_str) {
    size_t len = std::strlen(c_str);
    m_size = len;
    if (len < sizeof(heap)) {
      std::memcpy(buffer, c_str, len + 1);
      m_capacity = sizeof(heap) - 1;
    } else {
      heap.m_ptr = new char[len + 1];
      m_capacity = len;
      std::memcpy(heap.m_ptr, c_str, len + 1);
    }
  }

  ~String() {
    if (m_capacity >= sizeof(heap)) {
      delete[] heap.m_ptr;
    }
  }

  const char* data() const noexcept {
    return m_capacity == sizeof(heap) - 1 ? buffer : heap.m_ptr;
  }

  friend std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.data();
  }
};

int main() {
  String small("small");
  std::cout << small << std::endl;
  
  String test("this is a test");
  std::cout << test << std::endl;
}
