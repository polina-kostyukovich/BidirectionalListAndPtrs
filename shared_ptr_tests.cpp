#include "shared_ptr.h"
#include "gtest/gtest.h"
#include <string>

template<typename T>
class Testing {
 public:
  static int NumberOfShared(const pointers::SharedPtr<T>& shared_ptr);
};

template<typename T>
int Testing<T>::NumberOfShared(const pointers::SharedPtr<T>& shared_ptr) {
  if (shared_ptr.number_of_ptrs_ == nullptr) {
    return -1;
  }
  return shared_ptr.number_of_ptrs_->shared;
}

TEST(SharedPtr, ConstructorsAndDestructor) {
  {
    pointers::SharedPtr<int> ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(ptr), -1);
  }
  {
    int* ptr = new int(7);
    pointers::SharedPtr<int> shared_ptr1(ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr1), 1);
    pointers::SharedPtr<int> shared_ptr2(shared_ptr1);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr2), 2);
    {
      pointers::SharedPtr<int> shared_ptr3(shared_ptr1);
      EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr1), 3);
    }
    ASSERT_EQ(*(shared_ptr1.Get()), 7);
    ASSERT_EQ(*(shared_ptr2.Get()), 7);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr1), 2);
  }
  {
    int* ptr = nullptr;
    pointers::SharedPtr<int> shared_ptr(ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), -1);
    pointers::SharedPtr<int> shared_ptr2(shared_ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr2), -1);
  }
}

TEST(SharedPtr, GetAndReset) {
  std::string* ptr = new std::string("wonderland");
  pointers::SharedPtr<std::string> ptr_to_string(ptr);
  EXPECT_EQ(Testing<std::string>::NumberOfShared(ptr_to_string), 1);
  EXPECT_EQ(ptr_to_string.Get(), ptr);
  {
    const pointers::SharedPtr<std::string> const_ptr(ptr_to_string);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(ptr_to_string), 2);
    const std::string* const_raw_ptr = const_ptr.Get();
    EXPECT_EQ(const_raw_ptr, ptr);
  }
  EXPECT_EQ(Testing<std::string>::NumberOfShared(ptr_to_string), 1);
  ptr_to_string.Reset();
  EXPECT_EQ(ptr_to_string.Get(), nullptr);
  EXPECT_EQ(Testing<std::string>::NumberOfShared(ptr_to_string), -1);
}

TEST(SharedPtr, ConstructorFromRvelueSharedPtr) {
  {
    int* ptr = new int(777);
    pointers::SharedPtr<int> shared_ptr(ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
    pointers::SharedPtr<int> moved_ptr = std::move(shared_ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(moved_ptr), 1);
  }
  {
    int* ptr = new int(78);
    pointers::SharedPtr<int> shared_ptr(ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
    shared_ptr = std::move(shared_ptr);
    EXPECT_EQ(Testing<int>::NumberOfShared(shared_ptr), 1);
  }
  {
    std::shared_ptr<int> stl_ptr = std::make_shared<int>(45);
    EXPECT_EQ(*stl_ptr, 45);
    std::shared_ptr<int> stl_ptr2 = std::make_shared<int>(49);
    stl_ptr = std::move(stl_ptr2);
    EXPECT_EQ(*stl_ptr, 49);
    stl_ptr = std::move(stl_ptr);
    EXPECT_EQ(*stl_ptr, 49);
  }
}

TEST(SharedPtr, AssignmentOperators) {
  {
    std::string* raw_ptr = new std::string("Good luck...");
    pointers::SharedPtr<std::string> first_ptr(raw_ptr);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(first_ptr), 1);

    std::string* raw_ptr2 = new std::string("Alright");
    pointers::SharedPtr<std::string> second_ptr(raw_ptr2);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(second_ptr), 1);

    second_ptr = first_ptr;
    EXPECT_EQ(Testing<std::string>::NumberOfShared(second_ptr), 2);
  }
  {
    std::string* raw_ptr = new std::string("Good luck...");
    pointers::SharedPtr<std::string> first_ptr(raw_ptr);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(first_ptr), 1);

    std::string* raw_ptr2 = new std::string("Alright");
    pointers::SharedPtr<std::string> second_ptr(raw_ptr2);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(second_ptr), 1);

    second_ptr = std::move(first_ptr);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(second_ptr), 1);
    EXPECT_EQ(*(second_ptr.Get()), "Good luck...");
  }
  {
    std::string* raw_ptr = new std::string("Good luck...");
    pointers::SharedPtr<std::string> first_ptr(raw_ptr);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(first_ptr), 1);
    first_ptr = first_ptr;
    EXPECT_EQ(Testing<std::string>::NumberOfShared(first_ptr), 1);
    EXPECT_EQ(*(first_ptr.Get()), "Good luck...");
  }
  {
    pointers::SharedPtr<std::string> shared_ptr(nullptr);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(shared_ptr), -1);
    pointers::SharedPtr<std::string> shared_ptr2(shared_ptr);
    EXPECT_EQ(Testing<std::string>::NumberOfShared(shared_ptr2), -1);
  }
}

TEST(SharedPtr, Operators) {
  pointers::SharedPtr<int> shared_ptr1(new int(543));
  EXPECT_EQ(*shared_ptr1, 543);

  const pointers::SharedPtr<int> shared_ptr2(new int(9999));
  EXPECT_EQ(*shared_ptr2, 9999);

  struct TestStruct {
    int number;
    std::string str;
  };

  pointers::SharedPtr<TestStruct> shared_ptr3(new TestStruct{87, "hi"});
  EXPECT_EQ(shared_ptr3->number, 87);
  EXPECT_EQ(shared_ptr3->str, "hi");

  const pointers::SharedPtr<TestStruct>
      shared_ptr4(new TestStruct{93, "world"});
  EXPECT_EQ(shared_ptr4->number, 93);
  EXPECT_EQ(shared_ptr4->str, "world");
}

TEST(SharedPtr, EqualityAndInequality) {
  {
    pointers::SharedPtr<int> shared_ptr1(new int(432));
    const pointers::SharedPtr<int> shared_ptr2(shared_ptr1);
    EXPECT_TRUE(shared_ptr1 == shared_ptr2);
    EXPECT_FALSE(shared_ptr1 != shared_ptr2);
    pointers::SharedPtr<int> shared_ptr3(new int(432));
    EXPECT_FALSE(shared_ptr1 == shared_ptr3);
    EXPECT_TRUE(shared_ptr1 != shared_ptr3);
  }
  {
    int* raw_ptr1 = new int(67);
    const pointers::SharedPtr<int> shared_ptr(raw_ptr1);
    EXPECT_TRUE(shared_ptr == raw_ptr1);
    EXPECT_TRUE(raw_ptr1 == shared_ptr);
    EXPECT_FALSE(shared_ptr != raw_ptr1);
    EXPECT_FALSE(raw_ptr1 != shared_ptr);

    int* raw_ptr2 = new int(34);
    EXPECT_FALSE(shared_ptr == raw_ptr2);
    EXPECT_FALSE(raw_ptr2 == shared_ptr);
    EXPECT_TRUE(shared_ptr != raw_ptr2);
    EXPECT_TRUE(raw_ptr2 != shared_ptr);
  }
}
