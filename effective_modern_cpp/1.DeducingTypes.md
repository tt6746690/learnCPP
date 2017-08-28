
### 1 Deducing Types


+ _motivation_ 
    + free from spelling out types that are obvious
    + more adaptable code, changing types propagates through type deduction
+ _category_ 
    + function templates
    + `auto`
    + `decltype`



#### Item 1: understand template type deduction


```cpp 
template<typename T>
void f(ParamType param);

f(expr);
```
+ _function template deduction_ 
    + compiler use `expr` to deduce types for `T` and `param`
    + During compilation `expr` used to deduce 
        ```cpp
        template<typename T>
        void f(const T& param);     // ParamType is const T&

        itn x = 0;
        f(x);  
        ```
        + `T` (`int`)
            + is not always the same type as `expr` 
                + depends on `expr` and `ParamType`
            + _3 cases_
                + `ParamType` is a pointer or reference type (not universal reference)
                + `ParamType` is a universal reference
                + `ParamType` is neither a pointer nor a reference
        + `ParamType` (`const int&`)
            + different from `T` as it might contain addornments like `const` or `&`
+ _Case 1: `ParamType` is a reference/pointer but not universal ref_
    + _steps_
        + if `expr` is a reference, ignore reference part 
        + then pattern-match `expr`'s type agianst `ParamType` to determine `T`
    ```cpp 
    template<typename T>
    void f(T& param);

    int x = 27;                 // int 
    const int cx = x;           // const int 
    const int& rx = x;          // reference to x as const int
    ```
    ```cpp 
    f(x);           // T is int, ParamType is int&
    f(cx);          // T is const int, ParamType is const int&
    f(rx);          // T is const int, ParamType is const int&
    ```
    + _note_
        + for `const` values, (2,3) 
            + `T` deduced to `const int` 
            + `ParamType` then is of type `const int&`
            + passing `const` object to a reference parameter, we'd expect object to remain unmodifiable
        + for reference (3)
            + `T` deduced to `const int` as a non-reference
                + since reference-ness ignored during type deduction
    ```cpp
    template<typename T>
    void f(const T& param)  // param not a ref-to-const
    ```
    ```cpp 
    f(x);           // T is int, ParamType is const int&
    f(cx);          // T is int, ParamType is const int&
    f(rx);          // T is int, ParamType is const int&
    ```
    + since requires `param` is a _reference-to-`const`_
        + no need for `const` to be deduced as part of `T`
    ```cpp
    template<typename T>
    void f(T* param);

    int x = 27;             // x is int
    const int*px = &x;      // pointer to x as a const int
    ```
    ```cpp
    f(&x);              // T is int, ParamType is int*
    f(px);              // T is const int, ParamType is const int*
    ```
    + _param is a pointer_
+ _Case 2: `ParamType` is a universal reference_
    + _universal reference_
        + declared like `rvalue reference` 
        + but behave diferently when lvalue args are passed in
    + _steps_
        + if `expr` is `lvalue`
            + `T` and `ParamType` are deduced to be `lvalue reference`
        + if `expr` is `rvalue`
            + Case 1 rules apply
    ```cpp
    template<typename T>
    void f(T&& param);      // param a universal reference

    int x = 27;
    const int cx = x;
    const int& rx = x;
    ```
    ```cpp
    f(x);           // x is lvalue, so 
                    // T is int&, ParamType also int&
    f(cx);          // cx is lvalue, so 
                    // T is const int&, ParamType also const int&
    f(rx);          // rx is lvalue, so
                    // T is const int&, ParamType also const int&
    f(27);          // 27 is rvalue, so
                    // T is int, ParamType is int&&
    ```
    + _note_
        + type system distinguish `lvalue` args and `rvalue` args
