#include "gtest/gtest.h"
#include "shared_ptr.h"
#include "weak_ptr.h"
#include <string>

template<typename T>
class Testing {
 public:
  static int NumberOfShared(const pointers::WeakPtr<T>& weak_ptr);
  static int NumberOfWeak(const pointers::WeakPtr<T>& weak_ptr);
  static int NumberOfShared(const pointers::SharedPtr<T>& shared_ptr);
  static int NumberOfWeak(const pointers::SharedPtr<T>& shared_ptr);
};

template<typename T>
int Testing<T>::NumberOfShared(const pointers::WeakPtr<T>& weak_ptr) {
  if (weak_ptr.number_of_ptrs_ == nullptr) {
    return -1;
  }
  return weak_ptr.number_of_ptrs_->shared;
}

template<typename T>
int Testing<T>::NumberOfWeak(const pointers::WeakPtr<T>& weak_ptr) {
  if (weak_ptr.number_of_ptrs_ == nullptr) {
    return -1;
  }
  return weak_ptr.number_of_ptrs_->weak;
}

template<typename T>
int Testing<T>::NumberOfShared(const pointers::SharedPtr<T>& shared_ptr) {
  if (shared_ptr.number_of_ptrs_ == nullptr) {
    return -1;
  }
  return shared_ptr.number_of_ptrs_->shared;
}

template<typename T>
int Testing<T>::NumberOfWeak(const pointers::SharedPtr<T>& shared_ptr) {
  if (shared_ptr.number_of_ptrs_ == nullptr) {
    return -1;
  }
  return shared_ptr.number_of_ptrs_->weak;
}

TEST(WeakPtr, ConstructorsAndDestructor) {
  {
    pointers::WeakPtr<int> ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(ptr), -1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(ptr), -1);
  }
  const pointers::SharedPtr<int> shared_ptr(new int(776));
  EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
  EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
  {
    pointers::WeakPtr<int> weak_ptr(shared_ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(weak_ptr), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(weak_ptr), 1);
    EXPECT_EQ(weak_ptr.Get(), shared_ptr.Get());
  }
  EXPECT_EQ(*shared_ptr, 776);
  EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
  EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
  {
    const pointers::WeakPtr<int> weak_ptr1(shared_ptr);
    pointers::WeakPtr<int> weak_ptr2(weak_ptr1);
    EXPECT_EQ(Testing<int>::NumberOfShared(weak_ptr1), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(weak_ptr1), 2);
    EXPECT_EQ(weak_ptr1.Get(), shared_ptr.Get());
    EXPECT_EQ(weak_ptr2.Get(), shared_ptr.Get());
  }
  EXPECT_EQ(*shared_ptr, 776);
  EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
  EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
  {
    pointers::WeakPtr<int> weak_ptr1(shared_ptr);
    pointers::WeakPtr<int> weak_ptr2(weak_ptr1);
    EXPECT_EQ(Testing<int>::NumberOfShared(weak_ptr1), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(weak_ptr1), 2);
    EXPECT_EQ(weak_ptr1.Get(), shared_ptr.Get());
    EXPECT_EQ(weak_ptr2.Get(), shared_ptr.Get());
  }
  EXPECT_EQ(*shared_ptr, 776);
  EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
  EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
  {
    pointers::WeakPtr<int> weak_ptr1(shared_ptr);
    pointers::WeakPtr<int> weak_ptr2(std::move(weak_ptr1));
    EXPECT_EQ(weak_ptr1.Get(), nullptr);
    EXPECT_EQ(weak_ptr2.Get(), shared_ptr.Get());
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 1);
  }
  EXPECT_EQ(*shared_ptr, 776);
  EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
  EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
}

