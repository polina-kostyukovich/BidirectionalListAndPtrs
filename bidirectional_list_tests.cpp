#include <string>
#include "bidirectional_list.h"
#include "gtest/gtest.h"

TEST(BiDirectionalList, Clear) {
  containers::BiDirectionalList<int> list({1, 2, 3, 4, 5});
  EXPECT_EQ(list.Size(), 5);
  EXPECT_FALSE(list.IsEmpty());
  list.Clear();
  EXPECT_EQ(list.Size(), 0);
  EXPECT_TRUE(list.IsEmpty());

  // here assertion fails
  // EXPECT_EQ(list.Front(), nullptr);
  // EXPECT_EQ(list.Back(), nullptr);
}

TEST(BiDirectionalList, DefaultConstructor) {
  containers::BiDirectionalList<int> list;
  EXPECT_EQ(list.Size(), 0);
  EXPECT_TRUE(list.IsEmpty());
}

TEST(BiDirectionalList, ConstructorFromInitializerList) {
  {
    containers::BiDirectionalList<int> list({});
    EXPECT_EQ(list.Size(), 0);
    EXPECT_TRUE(list.IsEmpty());
  }
  {
    containers::BiDirectionalList<int> list({1});
    EXPECT_EQ(list.Size(), 1);
    EXPECT_FALSE(list.IsEmpty());
    EXPECT_EQ(list.Front()->value, 1);
    EXPECT_EQ(list.Back()->value, 1);
  }
  {
    containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
    EXPECT_EQ(list.Size(), 3);
    EXPECT_FALSE(list.IsEmpty());
    EXPECT_EQ(list.Front()->value, "a");
    EXPECT_EQ(list.Back()->value, "cd");
  }
}

TEST(BiDirectionalList, ConstructorFromOtherList) {
  {
    const containers::BiDirectionalList<int> list({});
    containers::BiDirectionalList<int> list_copy(list);
    EXPECT_EQ(list_copy.Size(), 0);
    EXPECT_TRUE(list_copy.IsEmpty());
  }
  {
    const containers::BiDirectionalList<int> list({1});
    {
      containers::BiDirectionalList<int> list_copy(list);
      EXPECT_EQ(list_copy.Size(), 1);
      EXPECT_FALSE(list_copy.IsEmpty());
      EXPECT_EQ(list_copy.Front()->value, 1);
      EXPECT_EQ(list_copy.Back()->value, 1);
    }
    EXPECT_EQ(list.Size(), 1);
    EXPECT_FALSE(list.IsEmpty());
    EXPECT_EQ(list.Front()->value, 1);
    EXPECT_EQ(list.Back()->value, 1);
  }
  {
    containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
    const containers::BiDirectionalList<std::string> list_copy(list);
    EXPECT_EQ(list_copy.Size(), 3);
    EXPECT_FALSE(list_copy.IsEmpty());
    EXPECT_EQ(list_copy.Front()->value, "a");
    EXPECT_EQ(list_copy.Back()->value, "cd");
  }
}

TEST(BiDirectionalList, ConstructorFromOtherListWithRValue) {
  {
    containers::BiDirectionalList<int> list({});
    containers::BiDirectionalList<int> list_moved(std::move(list));
    EXPECT_EQ(list_moved.Size(), 0);
    EXPECT_TRUE(list_moved.IsEmpty());
  }
  {
    containers::BiDirectionalList<int> list({1});
    containers::BiDirectionalList<int> list_moved(std::move(list));
    EXPECT_EQ(list_moved.Size(), 1);
    EXPECT_FALSE(list_moved.IsEmpty());
    EXPECT_EQ(list_moved.Front()->value, 1);
    EXPECT_EQ(list_moved.Back()->value, 1);
  }
  {
    containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
    const containers::BiDirectionalList<std::string>
        list_moved(std::move(list));
    EXPECT_EQ(list_moved.Size(), 3);
    EXPECT_FALSE(list_moved.IsEmpty());
    EXPECT_EQ(list_moved.Front()->value, "a");
    EXPECT_EQ(list_moved.Back()->value, "cd");
  }
}

