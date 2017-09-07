# Chapter 5 Rvalue reference, Move Semantics and Perfect Forwarding


+ _move semantics_
    + make it possible for compiler to replace expensive copying operations with less expensive moves
    + move constructor and assignment operator gives control over semantics of moving 
    + enables creation of move-only types,
        + `unique_ptr`, `future`, `thread`
+ _Perfect forwarding_
    + make it possible to write function templates that take arbitrary arguments and forward them to other fucntions such that the target function receive exactly the same arguments as were passed to the forwarding function
+ _rvalue reference_ 
    + underlying glue tying 2 features above together
    + a language mechanism and makes both move semantics and perfect forwarding possible
+ _keep in mind_    
    + a parameter is always a `lvalue` even if its type is rvalue reference 
    ```cpp
    void f(Widget&& w);
    ```
    + _note_
        + `w` is `lvalue` 
        + `w`'s type is rvalue-reference-to-`Widget`


#### Item 23: Understand `std::move` and `std::forward`

+ `move` and `forward`
    + _surprises_
        + `move` does not move anything 
        + `foward` does not forward anything
    + _both_ 
        + do nothing at runtime 
        + generates no executable code (not a single byte!)
        + are merely function templates and performs cast
    + `move`
        + unconditionally casts arguments to `rvalue`
    + `forward` 
        + cast to `rvalue` only if a particular condition is met 
+ `move`
    + _sample impl_
        ```cpp 
        // C++11
        template<typename T>
        typename remove_reference<T>::type&& 
        move(T&&param)
        {
            using ReturnType = 
                typename remove_reference<T>::type&&;
            return static_cast<ReturnType>(param);
        }

        // C++14
        template<typename T>
        decltype(auto) move(T&& param)
        {
            using ReturnType = remove_reference_t<T>&&;
            return static_cast<ReturnValue>(param);
        }
        ```
        + _note_
            + function takes a _universal reference_ and returns a reference to the same object
            + `remove_reference` 
                + if `T` happens to be lvalue reference, `T&&` would become lvalue reference `T&` 
                + `remove_reference` guarantees that _`move` returns a rvalue reference_
            + Note rvalue reference returned from functions is a rvalue, so `move` cast arguments to rvalue
    + `rvalue`
        + candidates for moving 
        + _usually_ that is...
    ```cpp 
    class Annotation {
        public:
            explicit Annotation(std::string text);  // copied
    };
    ```
    ```cpp
    class Annotation {
        public:
            // read only
            explicit Annotation(const std::string text);  // copied
    };
    ```
    ```cpp
    namespace std{
        class string {
            public:
                string(const string& rhs);  // copy ctor
                string(string&& rhs);       // more ctor
        }
    }
    class Annotation {
        public:
            // move in constructor
            explicit Annotation(const std::string text)
                : value(std::move(text)){}      
    }; 
    ```
    + _note_
        + `text` is copied! (instead of moved) to `value`
        + because
            + `text` is lvalue `const string`
            + `std::move(text)` is rvalue `const string`
            + during type induction, 
                + _cannot be passed to move ctor for `string`_ since move constructor takes rvalue reference to non-`const` `std::string` 
                + _will be passed to copy ctor for `string`_ since lvalue-reference-to-const is permitted to bind to a `const` rvalue
            + so _copy constructor_ is invoked during member initialization
                + even though `text` is cast to `rvalue`
    + _lesson_ 
        + dont declare objects `const` if want to be able to move them
            + as move request on `const` objects are silently transformed to copy operations 
        + `std::move` not only doesn't actually move anything, it doesn't even guarantee that object its casting will be eligible to be moved
+ `forward`
    + a _conditional cast_ to `rvalue`
    + _use case_
        + take universal reference parameter in function templates
    ```cpp 
    void process(const Widget& lvalArg);        // process lvalue
    void process(Widget&& rvalArg);             // process rvalue

    template<typename T>
    void logAndProcess(T&& param)
    {
        auto new = std::chrono::system_clock::now();

        makeLOgEntry("Calling 'process'", now);
        process(std::forward<T>(param));
    }

    Widget w;
    logAndProcess(w);               // call with lvalue
    logAndProcess(std::move(w));    // call with rvalue
    ```
    + _note_
        + 


    