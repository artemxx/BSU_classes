#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

// Бинарное поисковое дерево.
class BinarySearchTree {
 public:
  // Конструктор.
  // Инициализирует значения полей 'root_' и 'size_'.
  BinarySearchTree();

  // Деструктор.
  // Удаляет все элменты дерева путём удаления корневой вершины.
  ~BinarySearchTree();

  // Возвращает количество элементов в дереве.
  size_t Size() const;

  // Метод возвращает "true" если элемент со значением 'value' имеется в дереве
  // поиска и "false" в противном случае.
  bool Contains(int value) const;

  // Помещает значение 'value' в поисковое дерево.
  void Add(int value);

  // Удаляет элемент со значением 'value' из поискового дерева.
  // Если элементов с таким значением несколько, удаляется лишь одно из них.
  void Erase(int value);

  // Возвращает массив из элементов, содержащихся в дереве поиска,
  // расположенными в порядке неубывания.
  std::vector<int> ToSortedArray() const;

 protected:
  // Вершина в дереве поиска.
  struct Node {
    int value;
    Node* parent;
    Node* left;
    Node* right;

    // Конструктор, принимающий значение и указатель на предка.
    Node(int value, Node* parent);

    // Деструктор, удаляющий левое и правое поддеревья при удалении
    // данной вершины.
    ~Node();
  };

  Node* root_;
  size_t size_;

  // Вспомогательный метод, возвращающий указатель на одну из вершин со
  // значением 'value' (любую из них), либо nullptr при отсутствии таких вершин.
  Node* Find(int value) const;

  // Вспомогательные методы, которые позволяют получать указатели на самую
  // левую и самую правую вершины в поддереве, образуемом вершиной node,
  // соответствтенно.
  Node* GetLeftmostNode(Node* node) const;
  Node* GetRightmostNode(Node* node) const;

  // Связывает предка first_node и вершину second_node
  void ReplacePointers(Node* first_node, Node* second_node);

  // Вспомогательный метод, дописывающий все элементы в поддереве, образуемом
  // вершиной node, в конец вектора *res в порядке неубывания хранимых значений.
  void AppendToSortedArray(Node* node, std::vector<int>* res) const;
};

BinarySearchTree::Node::Node(int value, BinarySearchTree::Node* parent)
    : value(value),
      parent(parent),
      left(nullptr),
      right(nullptr) {}

BinarySearchTree::Node::~Node() {
  delete left;
  delete right;
}

BinarySearchTree::BinarySearchTree() : root_(nullptr), size_(0) {}

BinarySearchTree::~BinarySearchTree() {
  delete root_;
}

size_t BinarySearchTree::Size() const {
  return size_;
}

bool BinarySearchTree::Contains(int value) const {
  return Find(value);
}

void BinarySearchTree::Add(int value) {
  ++size_;
  if (root_ == nullptr) {
    root_ = new Node(value, nullptr);
    return;
  }
  Node* node = root_;
  while (true) {
    if (node->value > value) {
      if (node->left != nullptr) {
        node = node->left;
      } else {
        node->left = new Node(value, node);
        return;
      }
    } else {
      if (node->right != nullptr) {
        node = node->right;
      } else {
        node->right = new Node(value, node);
        return;
      }
    }
  }
}

void BinarySearchTree::Erase(int value) {
  Node* node = Find(value);
  if (node == nullptr) return;
  --size_;
  if (node->left == nullptr) {
    ReplacePointers(node, node->right);
  } else if (node->right == nullptr) {
    ReplacePointers(node, node->left);
  } else {
    Node* leftmost_node = GetLeftmostNode(node->right);
    if (leftmost_node->parent != node) {
      ReplacePointers(leftmost_node, leftmost_node->right);
      leftmost_node->right = node->right;
      leftmost_node->right->parent = leftmost_node;
    }
    ReplacePointers(node, leftmost_node);
    leftmost_node->left = node->left;
    leftmost_node->left->parent = leftmost_node;
  }
  node->left = nullptr;
  node->right = nullptr;
  delete node;
}

std::vector<int> BinarySearchTree::ToSortedArray() const {
  std::vector<int> res;
  AppendToSortedArray(root_, &res);
  return res;
}

BinarySearchTree::Node* BinarySearchTree::Find(int value) const {
  Node* node = root_;
  while (node != nullptr && node->value != value) {
    if (node->value > value) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return node;
}

BinarySearchTree::Node* BinarySearchTree::GetLeftmostNode(Node* node) const {
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

BinarySearchTree::Node* BinarySearchTree::GetRightmostNode(Node* node) const {
  while (node->right != nullptr) {
    node = node->right;
  }
  return node;
}

void BinarySearchTree::ReplacePointers(Node* first_node, Node* second_node) {
  if (first_node->parent == nullptr) {
    root_ = second_node;
  } else if (first_node->parent->left == first_node) {
    first_node->parent->left = second_node;
  } else {
    first_node->parent->right = second_node;
  }
  if (second_node != nullptr) {
    second_node->parent = first_node->parent;
  }
}

void BinarySearchTree::AppendToSortedArray(Node* node,
                                           std::vector<int>* res) const {
  if (node == nullptr) return;
  AppendToSortedArray(node->left, res);
  res->push_back(node->value);
  AppendToSortedArray(node->right, res);
}

#ifndef IGNORE_MAIN
int main() {
  const int kQueriesCount = 50'000;
  const std::vector<int>
      kElementsMaxValues({1, 2, 5, 13, 42, 1024, 1'000'000'000});

  std::mt19937_64 random_generator(2018);
  for (const int max_element_value : kElementsMaxValues) {
    std::vector<int> elements;
    BinarySearchTree tree;
    for (int i = 0; i < kQueriesCount; ++i) {
      int query_type = static_cast<int>(random_generator() % 3);
      switch (query_type) {
        case 0: {
          int value;
          if (elements.empty() || random_generator() % 100 < 75) {
            value = static_cast<int>(random_generator() % max_element_value);
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          elements.push_back(value);
          sort(elements.begin(), elements.end());  // Never do like this :)
          tree.Add(value);
          break;
        }
        case 1: {
          int value;
          if (elements.empty() || random_generator() % 100 < 40) {
            value = static_cast<int>(random_generator() % max_element_value);
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          bool result = tree.Contains(value);
          bool expected_result =
              std::find(elements.begin(), elements.end(), value)
                  != elements.end();
          assert(result == expected_result);
          break;
        }
        case 2: {
          int value;
          if (elements.empty() || random_generator() % 100 < 25) {
            value = static_cast<int>(random_generator() % max_element_value);
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          auto element_it = std::find(elements.begin(), elements.end(), value);
          if (element_it != elements.end()) {
            elements.erase(element_it);
          }
          tree.Erase(value);
          break;
        }
        default: {
          break;  // IMPOSSIBLE
        }
      }

      assert(elements.size() == tree.Size());
      assert(elements == tree.ToSortedArray());
    }

    std::cout << "Passed: max_element_value = " << max_element_value << std::endl;
  }

  return 0;
}
#endif
