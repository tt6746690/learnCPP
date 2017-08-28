

### 3 Moving to Modern C++


#### Item 7: Distinguish between `()` and `{}` when creating objects


+ _initialization_ 
    ```cpp 
    int x(0);       // parenthesis
    int y = 0;      // =
    int z{0};       // braces
    int z = {0};    // = and braces, equivalent to above
    ```
    + _3 ways to initialize_
    ```cpp 
    Widget w1;          // default constructor
    Widget w2 = w1;     // not assignment; calls copy ctor
    w1 = w2;            // an assignment; calls copy operator=
    ```
    + _note_
        + confusion with assignment and initialization
+ _uniform initialization_ 
    + a single initialization syntax that can be used anywhere and express everything
    + using _braced initialization_ as a syntactic construct
    + _usage_
        + _initialize container content_
        ```cpp 
        std::vector<int> v{1,3,5};      // initial content of containers 
        ```
        + _specify default value_
            + for _non-static data members_
            + `=` initialization syntax also OK
        ```cpp
        class Widget {
            private:
                int x{0};      // find x defaults to 0
                int y = 0;     // also fine
                int z(0);      // error 
        }
        ```
        + _initialize uncopyable objects_  
            + `()` initialization syntax also OK 
        ```cpp 
        std::atomic<int> ai1{0};        // OK
        std::atomic<int> ai2(0);        // OK 
        std::atomic<int> ai3 = 0;       // error!
        ```
    + _prohibits narrowing conversion amongst built-in types_ 
        ```cpp 
        double x,y,z;
        int sum1{x+y+z};        // error, sum of doubles cannot be ints
        int sum2(x+y+z);        // OK, value of expr truncated to int
        int sum3 = x + y + z;   // OK, ...
        ```
        + _note_
            + if the value of expression in braced initializer isnt guaranteed to be the type of object being initialized, the code wont compile
    + _immune to most vexing parse_
        ```cpp 
        Widget w1(10);      // call Widget ctor with arg 10
        Widget w2();        // most vexing parse!, declares a function 
        Widget w3{};        // calls Widget ctor with no args
        ```
        + _most vexing parse_
            + anything can be paresd as a declaration must be interpreted as one.
            + _problem_: 
                + want to default-construct object, 
                + but inadvertently end up declaring a function
            + _solution_ 
                + use `{}` since function cannot be declared using braces for the parameter list, 
                + so default-constructing object using braces doesnt have this problem
    + _problems_ 
        + if the compiler can use an initializer list constructor with a brace-initialized list, then it will.
        ```cpp 
        class Widget {
            Widget(int i, bool b);      // ctors not declaring
            Widget(int i, double b);    // std::initializer_list params
        };

        Widget w1(10, true);        // calls first ctor
        Widget w2{10, true};        // calls first ctor
        Widget w3(10, 0.5);         // calls second ctor 
        Widget w4{10, 0.5};         // calls second ctor
        ```
        + _note_
            + In constructor call, `()` and `{}` initialization are equivalent as long as `std::initializer_list` parameter are not invovled
        ```cpp
        class Widget {
            Widget(int i, bool b);     
            Widget(int i, double b);    
            Widget(std::initializer_list<long double> il);  
        }; 

        Widget w1(10, true);        // calls first ctor
        Widget w2{10, true};        // calls std::initializer_list ctor
                                    // (10 and true converted to long double)
        Widget w3(10, 0.5);         // calls second ctor 
        Widget w4{10, 0.5};         // calls std::initializer_list ctor
                                    // (10 and 0.5 converted to long double)
        ```
        + _note_ 
             + however, if `>=1` ctor declares parameter of type `std::initializer_list`, calls using braced initialization syntax __strongly__ prefer the overloads taking `std::initializer_list`
        ```cpp 
        class Widget {
            Widget(int i, bool b);     
            Widget(int i, double b);    
            Widget(std::initializer_list<long double> il);  

            operator float() const;     // convert to float
        }; 

        Widget w5(w4);          // calls copy ctor
        Widget w6{w4};          // uses braces, calls std::initializer_list ctor, 
                                // (w4 converts to float, and float converts to long double)
        Widget w7(std::move(w4));   // calls move ctor
        Widget w8{std::move(w4)};   // calls std::initializer_list ctor    
        ```
        + _note_
            + copy and move construction may be hijacked by `std::initializer_list` constructors
        ```cpp 
        class Widget {
            Widget(int i, bool b);     
            Widget(int i, double b);    
            Widget(std::initializer_list<bool> il); 
        }; 

        Widget w{10, 0.5};          // error! requires narrowing conversions
        ```
        + _note_  
            + compiler will match __strongly__ braced initializer with `std::initializer_list` even when the constructor cannot be called...
        ```cpp 
        class Widget {
            public:
                Widget(int i, bool b);
                Widget(int i, double d);
                Widget(std::initializer_list<std::string> il);
        };

        Widget w1(10, true);        // calls first ctor
        Widget w2{10, true};        // calls first ctor
        Widget w3(10, 0.5);         // calls second ctor 
        Widget w4{10, 0.5};         // calls second ctor
        ```
        + _note_
            + only if there is no way to convert types of arguments in a braced initializer to `T` in `std::initializer_list<T>` do compiler fall back to normal overload resolution
        ```cpp 
        class Widget {
            public:
                Widget();
                Widget(std::initializer_list<int> il);
        };

        Widget w1;        // default ctor 
        Widget w2{};      // default ctor 
        Widget w3();      // most vexing parse! declares a function!

        Widget w4({});    // calls std::initializer_list with empty list
        Widget w5{{}};    // ditto...
        ```
        + _edge case_
            + using empty set of braces `{}` when both default ctor and `std::initializer_list` ctor exists, _default construction_ is used
            + `{}` empty braces means no arguments, 
            + `{{}}` double empty braces means empty initializer list
        ```cpp 
        std::vector<int> v1(10, 20);    // creates 10-elem with value 20
        std::vector<int> v2{10, 20};    // create 2-element vector with value 10 & 20
        ```
        + _note_
    + _takeaway_
        + _library writer_
            + aware of the set of overloaded ctors, which includes one or more taking `std::initializer_list`, 
            + design ctors so that overloaded isnt affected by whether clients use parenthese or braces
            + take caution in adding `std::initializer_list` to ctors
        + _library clients_
            + _`{}` braces-by-default_ 
                + widely applicable, 
                + prohibits narrowing conversion, 
                + immune to most vexing parse 
                + understand that in some cases, `()` is required (i.e. `std::vector`)
            + _`()` parentheses-by-default_
                + consistency with C98 syntax
                + avoid `auto`-deduced `std::initializer_list` problem 
                + knowledge that object creation wont be inadvertently wayload by `std::initializer_list` ctor
        + _template writer_
            + in general, not possible to know which should be used...
            ```cpp 
            template<typename T, typename... Ts>
            void doSomeWork(Ts&&...params){
                // create local T from params
                T localObject(std::forward<Ts>(params)...);     // use ()
                T localObject{std::forward<Ts>(params)...};     // use {}
            }

            std::vector<int> v;
            doSomeWork<std::vector<int>>(10, 20);
            ```
            + _note_
                + if use `()` to create `localObject`, then resulting a vector of with 10 elements
                + if use `{}` to create `localObject`, then result in `std::vector` with 2 elements
            + `make_unique` and `make_shared` has exactly the problem 
                + use `()` by default
