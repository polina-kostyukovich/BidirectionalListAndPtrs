#ifndef WEAK_PTR_H_
#define WEAK_PTR_H_

#include "shared_ptr.h"

namespace pointers {

template<typename T>
class WeakPtr {
 public:
  WeakPtr();
  explicit WeakPtr(const SharedPtr<T>&);
  WeakPtr(const WeakPtr<T>&);
  WeakPtr(WeakPtr<T>&&);
  WeakPtr& operator=(const WeakPtr<T>&);
  WeakPtr& operator=(WeakPtr<T>&&);

  ~WeakPtr();

  T* Get();
  const T* Get() const;
  void Reset();

  bool Expired() const;
  SharedPtr<T> Lock() const;

 private:
  T* ptr_;
  typename SharedPtr<T>::NumberOfPtrs* number_of_ptrs_;

  friend class Testing<T>;
};

}  // namespace pointers

template<typename T>
pointers::WeakPtr<T>::WeakPtr() : ptr_(nullptr), number_of_ptrs_(nullptr) {}

template<typename T>
pointers::WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared_ptr)
    : ptr_(shared_ptr.ptr_), number_of_ptrs_(shared_ptr.number_of_ptrs_) {
  if (ptr_ != nullptr) {
    ++(number_of_ptrs_->weak);
  }
}

template<typename T>
pointers::WeakPtr<T>::WeakPtr(const WeakPtr<T>& other)
    : ptr_(other.ptr_), number_of_ptrs_(other.number_of_ptrs_) {
  if (ptr_ != nullptr) {
    ++(number_of_ptrs_->weak);
  }
}

template<typename T>
pointers::WeakPtr<T>::WeakPtr(WeakPtr<T>&& other)
    : ptr_(other.ptr_), number_of_ptrs_(other.number_of_ptrs_) {
  other.ptr_ = nullptr;
  other.number_of_ptrs_ = nullptr;
}

template<typename T>
pointers::WeakPtr<T>& pointers::WeakPtr<T>::operator=(
    const pointers::WeakPtr<T>& other) {
  if (this == &other) {
    return *this;
  }
  Reset();
  ptr_ = other.ptr_;
  number_of_ptrs_ = other.number_of_ptrs_;
  if (ptr_ != nullptr) {
    ++(number_of_ptrs_->weak);
  }
  return *this;
}

template<typename T>
pointers::WeakPtr<T>& pointers::WeakPtr<T>::operator=(
    pointers::WeakPtr<T>&& other) {
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
pointers::WeakPtr<T>::~WeakPtr() {
  Reset();
}

template<typename T>
T* pointers::WeakPtr<T>::Get() {
  return ptr_;
}

template<typename T>
const T* pointers::WeakPtr<T>::Get() const {
  return ptr_;
}

template<typename T>
void pointers::WeakPtr<T>::Reset() {
  if (ptr_ == nullptr) return;
  if (number_of_ptrs_->shared == 0 && number_of_ptrs_->weak == 1) {
    delete number_of_ptrs_;
  } else {
    --(number_of_ptrs_->weak);
  }
  ptr_ = nullptr;
  number_of_ptrs_ = nullptr;
}

template<typename T>
bool pointers::WeakPtr<T>::Expired() const {
  if (ptr_ == nullptr) {
    return true;
  }
  return number_of_ptrs_->shared == 0;
}

template<typename T>
pointers::SharedPtr<T> pointers::WeakPtr<T>::Lock() const {
  if (Expired()) {
    return SharedPtr<T>();
  }
  SharedPtr<T> shared_ptr;
  shared_ptr.ptr_ = ptr_;
  shared_ptr.number_of_ptrs_ = number_of_ptrs_;
  ++(number_of_ptrs_->shared);
  return shared_ptr;
}

#endif  // WEAK_PTR_H_
