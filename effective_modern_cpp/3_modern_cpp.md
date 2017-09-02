

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



+ _motivation_ 
    + prevent client from calling functions C++ automatically generates
        + _copy ctor_
        + _copy assignment operator_
+ _C++98 private undefined functino_
    + `basic_ios`
        + copying `istream` and `ostream` is not desirable, since not clear what such operations should do
    ```cpp 
    template<class charT, class traits = char_traits<charT>>
    class basic_ios: public ios_base {
        public: 
            ...
        private:
            basic_ios(const basic_ios&);        // copy ctor
            basic_ios& operator=(const basic_ios&); // copy assign
    }
    ```
    + _disable by_ 
        + _declare functions as `private`_, and
            + prevent clients from calling them 
        + _dont define them_
            + if code still has access to them, linking will fail due to missing function definition
+ _C++11: deleted function_
    + use `= delete` to mark member function as __deleted function__
        + `friend` function cant use it as well
        ```cpp 
        template<class charT, class traits = char_traits<charT>>
        class basic_ios: public ios_base {
            public:
                basic_ios(const basic_ios&) = delete;
                basic_ios& operator=(const basic_ios&) = delete;
        }
        ```        
    + __any__ function can be deleted, (while only member function may be private)
        ```cpp 
        // vaguely numeric type implicitly converted to int
        bool isLucky(int number);       
        // explicitly disable such overloads 
        bool isLucky(char) = delete;        // reject chars
        bool isLucky(bool) = delete;        // rejects bools    
        bool isLucky(double) = delete;      // rejects doubles/flats

        if(isLucky('a'))        // error, call to deleted function
        if(isLucky(true))       // error!
        if(isLucky(3.5f))       // error!
        ```
    + _deleted function can prevenrt use of template instantiation that should be disabled_
        ```cpp 
        template<typename T>
        void processPointer(T* ptr);

        template<>
        void processPointer<void>(void *) = delete;
        template<>
        void processPointer<char>(char *) = delete;
        template<>
        void processPointer<const void>(void *) = delete;
        template<>
        void processPointer<const char>(char *) = delete;

        // and other char types... std::wchar_t, std::char16_t
        ```
        + _note_
            + base template should reject 
                + `void *`: no way to dereference them
                + `char *`: decayed from C-style strings
        + _cannot be solved with by declaring template specialization `private`_ 
            + because template specialization must be written at namespace scope, not class scope,
                + so cant declare `private` and not define template specialization 
            ```cpp 
            // C++98
            class Widget {
                public:
                    template<typename T>
                    void processPointer(T *ptr){
                        ...
                    }
                private:
                    template<>
                    void processPointer<void>(void*);   // error!
            };
            ```
            ```cpp 
            // C++11
            class Widget {
                public:
                    template<typename T>
                    void processPointer(T* ptr){
                        ...
                    }
            };

            template<>
            void Widget::processPointer<void>(void *) = delete;
            ```
+ _summary_
    + prefer _deleted fucntion_ to _privately undefined ones_
    + any function may be deleted, including 
        + _non-member functions_
        + _template instantiation_ 


--- 

#### Item 12: Declaring overriding functions `override`

+ _motivation_ 
    + virtual function implementations in derived class _overrides_ the implementations of their base class counterparts
    + makes it possible to _invoker derived class function through base class interface_
        ```cpp 
        class Base {
            public:
                virtual void doWork();
                ...
        };

        class Derived: public Base {
            public:
                virtual void doWork();      // overrides Base::doWork
        };

        // Creates base class pointer to derived class object
        std::unique_ptr<Base> upb = std::make_unique<Derived>();
        // call doWork via base class ptr, derived class function invoked
        upb->doWork();         
        ```
+ _Requirements for overriding to occur_
    + base class function must be virtual 
    + base and derived function name must be identical (except destructor)
    + parameter types of base and derived functions must be identical 
    + the `const`ness of the base and derived functions must be identical 
    + the return type and exception specifications of base and derived functions must be compatible
    + (_added in C++11_) the functions' _reference qualifiers_ must be identical
