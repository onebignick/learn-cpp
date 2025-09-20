#include <iostream>

template<typename T>
class SharedPointerControlBlock {
public:
  T* m_object_ptr;
  size_t m_strong_count;
  size_t m_weak_count;

  SharedPointerControlBlock(
    T* object_ptr,
    size_t strong_count = 0,
    size_t weak_count = 0
  ) : m_strong_count{strong_count},
    m_weak_count{weak_count},
    m_object_ptr{object_ptr}
  {}

  ~SharedPointerControlBlock() {}
  SharedPointerControlBlock(const SharedPointerControlBlock&) = delete;
  SharedPointerControlBlock& operator=(const SharedPointerControlBlock&) = delete;
  SharedPointerControlBlock(SharedPointerControlBlock&& other) = delete;
  SharedPointerControlBlock& operator=(SharedPointerControlBlock&& other) = delete;
};

template<typename T>
class SharedPointer {
private:
  T* m_obj_pointer;
  SharedPointerControlBlock<T>* m_ctrl_pointer;

  void handle_dereference() {
    if (m_ctrl_pointer->m_strong_count == 0) {
      m_ctrl_pointer->m_object_ptr = nullptr;
      delete m_obj_pointer;

      if (m_ctrl_pointer->m_weak_count == 0) {
        delete m_ctrl_pointer;
      }
    }
  }

public:
  SharedPointer(T* object_pointer) : m_obj_pointer{object_pointer} {
    m_ctrl_pointer = new SharedPointerControlBlock<T>(object_pointer);
    m_ctrl_pointer->m_strong_count += 1;
  }

  ~SharedPointer() {
    m_obj_pointer = nullptr;
    m_ctrl_pointer->m_strong_count -= 1;
    handle_dereference();
  }

  SharedPointer(const SharedPointer& other) :
    m_obj_pointer{other.m_obj_pointer},
    m_ctrl_pointer{other.m_ctrl_pointer}
  {
    m_ctrl_pointer->m_strong_count += 1;
  }

  SharedPointer& operator=(const SharedPointer& other) {
    if (*this == other) return *this;

    if (m_obj_pointer != nullptr) {
      m_obj_pointer = nullptr;
      m_ctrl_pointer->m_strong_count -= 1;
      handle_dereference();
    }

    m_obj_pointer = other.m_obj_pointer;
    m_ctrl_pointer = other.m_ctrl_pointer;
    m_ctrl_pointer->m_strong_count += 1;
    return *this;
  }

  SharedPointer(SharedPointer&& other) :
    m_obj_pointer{other.m_obj_pointer},
    m_ctrl_pointer{other.m_ctrl_pointer}
  {
    m_ctrl_pointer->m_strong_count += 1;
  }

  SharedPointer& operator=(SharedPointer&& other) {
    if (*this == other) return *this;

    if (m_obj_pointer != nullptr) {
      m_obj_pointer = nullptr;
      m_ctrl_pointer->m_strong_count -= 1;
      handle_dereference();
    }

    m_obj_pointer = other.m_obj_pointer;
    m_ctrl_pointer = other.m_ctrl_pointer;

    other.m_obj_pointer = nullptr;
    other.m_ctrl_pointer = nullptr;

    return *this;
  }
};
