# LearnCPP





### Msic 

  
+ _expression_ 
    + An expression is a sequence of operators and operands, that specify a computation 
    + may
        + produce result 
        + generate side-effect
    + _value categories_ classifies expression
        + non-reference `type` and 
        + belong to one of `value` category
            + `prvalue`
                + literal, except string literal 
                    + `42, true, nullptr`
                + function call or overloaded operator of non-reference return type 
                    + `str.substr(1,2)`
                    + `str1 + str2`
                    + `it++`
                + post-increment/decrement
                    + `a++; b--`
                + arithmetic, logical, comparison expression 
                    + `a + b`, `a && b`, `a < b`
                + address-of expression 
                    + `&a`
                + cast to non-reference type
                    + `static_case<double>(x)`
                + lambda expression 
                    + `[](int x){ return x * x; }`
                + properties 
                    + cannot be polymorphic, 
            + `xvalue`
                + function call or overloaded operator expression of `rvalue` reference to object return type 
                    + `std::move(x)`
            + `lvalue`
                + represents an object
                + name of variable/function in scope
                    + `std::cin`
                + function call or overloaded operator expression of lvalue reference return type 
                    + `std::getline(std::cin, str)`
                    + `std::cout << 1`
                    + `str1 = str2`
                    + `++it`
                + assignment 
                    + `a = b`
                    + `a += b`
                + pre-increment/decrement 
                    + `++a; --b`
                + indirection 
                    + `*p`
                + subscript expression 
                    + `a[n]`, `p[n]`
                + member of object expression
                    + `a.m`
                + member of pointer 
                    + `a->m`
                + ...
                + properties 
                    + address of `lvalue` may be taken `&--i`
            + `glvalue`
                + either `lvalue` or `xvalue`
            + `rvalue`
                + either `prvalue` or `xvalue`
            