TEST(BiDirectionalList, AssignmentOperator) {
  {
    const containers::BiDirectionalList<int> list({});
    containers::BiDirectionalList<int> assign_list({});
    assign_list = list;
    EXPECT_EQ(assign_list.Size(), 0);
    EXPECT_TRUE(assign_list.IsEmpty());
  }
  {
    containers::BiDirectionalList<int> list({1});
    containers::BiDirectionalList<int> assign_list;
    assign_list = list;
    EXPECT_EQ(assign_list.Size(), 1);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, 1);
    EXPECT_EQ(assign_list.Back()->value, 1);
  }
  {
    const containers::BiDirectionalList<int> list({});
    containers::BiDirectionalList<int> assign_list({3});
    assign_list = list;
    EXPECT_EQ(assign_list.Size(), 0);
    EXPECT_TRUE(assign_list.IsEmpty());
  }
  {
    containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
    containers::BiDirectionalList<std::string> assign_list({"f"});
    assign_list = list;
    EXPECT_EQ(assign_list.Size(), 3);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, "a");
    EXPECT_EQ(assign_list.Back()->value, "cd");
  }
  {
    const containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
    containers::BiDirectionalList<std::string> assign_list({"f", "dd", "n"});
    assign_list = list;
    EXPECT_EQ(assign_list.Size(), 3);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, "a");
    EXPECT_EQ(assign_list.Back()->value, "cd");
  }
  {
    containers::BiDirectionalList<std::string> assign_list({"f", "dd", "n"});
    {
      const containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
      assign_list = list;
    }
    EXPECT_EQ(assign_list.Size(), 3);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, "a");
    EXPECT_EQ(assign_list.Back()->value, "cd");
  }
  {
    containers::BiDirectionalList<std::string>
        assign_list({"t", "f", "d", "n"});
    {
      containers::BiDirectionalList<std::string> list({"a", "bk"});
      assign_list = list;
    }
    EXPECT_EQ(assign_list.Size(), 2);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, "a");
    EXPECT_EQ(assign_list.Back()->value, "bk");
  }
  {
    containers::BiDirectionalList<std::vector<int>>
        assign_list({{3, 8, 0, 33}, {4}, {}, {7, 3}});
    {
      containers::BiDirectionalList<std::vector<int>> list({{}, {9, 1, 4}});
      assign_list = list;
    }
    EXPECT_EQ(assign_list.Size(), 2);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, std::vector<int>({}));
    EXPECT_EQ(assign_list.Back()->value, std::vector<int>({9, 1, 4}));
  }
  {
    containers::BiDirectionalList<std::vector<int>>
        list({{3, 8, 0, 33}, {4}, {}, {7, 3}});
    list = list;
    EXPECT_EQ(list.Size(), 4);
    EXPECT_FALSE(list.IsEmpty());
    EXPECT_EQ(list.Front()->value, std::vector<int>({3, 8, 0, 33}));
    EXPECT_EQ(list.Back()->value, std::vector<int>({7, 3}));
  }
}

