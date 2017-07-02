#include <utility>
#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <functional>

using namespace std;

namespace pair_ {

    template<typename T1, typename T2>
        ostream& operator<< (ostream& os, const pair<T1, T2>& p){
            return os << "[" << p.first << ", " << p.second << "]" << endl;
        }

    void print_pair(){
        pair<int, string> p {1, "hi"};
        cout << p << endl;
    } 


    class Foo {
        public: 
            Foo(tuple<int, float>){
                cout << "Foo:Foo(tuple)" << endl;
            }

            template<typename...Args>
                Foo(Args...){
                    cout << "Foo::Foo(args...)" << endl;
                }

    };

    void piecewise_construction(){

        tuple<int, float> t(1, 2.2);
        // pass tuple t as whole to Foo::Foo(tuple)
        pair<int, Foo> p1(42, t);
        // pass elements of t to Foo::Foo(args...)
        pair<int, Foo> p2(piecewise_construct, make_tuple(42), t);

    }


    void accept_charptr_pair(pair<int, const char*> p){
        cout << "accept_charptr_pair: " << p << endl;
    }

    void accept_string_pair(pair<int, const char*> p){
        cout << "accept_string_pair: " << p << endl;
    }

    void pass_pair_to_function(){
        accept_charptr_pair(make_pair(1, "foo"));
        accept_string_pair(make_pair(1, "bar"));        // type conversion
        accept_charptr_pair({1, "baz"});
        accept_string_pair({1, "bzz"});        // type conversion
    }


    void pair_move_semantics(){

        string s = "s";
        string t = "t";
        cout << "before move: s = " << s << "; t = " << t << endl;
        auto p = make_pair(move(s), move(t));
        cout << "after move: s = " << s << "; t = " << t << endl;
    }

    void pair_ref_semantics(){

        int i = 0;
        auto p = make_pair(ref(i), ref(i)); 
        ++p.first;
        ++p.second;
        std::cout << "i: " << i << std::endl;               // prints i: 2
    }


    void extract_from_pair(){

        auto p = make_pair("p.first", "p.second");

        string s;
        cout << "extract from pair: " << s;
        tie(ignore, s) = p;
        cout << " -> " << s << endl;
    }



} 


namespace tuple_ {

    void basic_usage(){

        // default initialized, (0 for built-in types)
        tuple<string, int, int, vector<double>> t;

        // create and initialize tuples explicitly
        tuple<int, float,string> t1(1, 1.1, "hi");

        cout << get<0>(t1) << " ";
        cout << get<1>(t1) << " ";
        cout << get<2>(t1) << " ";
        cout << endl;

        // make_tuple
        auto t2 = make_tuple(1, 2, "hi");

        // assign second value in t2 to t1
        get<1>(t1) = get<1>(t2);

        cout << get<1>(t1) << endl;

        if (t1 < t2) { // compares value for value
            t1 = t2; // OK, assigns value for value 
        }
    }

    void make_tuple_with_ref(){
         string s;
        auto x = make_tuple(s);
        get<0>(x) = "value";         // modifies x but not s
        cout << "s: " << s << "; x:" << get<0>(x) << endl;
        auto y = make_tuple(ref(s));
        get<0>(y) = "value";         // modifies y
        cout << "s: " << s << "; y:" << get<0>(x) << endl;
    }

    void extract_from_tuple(){

        auto t = make_tuple(1, 2.2, "hi");

        int i{};
        double j{};
        string s{};

        // assign values of t to i, j, s with tie and ignore
        tie(i, ignore, ignore) = t;
        cout << "i:" << i << "; j:" << j << "; s:" << s << endl;

        // assign values of t to i, j, s
        make_tuple(ref(i), ref(j), ref(s)) = t;
        cout << "i:" << i << "; j:" << j << "; s:" << s << endl;
    }

    template<typename...Args>
        void foo(const tuple<Args...>){ cout << "foo(tuple)" << endl; }

    void foo(int){ cout << "foo(int)" <<endl; }
    
    void explicit_tuple_constructor(){

        tuple<int, double> t1 {1, 1.1};      //OK
        tuple<int, double> t2 = {1, 1.1};    //actually is fine...

        cout << get<0>(t2) << endl;

        vector<tuple<int, float>> v {{1, 1.1}, {2, 2.2}};   // actually works OK
        for(auto t: v){
            cout << get<1>(t) << endl;      
        }

        // test if initializer list conversion is used here
        foo(1);                     // foo(int)     so not converted to tuple...
        foo(make_tuple(1, 1.1));    // foo(tuple)

    }
}

int main(){


    pair_::print_pair();
    pair_::piecewise_construction();
    pair_::pass_pair_to_function();
    pair_::pair_move_semantics();
    pair_::pair_ref_semantics();
    pair_::extract_from_pair();

    tuple_::basic_usage();
    tuple_::make_tuple_with_ref();
    tuple_::extract_from_tuple();
    tuple_::explicit_tuple_constructor();



}