+ _reference qualifiers_
    + limit use of member function `lvalue` only or `rvalue` only 
    ```cpp 
    class Widget{
        public: 
            void doWork() &;    // apply only when *this is lvalue
            void doWork() &&;   // apply only when *this is rvalue
    };

    Widget makeWidget();        // factory. return rvalue
    Widget w;                   // normal object (lvalue)

    w.doWork();                 // call Widget::doWork &
    makeWidget().doWork()       // calls Widget::doWork &&
    ```
    + _note_ 
        + `func() &` impose that `*this` is `lvalue`    
        + `func() &` impose that `*this` is `rvalue`
    ```cpp 
    class Widget {
        public:
            using DataType = std::vector<double>;
            DataType & data() {
                return values;
            }
            DataType data() && {
                return std::move(vaules);
            }
        private:    
            DataType values;
    };
    ```
    ```cpp 
    Widget w;
    auto vals1 = w.data();      // calls Widget::data() &
                                // copy w.values into vals1
                                // since returning lvalue reference
                                // vals1 copy constructed
    Widget makeWidget();
    auto vals2 = makeWidget().data();   // calls Widget::data() &&
                                        // vals2 move constructed
    ```
    + _note_
        + if `*this` is `rvalue` we can avoid extra copy and `move` the `data` inside of `rvalue` widget.
+ _problem_ 
    + easy to make mistake, 
    + may compile, but may not override as intended
    ```cpp 
    class Base {
        public:
            virtual void mf1() const;
            virtual void mf2()(int x);
            virtual void mf3() &;
            void mf4() const;
    };
    class Derived: public Base {
        public:
            virtual void mf1();     
            virtual void mf2(unsigned int x);
            virtual void mf3() &&;
            void mf4() const;
    }
    ```
    + _note_ all 4 member function do not override
        + `mf1` `const`ness different 
        + `mf2` has different argument types 
        + `mf3` has different referene qualifier
        + `mf4` is not declared `virtual` in `Base`
+ _Declase `override` in derived member_ 
    ```cpp 
    class Derived: public Base {
        virtual void mf1() override;
        virtual void mf2(unsigned int x) override;
        virtual void mf3() && override;
        virtual void mf4() const override;
    }
    ```
    + _note_ 
        + gives compiler warnings...
    + _`override` and `final` are contextual keyword_
        + meaning they are reserved, but only in context where they occur at end of a member function declaration
+ _summary_
    + delcaring overriding functions in derived class with `override`
    + member function reference qualifiers makes it possible to reat `lvalue` and `rvalue` objects `*this` differently


--- 

#### Item 13: Prefer `const_iterator`s to `iterator`s

+ _motivation_ 
    + `const_iterator`
        + equivalent to pointer-to-`const`
        + point to values that may not be modified
    ```cpp
    std::vector<int> values;
    std::vector<int>::iterator it = 
        std::find(values.begin(), values.end(), 1983);
    values.insert(it, 1998);
    ```
    + _note_
        + `it` not modifying `values` so should be `const` 
    ```cpp 
    // C++98
    typedef std::vector<int>::iterator IterT;
    typedef std::vector<int>::const_iterator ConstIterT;

    std::vector<int> values;

    ConstIterT ci = 
        std::find(static_cast<ConstIterT>(values.begin()),
            static_cast<ConstIterT>(valules.end()),
            1983);
    values.insert(static_cast<IterT>(ci), 1998);        // wont compile
    ```
    + _note_
        + locations for insertions could specified by `iterator` only `const_iterator` not acceptable
        + problem arises because `const_iterator` cannot be converted to `iterator`...
