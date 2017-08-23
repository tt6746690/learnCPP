
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace std;

namespace lambdas {

    void test_lambdas(){
        list<int> coll = {1,2,3,4,5,6,7,8,9,10};
        int count = 0;

        // removes 3rd element
        auto pos = remove_if(coll.begin(), coll.end(), 
            [count](int) mutable {
                return ++count == 3;
            });

        coll.erase(pos, coll.end());

        for(auto elem: coll){
            cout << elem;
        }
        cout << endl;
    }
}


int main(int argc, char ** argv){
    lambdas::test_lambdas();
    return 0;
}
