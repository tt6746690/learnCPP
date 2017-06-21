template <class T> class Vec {
  public:
    // type definition
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    // constructor
    Vec () { create(); }
    explicit Vec(size_type n, const T& val = T()) { create(n, val); }
    Vec (const Vec& v) { create(v.begin(), v.end())};    // copy constructor

    // destructor
    ~Vec() { uncreate(); }

    // size and index
    size_type size() const { return limit - data; }
    T& operator[](size_type i) { return data[i]; }  // reference is returned to avoid copy overhead if T is large
    const T& operator[](size_type i) const { return data[i]; }  // overload index iterator (even with same arg signature)

    // assignemnt
    Vec& operator=(const Vec&);

    // functions returning iterators
    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return limit; }
    const_iterator end() const { return limit; }

    // member function
    void push_back(const T& val) {
      if(avail == limit)      // get space
        grow();
      unchecked_append(val);
    }
  private:
    iterator data;      // first element in Vec
    iterator avail;     // first available element, or one past last constructed element
    iterator limit;     // one past last available element in Vec
}


template <class T> Vec<T>& Vec<T>::operator=(const Vec& rhs){
  // check for self assignment
  if(&rhs != this){
    // free array in left hand side
    uncreate();
    // copy elements from rhs to lhs
    create(rhs.begin(), rhs.end());
  }
  return *this;   // dereference this ...
}