+ _Case 3: `ParamType` is neither a pointer nor a reference_
    + _implies parameter passed-by-value (a copy)_
    + _steps_
        + if `expr` is reference, ignore reference part 
        + if,`expr` is `const` or `volatile`, ignore that, too. 
    ```cpp
    template<typename T>
    void f(T param);        // param passed by value
    ```
    ```cpp
    f(x);               // T and ParamType both int 
    f(cx);              // T and ParamType both int 
    f(rx);              // T and ParamType both int
    ```
    + _note_
        + even if `cx` and `rx` are `const`, `param` is not `const`
            + makes sense since `param` is a copy, new object completely independent from `cx` or `rx`, 
            + the fact that `expr` can't be modified says anything about whether `param` can be. 
    ```cpp
    template<typename T>
    void f(T param);

    const char* const ptr = "Fun with pointers";
    f(ptr);             // ParamType is const char* 
    ```
    + `const char* const ptr`
        + `const ptr`
            + pointer cannot be made to point to a different location
        + `const char`
            + `char *` is const, hence cannot be modified 
    + _condition_
        + if `expr` is a `const` pointer to a `const` object, and 
        + `expr` passed by value, 
    + _then_
        + `ptr` itself will be passed by value 
            + so `const` of `ptr` is dropped
        + `ParamType` is `const char*`
            + a modifiable pinter to a `const` char string
+ _Array arguments_
     + _array decays to pointers_
        ```cpp 
        const char name[] = "J. P. Briggs";   // type: const char[13]
        const char* ptrToName = name;         // array decays to pointer
        ```
    ```cpp 
    template<typename T>
    void f(T param);            // by-value

    f(name);                    // T is const char*
    ```
    + _note_
        + _type of array passed to template function by value is deduced to be a pointer type_
        + _array function parameter_
            + _equivalence_
            ```cpp 
            void f(int param[]);        // declaration valid 
            void f(int * parm);         // converted from above
            ```
        + _rule_
            + array parameter declaration are treated as if they were pointer parameters, 
            + hence `T` is deduced to be `const char*`
    ```cpp 
    template<typename T>
    void f(T& param);

    f(name);            // pass array to f 
    ```
    + _note_
        + _declaring reference to array is possible!_
        + passing in array by-reference to `(T& param)`
        + `T` is actual type of array
            + i.e. `const char [13]`
        + `ParamType` is `const char(&) [13]`
    + _Template that deduces number of elements an array contains_
    ```cpp 
    // return size of array as compile time constant 
    template<typename T, std::size_t N>
    constexpr std::size_t arraySize(T (&)[N]) noexcept {
        return N;
    }
    ```
    ```cpp 
    int keyVals[] = {1, 3, 7, 9, 11, 22, 35};           // 7
    int mappedVals[arraySize(keyVals)];                 // 7
    std::array<int, arraySize(keyVals)> mappedVals;     // 7
    ```
+ _Function arguments_
    + _function types decays to function pointers_
    ```cpp 
    void someFunc(int, double);     // void(int, double)

    template<typename T>
    void f1(T param);               // passed by value 
    template<typename T>
    void f2(T& param);              // passed by ref

    f1(someFunc);                   // param deduced as ptr-to-func
                                    // type is void(*)(int, double)
    f2(someFunc);                   // param deduced as ptr-to-ref
                                    // type is void (&)(int, double)
    ```
+ _things to remember_
    + `reference`-ness is ignored during template type deduction 
    + `lvalue` for universal reference parameter gets special treatment
    + `const` and `volatile` arguments are treated as `non-const` `non-volatile` for by-value parameter
    + array/function argument names decay to pointers, unless they are used to initialize references (i.e. `(T& param)`)


--- 

#### Item 2: Understand `auto` type deduction