+ _summary_
    + use `{}` intentionally for
        + default initialization to prevent most vexing parse
        + initialize non-copyable objects (`()` also OK)
    + use `{expr}` to prevent narrowing conversion
    + use `{}` whenever possible, but understand the pitfalls...
        + `std::initializer_list` overload 



--- 

#### Item 8: Prefer `nullptr` to `0` and `NULL`
        
    
    
+ _motiation_ 
    + `0` is `int` not a pointer 
    + `NULL` is impl defined, not a pointer
    + but both can be treated as pointers...
+ _C++98_
    ```cpp 
    void f(int);
    void f(bool);
    void f(void*);

    f(0);           // calls f(int), not f(void*)
    f(NULL);        // might not compile, but typically calls f(int)
                    // never calls f(void*)
    ```
    + _note_
        + `f(NULL)` yields undefined behavior       
            + if `NULL` defined to be `0L` 
            + then conversion from `0L` to `bool`/`void*` are equally good
        + syntax does not reflect intention 
            + _want_: call `f` with a pointer 
            + _reality_: call `f` with some kind of `int` (or `long`)
+ `nullptr`
    + a pointer of all types
    + actual type is `std::nullptr_t`, which is defined to be type of `nullptr`
        + `std::nullptr_t` implicitly converts to all raw pointers
    ```cpp 
    f(nullptr);         // calls f(void*) overload
    ```
    + _note_
        + since `nullptr` cannot be viewed as anything integral
    ```cpp 
    int f1(std::shared_ptr<Widget> spw);
    int f2(std::unique_ptr<Widget> upw);
    bool f3(Widget* pw);

    std::mutex f1m, f2m, f3m;       // mutex for f1, f2, f3

    using MuxGuard = std::lock_guard<std::mutex>;

    {
        MuxGuard g(f1m);        // lock mutex for f1;
        auto result = f1(0);
    }                           // unlock mutex

    {
        MuxGuard g(f2m);        // lock mutex for f2;
        auto result = f2(NULL);
    }                           // unlock mutex

    {
        MuxGuard g(f3m);        // lock mutex for f3;
        auto result = f3(nullptr);
    }                           // unlock mutex
    ```
    + _note_
        + code works for all 3 cases, but redundant code
        + use templates 
    ```cpp 
    template<typename FuncType
                typename MuxType
                typename PtrType>
    auto lockAndCall(FuncType func, 
                    MuxType& mutex, 
                    PtrType ptr) -> decltype(func(ptr))
    {
        MuxGuard g(mutex);
        return func(ptr);
    }

    auto result1 = lockAndCall(f1, f1m, 0);         // error!
    auto result2 = localAndCall(f2, f2m, NULL);     // error!
    auto result3 = localAndCall(f3, f3m, nullptr);  // OK
    ```
    + _note_
        + _template type deduction_ try to figure out type 
        + `0` implies `ptr` is type `int`, 
            + which is not compatible with `std::shared_ptr<Widget>`, parameter `f1` expects 
            + in essence `int` is passed to a function that requires `std::shared_ptr<Widget>`
        + same thing for `NULL`
        + `nullptr` OK because 
            + `ptr` is deduced to be `std::nullptr_t`
            + when `ptr` passed to `f3`, exists implicit conversion from `std::nullptr_t` to `Widget*` because all `std::nullptr_t` implicitly converts to all pointer types