+ _solution_ 
    + `const_iterator` is easy to get/use
        + `cbegin()`, `cend()` produce `const_iterator`s
    + STL member function that use iterator to identify positions (i.e. `insert`, `erase`) uses `const_iterator`
        ```cpp 
        auto it = std::find(values.cbegin(), values.cend(), 1983);
        values.insert(it, 1998);
        ```
    + _extending support to_ 
        + container-like data structure as non-member function 
        + 3rd party library with free functions only 
        ```cpp 
        // C++11 Error! C++14 OK
        template<typename C, typename V>
        void findAndInsert(C& Container, 
                            const V& targetVal, 
                            const V& insertVal)
        {
            using std::cbegin;
            using std::cend;

            auto it = std::find(cbegin(container),
                                cend(container),
                                targetVal);
            container.insert(it, insertVal);
        }
        ```
        + _note_
            + C++14 added free function `cbegin()`, ...
        ```cpp 
        // impl of cbegin() in C++11
        template<typename C>
        auto cbegin(const C& container) -> decltype(std::begin(container))
        {
            return std::begin(container);
        }
        ```
        + _note_
            + works because `std::begin()` on `const` container yields a `const_iterator`
            ```cpp 
            iterator begin() noexcept;
            const_iterator begin() const noexcept;
            ```
+ _summary_
    + prefer `const_iterator` to `iterator`s
    + in maximally generic code, prefer non-member version of `begin()`, `end()`, `rbegin()` over their member function counterparts

--- 

#### Item 14: Declare functions `noexcept` if they won't emit exceptinos



+ _motivation_ 
    + _exception specification_ 
        + _C++98_
            + have to summarize exception types a function might emit
            + breaks code if change it
            + doesnt worth the trouble
        + _C++11_
            + truly meaningful info is if it had any
            + either a function might emit an exception or it wouldnt
+ `noexcept`
    + functions that guarantee they won't emit exceptions 
+ _insentives to `noexcept`_
    + allows compiler to generate better object code
    ```cpp 
    int f(int x) throw();       // no exception in C++98
    int f(int x) noexcept;      // no exception in C++11
    ```
    + _note_ at runtime 
        + `throw()`
            + call stack unwound to `f`'s caller 
            + program terminates '
        + `noexcept`
            + stack possibly unwound before program execution terminatess
            + _optimizable_, since compiler 
                + do not need to keep runtime stack in an unwindable state if an exception would propagate out of the function
                + do not need to ensure objects in `noexcept` are destroyed in reverse order of construction
    ```cpp 
    ret func(param) noexcept;       // most optimizable
    ret func(param) throw();        // less optimizable 
    ret func(param);                // less optimizable
    ```
+ _A case study_
    ```cpp
    // C++98
    std::vector<Widget> vw;
    Widget w;
    vw.push_back(w);      // add w to vw
    ```
    + `push_back()`
        + if size equal to capacity, allocates space and copy elements from existing memory to new memory
            + _strong exception safety guarantee_ since
            + if exception thrown during copy, state of original vector remains unchanged
        + if want to support `move` semantics during reallocation 
            + _risks violating exception safety guarantee_ since
            +  if exception thrown during move, state of of original vector has been modified, and maybe impossible to rollback
    + _idea_
        + move if you can, but copy if you must 
        + replace calls to copy operations with move operations in C++11 only if move operation is known to not emit exceptions
        + i.e. 
            + `vector::reserve`, `deque::insert`, `swap`
        + done by checking if `move` is declared `noexcept`
+ _conditionally `noexcept`_
    _impl of `swap` for array and `std::pair`_
    ```cpp 
    template<class T, size_t N>
    void swap(T (&a)[N],
            T (&b)[N]) noexcept(noexcept(swap(*a, *b)));
    
    template<class T1, class T2>
    struct pair {
        void swap(pair& p) 
            noexcept(noexcept(swap(first, p.first)) && 
                    noexcept(swap(second, p.second)));
    }
    ```
    + _note_
        + expression inside `noexcept` determines if the function is `noexcept` or not
        + _after-thought_
            + the fact that swapping higher-level data structures can generally be `noexcept` only if swapping their lowr-level constituents is `noexcept` should motivate implementer to offer `noexcept swap()` function whenever possible
    + important to maintain correctness of `noexcept` first, otherwise
        + exception escape from a `noexcept` code
    + hence most function are _exception-neutral_
        + but `move` and `swap` functions for higher level classes may worth while to implement `noexcept` if possible