+ _`auto` is template type deduction_
    ```cpp 
    auto x = 27;            // T is auto 
                            // ParamType is type-specifier for variable
    const auto cx = x;      // type specifier is const auto
    const auto& rx = x;     // type specifier is const auto&
    ```
    ```cpp 
    template<typename T>
    void func_for_x(T param);  // conceptual template for deducing x type

    func_for_x(x);          // conceptual call to deduce x's type

    template<typename T>
    void func_for_cx(const T param); 

    func_for_cx(x);

    template<typename T>
    void func_for_rx(const T& param);

    func_for_rx(x);
    ```
    + _note_
        + compiler act as if there were a template for each declaration as well as a call to that template with corresponding initailizing expression
    + _3 cases depending on type specifier (instead of `ParamType`)_
        + _1: type specifier is pointer/reference, but not universal reference_
        + _2: type specifier is universal reference_
        + _3: type specifier is neither pointer nor reference_
    + _case 1 and 3_
        ```cpp
        auto x = 27;        // case 3
        const auto cx = x;  // case 3
        const auto& rx = x; // case 1 (rx is non-universal ref)
        ```
    + _case 2_
        ```cpp 
        auto&& uref1 = x;   // x is int and lvalue, uref1's type is int&
        auto &&uref2 = cx;  // cx is const int and lvalue, 
                            // uref2's type is const int&
        auto &&uref3 = 27;  // 27 is int and rvalue 
                            // uref3's type is int&&
        ```
    + _array/function decary_
        ```cpp 
        const char name[] = "R. N. Briggs";         // const char[13]

        auto arr1 = name;       // const char * (decay, by-value)
        auto& arr2 = name;      // const char (&)[13]

        void someFunc(int, double)      // void(int, double)

        auto func1 = someFunc;          // void (*) (int, double)
        auto& func2 = someFunc;         // void (&) (int, double)
        ```
+ _One major Exception_
    + `auto` _assumes that a braced initializer represents `std::initializer_list` but template type deduction doesnt_
    ```cpp 
    int x1 = 27;            // int
    int x2(27);             // int  
    int x3 = {27};          // int
    int x4{27};             // int
    ```
    ```cpp 
    auto x1 = 27;           // x1 is int, with value 27 
    auto x2(27);            // ditto
    auto x3 = {27};         // x3 is std::initializer_list<int>, 
                            // with value { 27 }
    auto x4{27};            // ditto
    ```
    + _special deduction rule for `auto` with brace initalization_
        + `auto`-declared variable initailized with braced initializer, _the deduced type is always `std::initializer_list`_
        + if braced value are of different type, code will be rejected 
            ```cpp 
            auto x5 = {1, 2, 3.0};
            ```
            + because `initalizer_list<T>` have to deduce type for `T`, which is not possible with different types
    ```cpp 
    auto x = {11, 23, 9};       // std::initializer_list<int>

    template<typename T>
    void f(T param);

    f({11, 23, 9});             // error!, cant deduce type for T

    template<typename T>
    void f(std::initializer_list<T> initlist);

    f({11, 23, 9});             // T deduced as int, initList as initializer_list<int>
    ```
    + _note_
        + type deduction fails for template function 
        + have to _explicitly_ specify `std::initializer_list<T> param`
+ _`auto` as return type (C++14) and lambda param_
    ```cpp
    auto createInitList(){
        return {1,2,3};     // cant deduce type for {1,2,3}
    }

    std::vector<int> v;
    auto resetV = 
        [&v](const auto& newValue){v = newValue;}'
    
    resetV({1,2,3});        // error! cant deduce type for {1,2,3}
    ```
    + _note_
        + use _template type deduction_ instead of _`auto` type deduction_
        + so cant deduce `{1,2,3}`, does not assume its `initializer_list`
+ _summary_
    + `auto` type deduction usually the same as template type deductin, but `auto` assumes a braced initializer represents `std::initializer_liist` and template type deduction doesnt
    + `auto` in function return type or lambda implies template type deduction not `auto` type deduction


#### Item 3: Understand `decltype`