+ _summary_
    + prefer `nullptr` to `0` and `NULL` because
        + no overload resolution surprises
        + works well with _template type deduction_ while `0` and `NULL` do not
    

--- 

#### Item 9: Prefer alias declaration to typedefs


+ _motivation_
    + `typedef`
    ```cpp 
    typedef 
        std::unique_ptr<std::unordered_map<std::string, std::string>>
        UPtrMapSS;
    ```
    + _alias declarations_ 
    ```cpp 
    using UPtrMapSS = 
        std::unique_ptr<std::unordered_map<std::string, std::string>>;
    ```
+ _alias declaration are easier to read_
    ```cpp 
    typedef void (*FP)(int, const std::string&);
    using FP = void(*)(int, const std::string&);
    ```
+ _alias declaration works well with templates_ 
    + _alias templates_
        + alias declaration that are templatized, while `typedef` cannot
    ```cpp 
    // typedef 
    template<typename T>
    struct MyAllocList {
        typedef std::list<T, MyAlloc<T>> type;
    };
    // MyAllocList<T>::Type is synonym for std::list<T, MyAlloc<T>>

    MyAllocList<Widget>::type lw;
    ```
     ```cpp 
    // alias templates
    template<typename T>
    using MyAllocList = std::list<T, MyAlloc<T>>;
    // MyAllocList<T> is synonym for std::list<T, MyAlloc<T>>

    MyAllocList<Wiget> lw;
    ```
    + _note_
        + define synonym for linked list and uses a custom allocator
        + using alias templates is shorter, and no need to create struct wrapping the types
    ```cpp 
    // typedef
    template<typename T>
    class Widget {
        private:    
            typename MyAllocList<T>::type list;
    }
    ```
    ```cpp 
    // alias templates
    template<T>
    class Widget {
        private:    
            MyAllocList<T> list;
    }
    ```
    + _note_
        + `typedef`
            + have to use `typename` since `MyAllocList<T>::type` is a _dependent type_
            + why is `MyAllocList<T>::type` a _dependent type_?
                + compiler dont know for sure that it names a type, 
                + because there might be a specialization of `MyAllocList` that haven't yet seen where `MyAllocList<T>::type` refers to something other than a type
                ```cpp 
                class Wine {};

                template<>
                class MyAllocList<Wine>{
                    private:
                        enum class WineType
                            { White, Red, Rose};
                        WineType type;      // type is a data member! 
                }
                ```
                + so what `MyAllocList<T>::type` refer to depends on what `T` is, and thats why compilers insist that programmer assert that it is a type by preceding with `typename`
            + _rule_: dependent types must be preceded by `typename`
        + _alias declaration_ 
            + `MyAllocList` is an alias templates, 
                + dont have to write `typename`
                + dont have to write `::type`
            + why is `MyAllocList<T>` not a _dependent type_?
                + compiler knows that `MyAllocList<T>` is name of a type,
                    + so no need for `typename`, which serves the purpose of indicating that a _name_ is a _type_ 
                + since `MyAllocList` is an alias template, it __must__ name a type
                + so `MyAllocList<T>` is a _non-dependent type_
    + `<type_traits>`
        + `T -> std::transformation<T>::type`
        ```cpp 
        // C++11
        std::remove_const<T>::type          // yields T from const T
        std::remove_reference<T>::type      // yields T from T& and T&&
        std::add_lvalue_referene<T>::type   // yields T& from T
        // C++14 equivalence
        std::remove_const_t<T>
        std::remove_reference_t<T>
        std::add_lvalue_reference_t<T>
        ```
        + _note_
            + _problem: `type_trait` impl as `typedef`s inside `struct`_
                + need to write `::type` at the end
                + if apply a type parameter inside a template, need to precede with `typename`
            + _solution_
                + use template alias
        ```cpp 
        template<typename T>
        using remove_const_t = typename remove_const<T>::type;

        template<typename T>
        using remove_reference_t = typename remove_reference<T>::type;

        template<typename T>
        using add_lvalue_reference_t = 
            typename add_lvalue_reference<T>::type;
        ```
        + _note_
            + impl of `_t` for `type_trait` 
