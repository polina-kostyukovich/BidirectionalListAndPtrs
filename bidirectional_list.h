#ifndef BIDIRECTIONAL_LIST_H_
#define BIDIRECTIONAL_LIST_H_

#include <cassert>
#include <initializer_list>
#include <vector>

namespace containers {

template<typename T>
class BiDirectionalList {
 public:
  struct Node {
    T value;
    Node* previous_ptr;
    Node* next_ptr;

   private:
    explicit Node(const T& value);
    explicit Node(T&& val);

    friend class BiDirectionalList;
  };

 public:
  BiDirectionalList();
  BiDirectionalList(std::initializer_list<T>);
  BiDirectionalList(const BiDirectionalList&);
  BiDirectionalList& operator=(const BiDirectionalList&);
  BiDirectionalList(BiDirectionalList&&);
  BiDirectionalList& operator=(BiDirectionalList&&);

  ~BiDirectionalList();

  int Size() const;
  bool IsEmpty() const;
  std::vector<T> ToVector() const;
  Node* Front();
  Node* Back();
  const Node* Front() const;
  const Node* Back() const;

  void PushFront(const T& value);
  void PushBack(const T& value);
  void PushFront(T&& value);
  void PushBack(T&& value);
  void PopFront();
  void PopBack();

  void InsertBefore(Node* element, const T& value);
  void InsertAfter(Node* element, const T& value);
  void InsertBefore(Node* element, T&& value);
  void InsertAfter(Node* element, T&& value);

  void Erase(Node* element);

  int Find(const T& value) const;
  std::vector<int> FindAll(const T& value) const;

  Node* operator[](int index);
  const Node* operator[](int index) const;
  bool operator==(const BiDirectionalList& other) const;
  bool operator!=(const BiDirectionalList& other) const;

  // helpful methods
  void Clear();

 private:
  void MakeNewListFromOther(const BiDirectionalList&);

 private:
  Node* first_;
  Node* last_;
  int size_;
};

}  // namespace containers

template<typename T>
containers::BiDirectionalList<T>::Node::Node(const T& value) :
    value(value), previous_ptr(nullptr), next_ptr(nullptr) {}

template<typename T>
containers::BiDirectionalList<T>::Node::Node(T&& val)
    : value(std::move(val)), previous_ptr(nullptr), next_ptr(nullptr) {}

template<typename T>
containers::BiDirectionalList<T>::BiDirectionalList()
    : first_(nullptr), last_(nullptr), size_(0) {}

template<typename T>
containers::BiDirectionalList<T>::BiDirectionalList(
    std::initializer_list<T> list) : size_(list.size()) {
  if (list.size() == 0) {
    first_ = nullptr;
    last_ = nullptr;
    return;
  }
  auto current_iterator = list.begin();
  first_ = new Node(*current_iterator);
  Node* previous_node = first_;
  while (++current_iterator != list.end()) {
    previous_node->next_ptr = new Node(*current_iterator);
    previous_node->next_ptr->previous_ptr = previous_node;
    previous_node = previous_node->next_ptr;
  }
  last_ = previous_node;
}

template<typename T>
containers::BiDirectionalList<T>::BiDirectionalList(
    const BiDirectionalList& other) {
  MakeNewListFromOther(other);
}

template<typename T>
containers::BiDirectionalList<T>& containers::BiDirectionalList<T>::operator=(
    const BiDirectionalList& other) {
  if (this == &other) {
    return *this;
  }
  if (other.IsEmpty()) {
    Clear();
    return *this;
  }
  if (IsEmpty()) {
    MakeNewListFromOther(other);
    return *this;
  }
  Node* current_from_this = first_;
  Node* current_from_other = other.first_;
  for (int i = 0; i < std::min(size_, other.size_); ++i) {
    current_from_this->value = current_from_other->value;
    current_from_this = current_from_this->next_ptr;
    current_from_other = current_from_other->next_ptr;
  }
  if (size_ <= other.size_) {
    current_from_this = last_;
    for (int i = size_; i < other.size_; ++i) {
      current_from_this->next_ptr = new Node(current_from_other->value);
      current_from_this->next_ptr->previous_ptr = current_from_this;
      current_from_this = current_from_this->next_ptr;
      current_from_other = current_from_other->next_ptr;
    }
    last_ = current_from_this;
  } else {
    last_ = current_from_this->previous_ptr;
    for (int i = other.size_; i < size_; ++i) {
      Node* next_to_delete = current_from_this->next_ptr;
      delete current_from_this;
      current_from_this = next_to_delete;
    }
    last_->next_ptr = nullptr;
  }
  size_ = other.size_;
  return *this;
}

