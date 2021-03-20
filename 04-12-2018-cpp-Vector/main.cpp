#include <cassert>
#include <iostream> // danil loh
#include <string>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
class VectorInternalsAccessor;

// Контейнер Vector (динамический массив)
template<typename T>
class Vector {
 public:
  // Конструктор по умолчанию, выполняющий резервацию в 'data_'
  // памяти для хранения одного элемента.
  Vector();

  // Деструктор, выполняющий очистку занимаемой элементами
  // вектора памяти.
  ~Vector();

  // Возможность копирования объектов типа Vector<T>
  // через конструктор копирования или оператор копирующего присваивания.
  Vector(const Vector& other);
  Vector& operator=(const Vector& other);

  // Возможность копирования объектов типа Vector<T>
  // через конструктор перемещения или оператор перемещающего присваивания.
  Vector(Vector&& other) noexcept;
  Vector& operator=(Vector&& other) noexcept;

  // Возвращает количество элементов в векторе.
  size_t Size() const;

  // Проверяет вектор на пустоту.
  bool IsEmpty() const;

  // Метод, добавлялющий элемент в конец вектора.
  void PushBack(const T& value);

  // Метод, удаляющий элемент с конца вектора.
  void PopBack();

  // Метод, добавлялющий элемент в начало вектора.
  void PushFront(const T& value);

  // Метод, удаляющий элемент с начала вектора.
  void PopFront();

  // оператор [], возращающий (неизменяемую) ссылку
  // на объект по переданному индексу.
  T& operator[](size_t ind);
  const T& operator[](size_t ind) const;

  // Метод, добавлялющий элемент в конец вектора
  // аналогично соответствующему методу в std::vector
  template<typename... Args>
  void EmplaceBack(Args&& ... args);

  // Метод, добавлялющий элемент в начало вектора
  // аналогично соответствующему методу в std::vector
  template<typename... Args>
  void EmplaceFront(Args&& ... args);

  // Метод, возвращающий позицию в векторе
  // либо "-1", если элемент в векторе отстутствует.
  int Find(const T& value) const;

 protected:
  friend class VectorInternalsAccessor<T>;  // DO_NOT_CHANGE

  // Количество элементов, содержащихся в массиве
  size_t size_;

  // Размер массива data_
  size_t allocated_size_;

  //указатель на массив с элементами.
  T* data_;

  // Метод, изменяющий размер хранимого в data_ массива.
  void Relocate(size_t new_size);
};

template<typename T>
class VectorInternalsAccessor {
 public:
  VectorInternalsAccessor() = delete;
  static size_t Size(const Vector<T>& vector) {
    return vector.size_;
  }
  static T* AllocData(const Vector<T>& vector) {
    return vector.data_;
  }
  static size_t AllocSize(const Vector<T>& vector) {
    return vector.allocated_size_;
  }
};

template<typename T>
Vector<T>::Vector() :
    size_(0),
    allocated_size_(1),
    data_(new T[1]) {}

template<typename T>
Vector<T>::~Vector() {
  delete[] data_;
}

template<typename T>
Vector<T>::Vector(const Vector& other)
    : size_(other.size_),
      allocated_size_(other.allocated_size_),
      data_(new T[allocated_size_]) {
  for (size_t i = 0; i < size_; ++i) {
    data_[i] = other.data_[i];
  }
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (allocated_size_ != other.allocated_size_) {
    allocated_size_ = other.allocated_size_;
    delete[] data_;
    data_ = new T[allocated_size_];
  }
  size_ = other.size_;
  for (size_t i = 0; i < size_; ++i) {
    data_[i] = other.data_[i];
  }
  return *this;
}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : size_(other.size_),
      allocated_size_(other.allocated_size_),
      data_(other.data_) {
  other.size_ = 0;
  other.allocated_size_ = 1;
  other.data_ = new T[1];
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  delete[] data_;
  size_ = other.size_;
  allocated_size_ = other.allocated_size_;
  data_ = other.data_;

  other.size_ = 0;
  other.allocated_size_ = 1;
  other.data_ = new T[1];
  return *this;
}

template<typename T>
size_t Vector<T>::Size() const {
  return size_;
}

template<typename T>
bool Vector<T>::IsEmpty() const {
  return size_ == 0;
}

template<typename T>
void Vector<T>::PushBack(const T& value) {
  if (size_ == allocated_size_) {
    Relocate(size_ * 2);
  }
  data_[size_++] = value;
}

template<typename T>
void Vector<T>::PopBack() {
  assert(size_ != 0);
  --size_;
  if (size_ < allocated_size_ / 4) {
    Relocate(allocated_size_ / 2);
  }
}