TEST(BiDirectionalList, AssignmentOperatorWithRValue) {
  {
    containers::BiDirectionalList<int> list({});
    containers::BiDirectionalList<int> assign_list({});
    assign_list = std::move(list);
    EXPECT_EQ(assign_list.Size(), 0);
    EXPECT_TRUE(assign_list.IsEmpty());
  }
  {
    containers::BiDirectionalList<int> list({1});
    containers::BiDirectionalList<int> assign_list;
    assign_list = std::move(list);
    EXPECT_EQ(assign_list.Size(), 1);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, 1);
    EXPECT_EQ(assign_list.Back()->value, 1);
  }
  {
    containers::BiDirectionalList<int> list({});
    containers::BiDirectionalList<int> assign_list({3});
    assign_list = std::move(list);
    EXPECT_EQ(assign_list.Size(), 0);
    EXPECT_TRUE(assign_list.IsEmpty());
  }
  {
    containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
    containers::BiDirectionalList<std::string> assign_list({"f"});
    assign_list = std::move(list);
    EXPECT_EQ(assign_list.Size(), 3);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, "a");
    EXPECT_EQ(assign_list.Back()->value, "cd");
  }
  {
    containers::BiDirectionalList<std::string> list({"a", "b", "cd"});
    containers::BiDirectionalList<std::string> assign_list({"f", "dd", "n"});
    assign_list = std::move(list);
    EXPECT_EQ(assign_list.Size(), 3);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, "a");
    EXPECT_EQ(assign_list.Back()->value, "cd");
  }
  {
    containers::BiDirectionalList<std::string>
        assign_list({"t", "f", "d", "n"});
    {
      containers::BiDirectionalList<std::string> list({"a", "bk"});
      assign_list = std::move(list);
    }
    EXPECT_EQ(assign_list.Size(), 2);
    EXPECT_FALSE(assign_list.IsEmpty());
    EXPECT_EQ(assign_list.Front()->value, "a");
    EXPECT_EQ(assign_list.Back()->value, "bk");
  }
  {
    containers::BiDirectionalList<std::vector<int>>
        list({{3, 8, 0, 33}, {4}, {}, {7, 3}});
    list = std::move(list);
    EXPECT_EQ(list.Size(), 4);
    EXPECT_FALSE(list.IsEmpty());
    EXPECT_EQ(list.Front()->value, std::vector<int>({3, 8, 0, 33}));
    EXPECT_EQ(list.Back()->value, std::vector<int>({7, 3}));
  }
}

TEST(BiDirectionalList, ToVector) {
  {
    containers::BiDirectionalList<int> list({1, 7, 33, 42});
    EXPECT_EQ(list.ToVector(), std::vector<int>({1, 7, 33, 42}));
  }
  {
    containers::BiDirectionalList<std::string> list({});
    EXPECT_EQ(list.ToVector(), std::vector<std::string>({}));
  }
  {
    const containers::BiDirectionalList<std::vector<int>>
        list({{}, {1, 4}, {42}});
    EXPECT_EQ(list.ToVector(),
              std::vector<std::vector<int>>({{}, {1, 4}, {42}}));
  }
}

TEST(BiDirectionalList, ConstFrontBack) {
  const containers::BiDirectionalList<std::string> list({"a", "nh"});
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.Front()->next_ptr, list.Back());
  EXPECT_EQ(list.Front()->value, "a");
  EXPECT_EQ(list.Back()->previous_ptr, list.Front());
  EXPECT_EQ(list.Back()->next_ptr, nullptr);
  EXPECT_EQ(list.Back()->value, "nh");
}

