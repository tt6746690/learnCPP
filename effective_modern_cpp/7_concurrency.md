
# Chapter 7 The Concurrency API


#### Item 35: Prefer task-based programming to thread-based 


+ _choices for async_
    + _thread-based_
        + create a `std::thread` asnd run `doAsyncWork`
        ```cpp
        int doAsyncWork();
        std::thread t(doAsyncWork);
        ```
    + _task-based_
        + pass `doAsyncWork` to `std::async` 
        ```cpp 
        auto fut= std::async(doAsyncWork);
        ```
        + _note_
            + `fut` is a task
+ _task-based is superior to thread-base_
    + _ease of access to return value_ 
        + _task-based_: 
            + `get` function for future returned from `std::async` gets the return value of routine
            + `get` function also provides access to exception should `doAsyncWork` throw
        + _thread-based_: 
            + no easy way to access
            + and if `doAsyncWork` throws, the program dies (`std::terminate`)
+ _threads_
    + _hardware threads_
        + threads that actually perform computation 
        + one or more hardware threads per CPU core 
    + _software threads (OS threads)_
        + threads that OS manages cross all processes and schedules for execution on hardware threads
        + usually more software threads than hardware threads
            + since software threads is blocked (IO, wait for mutex ... )
            + can increase throughput by executing another unblocked thread 
    + _`std::thread`_
        + objects in C++ process that act as handles to underlying software threads 
        + can represent 
            + _null handles_, i.e. correspond to null software thread, since they are in default-constructed state (no function to execute)
            + _moved_
                + moved-to thread acts as handle to underlying software thread
            + _join_
                + function ran finished 
            + _detached_
                + connection between them and underlying software thread severed
+ _problems have to worry about with `std::thread`_   
    + _thread exhaution (throws)_ 
        ```cpp 
        int doAsyncWork() noexcept;
        std::thread t(doAsyncWork);     // throws if no more available
        ``` 
        + constructor throws if create more thread than OS provides cause `system_error`
        + _running on main thread_
            + unbalanced load 
            + responsiveness problem if current thread is a GUI thread
        + _wait for existing software thread to complete then try to create a new thread_
            + threads may wait for an action that `doAsyncWork` is supposed to perform, 
            + so blocks forever
    + _oversubscription_ 
        + more unblocked software threads than hardware threads
            + the scheduler time-slices sofware by performing context switch
        + _context switch_ is pure overhead for thread management
            + even more costly if threads are on different cores
            + leading to cold cache, pollutes cache 
        + _difficult to solve_
            + since optimal ratio of software to hardware threads depends on 
                + _IO / CPU bound_ 
                + _cost of context switch_ 
                + _how effective CPU caches_
                + _details of hardware_
    + _load balancing_
+ _solution: just use `std::async`_ 
    + an abstraction over threads
    + leaves thread management to implementers of C++ standard library
    + _so_ 
        + _out-of-thread exception likelihood reduced_  
            + since `async` when called doesnt guarantee it will create a new thread
            + rather, permits scheduler to arrange the specified function  to be run on thread requesting `doAsyncWork`'s result, 
            + so scheduler take advantage of this _freedom_ (create or not) if the system is oversubscribed or out of threads
        + _load-balancing left to runtime system_
        + _responsiveness of GUI threads_
            + still problematic, since scheduler has no way of knowing which threas has tight responseness requirements 
            + solution: pass `std::launch::async` launch policy to `async`
        + _solid implementation_ 
            + thread pool to avoid oversubscriptioon, and improve load balancing cross cores via work-stealing algorithm
+ _situations where `thread` is preferred_ 
    + need access to API of underlying threading impl   
        + `async` has a sparser API than `thread`
    + need to and are able to optimize thread usage for your application 
    + need to implement threading technology beyong C++ concurrency API 
        + thread pools on platforms where C++ impl doesnt offer
+ _summary_ 
    + `std::thread` API offers no direct way to get return values from async run functions and if those functions throw, the program is terminated 
    + thread-based programming calls for manual management of thread exhaustion, oversubscription, load balancing, and adaptation to new platforms 
    + task-based programming via `std::async` with defualt launch policy handles most of these issues for you


--- 

#### Item 36: Specify `std::launch::async` if asynchronicity is essential 