template <typename T>
containers::BiDirectionalList<T>::BiDirectionalList(BiDirectionalList&& other) {
  size_ = other.size_;
  first_ = other.first_;
  last_ = other.last_;
  other.first_ = nullptr;
  other.last_ = nullptr;
  other.size_ = 0;
}

template <typename T>
containers::BiDirectionalList<T>& containers::BiDirectionalList<T>::operator=(
    BiDirectionalList&& other) {
  if (this == &other) {
    return *this;
  }
  Clear();
  size_ = other.size_;
  first_ = other.first_;
  last_ = other.last_;
  other.first_ = nullptr;
  other.last_ = nullptr;
  other.size_ = 0;
  return *this;
}

template <typename T>
containers::BiDirectionalList<T>::~BiDirectionalList() {
  Clear();
}

template<typename T>
int containers::BiDirectionalList<T>::Size() const {
  return size_;
}

template<typename T>
bool containers::BiDirectionalList<T>::IsEmpty() const {
  return size_ == 0;
}

template<typename T>
std::vector<T> containers::BiDirectionalList<T>::ToVector() const {
  std::vector<T> result;
  result.reserve(size_);
  Node* current_element = first_;
  for (int i = 0; i < size_; ++i) {
    result.push_back(current_element->value);
    current_element = current_element->next_ptr;
  }
  return result;
}

template<typename T>
typename containers::BiDirectionalList<T>::Node*
    containers::BiDirectionalList<T>::Front() {
  assert(size_ > 0);
  return first_;
}

template<typename T>
typename containers::BiDirectionalList<T>::Node*
    containers::BiDirectionalList<T>::Back() {
  assert(size_ > 0);
  return last_;
}

template<typename T>
const typename containers::BiDirectionalList<T>::Node*
    containers::BiDirectionalList<T>::Front() const {
  assert(size_ > 0);
  return first_;
}

template<typename T>
const typename containers::BiDirectionalList<T>::Node*
    containers::BiDirectionalList<T>::Back() const {
  assert(size_ > 0);
  return last_;
}