+ _summary_
    + `typedef` dont support templatization, alias declarations do 
    + alias templates avoid`::type` suffix, in templates `typename` prefix often required to refer to `typedef`s
    + C++14 offers alias templates for all C++11 type trait transformations
            


---

#### Item 10: Prefer scoped enums to unscoped enums


+ _unscoped enum_
    + C++98-style `enum` is scoped by braces
    ```cpp 
    enum Color {black, white, red}; // black in same scope as Color
    auto white = false;             // error! white already declared in scope
    ```
+ _scoped enum reduces namespace pollution_
    ```cpp
    enum class Color {black, white, red};       // scoped to Color

    auto white = false;                 // fine, no other
    Color c = white;                    // Error! no enumerator named white
    Color c = Color::white;             // OK
    auto c = Color::white;              // OK
    ```
+ _scoped enum are much more strongly typed_
    + _unscoped enum_
        + enumerators implicitly convert to integral types 
        ```cpp 
        std::vector<std::size_t> primeFactors(std::size_t x);
        
        enum Color {black, white, red};     // unscoped enum
        Color c = red;

        if(c < 45.5)                        // compare Color to double!
            auto factors = primeFactors(c); // compute prime factor of a Color!
        ```
    + _scoped enum_
        + _no implicit conversion_ (to any type) allowed
        ```cpp 
        enum class Color {black, white, red};
        Color c = Color::Red;

        if(c < 4.5)         // error, cant compare Color and double
            auto factors = primeFactor(c);      
            // cant pass Color to function expecting std::size_t
        ```
        + _use `static_cast` for explicit conversion_
        ```cpp 
        if(static_cast<double>(c) < 14.5)
            auto factors = primeFactor(static_cast<std::size_t>(c));
        ```