+ _some function are `noexcept` by language specification_
    + all memory deallocation functions and destructors are implicitly `noexcept`
+ _narrow contract function may declare `noexcept`_ 
    + _wide contract_ 
        + functions with no preconditions 
        + maybe called regardless state of program
        + impose no contraints on arguments supplied
    + _narrow contract_
        + if a precondition is violated, results undefined
    + _narrow contract function may declare `noexcept`_
        + since `f` has no obligation to check precondition 
        + `f` assumes all precondition satisifed
+ _inconsistensies in `noexcept`_
    ```cpp 
    void setup();
    void cleanup();

    void doWork() noexcept {
        setup()
        ...
        cleanup();
    }
    ```
    + _note_
        + `doWork` is `noexcept` even if it calls non-`noexcept` functions
+ _summary_
    + `noexcept` is part of function's interface, hence caller may depend on it 
    + `noexcept` functions are more optimizable 
    + `noexcept` are particularly valuable for move operations, `swap`, memory deallocation functions, and destructors 
    + most functions are exception-neutral rather than `noexcept`


--- 

#### Item 15: Use `constexpr` whenever possible

+ `constexpr`
    + indicates a value that is not only constant, it's known during compilation 
    + _pitfalls_
        + results of `constexpr` functions cannot be assumed to be `const` 
        + values are not taken for granted available during compilation
+ `constexpr` objects 
    + objects in which they are `const` and have values known at compile time 
        + i.e. value determined during _translation_ (compiling + linking)
        + all `constexpr` objects are `const`, but not all `const` objects are `constexpr`
    + `constexpr` objects maybe placed in read-only memory 
        + useful in embedded systems ... 
    + integral values that are constant and known during compilation can be used in contexts where C++ requires _integral constant expression_  
        + array sizes 
        + integral template arguments 
        + enumerator values
        + alignment specifiers 
    ```cpp 
    int sz;     // non-constexpr variable 

    constexpr auto arraySize1 = sz;     // error! sz's value ont known 
    std::array<int, sz> data1;          // error!, ditto
    ```
    ```cpp 
    constexpr auto arraySize2 = 10;         // compile-time constant
    std::array<int, arraySize2> data2;      // OK
    ```
    + _note_
        + `const` objects does not give such guarantee to be known at compile time
+ `constexpr` function 
    + function that produce 
        + _compile-time constants_ when they are called with compile-time constants
        + _runtime values_ when they are called with values not known until runtime
    ```cpp 
    constexpr 
    int pow(int base, int exp) noexcept;
    
    constexpr auto numConds = 5;
    std::array<int, pow(3, numConds)> results;  
    ```
    ```cpp 
    // C++11
    constexpr int pow(int base, int exp) noexcept 
    {
        return (exp == 0 ? 1 : base * pow(base, exp - 1));
    }
    ```
    + C++11, may container no more than a single executable statement (i.e. one `return`)
        + can use `?:` in place of `if-else`
        + use recursion instead of loops
    
    + C++14
        + looser 
        + limited to taking and returning _literal types_ 
            + types which _can_ have values during compilation 
            + all build-in types except `void`
            + user-defined types with `constexpr` ctor and data member
        ```cpp 
        class Point {
            public: 
                constexpr Point(double xVal = 0, double yVal=0) 
                    noexcept :x(xVal), y(yVal){}

                constexpr double xValue() const noexcept {return x;}
                constexpr double yVaule() const noexcept {return y;}

                constexpr Point midpoint(
                    const Point& p1, const Point& p2) noexcept
                {
                    return {
                        (p1.xValue() + p2.xValue()) / 2,
                        (p1.yValue() + p2.yValue()) / 2
                    };
                }

                // C++14 but not C++11
                constexpr void setX(double newX) noexcept {x=newX;}
                constexpr void setY(double newY) noexcept {y=newY;}
            private:
                double x, y;
        };

        // C++14
        constexpr Pointer reflectino(const Point& p) noexcept{
            Point result;           // non-const Point
            result.setX(-p.xValue());
            result.setY(-p.yValue());
            return result;          // return a copy 
        }

        // all compile time constants
        constexpr Point p1(9.4, 27.7);
        constexpr Point p2(28.8, 5.3);
        constexpr auto mid = midpoint(p1, p2);
        constexpr auto reflectedMid = reflect(mid);
        ```
        + _note_
            + implies we can use `mid.xValue() * 10` (a constant expression) as 
                + _argument to template_
                + _expression specifying values of enum_
            + _Setters in C++11 cannot be declared `constexpr`_
                + `constexpr` function in C++11 is implicitly `const`
                + `constexpr` has `void` return type, which is not a _literal type_
            + _C++14 lifts these restrictions_
