

# Chapter 6 Lambda Expressions 


+ _lambdas as a game-changer_
    + STL algorithms can now be employed more easily 
        + `find_if`, `remove_if`, `count_if`
        + `sort`, `nth_element`, `lower_bound`
    + possible to create custom deleters quickly
        + `unique_ptr` and `shared_ptr`
    + facilitages on-the-fly specification of 
        + callbacks 
        + interface adaption functions 
        + context-specific functions 
        + one-off calls 
+ _definitions 
    + _lambda expression_ (compile-time)
        + an expression 
    + _closure_ (runtime)
        + the runtime object created by a lambda
        + holds copies or references to the captured data
        + _closures maybe copied_
            ```cpp 
            int x;
            auto c1 = [x](int y){return x * y > 55;};
            // copies of lambda
            auto c2 = c1;
            auto c3 = c2;
            ```
    + _closure class_ (compile-time)
        + a class from which a closure is instantiated. 
        + Each lambda causes compiler to generate a unique closure class

--- 

#### Item 31: Avoid default capture modes

+ _default capture mode_
    + _by-reference_
    + _by-value_ 
+ _avoid default by-reference capture_
    + causes a closure to contain a reference to a local variable or to a parameter that's available in the scope where the lambda is defined.
    + _risk of tangling_
        + if lifetime of _closure_ created from that _lambda_ exceeds lifetime of the local variable / parameter, reference in the closure will dangles
    ```cpp 
    // a container of filter functions
    using FilterContainer = std::vector<std::function<bool(int)>>;

    FilterContainer filters;

    filters.emplace_back(
        [](int value){ return value % 5 == 0;}
    )

    // want runtime specification of divisor (5)
    void addDivisorFilter(){
        auto calc1 = computeSomeValue1();
        auto calc2 = computeSomeValue2();

        auto divisor = computeDivisor(calc1, calc2);

        // danger!, ref to divisor will dangle
        filters.emplace_back([&](int value){
            return value % divisor = 0;
        });
    }
    ```
    + _note_
        + `divisor` cease to exists once function `addDivisorFilter()` finishes
        + undefined behavior if try to use the closure at runtime
    ```cpp 
    // reference still dangles
    filters.emplace_back(
        [&divisor](int value){
            return value % divisor == 0;
        }
    )
    ```
    + _note_
        + still dangles
        + but easier to see that 
            + _viability of lambda is dependent on `divisor`'s lifetime_
            + i.e. `divisor` should live as long as lambda's closures
    + _use case_
        + a closure that will be _used immediately_ (i.e. passing to STL algorithm) and won't be copied
            + so no risk that references it holds will outlie the local variables / parameters in enviornment where lambda is created
    ```cpp
    template<typename C>
    void workWithContainer(const C& container)
    {
        auto calc1 = computeSomeValue1();
        auto calc2 = compureSomeValue2();

        auto divisor = computeDivisor(calc1, calc2);

        using ContElemT = typename C::value_type;

        using std::begin;
        using std::end;

        if(std::all_of(
            begin(container), end(container),
            [&](const ContElemT& value){
                return value % divisor == 0;
            }
        )){
            // all values are multiple of divisor
        } else { ... }
    }
    ```
    ```cpp 
    // C++14
    if(std::all_if(begin(container), end(container), [&](const auto& value){
        return value % divisor == 0;
    }))
    ```
    + _note_
        + code is safe 
        + but if lambda is used into a context where the closure outlives `divisor`, continues to dangle
        + still better to list local variables and parameters that a lambda depends on 
