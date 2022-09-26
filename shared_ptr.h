#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

template<typename T>
class Testing;

namespace pointers {

template<typename T>
class WeakPtr;

template<typename T>
class SharedPtr {
 public:
  SharedPtr();
  explicit SharedPtr(T* ptr);
  SharedPtr(const SharedPtr& other);
  SharedPtr& operator=(const SharedPtr& other);
  SharedPtr(SharedPtr&& other);
  SharedPtr& operator=(SharedPtr&& other);

  T* Get();
  const T* Get() const;

  void Reset();

  T& operator*();
  const T& operator*() const;
  T* operator->();
  const T* operator->() const;

  bool operator==(const SharedPtr& other) const;
  bool operator!=(const SharedPtr& other) const;
  bool operator==(const T* raw) const;
  bool operator!=(const T* raw) const;

  ~SharedPtr();

 private:
  struct NumberOfPtrs {
    int shared;
    int weak;
  };

 private:
  T* ptr_;
  NumberOfPtrs* number_of_ptrs_;

  friend class WeakPtr<T>;
  friend class Testing<T>;
};

template<typename T>
bool operator==(const T* raw, const SharedPtr<T>& shared);
template<typename T>
bool operator!=(const T* raw, const SharedPtr<T>& shared);

}  // namespace pointers

template<typename T>
pointers::SharedPtr<T>::SharedPtr() : ptr_(nullptr), number_of_ptrs_(nullptr) {}

template<typename T>
pointers::SharedPtr<T>::SharedPtr(T* ptr)
    : ptr_(ptr), number_of_ptrs_(nullptr) {
  if (ptr != nullptr) {
    number_of_ptrs_ = new NumberOfPtrs{1, 0};
  }
}

template<typename T>
pointers::SharedPtr<T>::SharedPtr(const SharedPtr& other)
    : ptr_(other.ptr_),
      number_of_ptrs_(other.number_of_ptrs_) {
  if (ptr_ != nullptr) {
    ++(number_of_ptrs_->shared);
  }
}

template<typename T>
pointers::SharedPtr<T>& pointers::SharedPtr<T>::operator=(
    const SharedPtr& other) {
  if (this == &other) {
    return *this;
  }
  Reset();
  ptr_ = other.ptr_;
  number_of_ptrs_ = other.number_of_ptrs_;
  if (ptr_ != nullptr) {
    ++(number_of_ptrs_->shared);
  }
  return *this;
}

template<typename T>
pointers::SharedPtr<T>::SharedPtr(SharedPtr&& other)
    : ptr_(other.ptr_), number_of_ptrs_(other.number_of_ptrs_) {
  other.ptr_ = nullptr;
  other.number_of_ptrs_ = nullptr;
}

template<typename T>
pointers::SharedPtr<T>& pointers::SharedPtr<T>::operator=(SharedPtr&& other) {
  if (this == &other) {
    return *this;
  }
  Reset();
  ptr_ = other.ptr_;
  number_of_ptrs_ = other.number_of_ptrs_;
  other.ptr_ = nullptr;
  other.number_of_ptrs_ = nullptr;
  return *this;
}

template<typename T>
pointers::SharedPtr<T>::~SharedPtr() {
  Reset();
}

template<typename T>
T* pointers::SharedPtr<T>::Get() {
  return ptr_;
}

template<typename T>
const T* pointers::SharedPtr<T>::Get() const {
  return ptr_;
}

template<typename T>
void pointers::SharedPtr<T>::Reset() {
  if (ptr_ == nullptr) return;
  if (number_of_ptrs_->shared == 1) {
    delete ptr_;
    if (number_of_ptrs_->weak == 0) {
      delete number_of_ptrs_;
    } else {
      number_of_ptrs_->shared = 0;
    }
  } else {
    --(number_of_ptrs_->shared);
  }
  ptr_ = nullptr;
  number_of_ptrs_ = nullptr;
}

template<typename T>
T& pointers::SharedPtr<T>::operator*() {
  return *ptr_;
}

template<typename T>
const T& pointers::SharedPtr<T>::operator*() const {
  return *ptr_;
}

template<typename T>
T* pointers::SharedPtr<T>::operator->() {
  return ptr_;
}

template<typename T>
const T* pointers::SharedPtr<T>::operator->() const {
  return ptr_;
}

template<typename T>
bool pointers::SharedPtr<T>::operator==(const SharedPtr& other) const {
  return ptr_ == other.ptr_;
}

template<typename T>
bool pointers::SharedPtr<T>::operator!=(const SharedPtr& other) const {
  return ptr_ != other.ptr_;
}

template<typename T>
bool pointers::SharedPtr<T>::operator==(const T* raw) const {
  return ptr_ == raw;
}

template<typename T>
bool pointers::SharedPtr<T>::operator!=(const T* raw) const {
  return ptr_ != raw;
}

template<typename T>
bool pointers::operator==(const T* raw, const pointers::SharedPtr<T>& shared) {
  return shared == raw;
}

template<typename T>
bool pointers::operator!=(const T* raw, const pointers::SharedPtr<T>& shared) {
  return shared != raw;
}

#endif  // SHARED_PTR_H_
