


### 2 `auto`

#### Item 5: Prefer `auto` to explicit type declarations

+ _motivation_
    + type less 
    + prevents correctness and performance issues
+ _`auto` prevents uninitialized variable problem_
    + type deduced from initializer
    + hence must be initialized during declaration 
    ```cpp 
    int x1;         // potentially uninitialized
    auto x2;        // compiler error 
    auto x3 = 0;    // x3 is well defined
    ```
+ _`auto` declaring variable prevents verboseness_
    ```cpp 
    template<typename It>
    void dwim(It b, It e)
    {
        while(b != e){
            typename std::iterator_traits<It>::value_type currValue = *b;
            // or 
            auto currValue = *b;
        }
    }
    ```
+ _`auto` type deduction can refer to types only known to compiler_
    + every _lambda_ has unique type... 
        + ability to hold closures
    ```cpp 
    // C++11
    auto derefUPLess = 
        [](const std::unique_ptr<Widget>& p1,
            const std::unique_ptr<Widget>& p2)
        { return *p1 < *p2;};
    ```
    ```cpp
    // C++14
    auto derefLess =        // for anything pointer-like
        [](const auto& p1,
            const auto& p2){
                return *p1 < *p2;
            }
    ```
    + _alternative_: 
        + `std::function`
            + a C++11 library template that generalizes function pointer 
            + can refer to any callable object
            + so must specify type of function by assigning the template parameter 
            ```cpp 
            std::function<bool(const std::unique_ptr<Widget>&,
                                const std::unique_ptr<Widget>&)> func;
            ```
        + _problems_
            + _verbose_
            + _memory overhead_  
                + `auto`-declared variable holding a closure has same type as the closure, and as such it uses only as much memory as the closure requires.
                + `std::function`-declared variable holding an closure is an instantiation of `std::function` templates, that has a fixed size for any given signature
                    + fixed size for any given _signature_ 
                    + so may allocate heap memory to store closure
                    + hence occupy more memory 
            + _speed overhead_
                + `std::function` will always be slower by impl that restricts inlining and yield indirect function calls
                    + so almost certain to be slower than call via `auto`-declared object
+ _`auto` declaration avoids type shortcuts (prevents unintended conversion)_    
    ```cpp 
    std::vector<int> v;     

    // std::vector<int>::size_type is unsigned integral type
    unsigned sz = v.size();
    ```
    + _note_
        + on 64-bit machines    
            + `unsigned` is 32-bit
            + `std::vector<int>:size_type` is 64-bit
        + use `auto` instead 
    ```cpp 
    std::unordered_map<std::string, int> m;
    for(const std::pair<std::string, int>& p : m){
        ... 
    }
    ```
    + _note_
        + compiler will try to convert 
            + from `std::pair<std::string, int>` 
            + to `std::pair<const std::string, int>`
        + do so by 
            + create a temporary object of type `p` 
            + copy every object in `m` to `p`
            + bind reference `p` to the temporary object
            + each iteration, temporary object destroyed, 
    ```cpp 
    for(const auto& p: m){
        ...
    }
    ```
    + _note_
        + `&p` in for loop yields a poitner to an element in `m` and not some temporary object
+ _`auto`-declaration is more readable_
    + _type inference_ gains popularity by dynamically typed languages and adopted by many statically typed languages
    + naming would give hints to the kind of operation a variable is able to perform





--- 

#### Item 6: Use explicitly typed initializer idiom when `auto` deduces undesired types


+ _`auto` may introduce unintended behavior_
    ```cpp 
    std::vector<bool> features(const Widget& w);

    Widget w;
    bool highPriority = features(w)[5];
    process(w, highPriority);
    ```
    + _note_
        + `auto highPriority = features(w)[5]` breaks it
            + `highPriority` has type `std::vector<bool>::reference`
        + since `std::vector<bool>::operator[]` returns 
            + `std::vector<bool>::reference`, a _proxy class_ nested inside `std::vector<bool>`
                + because C++ forbits reference to bits i.e. `T&`
                + so have to create a wrapper class that acts like `bool&`
        + `bool highPriority = features(w)[5]`
            + declare `highPriority`'s type explicitly 
            + so `std::vector<bool>::reference` implicitly converted to `bool`
    ```cpp 
    Matrix sum = m1 + m2 + m3 + m4;
    ```
    + _note_
        + may return a proxy class `Sum<Matrix, Matrix>` instead for performance 
    + _takeway_
        + _invisible proxy do not play well with `auto`_
        + avoid 
            + `auto someVar = expression of invisible proxy class type`
+ _how to find invisible proxy class_
    + usually returned from function 
    ```cpp 
    namespace std {
    // from C++ Standards
    template <class Allocator>
    class vector<bool, Allocator> {
        public:
            ...
            class reference { ... };
            reference operator[](size_type n);
            ... 
        };
    }
    ```
+ _explicitly typed initializer idiom_
    + _forces `auto` to deduce the type you want to have_
    + _solution_ 
        + use `auto` 
        + but cast initializing expression to the type you want `auto` to deduce
    + _usage case_
        + when `auto` does not deduce the type we want
            + invisible proxy class 
                ```cpp 
                auto highPriority = static_cast<bool>(features(w)[5]);
                ```
                + here cast changes type of `std::vector<bool>::reference` to `bool`
                ```cpp 
                auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4);
                ```
            + deliberatly creating a variable of type different from generated by initializing expression
                ```cpp 
                double calcEpsilon();
                float ep = calcEpsilon();       // implicit double -> float
                auto ep = static_cast<float>(calcEpsilon());
                ```