template<typename T>
void containers::BiDirectionalList<T>::PushFront(const T& value) {
  if (IsEmpty()) {
    ++size_;
    first_ = new Node(value);
    last_ = first_;
    return;
  }
  Node* temporary_ptr = first_;
  first_ = new Node(value);
  first_->next_ptr = temporary_ptr;
  temporary_ptr->previous_ptr = first_;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::PushBack(const T& value) {
  if (IsEmpty()) {
    ++size_;
    first_ = new Node(value);
    last_ = first_;
    return;
  }
  Node* temporary_ptr = last_;
  last_ = new Node(value);
  last_->previous_ptr = temporary_ptr;
  temporary_ptr->next_ptr = last_;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::PushFront(T&& value) {
  if (IsEmpty()) {
    ++size_;
    first_ = new Node(std::move(value));
    last_ = first_;
    return;
  }
  Node* temporary_ptr = first_;
  first_ = new Node(std::move(value));
  first_->next_ptr = temporary_ptr;
  temporary_ptr->previous_ptr = first_;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::PushBack(T&& value) {
  if (IsEmpty()) {
    ++size_;
    first_ = new Node(std::move(value));
    last_ = first_;
    return;
  }
  Node* temporary_ptr = last_;
  last_ = new Node(std::move(value));
  last_->previous_ptr = temporary_ptr;
  temporary_ptr->next_ptr = last_;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::PopFront() {
  assert(size_ > 0);
  if (size_ == 1) {
    delete first_;
    first_ = nullptr;
    last_ = nullptr;
    size_ = 0;
    return;
  }
  first_ = first_->next_ptr;
  delete first_->previous_ptr;
  first_->previous_ptr = nullptr;
  --size_;
}

template<typename T>
void containers::BiDirectionalList<T>::PopBack() {
  assert(size_ > 0);
  if (size_ == 1) {
    delete first_;
    first_ = nullptr;
    last_ = nullptr;
    size_ = 0;
    return;
  }
  last_ = last_->previous_ptr;
  delete last_->next_ptr;
  last_->next_ptr = nullptr;
  --size_;
}

template<typename T>
void containers::BiDirectionalList<T>::InsertBefore(
    BiDirectionalList::Node* element, const T& value) {
  if (element == first_) {
    PushFront(value);
    return;
  }
  Node* new_element = new Node(value);
  new_element->next_ptr = element;
  new_element->previous_ptr = element->previous_ptr;
  new_element->previous_ptr->next_ptr = new_element;
  element->previous_ptr = new_element;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::InsertAfter(
    BiDirectionalList::Node* element, const T& value) {
  if (element == last_) {
    PushBack(value);
    return;
  }
  Node* new_element = new Node(value);
  new_element->previous_ptr = element;
  new_element->next_ptr = element->next_ptr;
  element->next_ptr->previous_ptr = new_element;
  element->next_ptr = new_element;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::InsertBefore(
    BiDirectionalList::Node* element, T&& value) {
  if (element == first_) {
    PushFront(std::move(value));
    return;
  }
  Node* new_element = new Node(std::move(value));
  new_element->next_ptr = element;
  new_element->previous_ptr = element->previous_ptr;
  new_element->previous_ptr->next_ptr = new_element;
  element->previous_ptr = new_element;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::InsertAfter(
    BiDirectionalList::Node* element, T&& value) {
  if (element == last_) {
    PushBack(std::move(value));
    return;
  }
  Node* new_element = new Node(std::move(value));
  new_element->previous_ptr = element;
  new_element->next_ptr = element->next_ptr;
  element->next_ptr->previous_ptr = new_element;
  element->next_ptr = new_element;
  ++size_;
}

template<typename T>
void containers::BiDirectionalList<T>::Erase(BiDirectionalList::Node* element) {
  if (element == first_) {
    PopFront();
    return;
  }
  if (element == last_) {
    PopBack();
    return;
  }
  element->previous_ptr->next_ptr = element->next_ptr;
  element->next_ptr->previous_ptr = element->previous_ptr;
  --size_;
  delete element;
}

template<typename T>
int containers::BiDirectionalList<T>::Find(const T& value) const {
  Node* current_element = first_;
  for (int i = 0; i < size_; ++i) {
    if (current_element->value == value) {
      return i;
    }
    current_element = current_element->next_ptr;
  }
  return -1;
}

template<typename T>
std::vector<int> containers::BiDirectionalList<T>::FindAll(
    const T& value) const {
  std::vector<int> result;
  Node* current_element = first_;
  for (int i = 0; i < size_; ++i) {
    if (current_element->value == value) {
      result.push_back(i);
    }
    current_element = current_element->next_ptr;
  }
  return result;
}

template<typename T>
typename containers::BiDirectionalList<T>::Node*
    containers::BiDirectionalList<T>::operator[](int index) {
  assert(index >= 0 && index < size_);
  if (index <= size_ / 2) {
    Node* current_element = first_;
    for (int i = 1; i <= index; ++i) {
      current_element = current_element->next_ptr;
    }
    return current_element;
  }
  Node* current_element = last_;
  for (int i = size_ - 2; i >= index; --i) {
    current_element = current_element->previous_ptr;
  }
  return current_element;
}

template<typename T>
const typename containers::BiDirectionalList<T>::Node*
    containers::BiDirectionalList<T>::operator[](int index) const {
  assert(index >= 0 && index < size_);
  if (index <= size_ / 2) {
    Node* current_element = first_;
    for (int i = 1; i <= index; ++i) {
      current_element = current_element->next_ptr;
    }
    return current_element;
  }
  Node* current_element = last_;
  for (int i = size_ - 2; i >= index; --i) {
    current_element = current_element->previous_ptr;
  }
  return current_element;
}

template<typename T>
bool containers::BiDirectionalList<T>::operator==(
    const BiDirectionalList& other) const {
  if (size_ != other.size_) {
    return false;
  }
  Node* current_from_this = first_;
  Node* current_from_other = other.first_;
  for (int i = 0; i < size_; ++i) {
    if (current_from_this->value != current_from_other->value) {
      return false;
    }
    current_from_this = current_from_this->next_ptr;
    current_from_other = current_from_other->next_ptr;
  }
  return true;
}

template<typename T>
bool containers::BiDirectionalList<T>::operator!=(
    const BiDirectionalList& other) const {
  return !(*this == other);
}

template <typename T>
void containers::BiDirectionalList<T>::Clear() {
  if (IsEmpty()) return;
  Node* current_to_delete = first_;
  Node* next_to_delete = first_->next_ptr;
  for (int i = 0; i < size_ - 1; ++i) {
    delete current_to_delete;
    current_to_delete = next_to_delete;
    next_to_delete = current_to_delete->next_ptr;
  }
  delete current_to_delete;
  first_ = nullptr;
  last_ = nullptr;
  size_ = 0;
}

template <typename T>
void containers::BiDirectionalList<T>::MakeNewListFromOther(
    const BiDirectionalList& other) {
  size_ = other.size_;
  if (other.size_ == 0) {
    first_ = nullptr;
    last_ = nullptr;
    return;
  }
  first_ = new Node(other.first_->value);
  Node* previous_node = first_;
  Node* current_element_from_other = other.first_->next_ptr;
  for (int i = 1; i < size_; ++i) {
    previous_node->next_ptr = new Node(current_element_from_other->value);
    previous_node->next_ptr->previous_ptr = previous_node;
    previous_node = previous_node->next_ptr;
    current_element_from_other = current_element_from_other->next_ptr;
  }
  last_ = previous_node;
}

#endif  // BIDIRECTIONAL_LIST_H_