+ _avoid default by-value capture_
    + _risk of copy pointer_ 
        + copy pointer by value, does not prevent ouside lambda from deleting the pointer and causing the copies to dangle
        + i.e. arises when want to capture data members... `this` implicitly captured
    ```cpp 
    class Widget {
        public:     
            void addFilter() const;
        private:
            int divisor;
    };

    void Widget::addFilter() const {
        filters.emplace_back(
            []          // wont compile
            [=]         // divisor not captured
            [divisor]   // wont compile
            (int value) {
                return value % divisor == 0;
            }
        );
    }
    ```
    + _behavior_ 
        + `[]` or `[divisor]`: wont compile
        + _problem_ 
            + capture applies to non-`static` local variables (including parameters) visible in the scope where lambda is created
            + `divisor` is not a local variable but a class member of `Widget` 
        + `[=]`: compiles but not `divisor` captured
        + _problem_
            + _raw pointer `this` is captured by `[=]`, not `divisor`_
            + closures of this lambda is tied to the lifetime of `Widget` whose `this` pointer they contain a copy of 
            ```cpp 
            void Widget::addFilter() const{
                auto currentObjectPtr = this;
                filters.emplace_back(
                    [currentObjectPtr](int value){
                        return value % currentObjPtr->divisor == 0;
                    }
                )
            }
            ```
            ```cpp 
            using FilterContainer = 
                std::vector<std::function<bool(int)>>;
            
            FilterContainer filters;

            void doSomeWork(){
                auto pw = std::make_unique<Widget>();
                pw->addFilter();
            } 
            // destroy Widget, filters now holds dangling pointer
            ```
            + _here lifetime of the by-value pointer `this` exceeds lifetime of the object `this` points to_
    ```cpp 
    // C++11
    void Widget::addFilter() const{
        auto divisorCopy = divisor;
        filters.emplace_back(
            [divisorCopy](int value)
            {
                return value % divisorCopy == 0;
            }
        );
    }
    ```
    ```cpp 
    // C++14: generalized lambda capture
    void Widget::addFilter() const {
        filters.emplace_back(
            [divisor=divisor](int value){
                return value % divisor == 0;
            }
        );
    }
    ```
    + _solution_
        + _make a local copy of data member you want to capture and then capturing the copy_
        + `[=]`
            + works
            + but still should avoid since don't know semantically if `this` is captured or not (which we want to avoid)
    + _risk of thinking that closures are self-contained and insulates changes to data outside the closure_
        + _normally_
            + `[=]` is self contained, and insulates to data changes
        + _lamda depends on_
            + local variables
            + parameters 
            + objects with _static storage duration_ 
        + `static` variables
            + can be used inside lambdas 
            + but not captured by lambdas
            + changes to `static` variables reflect in lambda (since not captued by-value!)
    ```cpp 
    void addDivisorFilter()
    {
        static auto calc1 = computeSomeValue1();
        static auto calc2 = computeSomeValue2();

        static auto divisor = computeDivisor(calc1, calc2);

        filters.emplace_back(
            [=](int value){     // captures nothing!
                return value % divisor == 0;
            }
        );

        ++divisor;      // modified
    }
    ```
    + _note_
        + `[=]` captuers nothing!
        + lambda not self-contained! 
            + i.e. after `++divisor`, lambdas will exhibit new behaivor
        + _contradiction to what by-value capture seems to imply_
        + so use `[]` instead to avoid such misconception 
+ _summary_ 
    + defualt by-reference capture can lead to dangling references 
    + default by-value capture is 
        + susceptible to dangling pointrs (especially `this`), and 
        + misleadingly suggests that lambdas are self-contained (if using `static` variables)

--- 

#### Item 32: Use init capture to move objects into closures


+ _motivation_ 
    + _scenario_ 
        + want to capture move-only objects (`unique_ptr`, `future`)
        + want to capture cheap to move objects (most STL containers)
    + _supported natively in C++14_
+ _init capture (generalized lambda capture)_ 
    + _C++11_ 
        + not possible to capture result of an expression 
    + _C++14_ 
        + OK with init capture
    + `name = init-expr`
        + `name`
            + _name of a data member_ in the closure class generated from the lambda 
            + scoped to the _closure class_
        + `init-expr`
            + _an expression_ that initializes the data member
            + same scope as where _lambda is defined_
    ```cpp 
    // C++14
    class Widget {
        public:
            bool isValidated() const;
            bool isProcessed() const;
            bool isArchived() const;
        private:
            ...
    };

    auto pw = std::make_unique<Widget>();
    auto func = [pw = std::move(pw)]
        {
            return pw->isValidated() && pw->isArchived();
        };
    ```
    + `pw=std::move(pw)`
        + create data member `pw` in closure 
        + initialize that data member with result of applying `std::move()` to a local variable `pw`
    ```cpp 
    // C++14
    auto func = [pw = std::make_unique<Widget>()]
    {
        return pw->isValidated() && pw->isArchived();
    };
    ```
    + _direct initialization of closure class data member is possible with init capture_
        + avoid generating local variable before lambda
    ```cpp 
    class IsValAndArch {
        public: 
            using DataType = std::unique_ptr<Widget>;

            explicit isValAndArch(DataType&& ptr)
                : pw(std::Move(ptr)) {}
            
            bool operator()() const {
                return pw->isValidated() && pw->isArchived();
            }

        private:
            DataType pw;
    };

    auto func = isValAndArch(std::make_unique<Widget>());
    ```
    ```cpp 
    std::vector<double> data;
    auto func = 
        std::bind(
            [](const std::vector<double>& data)
            { ... }, 
            std::move(data)
        );
    ```
    ```cpp 
    auto func = 
        std::bind(
            [](std::vector<double>& data) mutable
            { ... },
            std::move(data)
        );
    ```
    + _C++11 workaround_ 
        + create a function object
        + or use `bind`
            + arguments are 
                + _copy-constructed_ if `lvalue`
                + _move-constructed_ if `rvalue` 
    + `mutable`
        + by default, `operator()` member function inside _closure class_ generated from labmda is `const`
            + in effect, all data members in the closure are `const` within body of lambda
            + so lambda's parameter is ref-to-`const`, preventing modification 
        + if `mutable` `operator()`, would not be declared `const`
    + _summary_ 
        + not possible to move-construct object into C++11 closure, but it is possible to move-construct an object into C++11 bind object 
        + emulate move-capture in C++11 by
            + move-constrct object into a bind object 
            + pass move-constructed object to lambda by reference
        + lifetime of bind object is same as that of closure, possible to treat objects in bind object as if they were in closure
    ```cpp 
    // C++11
    auto func = std::bind(
        [](const std::unique_ptr<Widget>& pw)
        {
            return pw->isValidated() 
                && pw->isArchived();
        },
        std::make_uniqe<Widget>()
    );
    ```