TEST(BiDirectionalList, PushLValue) {
  {
    const int value = 42;
    containers::BiDirectionalList<int> list;
    list.PushFront(value);
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, nullptr);
    EXPECT_EQ(list.Front()->value, 42);
    EXPECT_EQ(list.Back()->previous_ptr, nullptr);
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 42);
    EXPECT_EQ(list.ToVector(), std::vector<int>({42}));
  }
  {
    const int value = 42;
    containers::BiDirectionalList<int> list;
    list.PushFront(value);
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, nullptr);
    EXPECT_EQ(list.Front()->value, 42);
    EXPECT_EQ(list.Back()->previous_ptr, nullptr);
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 42);
    EXPECT_EQ(list.ToVector(), std::vector<int>({42}));
  }
  {
    const int value = 42;
    containers::BiDirectionalList<int> list({1});
    list.PushFront(value);
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, list.Back());
    EXPECT_EQ(list.Front()->value, 42);
    EXPECT_EQ(list.Back()->previous_ptr, list.Front());
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 1);
    EXPECT_EQ(list.ToVector(), std::vector<int>({42, 1}));
  }
  {
    const int value = 42;
    containers::BiDirectionalList<int> list({1});
    list.PushBack(value);
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, list.Back());
    EXPECT_EQ(list.Front()->value, 1);
    EXPECT_EQ(list.Back()->previous_ptr, list.Front());
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 42);
    EXPECT_EQ(list.ToVector(), std::vector<int>({1, 42}));
  }
  {
    const int value = 42;
    containers::BiDirectionalList<int> list({1, 2});
    list.PushFront(value);
    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, list.Back()->previous_ptr);
    EXPECT_EQ(list.Front()->value, 42);
    EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
    EXPECT_EQ(list.Front()->next_ptr->next_ptr, list.Back());
    EXPECT_EQ(list.Back()->previous_ptr, list.Front()->next_ptr);
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 2);
    EXPECT_EQ(list.ToVector(), std::vector<int>({42, 1, 2}));
  }
  {
    const int value = 42;
    containers::BiDirectionalList<int> list({1, 2});
    list.PushBack(value);
    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, list.Back()->previous_ptr);
    EXPECT_EQ(list.Front()->value, 1);
    EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
    EXPECT_EQ(list.Front()->next_ptr->next_ptr, list.Back());
    EXPECT_EQ(list.Back()->previous_ptr, list.Front()->next_ptr);
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 42);
    EXPECT_EQ(list.ToVector(), std::vector<int>({1, 2, 42}));
  }
}

TEST(BiDirectionalList, PushRValue) {
  {
    containers::BiDirectionalList<int> list;
    list.PushFront(7);
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, nullptr);
    EXPECT_EQ(list.Front()->value, 7);
    EXPECT_EQ(list.Back()->previous_ptr, nullptr);
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 7);
    EXPECT_EQ(list.ToVector(), std::vector<int>({7}));
  }
  {
    containers::BiDirectionalList<int> list;
    list.PushFront(7);
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, nullptr);
    EXPECT_EQ(list.Front()->value, 7);
    EXPECT_EQ(list.Back()->previous_ptr, nullptr);
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 7);
    EXPECT_EQ(list.ToVector(), std::vector<int>({7}));
  }
  {
    containers::BiDirectionalList<int> list({1});
    list.PushFront(42);
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, list.Back());
    EXPECT_EQ(list.Front()->value, 42);
    EXPECT_EQ(list.Back()->previous_ptr, list.Front());
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 1);
    EXPECT_EQ(list.ToVector(), std::vector<int>({42, 1}));
  }
  {
    containers::BiDirectionalList<int> list({1});
    list.PushBack(42);
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, list.Back());
    EXPECT_EQ(list.Front()->value, 1);
    EXPECT_EQ(list.Back()->previous_ptr, list.Front());
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 42);
    EXPECT_EQ(list.ToVector(), std::vector<int>({1, 42}));
  }
  {
  containers::BiDirectionalList<int> list({1, 2});
  list.PushFront(42);
  EXPECT_EQ(list.Size(), 3);
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.Front()->next_ptr, list.Back()->previous_ptr);
  EXPECT_EQ(list.Front()->value, 42);
  EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
  EXPECT_EQ(list.Front()->next_ptr->next_ptr, list.Back());
  EXPECT_EQ(list.Back()->previous_ptr, list.Front()->next_ptr);
  EXPECT_EQ(list.Back()->next_ptr, nullptr);
  EXPECT_EQ(list.Back()->value, 2);
  EXPECT_EQ(list.ToVector(), std::vector<int>({42, 1, 2}));
  }
  {
    containers::BiDirectionalList<int> list({1, 2});
    list.PushBack(42);
    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list.Front()->previous_ptr, nullptr);
    EXPECT_EQ(list.Front()->next_ptr, list.Back()->previous_ptr);
    EXPECT_EQ(list.Front()->value, 1);
    EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
    EXPECT_EQ(list.Front()->next_ptr->next_ptr, list.Back());
    EXPECT_EQ(list.Back()->previous_ptr, list.Front()->next_ptr);
    EXPECT_EQ(list.Back()->next_ptr, nullptr);
    EXPECT_EQ(list.Back()->value, 42);
    EXPECT_EQ(list.ToVector(), std::vector<int>({1, 2, 42}));
  }
}

