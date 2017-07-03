#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace shared_ptr_ {


    void basic(){

        shared_ptr<string> pNico(new string("nico"));
        shared_ptr<string> pJutta(new string("jutta"));

        // capitalize name
        (*pNico)[0] = 'N';
        pJutta->replace(0, 1, "J");

        // put them in containers
        vector<shared_ptr<string>> whoMadeCoffee;
        whoMadeCoffee.push_back(pJutta);
        whoMadeCoffee.push_back(pJutta);
        whoMadeCoffee.push_back(pNico);
        whoMadeCoffee.push_back(pJutta);
        whoMadeCoffee.push_back(pNico);

        // print
        for(auto ptr: whoMadeCoffee){
            cout << *ptr << endl;
        }
        cout << endl;

        // overwrite name
        *pNico = "Nicolai";

        // print again
        for(auto ptr: whoMadeCoffee){
            cout << *ptr << endl;
        }
        cout << endl;

        // print internal data
        cout << "user_cout: " << whoMadeCoffee[0].use_count() << endl;

    }
}


int main(){


    shared_ptr_::basic();


}
