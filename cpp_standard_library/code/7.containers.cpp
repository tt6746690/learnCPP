#include <iostream> 
#include <vector> 
#include <set>
#include <algorithm>
#include <list>
#include <deque>


using namespace std;


namespace containers_ {

    void use_vector() {
        vector<string> sentence;
        sentence.reserve(5);

        // append 
        sentence.push_back("hello: ");
        sentence.insert(sentence.end(), {"hi", "wt", "?"});

        copy(sentence.cbegin(), sentence.cend(), ostream_iterator<string>(cout, " "));
        cout << endl;
        
        //print stats
        cout << "  max_size(): " << sentence.max_size() << endl;
        cout << "  size(): " << sentence.size()     << endl;
        cout << "  capacity(): " << sentence.capacity() << endl;

        swap(sentence[1], sentence[2]);

        // insert element always before ?
        sentence.insert(find(sentence.begin(), sentence.end(), "?"), "always");

        // replace last element by !
        sentence.back() = "!";

        // print by spaces
        copy(sentence.cbegin(), sentence.cend(), ostream_iterator<string>(cout, " "));
        cout << endl;
        

        // delete last 2 elemnt 
        sentence.pop_back();
        sentence.pop_back();
        // shrink capacity 
        sentence.shrink_to_fit();

        cout << "  size(): " << sentence.size()     << endl;
        cout << "  capacity(): " << sentence.capacity() << endl;
        
    }
}


int main(){

    containers_::use_vector();

}