+ _summary_ 
    + use C++14 init capture to move objects to closure
    + C++11, emulate init capture by hand-written function object class, or `std::bind`

--- 

#### Item 33: Use `decltype` on `auto&&` parameters to `std::forward` them 

+ _generic lambdas_
    + lambda that use `auto` in parameter specifications 
    ```cpp 
    // generic lambda
    auto f = [](auto x){
        return func(normalize(x));
    };

    // closure class
    class someCompilerGeneratedClassName {
        public:
            template<typename T>
            auto operator()(T x) const {
                return func(normalize(x));
            }
    };
    ```
    + _note_
        + `operator()` in lambda's closure class is a template
        + does forwrading to `normalize` 
        + Wrong if 
            + `normalize` treat `lvalue` `rvalue` differently,
            + parameter is always `lvalue`
    ```cp 
    auto f = [](auto&& x)
    { 
        return func(normalize(std::forward<???>(x)));
    };
    ```
    + _note_
        + perfect foward `x`
        + _problem_ 
            + normally, template function taking `T` so just write `forward<T>`
            + but in generic lambda, no type parameter `T` available to client (although there is a `T` in the templatized `operator()` inside closure class)
    + _`decltype`_ 
        + `decltype(x)` will produce a lvalue reference type if `x` is lvalue 
        + `decltype(x)` will produce a rvalue reference if `x` is rvalue
    ```cpp 
    template<typename T>
    T&& forward(remove_reference_t<T>& param)
    {
        return static_cast<T&&>(param);
    }
    ```
    + _`std::forward` behavior_ 
        + type argument is lvalue reference if to indicate lvalue 
            + corresponds to `decltype`, both generates `T&`
            + lvalue behave as expected 
        + non-reference to indicate rvalue
            + different from `decltype` (`T&&`) vs the desired deduced type (`T`)
    ```cpp 
    // instantiation when T is Widget (forward + rvalue)
    Widget&& forward(Widget& param)
    {
        return static_cast<Widget&&>(param);
    }

    // instantiation when T is Widget&& (decltype + rvalue)
    Widget && forward(Widget& param)
    {
        return static_cast<Widget&&>(param);
    }
    ```
    + _note_ 
        + _instantiation with rvalue reference type (`T&&`) yields same result as instantiating it with non-reference type (`T`)_
        + _consequence_ (for `rvalue`) 
            + `decltype(x)` yields _rvalue reference type_ when rvalue is passed as argument to lambda parameter `x`
            + but the result of forwarding yields the same type (i.e. rvalue `T&&`)
    ```cpp 
    auto f = 
        [](auto && param)
        {
            return func(normalize(
                std::forward<decltype(param)>(param)
            ))
        };
    
    auto f = 
        [](auto&&...params)
        {
            return func(normalize(
                std::forward<decltype(params)>(params)...
            ))
        }
    ```
    + _solution_ 
        + a perfect-forwrading lambda
+ _summary_ 
    + use `decltype` on `auto&&` parameter to `std::forward` them

--- 

#### Item 34: Prefer lambdas to `std::bind`

+ `bind`
    + C++11 successor to C++98 `std::bind1st` `std:bind2nd`
    + returns a function object, i.e. _bind object_
