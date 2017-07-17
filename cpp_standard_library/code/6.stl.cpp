#include <iostream> 
#include <vector> 
#include <set>
#include <algorithm>
#include <list>
#include <deque>


using namespace std;

namespace algorithm_ {


    void basics(){
        vector<int> coll = { 2, 5, 4, 1, 6, 3};

        // find and print min/max 
        auto minpos = min_element(coll.cbegin(), coll.cend());
        cout << "min: " << *minpos << endl;
        auto maxpos = max_element(coll.cbegin(), coll.cend());
        cout << "max: " << *maxpos << endl;

        // sort elments 
        sort(coll.begin(), coll.end());

        // find first element with value 3
        auto pos3 = find(coll.begin(), coll.end(), 3);

        // reverse order of found element with value 3 and all following elements 
        reverse(pos3, coll.end());

        //print 
        for(auto elem: coll){ cout << elem << " ";}
        cout << endl;
    }


    void insert_iterator(){

        list<int> coll1 = {1,2,3,4,5};

        // copy elements of coll1 into coll2 by appending them 
        vector<int> coll2;
        copy(coll1.cbegin(), coll1.cend(), back_inserter(coll2));

        // copy and insert them at the front 
        deque<int> coll3;
        copy(coll1.cbegin(), coll1.cend(), front_inserter(coll3));

        // copy 
        set<int> coll4;
        copy(coll1.cbegin(), coll1.cend(), inserter(coll4, coll4.begin()));

        for(auto &elem:coll4){
            cout << elem << " ";
        }
        cout << endl;
    }


    void stream_iterator(){
        vector<string> coll;

        // read all words from the standard input
        // - source: all strings until end-of-file (or error) 
        // - destination: coll (inserting)
        copy (istream_iterator<string>(cin),
                istream_iterator<string>(),
                back_inserter(coll));

        // sort elements
        sort (coll.begin(), coll.end());

        // print all elements without duplicates
        // - source: coll
        // - destination: standard output (with newline between elements) 
        unique_copy (coll.cbegin(), coll.cend(),
                        ostream_iterator<string>(cout,"\n")); // destination
    }

    void reverse_iterator(){
         vector<int> coll;
        // insert elements from 1 to 9
        for (int i=1; i<=9; ++i) {
                coll.push_back(i);
            }
        // print all element in reverse order
        copy (coll.crbegin(), coll.crend(),
                ostream_iterator<int>(cout," "));
        cout << endl;
    }

    void manipulating_algo(){
        // unordered set with elements from 1 to 9
        set<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        // print all elements of the collection
        copy (coll.cbegin(), coll.cend(),
                ostream_iterator<int>(cout," "));
        cout << endl;
        // Remove all elements with value 3
        // - algorithm remove() does not work
        // - instead member function erase() works 
        int num = coll.erase(3);
        // print number of removed elements
        cout << "number of removed elements: " << num << endl;
        // print all elements of the modified collection
        copy (coll.cbegin(), coll.cend(),
                ostream_iterator<int>(cout," "));
        cout << endl;

    }

    template<typename T>
        void PRINT_ELEMENTS(const T& coll, const string& optstr = ""){
            cout << optstr << ": ";
            for(const auto& elem: coll){
                cout << elem << " ";
            }
            cout << endl;
        }

    int square (int value){
        return value*value;
    }

    void function_in_algo(){

        std::set<int>    coll1;
        std::vector<int> coll2;
        // insert elements from 1 to 9 into coll1 
        for (int i=1; i<=9; ++i) {
            coll1.insert(i);
        }
        PRINT_ELEMENTS(coll1,"initialized: ");
        // transform each element from coll1 to coll2
        // - square transformed values
        transform (coll1.cbegin(),coll1.cend(),    // source
                            back_inserter(coll2),  // dest
                            square);               // operation
        PRINT_ELEMENTS(coll2,"squared: ");
    }

    bool isPrime (int number)
    {
        // ignore negative sign
        number = abs(number);
        // 0 and 1 are no prime numbers
        if (number == 0 || number == 1) {
            return false;
        }
        // find divisor that divides without a remainder
        int divisor;
        for (divisor = number/2; number%divisor != 0; --divisor) {
        ; }
        // if no divisor greater than 1 is found, it is a prime number
        return divisor == 1;
    }

    void predicate_in_algo(){
        list<int> coll;
        // insert elements from 24 to 30
        for (int i=24; i<=30; ++i) {
            coll.push_back(i);
        }
        // search for prime number
        auto pos = find_if (coll.cbegin(), coll.cend(),     // range
                            isPrime);                       // predicate
        if (pos != coll.end()) {
            // found
            cout << *pos << " is first prime number found" << endl; }
        else {
            // not found
            cout << "no prime number found" << endl;
        }
    }

    // -- runtime function object 
    class AddValue {
        private:
            int theValue; // the value to add public:
        public:
            // constructor initializes the value to add
            AddValue(int v) : theValue(v) {}
            // the ‘‘function call’’ for the element adds the value
            void operator() (int& elem) const {
            elem += theValue;
            } 
    };


    void function_obj(){


        list<int> coll;

        // insert elements from 1 to 9
        for (int i=1; i<=9; ++i) {
                coll.push_back(i);
            }
        PRINT_ELEMENTS(coll,"initialized: ");

        for_each (coll.begin(), coll.end(),
                AddValue(10));
        PRINT_ELEMENTS(coll,"after adding 10:");

        // add value of first element to each element
        for_each (coll.begin(), coll.end(),
                AddValue(*coll.begin()));

        PRINT_ELEMENTS(coll,"after adding first element: ");

    }

    void predefined_functionobj(){
            deque<int> coll = { 1, 2, 3, 5, 7, 11, 13, 17, 19 }; PRINT_ELEMENTS(coll,"initialized: ");

        // negate all values in coll
         transform (coll.cbegin(),coll.cend(),
                    coll.begin(),
                    negate<int>()); 
        PRINT_ELEMENTS(coll,"negated: ");

        // square all values in coll
        transform (coll.cbegin(),coll.cend(),
                     coll.cbegin(),
                    coll.begin(),
                    multiplies<int>());
        PRINT_ELEMENTS(coll,"squared: ");
    }


}




int main(){
    algorithm_::basics();
    algorithm_::insert_iterator();
    // algorithm_::stream_iterator();
    algorithm_::reverse_iterator();
    algorithm_::manipulating_algo();
    algorithm_::function_in_algo();
    algorithm_::predicate_in_algo();
    algorithm_::function_obj();
    algorithm_::predefined_functionobj(); 
    
}


