


## Q/A

Q: Use C standard library in C++?   
A:  
+ use a C header file in C++ as 
```h
extern "C" {
#include <c_header_file.h>
}
```
+ Actually, most existing C header are designed to incoporate C++ by including 
    + some even in `namespace std`
```h
 #ifdef __cplusplus
 extern "C" {
 #endif
// ...
 #ifdef __cplusplus
 }; // end extern "C"
 #endif
```

+ [C++ std library](http://en.cppreference.com/w/cpp/header) 