template<typename T>
void Vector<T>::PushFront(const T& value) {
  if (size_ == allocated_size_) {
    Relocate(size_ * 2);
  }
  ++size_;
  for (size_t i = size_; i > 1; --i) {
    data_[i - 1] = data_[i - 2];
  }
  data_[0] = value;
}

template<typename T>
void Vector<T>::PopFront() {
  assert(size_ != 0);
  for (size_t i = 0; i < size_ - 1; ++i) {
    data_[i] = data_[i + 1];
  }
  --size_;
  if (size_ < allocated_size_ / 4) {
    Relocate(allocated_size_ / 2);
  }
}

template<typename T>
T const& Vector<T>::operator[](size_t ind) const {
  assert(ind < size_);
  return data_[ind];
}

template<typename T>
T& Vector<T>::operator[](size_t ind) {
  assert(ind < size_);
  return data_[ind];
}

template<typename T>
template<typename... Args>
void Vector<T>::EmplaceBack(Args&& ... args) {
  if (size_ == allocated_size_) {
    Relocate(size_ * 2);
  }
  data_[size_++] = T(std::forward<Args>(args)...);
}

template<typename T>
template<typename... Args>
void Vector<T>::EmplaceFront(Args&& ... args) {
  if (size_ == allocated_size_) {
    Relocate(size_ * 2);
  }
  ++size_;
  for (size_t i = size_; i > 1; --i) {
    data_[i - 1] = data_[i - 2];
  }
  data_[0] = T(std::forward<Args>(args)...);
}

template<typename T>
int Vector<T>::Find(const T& value) const {
  for (size_t i = 0; i < size_; ++i) {
    if (data_[i] == value) {
      return i;
    }
  }
  return -1;
}

template<typename T>
void Vector<T>::Relocate(size_t new_size) {
  allocated_size_ = new_size;
  T* tmp = data_;
  data_ = new T[allocated_size_];
  for (size_t i = 0; i < size_; ++i) {
    data_[i] = tmp[i];
  }
  delete[] tmp;
}

#ifndef IGNORE_MAIN
int main() {
#ifndef SKIP_BASIC
  {
    Vector<int> v;
    const Vector<int>& const_v = v;

    assert(const_v.Size() == 0 && const_v.IsEmpty());

    v.PushBack(42);
    assert(const_v.Size() == 1 && !const_v.IsEmpty());
    assert(v[0] == 42);
    assert(const_v[0] == 42);

    v[0] *= 10101;

    // Строчка ниже должна вызывать ошибку компиляции.
    // const_v[0] *= 10101;

    assert(v[0] == 424242);
    assert(const_v[0] == 424242);

    v.PopBack();
    assert(const_v.Size() == 0 && const_v.IsEmpty());
  }
  std::cout << "[PASS] Basic" << std::endl;
#else
  std::cout << "[SKIPPED] Basic" << std::endl;
#endif  // SKIP_BASIC

#ifndef SKIP_RELOC
  {
    Vector<int> v;
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 1);
    for (int i = 0; i < 1025; ++i) {
      v.PushBack(i);
    }
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 2048);
    for (int i = 0; i < 1025; ++i) {
      assert(v[i] == i);
    }
  }
  std::cout << "[PASS] Relocation" << std::endl;
#else
  std::cout << "[SKIPPED] Relocation" << std::endl;
#endif  // SKIP_RELOC

#ifndef SKIP_XFRONT
  {
    Vector<int> v;

    v.PushBack(42);
    assert(v.Size() == 1 && !v.IsEmpty());
    assert(v[0] == 42);

    v.PushFront(1);
    assert(v.Size() == 2 && !v.IsEmpty());
    assert(v[0] == 1);
    assert(v[1] == 42);

    v.PopFront();
    assert(v.Size() == 1 && !v.IsEmpty());
    assert(v[0] == 42);

    v.PopBack();
    assert(v.Size() == 0 && v.IsEmpty());

    v.PushFront(777);
    assert(v.Size() == 1 && !v.IsEmpty());
    assert(v[0] == 777);

    v.PopFront();
    assert(v.Size() == 0 && v.IsEmpty());
  }
  std::cout << "[PASS] XFront" << std::endl;
#else
  std::cout << "[SKIPPED] XFront" << std::endl;
#endif  // SKIP_XFRONT

#ifndef SKIP_FIND
  {
    Vector<int> v;
    v.PushBack(0);
    v.PushBack(1);
    v.PushBack(2);
    v.PushBack(1);
    const auto& const_v = v;

    assert(const_v.Find(0) == 0);
    assert(const_v.Find(1) == 1);
    assert(const_v.Find(2) == 2);
    assert(const_v.Find(3) == -1);
  }
  std::cout << "[PASS] Find" << std::endl;
