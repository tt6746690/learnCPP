#include <iostream> 
#include <vector> 
#include <set>
#include <algorithm>


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


}




int main(){
    algorithm_::basics();
}