+ _lambdas are more readable than `std::bind`_
    ```cpp
    using Time = std::chrono::steady_clock::time_point;
    using Duration = std::chrono::steady_clock::duration;

    enum class Sound { Beep, Siren, Whistle};

    void setAlarm(Time t, Sound s, Duration d);

    // function object for allowing sounds specified for 30s alarm go off in 1hr after its set
    ```
    ```cpp 
    // C++11
    auto setSoundL = 
        [](Sound s){
            using namespace std::chrono;

            setAlarm(steady_clock::now() + hours(1), 
            s,
            seconds(30));
        };
    // C++14
    auto setSoundL = 
        [](Sound s){
            using namespace std::chrono;
            using namespace std::literals;

            setAlarm(steady_clock::now() + 1h,  
                s,
                30s);
        };
    ```
    ```cpp 
    // bind
    using namespace std::chrono;
    using namespace std::literals;
    using namespace std::placeholders;

    // incorrect!
    auto setSoundB = 
        std::bind(setAlarm,
                steady_clock::now() + 1h,
                _1, 
                30s);
    ```
    + _caveats with `bind`_ 
        + have to mentally map `_1` to position in the `bind` parameter list, (2nd arg to ` bind`)
        + might have to consult `setAlarm` to determine type of `_1`
        + _incorrect_ 
            + `steady_clock::now() + 1h` 
                + passed as args to `bind` 
                + not as arg to `setAlarm`
            + _consequence_ 
                + expression will be evaluated when `bind` is called, and the time resulting from the expression will be stored inside the bind objet 
                + alarm will set off 1hr _after call to `bind`_ not 1 hr _after call to `setAlarm`_ 
    ```cpp 
    auto setSoundB = 
        std::bind(
            setAlarm, 
            std::bind(std::plus<>(),    steady_clock::now(), 1h),   // C++14
            _1, 
            30s
        );
    ```
    + _defered evaluation of expression_ 
        + tell `bind` to defer evaluation until `setAlarm` is called 
        + not readable ! 
    ```cpp 
    enum class Volume {Normal, Loud, LoudPlusPlus};

    // overload
    void setAlarm(Time t, Sound s, Duration d, Volume v);
    ```
    + _note_ 
        + lambda version works fine, by overload resolution
        + `bind` version fails to compile, since compiler cannot determine which of 2 `setAlarm` functions should pass to `bind`
    ```cpp 
    using SetAlarm3ParamType = 
        void(*)(Time t, Sound s, Duration d);
    
    auto setSoundB = 
        std::bind(
            static_cast<SetAlarm3ParamType>(setAlarm),
            std::bind(std::plus<>(), steady_clock::now(), 1h),
            _1,
            30s
        );
    ```
    + _solution: specify overloaded function type_
    ```cpp 
    auto betweenL = 
        [lowVal, highVal]
        (const auto& val)       // C++14
        { return lowVal <= val && 
            val << highVal;};
    ```
    ```cpp 
    using namespace std::placeholders;

    auto betweenB = 
        std::bind(std::logical_and<>(), 
            std::bind(std::less_equal<>(), lowVal, _1),
            std::bind(std::less_equal<>(), _1, highVal));
    ```
    + _lambda rocks!_ 
    ```cpp 
    enum class CompLevel { Low, Normal, High };
    Widget compress(const Widget& w,    
        CompLevel lev);

    Widget w;

    // bind
    using namespace std::placeholders;
    auto compressRateB = std::bind(compress, w, _1);

    // lambda 
    auto compressRateL = 
        [w](CompLevel lev){
            return compress(w, lev);
        };
    ```
    ```cpp 
    compressRateL(CompLevel::High);     // by value 
    compressRateB(CompLevel::High);     // by reference
    ```
    + _note_
        + _`bind` has no indication if pass by-value or by-reference_
            + captures are passed by value
                + i.e., `w` is stored by-value
                + use `bind(compress, std::ref(w), _1)` for by-reference semantics
            + parameters passed to bind object are by reference
                + since use perfect forwarding
                + i.e. `CompLevel::High` by-reference
        + _lambda makes this explicit via capture_
+ _verdict_ 
    + No need to use `bind` in C++14
    + _C++11_ 2 restricted case
        + _move capture_
            + via a lambda and a `std::bind`
            + _succeeded by_ 
                + C++14 init capture 
        + _polymorphic function objects_
            + `bind`'s `operator()` uses perfect forwarding, accepts arguments of any type 
            ```cpp 
            class PolyWidget {
                public:
                    template<typename T>
                    void operator()(const T& param);
            };

            PolyWidget pw;
            auto boundPW = std::bind(pw, _1);

            boundPW(1930);
            boundPW(nullptr);
            boundPW("rosebud");
            ```
            + _note_
                + calls to `boundPW` accepts arbitrary arguments 
            + _succeeded by_
                + C++14 `auto` parameter (generic lambda)
            ```cpp 
            auto boundPW = [pw](const auto& param){
                pw(param);
            };
            ```
+ _summary_ 
    + lambdas are more readable, more expressive, maybe more efficient 
    + in C++11 only, `bind` maybe useful for implementing 
        + _move capture_
        + binding objects with templatized function call operators



        

    

    