#else
  std::cout << "[SKIPPED] Find" << std::endl;
#endif  // SKIP_FIND

#ifndef SKIP_EMPLACE
  {
    struct MyStruct {
      int i_field;
      std::string s_field;
      MyStruct() : i_field(0), s_field("<EMPTY>") {}
      MyStruct(int i_field, std::string s_field)
          : i_field(i_field), s_field(std::move(s_field)) {}
    };
    Vector<MyStruct> v;

    v.EmplaceBack(MyStruct());
    v.EmplaceBack();
    v.EmplaceBack(42, "4242");
    v.EmplaceFront(MyStruct());
    v.EmplaceFront();
    v.EmplaceFront(1111, "2222");

    assert(v[0].i_field == 1111 && v[0].s_field == "2222");
    assert(v[1].i_field == 0 && v[1].s_field == "<EMPTY>");
    assert(v[2].i_field == 0 && v[2].s_field == "<EMPTY>");
    assert(v[3].i_field == 0 && v[3].s_field == "<EMPTY>");
    assert(v[4].i_field == 0 && v[4].s_field == "<EMPTY>");
    assert(v[5].i_field == 42 && v[5].s_field == "4242");
  }
  std::cout << "[PASS] Empalce" << std::endl;
#else
  std::cout << "[SKIPPED] Empalce" << std::endl;
#endif  // SKIP_EMPLACE

#ifndef SKIP_COPY
  {
    Vector<int> v;
    for (int i = 0; i < 67; ++i) {
      v.PushBack(i);
    }

    Vector<int> other(v);
    for (int i = 0; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }

    v[0] = 1111;
    other[1] = 2222;

    assert(v[0] == 1111);
    assert(v[1] == 1);
    assert(other[0] == 0);
    assert(other[1] == 2222);

    for (int i = 2; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }
  }
  {
    Vector<int> v;
    for (int i = 0; i < 67; ++i) {
      v.PushBack(i);
    }

    Vector<int> other = v;
    for (int i = 0; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }

    v[0] = 1111;
    other[1] = 2222;

    assert(v[0] == 1111);
    assert(v[1] == 1);
    assert(other[0] == 0);
    assert(other[1] == 2222);

    for (int i = 2; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }
  }

  {
    Vector<int> v;
    for (int i = 0; i < 67; ++i) {
      v.PushBack(i);
    }

    Vector<int> other;
    for (int i = 0; i < 6; ++i) {
      v.PushBack(i * 55);
    }
    other = v;
    for (int i = 0; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }

    v[0] = 1111;
    other[1] = 2222;

    assert(v[0] == 1111);
    assert(v[1] == 1);
    assert(other[0] == 0);
    assert(other[1] == 2222);

    for (int i = 2; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }
  }
  std::cout << "[PASS] Copy" << std::endl;
#else
  std::cout << "[SKIPPED] Copy" << std::endl;
#endif  // SKIP_COPY
#ifndef SKIP_MOVE
  {
    Vector<int> v;
    v.PushBack(0);
    v.PushBack(1);
    v.PushBack(2);
    const int* const v_data = VectorInternalsAccessor<int>::AllocData(v);
    Vector<int> other(std::move(v));

    assert(other.Size() == 3);
    assert(VectorInternalsAccessor<int>::AllocData(other) == v_data);
    assert(VectorInternalsAccessor<int>::AllocSize(other) == 4);
    assert(v.Size() == 0);
    assert(VectorInternalsAccessor<int>::AllocData(v)
               != VectorInternalsAccessor<int>::AllocData(other));
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 1);
  }
  {
    Vector<int> v;
    v.PushBack(0);
    v.PushBack(1);
    v.PushBack(2);
    const int* const v_data = VectorInternalsAccessor<int>::AllocData(v);

    Vector<int> other;
    const int* const other_intitial_data =
        VectorInternalsAccessor<int>::AllocData(v);

    other = std::move(v);

    assert(other.Size() == 3);
    assert(VectorInternalsAccessor<int>::AllocData(other) == v_data);
    assert(VectorInternalsAccessor<int>::AllocSize(other) == 4);

    assert(v.Size() == 0);
    assert(VectorInternalsAccessor<int>::AllocData(v)
               != VectorInternalsAccessor<int>::AllocData(other));
    assert(VectorInternalsAccessor<int>::AllocData(v) != other_intitial_data);
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 1);
  }
  std::cout << "[PASS] Move" << std::endl;
#else
  std::cout << "[SKIPPED] Move" << std::endl;
#endif  // SKIP_MOVE

  std::cout << "Finished!" << std::endl;

  return 0;
}
#endif
