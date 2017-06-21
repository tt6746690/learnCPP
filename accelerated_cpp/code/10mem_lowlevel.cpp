#include <iostream>
#include <vector>

using namespace std;


// implement find_if
template<class In, class Pred> In find_if(In begin, In end, Pred f){
  while(begin != end && !f(*begin))
    ++begin;
  return begin;
}


char* duplicate_chars(const char* p){
  // allocate space
  size_t length = strlen(p) + 1;
  char* result = new char[length];

  // copy to newly allocated space and return pointer to first element
  copy(p, p + length, result);
  return result;
}
