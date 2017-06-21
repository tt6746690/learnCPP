#include <map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<class T>


// input iterator
template <class In, class X> In find(In begin, In end, const X& x){
  while(begin != end && *begin != x)
    ++begin;
  return begin;
}

// output iterator
template<class In, class Out> Out copy(In begin, In end, Out dest){
  while(begin != end)
    *dest++ = *begin++;
  return dest;
}

// foward iterator
template<class For, class X> void replace(For beg, For end, const X& x, const X& y){
  while(beg != end){
    if(*beg == x)
      *beg = y;
    ++beg;
  }
}

// bidirectional iterator
template<class Bi> void reverse(Bi begin, Bi, end){
  while(begin != end){
    --end;
    if(begin != end)
      swap(*begin++, *end)
  }
}


// random access iterator
template<template Ran, class X> bool binary_search(Ran begin, Ran end, const X& x){
  while(begin < end){
    // find mid point
    Ran mid = begin + (end - begin) / 2;

    if(x < *mid)
      end = mid;
    else if(*mid < x)
      begin = mid + 1;
    else return true;

  }
  return false
}

// split
template<class Out> void split(const string& str, Out os){

  typedef string::const_iterator iter;

  iter i = str.begin();
  while(i != str.end()){
    // ignore leading spaces
    i = find_if(i, str.end(), not_space);

    // find next word
    iter j = find_if(i, str.end(), space);

    // copy chars in [i, j)
    if(i != str.end())
      *os++ = string(i, j);

    i = j;
  }
}
