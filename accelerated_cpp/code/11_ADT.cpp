#include <memory>

template <class T>
class Vec
{
public:
  // type definition
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef size_t size_type;
  typedef T value_type;
  // constructor
  Vec() { create(); };
  explicit Vec(size_type n, const T &val = T()) { create(n, val); };
  // copy constructor
  Vec(const Vec &v) { create(v.begin(), v.end()); };
  // assignemnt operator
  Vec &operator=(const Vec &);
  // destructor
  ~Vec() { uncreate(); }

  // index
  /* reference is returned to avoid copy overhead if T is large and allows mutation */
  T &operator[](size_type i) { return data[i]; }
  const T &operator[](size_type i) const { return data[i]; } // overload index iterator (even with same arg signature)

  // member function
  void push_back(const T &val)
  {
    if (avail == limit) // get space
      grow();
    unchecked_append(val);
  }
  size_type size() const { return avail - data; }

  // iterators
  iterator begin() { return data; }
  const_iterator begin() const { return data; }

  iterator end() { return avail; }
  const_iterator end() const { return avail; }

private:
  iterator data;  // first element in Vec
  iterator avail; // first available element, or one past last constructed element
  iterator limit; // one past last available element in Vec

  // memory allocation
  std::allocator<T> alloc;

  // allocate and initialize underlying array
  void create();
  void create(size_type, const T &);
  void create(const_iterator, const_iterator);

  // destroy elements in array and free memory
  void uncreate();

  // support functions for push_back
  void grow();
  void unchecked_append(const T &);
};

template <class T>
Vec<T> &Vec<T>::operator=(const Vec &rhs)
{
  // check for self assignment
  if (&rhs != this)
  {
    // free array in left hand side
    uncreate();
    // copy elements from rhs to lhs
    create(rhs.begin(), rhs.end());
  }
  return *this; // dereference this ...
}

template <class T>
void Vec<T>::create()
{
  data = avail = limit = 0;
}

template <class T>
void Vec<T>::create(size_type n, const T &val)
{
  data = alloc.allocate(n);
  limit = avail = data + n;
  unitialized_fill(data, limit, val);
}

template <class T>
void Vec<T>::create(const_iterator i, const_iterator j)
{
  data = alloc.allocate(j - i);
  limit = avail = uninitialized_copy(i, j, data);
}

template <class T>
void Vec<T>::uncreate()
{
  if (data)
  {
    // destroy in reverse order elements constructed
    iterator it = avail;
    while (it != data)
      alloc.destroy(--it);

    // free up space
    alloc.deallocate(data, limit - data);
  }
  //reset pionters to indicate empty Vec
  data = limit = avail = 0;
}

template <class T>
void Vec<T>::grow()
{
  // when grow, allocate twice as much space as currently in use
  size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

  // allocate new space and copy existing elements to new space
  iterator new_data = alloc.allocate(new_size);
  iterator new_avail = uninitialized_copy(data, avail, new_data);

  // free up old space
  uncreate();

  // reset pointers
  data = new_data;
  avail = new_avail;
  limit = data + new_size;
}

// assume avail points at allocated, but uninitialized space
template <class T>
void Vec<T>::unchecked_append(const T &val)
{
  alloc.construct(avail++, val);
}