TEST(WeakPtr, AssignmentOperators) {
  pointers::SharedPtr<std::string> shared_ptr(new std::string("Hi"));
  const pointers::SharedPtr<std::string> shared_ptr2(new std::string("hey"));
  const pointers::WeakPtr<std::string> weak_ptr1(shared_ptr);
  pointers::WeakPtr<std::string> weak_ptr2(shared_ptr2);
  weak_ptr2 = weak_ptr1;
  EXPECT_EQ(Testing<std::string>::NumberOfShared(weak_ptr1), 1);
  EXPECT_EQ(Testing<std::string>::NumberOfWeak(weak_ptr1), 2);
  EXPECT_EQ(weak_ptr1.Get(), shared_ptr.Get());
  EXPECT_EQ(weak_ptr2.Get(), shared_ptr.Get());
  EXPECT_EQ(Testing<std::string>::NumberOfShared(shared_ptr2), 1);
  EXPECT_EQ(Testing<std::string>::NumberOfWeak(shared_ptr2), 0);

  pointers::WeakPtr<std::string> weak_ptr3(shared_ptr2);
  weak_ptr2 = std::move(weak_ptr3);
  EXPECT_EQ(weak_ptr2.Get(), shared_ptr2.Get());
  EXPECT_EQ(weak_ptr3.Get(), nullptr);
  EXPECT_EQ(Testing<std::string>::NumberOfShared(shared_ptr2), 1);
  EXPECT_EQ(Testing<std::string>::NumberOfWeak(shared_ptr2), 1);
  EXPECT_EQ(Testing<std::string>::NumberOfShared(shared_ptr), 1);
  EXPECT_EQ(Testing<std::string>::NumberOfWeak(shared_ptr), 1);

  weak_ptr2 = weak_ptr2;
  EXPECT_EQ(weak_ptr2.Get(), shared_ptr2.Get());
  EXPECT_EQ(Testing<std::string>::NumberOfShared(weak_ptr2), 1);
  EXPECT_EQ(Testing<std::string>::NumberOfWeak(weak_ptr2), 1);
  weak_ptr2 = std::move(weak_ptr2);
  EXPECT_EQ(weak_ptr2.Get(), shared_ptr2.Get());
  EXPECT_EQ(Testing<std::string>::NumberOfShared(weak_ptr2), 1);
  EXPECT_EQ(Testing<std::string>::NumberOfWeak(weak_ptr2), 1);

  {
    pointers::WeakPtr<int> weak_ptr;
    pointers::SharedPtr<int> shared_ptr1(new int(8));
    pointers::WeakPtr<int> weak_ptr4(shared_ptr1);
    EXPECT_EQ(Testing<int>::NumberOfShared(weak_ptr), -1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(weak_ptr), -1);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr1), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr1), 1);
    weak_ptr4 = weak_ptr;
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr1), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr1), 0);
    EXPECT_EQ(Testing<int>::NumberOfShared(weak_ptr4), -1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(weak_ptr4), -1);
    EXPECT_EQ(weak_ptr4.Get(), nullptr);
    pointers::WeakPtr<int> weak_ptr5(shared_ptr1);
    weak_ptr5 = std::move(weak_ptr4);
    EXPECT_EQ(Testing<int>::NumberOfShared(weak_ptr5), -1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(weak_ptr5), -1);
    EXPECT_EQ(weak_ptr5.Get(), nullptr);
  }
}

TEST(WeakPtr, ConstGetAndReset) {
  pointers::SharedPtr<int> shared_ptr(new int(87));
  {
    pointers::WeakPtr<int> weak_ptr(shared_ptr);
    const int* const_ptr = weak_ptr.Get();
    EXPECT_EQ(const_ptr, shared_ptr.Get());
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 1);

    weak_ptr.Reset();
    EXPECT_EQ(weak_ptr.Get(), nullptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
    weak_ptr.Reset();
    EXPECT_EQ(weak_ptr.Get(), nullptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
  }
  EXPECT_EQ(*shared_ptr, 87);
  EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
  EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr), 0);
}

TEST(WeakPtr, Expired) {
  pointers::WeakPtr<int> weak_ptr;
  EXPECT_TRUE(weak_ptr.Expired());
  {
    pointers::SharedPtr<int> shared_ptr(new int(87654));
    pointers::WeakPtr<int> weak_ptr2(shared_ptr);
    EXPECT_FALSE(weak_ptr2.Expired());
    {
      const pointers::SharedPtr<int> shared_ptr2(shared_ptr);
      weak_ptr = weak_ptr2;
      EXPECT_EQ(weak_ptr.Get(), shared_ptr.Get());
      EXPECT_FALSE(weak_ptr.Expired());
      EXPECT_FALSE(weak_ptr2.Expired());
    }
    EXPECT_FALSE(weak_ptr.Expired());
    EXPECT_FALSE(weak_ptr2.Expired());
  }
  EXPECT_TRUE(weak_ptr.Expired());
}

TEST(WeakPtr, Lock) {
  {
    pointers::SharedPtr<std::string> shared_ptr1(new std::string("hello"));
    pointers::WeakPtr<std::string> weak_ptr(shared_ptr1);
    pointers::SharedPtr<std::string> shared_ptr2 = weak_ptr.Lock();
    EXPECT_TRUE(shared_ptr2 == shared_ptr1);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(shared_ptr1), 2);
    EXPECT_EQ(Testing<std::string>::NumberOfWeak(shared_ptr2), 1);
    const pointers::SharedPtr<std::string> shared_ptr3 = weak_ptr.Lock();
    EXPECT_TRUE(shared_ptr3 == shared_ptr1);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(shared_ptr1), 3);
    EXPECT_EQ(Testing<std::string>::NumberOfWeak(shared_ptr2), 1);
  }
  {
    pointers::SharedPtr<int> shared_ptr(nullptr);
    pointers::WeakPtr<int> weak_ptr(shared_ptr);
    EXPECT_TRUE(weak_ptr.Expired());
    pointers::SharedPtr<int> shared_ptr2 = weak_ptr.Lock();
    EXPECT_EQ(shared_ptr2, nullptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr2), -1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr2), -1);
  }
  {
    pointers::WeakPtr<int> weak_ptr;
    EXPECT_EQ(Testing<int>::NumberOfShared(weak_ptr), -1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(weak_ptr), -1);
    {
      pointers::SharedPtr<int> shared_ptr(new int(654));
      pointers::WeakPtr<int> weak_ptr2(shared_ptr);
      weak_ptr = weak_ptr2;
      EXPECT_FALSE(weak_ptr.Expired());
      EXPECT_FALSE(weak_ptr2.Expired());
    }
    EXPECT_TRUE(weak_ptr.Expired());
    pointers::SharedPtr<int> shared_ptr2 = weak_ptr.Lock();
    EXPECT_EQ(shared_ptr2, nullptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr2), -1);
    EXPECT_EQ(Testing<int>::NumberOfWeak(shared_ptr2), -1);
  }
}
