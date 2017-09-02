# 4 Smart Pointers 


+ _motivation_ 
    + _why raw pointers are bad_
        + declaration doesn't indicate whether it points to a single object or to an array 
        + declaration does not indicate ownership 
            + if we should destroy what it points when done using it, or
            + just a view to a non-owning piece of memory 
        + if determined to destroy pointer, there is no way to tell how
            + use `delete` or some other different destruction mechanism
        + if try to delete with `delete`, not sure if its `delete` or `delete[]`
        + Assume pointer is owning, and know how to destroy, it's difficult to ensure that destruction is performed _exactly_ once along every path in code (including exception), 
            + missing a path leads to resource leaks and 
            + doing destruction more than once leads to undefined behavior
        + no way to tell if a pointer dangles 
            + i.e. points to memory that no longer holds the pointer it supposed to pointer to 
    + _smart pointers_  
        + wrappers around raw pointers that act much like raw pointers they wrap, but avoid many of the pitfalls
        + implicit conversion deleted
        + `std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`
            + manage lifetime of dynamically allocated objects
            + avoid resource leaks 



#### Item 18: Use `std::unique_ptr` for exclusive-ownership resource management 


+ `std::unqiue_ptr`
    + _efficiency_ 
        + about same size as raw pointer 
        + most operations (include dereferencing), they execute the same instructions
        + 2 word size if use custom deleter
            + deleter size depends on how much state stored  (capture)
            + so captureless lambda is preferred
    + _exclusive ownership_  
        + a non-null `std::unique_ptr` always _owns_ what it points to 
    + _ownership transfer_
        + via moving a `std::unique_ptr` from the source pointer (later set to null) to target pointer
    + _move-only type_
        + _copy not allowed_,
        +  since end up hainvg multiple owning pointers, each thinking it owned, (hence should destroy) that resource
    + _destruction_ 
        + a non-null `unique_ptr` frees its resource on destruction
        + i.e. apply `delete` to raw pointer inside `std::unique_ptr`
+ _application_ 
    ```cpp 
    class Investment;
    class Stock: public Investment;
    class Bond: public Investment;
    class RealEstate: public Investment;

    template<typename... Ts>
    std::unique_ptr<Investment> makeInvestment(Ts&&... params);

    {
        auto pInvestment = 
            makeInvestment(args);      
    }       // destroy *pInvestment
    ```
    + _factory method_
        + factory methods for allocating an object on heap and returns pointer to it
        + caller required for deleting object when no longer needed
        + use `std::unique_ptr` since the caller acquires ownership and underlying resources are automatically deleted when `unique_ptr` destroyed
    + _owndership-migration_
        + `unique_ptr` returned from factory moved to a container as a data member of a class
        + when object destroyed, the resources are released as wells, 
        + if ownership chain got interrupted due to exception... `std::unique_ptr` owning the managed resource will eventually have its destructor called, and resources it manages will be destroyed
+ _custom deleters_ 
    + arbitrary callables to be invoked when its time for resources to be destroyed
    + accepts a raw pointer to object to be destroyed
    ```cpp 
    class Investment {
        pulic:
            virtual ~Investment();
    };

    auto delInvmt = [](Investment* pInvestment){
        makeLogEntry(pInvestment);
        delete pInvestment;
    };
    ```
    + _note_
        + write a log before destroying 
        + deleting all derived class from a base class pointer `Investment *`
            + implies have to make base class virtual destructor
    ```cpp
    template<typename... Ts>
    std::unique_ptr<Investment, decltype(delInvmt)>
    makeInvestment(Ts&&...params)
    {
        std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

        if(/* create stock */)
            pInv.reset(new Stock(std::forward<Ts>(params)...));
        
        if(/* create bond */)
            pInv.reset(new Bond(std::forward<Ts>(params)...));

        if(/* create realestate */)
            pInv.reset(new RealEstate(std::forward<Ts>(params)...));
        return pInv;
    }
    ```
    + _note_
        + must supply custom deleter's type as second type to `std::unique_ptr`
        + Note cannot just assign `new Stock(...)` since this implies implicit conversion
        + `std::forward` prefect-forward arguments 
    ```cpp 
    template<typename...Ts>
    auto makeInvestment(Ts&&... parmas)
    {
        auto delInvmt = [](Investment* pInvestment){
            makeLogEntry(pInvestment);
            delete pInvestment;
        };

        std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

        if()
            ...

        return pInv;
    }
    ```
    + _note_    
        + C++14 auto return type deduction 
        + better encapsulation
+ _2 forms of `std::unique_ptr`_
    + `std::unique_ptr<T>`
        + no indexing operator (`operator[]`)
    + `std::unique_ptr<T[]>`
        + lacks dereferencing operator (`operator*` and `operator->`)
+ _Conversion to `std::shared_ptr`_ 
    ```cpp 
    std::shared_ptr<Investment> sp = makeInvestment(args);      
    ```
    + _note_
        + implicit conversion of `unique_ptr` to `shared_ptr`
        + good in factory pattern, since function can't know if caller will want to use exclusive-ownership semantics or shared ownership
            + returning `unique_ptr` provides with most efficient smart pointer,
            + and dont hinder caller from replacing it with its more flexible siblings 
+ _summary_ 
    + `std::unique_ptr` is small, fast, move-only smart pointer for managing resources with exclusive-owndership semantics 
    +  By default resource destruction takes place via `delete`, but custom deleters can be specified. Stateful deleters and function pointers as deleteres increase size of `std::unique_ptr` objects 
    + Converting from `unique_ptr` to `shared_ptr` is easy
        
    