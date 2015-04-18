#include <cstring>
#include <memory>

/**
 * This is a fixed-size array that does not initialize its contents--no constructors are called.
 * This allows you to stack-allocate space for objects that cannot or should not be
 * initialized with a default constructor.
 */
template<class T, size_t N, class alloc_type = std::allocator<T>>
class LazyArray {
  alloc_type alloc;

  char raw_array_[sizeof(T) * N];
  T* array_;

  bool set_[N];

public:
  LazyArray() : array_(reinterpret_cast<T*>(raw_array_)) {
    memset(set_, false, sizeof(set_));
  }

  ~LazyArray() {
    for (size_t i = 0; i < N; ++i) {
      if (set_[i]) alloc.destroy(&array_[i]);
    }
  }

  //
  // Capacity
  //

  /**
   * This currently returns the size of the array, not how many objects have been initialized.
   */
  size_t size() const {
    return N;
  }

  /**
   * Check if any elements have been set.
   * Currently, this is O(n) in the size of the array.
   */
  bool empty() const {
    for (bool set : set_) {
      if (set) return false;
    }

    return true;
  }

  //
  // Adding and removing elements
  //

  /**
   * Construct an object in place.
   * If there already was an object there, it is destroyed.
   */
  template<typename... Args>
  void emplace(size_t position, Args&&... args) {
    if (set_[position]) {
      alloc.destroy(&array_[position]);
    }
    else {
      set_[position] = true;
    }

    alloc.construct(&array_[position], args...);
  }

  /**
   * Construct all of the objects in the array with the same parameters.
   */
  template<typename... Args>
  void emplaceAll(size_t position, Args&&... args) {
    for (int i = 0; i < N; ++i) {
      emplace(i, args...);
    }
  }

  /**
   * Destroy an object in the array.
   * If no object has been constructed at the position, then nothing happens.
   */
  void destroy(size_t position) {
    if (set_[position]) {
      alloc.destroy(&array_[position]);
      set_[position] = false;
    }
  }

  /** Destroy all of the objects in the array. */
  void clear() {
    for (int i = 0; i < N; ++i) {
      destroy(i);
    }
  }

  //
  // Element access
  //

  /** Check whether the object at index i has been initialized. */
  bool valid(size_t i) const { return set_[i]; }

  T& operator[](size_t i) {
    return array_[i];
  }

  const T& operator[](size_t i) const {
    return array_[i];
  }

  T& at(size_t i) {
    if (i > N) throw std::out_of_range("index out of range");
    if (!set_[i]) throw std::out_of_range("value not set");

    return array_[i];
  }

  const T& at(size_t i) const {
    if (i > N) throw std::out_of_range("index out of range");
    if (!set_[i]) throw std::out_of_range("value not set");

    return array_[i];
  }

  T& front() {
    return array_[0];
  }

  const T& front() const {
    return array_[0];
  }

  T& back() {
    return array_[N-1];
  }

  const T& back() const {
    return array_[N-1];
  }

  // iterators
  // iterators don't currently skip over uninitialized members
  typedef T* iterator;
  typedef const T* const_iterator;

  iterator begin() { return array_; }
  const_iterator begin() const { return array_; }
  const_iterator cbegin() const { return array_; }
  iterator end() { return &array_[N]; }
  const_iterator end() const { return &array_[N]; }
  const_iterator cend() const { return &array_[N]; }
};
