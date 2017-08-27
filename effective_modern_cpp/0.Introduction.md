
###  0 Introduction 


+ _move semantics_ 
    + `rvalue` 
        + objects eligible for move 
        + usually temporary objects returned from functions
    + `lvalue`
        + not eligible for move,
        + can be refer to 
            + by name 
            + by pointer or lvalue reference
    + _heuristics_
        + ask if you can take its address
        + if yes, then `lvalue`, otherwise `rvalue`
        + type of an expression is independent of whether the expression is `lvalue` or `rvalue`
    ```cpp
    class Widget {
        public:
            Widget(Widget&& rhs);       
            // rhs is lvalue, though it has an rvalue reference type
    }
    ```
    + _note_
        + a parameter of rvalue reference type is itself an lvalue
        + since its valid to take `rhs`'s address inside `Widget`'s move constructor
            + so `rhs` is `lvalue`, even if its type is `rvalue reference`
        + in fact, all parameters are `lvalue`
    + _conventions_ 
        + `rhs` is a good parameter name for `move` operations (move constructor and move assignment) and `copy` operations (copy constructor and copy assignment operator)
    ```cpp
    void someFunc(Widget w);        // w pass by value 

    Widget wid
    someFunc(wid);      // w is a copy of wid created via copy construction
    someFunc(std::move(wid)); // w is a  copy of wid created via move construction
    ```
    + _note_
        + _copy_
            + when an object is initialize with another object of same type, the new object is a _copy_ of the initializing object, even if the copy was created via the move constructor
        + _copies of_
            + `rvalue`: usually _move constructed_
            + `lvalue`: usually _copy constructed_
        + _implication_
            + not possible to say how expensive it was to construct a copy (i.e. `w` in `someFunc(Widge w)`) without knowing whether `rvalue` or `lvalue` are passed in
        + _argument vs parameter_
            + _argument_
                + expression passed at call site 
                + may be `lvalue` or `rvalue`
            + _parameter_
                + initialized with argument to function 
                + always `lvalue`
        + _perfect forwarding_  
            + an argument passed to a function is passed to a second function such that the original argument's rvalueness or lvalueness is preserved
    + _exception safety (function)_
        + _basic guarantee_
            + program invariant holds (no data structure corrupted)
            + no resources leaked
        + _strong guarantee_
            + program state remains as it was prior to function call
    + _function object_
        + an object of a type supporting an `operator()`
        + generally, anything that can be invoked with `functionName(args)`
            + functor
            + function pointer
            + lambda
        + _closures_
            + function object created via lambda
    + _template function/class_
        + templates that generates function/class
    + _function/class templates_
        + functions/classes generated from function/class templates
    ```cpp
    extern int x;               // object declaration
    class Widget;               // class declaration 
    bool func(const Widget& x)  // function declaration 
    enum class Color;           // scoped enum declaration
    ```
    + _declaration_
        + introduce _name_ and _type_ without giving details
    ```cpp 
    int x;                          // object definition
    class Widget {                  // class definition
        ...
    };
    bool func(const Widget& x){     // function definition
        return w.size() < 10;
    }
    enum class Color{               // scoped enum definition
        Yellow, Red, Blue
    };
    ```
    + _definition_
        + provide _storage location_ or _implementation details_
    + _definition is declaration_
    + _signature_
        + part of a function declaration that specifies 
            + _parameter_ 
            + _return type_
        + not including 
            + `noexcept` or `constexpr`
    + _undefined behavior_
        + runtime behavior unpredictable