TEST(BiDirectionalList, Pop) {
  containers::BiDirectionalList<std::string> list({"a", "b", "c", "d", "e"});
  list.PopFront();
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(list.Front()->value, "b");
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.Front()->next_ptr->value, "c");
  EXPECT_EQ(list.ToVector(), std::vector<std::string>({"b", "c", "d", "e"}));
  list.PopBack();
  EXPECT_EQ(list.Size(), 3);
  EXPECT_EQ(list.Back()->value, "d");
  EXPECT_EQ(list.Back()->next_ptr, nullptr);
  EXPECT_EQ(list.Back()->previous_ptr->value, "c");
  EXPECT_EQ(list.ToVector(), std::vector<std::string>({"b", "c", "d"}));
  list.PopBack();
  list.PopBack();
  EXPECT_EQ(list.Size(), 1);
  EXPECT_FALSE(list.IsEmpty());
  EXPECT_EQ(list.Front()->value, "b");
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.Front()->next_ptr, nullptr);
  list.PopFront();
  EXPECT_EQ(list.Size(), 0);
  EXPECT_TRUE(list.IsEmpty());

  list.PushBack("b");
  EXPECT_EQ(list.Size(), 1);
  EXPECT_FALSE(list.IsEmpty());
  EXPECT_EQ(list.Front()->value, "b");
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.Front()->next_ptr, nullptr);
  list.PopBack();
  EXPECT_EQ(list.Size(), 0);
  EXPECT_TRUE(list.IsEmpty());

  // containers::BiDirectionalList<std::string> list2;
  // list2.PopFront();
  // list2.PopBack();
  // in these cases assertion fails
}

TEST(BiDirectionalList, InsertBeforeLValue) {
  containers::BiDirectionalList<int> list({2, 3, 4});
  const int value1 = 0;
  list.InsertBefore(list.Front(), value1);
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(list.Front()->value, 0);
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.Front()->next_ptr->value, 2);
  EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
  EXPECT_EQ(list.ToVector(), std::vector<int>({0, 2, 3, 4}));

  const int value2 = 1;
  list.InsertBefore(list.Front()->next_ptr, value2);
  EXPECT_EQ(list.Size(), 5);
  EXPECT_EQ(list.Front()->next_ptr->value, 1);
  EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->value, 2);
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->previous_ptr,
            list.Front()->next_ptr);
  EXPECT_EQ(list.ToVector(), std::vector<int>({0, 1, 2, 3, 4}));
}

TEST(BiDirectionalList, InsertBeforeRValue) {
  containers::BiDirectionalList<int> list({2, 3, 4});
  list.InsertBefore(list.Front(), 0);
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(list.Front()->value, 0);
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.Front()->next_ptr->value, 2);
  EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
  EXPECT_EQ(list.ToVector(), std::vector<int>({0, 2, 3, 4}));

  list.InsertBefore(list.Front()->next_ptr, 1);
  EXPECT_EQ(list.Size(), 5);
  EXPECT_EQ(list.Front()->next_ptr->value, 1);
  EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->value, 2);
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->previous_ptr,
            list.Front()->next_ptr);
  EXPECT_EQ(list.ToVector(), std::vector<int>({0, 1, 2, 3, 4}));
}