+ `decltype` 
    + tells you name's or expression's _exact type_
    ```cpp 

    const int i = 0;            //  decltype(i): const int
    bool f(const Widget& w);    // decltype(w): const Widget&

    struct Point {              // decltype(Point::x): int 
        int x, y;               // decltype(Point::y): int
    }

    Widge w;                    // decltype(w) is Widget 

    if(f(w))..                  // decltype(f(w)) is bool 

    template<typename T>        
    class vector {
    public:
        ...
        T& operator[](std::size_t index);
        ...
    }

    vector<int> v;              // decltype(v) is vector<int>

    if(v[0] == 0)...            // decltype(v[0]) is int&
    ```
+ _usage_
    + declaring function templates where function's return type depends on the parameter types
    ```cpp 
    template<typename Container, typename Index>
    auto authAndAccess(Container& c, Index i) 
        -> decltype(c[i])
    {
        authenticateUser();
        return c[i];
    }
    ```
    + _note_
        + `auto` 
            + use _trailing return type_ syntax 
            + return type will be declared after parameter list
            + advantage in that function's parameter can be used to specify the return type
        + `decltype` makes it easy to compute the return type which 
            + is usally `T&` on `Container<T>`
            + but exceptions like `std::vector<bool>` does not return `bool&`...
    ```cpp 
    template<typename Container, typename index>
    auto authAndAccess(Container& c, Index i){
        authenticateUser();
        return c[i];
    } 
    ```
    + `auto` return type deduction
        + C++11, return type for single-statement lambdas 
        + C++14, return type for all lambdas, and all functions, with a single `return`
        + in this case `auto` implies return type deduction
            + uses _template type deduction_
            + _problematic_ 
                + because `operator[]` for containers usually return `T&`
                + but during template type deduction, referenceness of `expr` is ignored
                ```cpp 
                std::deque<int> d;
                authAndAccess(d, 5) = 10;
                        // authenticate user, return d[5]
                        // assign 10 to int; wont compile
                ```
                + here return type deduced to be `int`
                + `rvalue` is not assignable
    ```cpp 
    template<typename Container, typename index>
    decltype(auto) authAndAccess(Container& c, Index i)
    {
        authenticateUser();
        return c[i];
    }
    ```
    + _use `decltype(auto)` for deducting return type_
        + specify that return type be exactly the same as `c[i]`
        + i.e. 
            + if `c[i]` returns `T&`, `T&` is the return type 
            + if `c[i]` returns an object, funtion will return the exact same object type
    + `decltype(auto)`
        + use `decltype`'s deduction rule on cases where types are inferred...
            + `auto`: species that type is to be deduced 
            + `decltype`: the `decltype` rules will be used during deduction
        + _usage_
            + _function return type_
            + _variable declaration_
                ```cpp 
                Widget w;
                const Widget& cw = w;
                auto myWidget1 = cw;        // auto type deduction
                                            // myWidge1 has type Widget
                decltype(auto) myWidget2 = cw;  // decltype deduction
                                                // myWidget2 has type const Widget&
                ```
    + _improvement_ 
        ```cpp 
        template<typename Container, typename Index>
        decltype(auto) autoAndAccess(Container& c, Index i);
        ```
        + _observation_ 
            + container passed by _lvalue-reference-to-non-const_
            + however, not able to pass `rvalue` containers to function
                + since `rvalue` cannot bind to lvalue reference
        + _solution_ 
            + overload, one provide `lvalue reference`,the other `rvalue reference`, or
            + use reference parameter that can bind to both `lvalue` and `rvalue`
        ```cpp 
        // C++14
        template<tyename Container, typename Index>
        decltype(auto) authAndAccess(Container&& c, Index i){
            authenticateUser();
            return std::forward<Container>(c)[i];
        }

        // C++11 (no decltype(auto))
        template<tyename Container, typename Index>
        auto authAndAccess(Container&& c, Index i)
            -> decltype(std::forward<Container>(c)[i]){
            authenticateUser();
            return std::forward<Container>(c)[i];
        }
        ```
        + _note_
            + `Index` also unknown, 
                + copy may be expensive, 
                + but reasonable as index types 