+ _summary_
    + `constexpr` objects are `const` and initialized with value known at compilation 
    + `constexpr` functions can produce compile-time results when called with arguments whose values are known during compilaiotn 
    + `constexpr` objects/functions may be used in contexts of non-`constexpr` objects/functions 
    + `constexpr` is part of object/function's interface, so change with caution 

--- 


#### Item 16: Make `const` member functions thread safe
    

+ _motivation_ 
    ```cpp 
    class Polynomial {
        public: 
            using RootsType = std::vector<double>;

        // cache roots, and compute just once
        RootsType roots() const{    
            if(!rootsAreValid){
                rootsAreValid = true;
            }
            return rootVals;
        }

        private:
            mutable bool rootsAreValid{ false };
            mutable RootsType rootVals{};
    }
    ```
    + _note_
        + `mutable`
            + applies to non-static class members of non-reference non-const type and specifies that the member does not affect the externally visible state of the class (as often used for mutexes, memo caches, lazy evaluation, and access instrumentation). 
            + `mutable` members of `const` class instances are modifiable. 
    ```cpp 
    Polynomial p;
    // thread 1                 // thread 2
    auto rootsOfP = p.roots();  auto rootsOfP = p.roots()
    ```
    + _note_
        + _scenario_ 
            + 2 threads simultaneously call `roots` on a `Polynomial` object 
            + fine since `roots` is `const`-member function, representing a read operation
            + but since its actually not read operation, it invovles mutating data member `rootsAreValid` and `rootsVals`
            + so reading/writing cross threads without synchronization, data race...
        + _problem_ 
            + `roots` declared `const` but its not thread safe
            + note `const` is valid since `Polynomial` not changed
    ```cpp 
    class Polynomial {
        public: 
            using RootsType = std::vector<double>;

        // cache roots, and compute just once
        RootsType roots() const{  

            std::lock_guard<std::mutex> g(m);

            if(!rootsAreValid){
                rootsAreValid = true;
            }
            return rootVals;
        }

        private:
            mutable std::mutex m;
            mutable bool rootsAreValid{ false };
            mutable RootsType rootVals{};
    }
    ```
    + _note_
        + `mutex` declared `mutable` since locking and unlocking it are `non-const` member functions, and otherwise within `roots` (`const` function) `m` would otherwise be considered `const`
        + `mutex` is move-only type (not copied)
            + hence `Polynomial` losses ability to be copied as well...
    ```cpp 
    class Pointer {
        public:
            double distanceFromOrigin() const noexcept 
            {
                ++callCounts;
                return std::sqrt((x*x) + (y*y));
            }
        private:
            mutable std::atomic<unsigned> callCount{0};
            double x, y;
    }
    ```
    + _note_
        +  use of `std::atomic` counter, which other threads are guaranteed to see its operations occur indivisibly 
        + `std::atomic` are move-only types 
            + hence `Point` is move only as well, cant copy
        + `std::atomic` are less expensive than mutex acquisition and release
            + so prefer `atomic`s over `mutex`s
    ```cpp 
    class Widget {
        public:
            int magicValue() const
            {
                if(cacheValid) return cachedValue;
                else {
                    auto val1 = expensiveCompute1();
                    auto val2 = expensiveCompute2();
                    cachedValue = val1 + val2;
                    cachedValid = true;
                    return cachedValue;
                }
            }
        private:
            mutable std::atomic<bool> cachedValid{false};
            mutable std::atomic<int> cachedValue;
    }
    ```
    + _problems_ 
        + A thread call `magicValue`, see `cacheValid` is `false`, perform 2 expensive computations 
        + another thread (maybe multiple) sees `cacheValid`, carries out expensive operations that the first just finished...
        + goes against intention of caching
    ```cpp 
    int magicValue() const
    {
        if(cacheValid) return cachedValue;
        else {
            auto val1 = expensiveCompute1();
            auto val2 = expensiveCompute2();
            cachedValid = true;
            cachedValue = val1 + val2;
            return cachedValue;
        }
    }
    ```
    + _note_
        + reversing assignments to `cachedValue` and `cacheValid` is even more problematic 
            + a thread executes just past setting `cacheValid` to `true`
            + a second thread checks `cacheValid` seeing it true, the thread returns `cachedValue`, even though the first thread has not yet made an assignment, the return value is hence incorrect 
    ```cpp 
    class Widget {
        public:
            int magicValue() const
            {
                std::lock_guard<std::mutex> guard(m);
                if(cacheValid) return cachedValue;
                else {
                    auto val1 = expensiveCompute1();
                    auto val2 = expensiveCompute2();
                    cachedValue = val1 + val2;
                    cachedValid = true;
                    return cachedValue;
                }
            }
        private:
            mutable std::mutex m;
            mutable std::atomic<bool> cachedValid{false};
            mutable std::atomic<int> cachedValue;
    }
    ```
    + _takeaway_
        + for a single variable or memory location requiring synchronization, use of `std::atomic` is adequate 
        + once >= 2 variables or memory location require manipulation as a unit, should use `mutex` instead