TEST(BiDirectionalList, InsertAfterLValue) {
  containers::BiDirectionalList<int> list({2, 3, 4});
  const int value1 = 5;
  list.InsertAfter(list.Back(), value1);
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(list.Back()->value, 5);
  EXPECT_EQ(list.Back()->next_ptr, nullptr);
  EXPECT_EQ(list.Back()->previous_ptr->value, 4);
  EXPECT_EQ(list.Back()->previous_ptr->next_ptr, list.Back());
  EXPECT_EQ(list.ToVector(), std::vector<int>({2, 3, 4, 5}));

  const int value2 = 7;
  list.InsertAfter(list.Front(), value2);
  EXPECT_EQ(list.Size(), 5);
  EXPECT_EQ(list.Front()->next_ptr->value, 7);
  EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->value, 3);
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->previous_ptr,
            list.Front()->next_ptr);
  EXPECT_EQ(list.ToVector(), std::vector<int>({2, 7, 3, 4, 5}));
}

TEST(BiDirectionalList, InsertAfterRValue) {
  containers::BiDirectionalList<int> list({2, 3, 4});
  list.InsertAfter(list.Back(), 5);
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(list.Back()->value, 5);
  EXPECT_EQ(list.Back()->next_ptr, nullptr);
  EXPECT_EQ(list.Back()->previous_ptr->value, 4);
  EXPECT_EQ(list.Back()->previous_ptr->next_ptr, list.Back());
  EXPECT_EQ(list.ToVector(), std::vector<int>({2, 3, 4, 5}));

  list.InsertAfter(list.Front(), 7);
  EXPECT_EQ(list.Size(), 5);
  EXPECT_EQ(list.Front()->next_ptr->value, 7);
  EXPECT_EQ(list.Front()->next_ptr->previous_ptr, list.Front());
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->value, 3);
  EXPECT_EQ(list.Front()->next_ptr->next_ptr->previous_ptr,
            list.Front()->next_ptr);
  EXPECT_EQ(list.ToVector(), std::vector<int>({2, 7, 3, 4, 5}));
}

TEST(BiDirectionalList, OperatorOfIndex) {
  containers::BiDirectionalList<int> list({0, 1, 2, 3, 4, 5});
  EXPECT_EQ(list[0], list.Front());
  EXPECT_EQ(list[1], list.Front()->next_ptr);
  EXPECT_EQ(list[2], list.Front()->next_ptr->next_ptr);
  EXPECT_EQ(list[3], list.Back()->previous_ptr->previous_ptr);
  EXPECT_EQ(list[4], list.Back()->previous_ptr);
  EXPECT_EQ(list[5], list.Back());

  // list[-3];
  // list[9];
  // in these cases assertion fails
}

TEST(BiDirectionalList, OperatorOfIndexConst) {
  const containers::BiDirectionalList<int> list({0, 1, 2, 3, 4, 5});
  EXPECT_EQ(list[0], list.Front());
  EXPECT_EQ(list[1], list.Front()->next_ptr);
  EXPECT_EQ(list[2], list.Front()->next_ptr->next_ptr);
  EXPECT_EQ(list[3], list.Back()->previous_ptr->previous_ptr);
  EXPECT_EQ(list[4], list.Back()->previous_ptr);
  EXPECT_EQ(list[5], list.Back());
}

TEST(BiDirectionalList, Erase) {
  containers::BiDirectionalList<int> list({1, 2, 3, 4, 5, 6});
  list.Erase(list[0]);
  EXPECT_EQ(list.Size(), 5);
  EXPECT_EQ(list.Front()->value, 2);
  EXPECT_EQ(list.Front()->previous_ptr, nullptr);
  EXPECT_EQ(list.ToVector(), std::vector<int>({2, 3, 4, 5, 6}));

  list.Erase(list[4]);
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(list.Back()->value, 5);
  EXPECT_EQ(list.Back()->next_ptr, nullptr);
  EXPECT_EQ(list.ToVector(), std::vector<int>({2, 3, 4, 5}));

  list.Erase(list[2]);
  EXPECT_EQ(list.Size(), 3);
  EXPECT_EQ(list.Back()->value, 5);
  EXPECT_EQ(list.Back()->previous_ptr->value, 3);
  EXPECT_EQ(list.Back()->previous_ptr->next_ptr, list.Back());
  EXPECT_EQ(list.Front()->next_ptr->next_ptr, list.Back());
  EXPECT_EQ(list.ToVector(), std::vector<int>({2, 3, 5}));
}