+ _Exceptional behavior_
    + `decltype`
        + `lvalue` expression   
            + names, `decltype` behave properly 
            + other than names, i.e. objects, `delctype` ensure the type reported is an `lvalue reference`
                + implies, `T&` deduced for objects of type `T` (other than name)
        ```cpp 
        int x = 0;      // decltype(x) is int 
                        // decltype((x)) is int&
        ```
        + _note_
            + happens because `(x)` is not merely a name
        ```cpp 
        decltype(auto) f1(){
            int x = 0;
            return x;       // decltype(x) is int, 
        }

        decltype(auto) f2(){
            int x = 0;
            return (x);     // decltype((x)) is int&
        }
        ```
        + _note_
            + second function is returning reference to a local variable, cause dangling reference!
+ _summary_
    + `decltype` yield types of variable or expression without any modification  
    + `lvalue` expression of tyep `T` other than names, `decltype` reports type of `T&`
    + C++14 supports `decltype(auto)`, which deduces a type from its initializer, but performs type deduction using `decltype` rules



--- 

#### Item 4: How to view deduced types


+ _Edit time (IDE)_
    ```cpp
    const int answer = 42;      
    auto x = answer;            // x: int 
    auto y = &answer;           // y: const int *
    ```
+ _Compile time (compiler diagnostics)_
    + let compiler report a type by using the type in a way that leads to compilation errors
    ```cpp
    template<typename T>
    class TD;

    TD<decltype(x)> xType;      // elicit error containing x,y's type
    TD<decltype(y)> yType;

    /*
    error: aggregate 'TD<int> xType' has incomplete type and cannot be defined
    error: aggregate 'TD<const int *> yType' has incomplete type and cannot be defined
    */
    ```
    + _note_
        + instantiating template will elicit error message 
        + use `variableNameType` convention to yield helpful error messages
+ _Runtime output_  
    + with `typeid` and `std::type_info::name`
    ```cpp 
    std::cout << typeid(x).name() << '\n'; >>>>
    std::cout << typeid(y).name() << '\n'; >>>>
    ```
    + _note_
        + `typid(obj)` returns `std::type_info`
        + `::name` member produces C-style string, of name of the type
    + `std::type_info::name` outputs mangled types
        + `i`: `int` 
        + `PKi`: `const int*`
    + use `c++filt` works!
        + decodes mangled types
    ```cpp 
    template<typename T>
    void f(const T& param){
        std::cout << "T = " << typeid(T).name() << '\n';
        std::cout << "Param = " << typeid(Param).name() << '\n';
        /*
            T = PK6Widget (pointer to Widget)
            Param = PK6Widget
        */
    }

    std::vector<Widget> createVec();        // factory function
    const auto vw = createVec();            // init vw

    if(!vw.empty()){
        f(&vw[0]);                          // call f
    }
    ```
    + _note_ 
        + complex situations
        + here `ParamType` is of `T&`, but `T` and `param` are the same 
            + not reasonable
            + `std::type_info::name` is not reliable
                + specifies that types passed in be treated as if it had been passed to a template function as a by-value parameter, the reference-ness and const-ness is ignored...
        + `param`'s type should be `const Widget* const &`
            + but is reported as `const Widget`
            + since first reference-ness is removed, then `const`ness removed
+ _With Boost_
    + `boost/type_index.hpp`
    ```cpp 
    template <boost/type_index.hpp>

    template<typename T>
    void f(const T& param)
    {
        using std::cout; 
        using boost::typeindex::type_id_with_cvr;

        std::cout << "T = "
                << type_id_with_cvr<T>().pretty_name();
        std::cout << "Param = "
            << type_id_with_cvr<decltype(param)>().pretty_name();
        
        /*
            T = Widget const *
            param = Widget const * const &
        */
    }
    ```
    + _note_
        + `const` `volatile` `reference` qualifier not removed