+ _scoped enum maybe forward-declared_
    + names may be declared without specifying their enumerators 
    ```cpp 
    enum Color;         // Error
    enum class Color;   // OK, forward declaration
    ```
    + _note_
        + by specifying underlying type, both `enum` can also be forward declared
    + `enum` has an _integral underlying type_
        + compiler may choose `char`
        ```cpp 
        enum Color {black, white, red};     
        ```
        + compiler may choose range from `0` to `0xFFFFFFFF`
        ```cpp 
        enum Status {
            good = 0;
            failed = 1;
            incomplete = 100;
            corrupt = 200;
            indeterminate = 0xFFFFFFFF
        };
        ```
        + generally, 
            + _size_: compiler chooses the smallest underlying type
            + _speed_: compiler may not choose the smallest underlying type but is optmized for speed
    + _C++98 `enum`_ cant forward declare
        + increase compilation dependencies
            + every part of `.c` file may depend on a widey used `enum` 
            + change to the `enum` causes all files to recompile
        + _can specify underlying type in definition_
            ```cpp 
            enum class Status: std::uint32_t {
                good = 0,
                ...
            };
            ```
        + _so maybe forward declared_
            ```cpp 
            enum Color: std::uint8_t;
            ```
    + _C++11 `class enum` reduce compilation dependencies_
        ```cpp      
        enum class Status;                  // forward declaration
        void continueProcessing(Status s);  // use fwd-declared enum
        ```
        + _note_
            + no need to recompile header 
            + impl no need to recompile as long as the enum changed is not used
        + _underlying type always known_    
            ```cpp 
            enum class Status;                  // underlying type is int 
            enum class Status: std::uint32_t;   // custum underlying type
            ```
+ _unscoped enum usage_
    ```cpp 
    using UserInfo = std::tuple<std::string, std::string, std::size_t>;

    // with get<i>
    UserInfo uInfo;
    auto val = std::get<1>(uInfo);      

    // with get<enum>
    enum UserInfoFields { uiName, uiEmail, uiReputation };
    auto val = std::get<uiEmail>(uInfo);
    ```
    ```cpp 
    enum class UserInfoFields { uiName, uiEmail, uiReputation };
    auto val = 
        std::get<static_cast<std::size_t>(UserInfoFields::uiEmail)>(uInfo);
    ```
    + _note_
        + works by implicit conversion from `UserInfoFields` to `std::size_t`
        + `enum class` is substantially more verbose
+ _templatized `get<enum class>`_
    + avoids namespace pollution
    + prevents inadvertent conversions invovling enumerators
    + and allow access to tuple fields
    ```cpp 
    template<typename E>
    constexpr auto
        toUType(E enumerator) noexcept
    {
        return static_cast<std::underlying_type_t<E>>(enumerator);
    }
    ```
    ```cpp 
    auto val = std::get<toUType(UserInfoFields::uiEmail)>(uInfo);
    ```
+ _summary_
    + C++98 `enum` are _unscoped enum_
    + enumerators of scoped `enum` visible only within `enum`, and only allows conversino with a cast 
    + both scoped/unscoped `enum` 
        + supports specification of underlying type. 
            + `int` is default underlying type for `enum class`
            + unscoped `enum` does not have default underlying type
        + hence can be forward declared
            + scoped `enum` by default 
            + unscoped `enum` only if specifying an underlying type


--- 
#### Item 11: Prefer deleted functions to private undefined ones