TEST(BiDirectionalList, Find) {
  {
    const containers::BiDirectionalList<std::string> list({});
    EXPECT_EQ(list.Find("abc"), -1);
  }
  {
    containers::BiDirectionalList<int> list({3});
    EXPECT_EQ(list.Find(2), -1);
    EXPECT_EQ(list.Find(3), 0);
    list.PushFront(33);
    EXPECT_EQ(list.Find(33), 0);
    EXPECT_EQ(list.Find(3), 1);
    EXPECT_EQ(list.Find(42), -1);
  }
  {
    const containers::BiDirectionalList<int> list({1, 2, 3, 4, 5, 6, 7, 3});
    EXPECT_EQ(list.Find(1), 0);
    EXPECT_EQ(list.Find(2), 1);
    EXPECT_EQ(list.Find(3), 2);
    EXPECT_EQ(list.Find(4), 3);
    EXPECT_EQ(list.Find(5), 4);
    EXPECT_EQ(list.Find(6), 5);
    EXPECT_EQ(list.Find(7), 6);
    EXPECT_EQ(list.Find(12), -1);
  }
}

TEST(BiDirectionalList, FindAll) {
  {
    const containers::BiDirectionalList<std::string> list({});
    EXPECT_EQ(list.FindAll("s"), std::vector<int>({}));
  }
  {
    const containers::BiDirectionalList<int> list({1, 2, 5, 2, 3, 2, 1, 3, 2});
    EXPECT_EQ(list.FindAll(1), std::vector<int>({0, 6}));
    EXPECT_EQ(list.FindAll(2), std::vector<int>({1, 3, 5, 8}));
    EXPECT_EQ(list.FindAll(5), std::vector<int>({2}));
    EXPECT_EQ(list.FindAll(3), std::vector<int>({4, 7}));
    EXPECT_EQ(list.FindAll(4), std::vector<int>({}));
    EXPECT_EQ(list.FindAll(-6), std::vector<int>({}));
  }
}

TEST(BiDirectionalList, EqualityAndInequality) {
  {
    const containers::BiDirectionalList<int> list1({});
    const containers::BiDirectionalList<int> list2({});
    EXPECT_TRUE(list1 == list2);
    EXPECT_FALSE(list1 != list2);
  }
  {
    const containers::BiDirectionalList<int> list1({});
    const containers::BiDirectionalList<int> list2({43});
    EXPECT_FALSE(list1 == list2);
    EXPECT_TRUE(list1 != list2);
  }
  {
    containers::BiDirectionalList<int> list1({1, 2, 3});
    containers::BiDirectionalList<int> list2({1, 2});
    EXPECT_FALSE(list1 == list2);
    EXPECT_TRUE(list1 != list2);
    list2.PushBack(3);
    EXPECT_TRUE(list1 == list2);
    EXPECT_FALSE(list1 != list2);
    list1.PushBack(4);
    list2.PushBack(5);
    EXPECT_FALSE(list1 == list2);
    EXPECT_TRUE(list1 != list2);
  }
}