+ _summary_ 
    + make `const` member function thread safe unless certain they'll never be used in concurrent context 
    + use of `std::atomic` variables may offer better performance than `std::mutex`, but they are suited for manipulation of only a single variable or memory location


--- 

#### Item 17: Understand special member function generation


+ _motivation_ 
    + _special member function_ 
        + functions that C++ is willing to generate on its own
        + _C++98_ 
            + default constructor 
                + if no constructor declared at all
            + destructor 
            + copy constructor 
            + copy assignment operator
        + _C++11_ (additional)
            + move constructor 
            + move assignment operator
        + only generated only if needed
        + implicitly `public` and `inline`
        + non-`virtual` unless function in question is a destructor in a derived class inheriting from a base class with a virtual destructor
    ```cpp 
    class Widget {
        public:
            Widget(Widget&& rhs);
            Widget& operator=(Widget&& rhs);
    }
    ```
    + _move constructor and assignment operator_
        + _generation_ 
            + _generated on demand if_
                + no copy operations are declared in class 
                + no move operations are declared in class 
                + no destructor declared in class
            + _not generated_
                + if defined either move operation
                + if explicitly declares a copy operations
                    + implies something wrong with default memberwise copy, since memberwise move uses memberwise copy, memberwise move probably wrong as well
                + if a class has a user-declared destructor 
                    + stems from _Rule of Three_
                    + destructor -> user-defined copy constructor/assignment operator -> no move constructor/assignment operator generated
            + _copy vs move_ 
                + _copy_
                    + 2 operations independent 
                    + declaring one doesn't prevent compiler from generating the other
                + _move_
                    + 2 operations not independent
                    + declaring either, prevents compiler from generating the other
                    + idea is if define a move constructor/assignment, then not using the defualt memberwise move assginment. Then if something's wrong with memberwise move construction, then probably memberwise move assignment is also wrong.
            + _if generated_
                + then compiler disable the copy operations by deleting them
        + _behavior_ 
            + peform _memberwise moves_ on non-static data members of class 
                + i.e. move constructor move-constructs each non-static data member of class from corresponding member of its parameter `rhs`, 
                + move assignment operator move-assigns each non-staic data-member from its parameter
            + also move-constructs its base class parts
            + _move-construct/move-assign_ 
                + does not guarantee that a move will take place 
                + in reality, 
                    + move operation on data members and base classes that support move operations 
                    + copy operation for those that don't
            + _under the hood_
                + memberwise _move_ entails application of `std::move` to object to be moved, and the result is used using function overload resolution to determine whether a move or a copy should be performed
    + _Rule of Three_
        + if declare any of a copy constructor, copy assignment operator, or destructor, you should declare all three.
        + _observation_ 
            + meaning of copy operation stemmed from class performing some kind of resource management, implies 
                + whenever resource management was being done in one copy operationprobably needed to be done in the other copy operation
                + the class destructor would also be participating in management of resources (releasing it)
            + presence of user-declared destructor
             implies 
                +  simple memberwise copy is unlikely to be appropriate for the copying operations in the class
        + remains valid 
    ```cpp 
    class Widget {
        public:
            ~Widget();

            Widget(const Widget&) = default;
            WIdget& operator=(const Widget&) = default;
    };
    ```
    + _note_
        + `default` indicates compiler-generated functions is correct
    ```cpp 
    class Base {
        public:
            virtual ~Base() = default;  // make dtor virtual 

            Base(Base&&) = default;     // support moving 
            Base& operator=(Base&&) = default;

            Base(const Base&) = default;    // support copying
            Base& operator(const Base&) = default;
    };
    ```
    + _`default` application_ 
        + _applies to polymorphic base class_
            + must have virtual destructors
            + only way is to declare function and prefix it with `virtual`
            + However, user-defined destructor suppress generation of move operations
            + so use `=default` if want movability
        + _applies to copy operations_ 
            + if user-declared move operation 
            + which suppress copy operations generation 
            + so use `=default` if want copyability 
        + _use `=default` to express intention_
    ```cpp 
    class StringTable {
        public:
            StringTable(){
                makeLogEntry("Create StringTable Object");
            }
            ~StringTable(){
                makeLogEntry("Destroy StringTable Object");
            }


        private:
            std::map<int, std::string> values;
    };
    ```
    + _note_
        + addition of logging in ctor/dtor
            + prevents move operations from generated
            + copy opreations unaffected
            + will compile fine, but cause copies to be made since no longer move-enabled
        + _solution_ 
            + declare copy/move operations `=default`
    + _Rules_ 
        + _default constructor_ 
            + same rules in C++98, generated only if class contains no user-declared constructors 
        + _destructor_ 
            + essentially same rules as C++98, difference is destructors are `noexcept` by default, `virtual` if base class destructor is virtual 
        + _copy constructor_ 
            + same runtime behavior as C++98, membewise copy construction of non-static members. 
                + _Generated_ only if class lacks user-declared copy constructor
                + _Deleted_ if class declares a move operation 
            + generation in a class with user-declared copy constructor/destructor is deprecated
        + _copy assignment operator_ 
            + same runtime behavior as C++98, memberwise copy assignment of non-static data member, 
                + _generated_ only if class lacks copy assignment operator 
                + _deleted_ if class declares a move operation 
        + _move constructor / move assignment operator_ 
            + Each performs memberwise moving of non-static data members. 
                + _generated_ only if class contains no user-declared copy operations, move operations, or destructor
    + _member function templates never suppress generation of special member functions_
        ```cpp 
        class Widget {
            template<typename T>
            Widgget(const T& rhs);

            template<typename T>
            WIdget& operator=(const Widget& rhs);
        }
        ```
        + _note_ 
            + compiler still generate copy/move operations for `Widget`
+ _summary_

            