TEST(BiDirectionalList, TrickyTest) {
  containers::BiDirectionalList<std::unique_ptr<int>> list;
  list.PushBack(std::make_unique<int>(1));
  list.PushBack(std::make_unique<int>(2));
  list.PushBack(std::make_unique<int>(3));
  EXPECT_EQ(list.Size(), 3);
  EXPECT_FALSE(list.IsEmpty());
  EXPECT_EQ(*(list.Front()->value), 1);
  EXPECT_EQ(*(list.Front()->next_ptr->value), 2);
  EXPECT_EQ(*(list.Back()->value), 3);

  list.PushBack(std::make_unique<int>(4));
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(*(list.Front()->value), 1);
  EXPECT_EQ(*(list.Front()->next_ptr->value), 2);
  EXPECT_EQ(*(list.Back()->previous_ptr->value), 3);
  EXPECT_EQ(*(list.Back()->value), 4);

  list.PushFront(std::make_unique<int>(0));
  EXPECT_EQ(list.Size(), 5);
  EXPECT_EQ(*(list.Front()->value), 0);
  EXPECT_EQ(*(list.Front()->next_ptr->value), 1);
  EXPECT_EQ(*(list.Front()->next_ptr->next_ptr->value), 2);
  EXPECT_EQ(*(list.Back()->previous_ptr->value), 3);
  EXPECT_EQ(*(list.Back()->value), 4);

  list.PopFront();
  EXPECT_EQ(list.Size(), 4);
  EXPECT_EQ(*(list.Front()->value), 1);
  EXPECT_EQ(*(list.Front()->next_ptr->value), 2);
  EXPECT_EQ(*(list.Back()->previous_ptr->value), 3);
  EXPECT_EQ(*(list.Back()->value), 4);

  list.PopBack();
  EXPECT_EQ(list.Size(), 3);
  EXPECT_FALSE(list.IsEmpty());
  EXPECT_EQ(*(list.Front()->value), 1);
  EXPECT_EQ(*(list.Front()->next_ptr->value), 2);
  EXPECT_EQ(*(list.Back()->value), 3);

  EXPECT_EQ(*(list[0]->value), 1);
  EXPECT_EQ(*(list[1]->value), 2);
  EXPECT_EQ(*(list[2]->value), 3);

  list.InsertBefore(list[1], std::make_unique<int>(7));  // 1, 7, 2, 3
  EXPECT_EQ(*(list[0]->value), 1);
  EXPECT_EQ(*(list[1]->value), 7);
  EXPECT_EQ(*(list[2]->value), 2);
  EXPECT_EQ(*(list[3]->value), 3);

  list.Erase(list[1]);
  EXPECT_EQ(*(list[0]->value), 1);
  EXPECT_EQ(*(list[1]->value), 2);
  EXPECT_EQ(*(list[2]->value), 3);

  list.InsertAfter(list[1], std::make_unique<int>(7));  // 1, 2, 7, 3
  EXPECT_EQ(*(list[0]->value), 1);
  EXPECT_EQ(*(list[1]->value), 2);
  EXPECT_EQ(*(list[2]->value), 7);
  EXPECT_EQ(*(list[3]->value), 3);

  list.Erase(list[2]);
  EXPECT_EQ(*(list[0]->value), 1);
  EXPECT_EQ(*(list[1]->value), 2);
  EXPECT_EQ(*(list[2]->value), 3);

  containers::BiDirectionalList<std::unique_ptr<int>> list2;
  list2.PushBack(std::make_unique<int>(1));
  list2.PushBack(std::make_unique<int>(2));
  list2.PushBack(std::make_unique<int>(3));
  EXPECT_FALSE(list == list2);
  EXPECT_TRUE(list != list2);

  containers::BiDirectionalList<std::unique_ptr<int>> list3;
  list3.PushBack(nullptr);
  list2 = std::move(list3);
  EXPECT_EQ(list2.Size(), 1);
  EXPECT_EQ(list2.Front()->value, nullptr);
  EXPECT_EQ(list2.Back()->value, nullptr);
  containers::BiDirectionalList<std::unique_ptr<int>> list4(std::move(list2));
  EXPECT_EQ(list4.Size(), 1);
  EXPECT_EQ(list4.Front()->value, nullptr);
  EXPECT_EQ(list4.Back()->value, nullptr);
  containers::BiDirectionalList<std::unique_ptr<int>> list5;
  list5.PushBack(nullptr);
  EXPECT_TRUE(list4 == list5);
  EXPECT_FALSE(list4 != list5);
}
