#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>

using namespace std;

namespace shared_ptr_ {


    void basic(){

        // declaring a deletor in shared_ptr() constructor with lambdas
        shared_ptr<string> pNico(new string("nico"), 
                        [](string* p){
                            cout << "delete " << *p << endl;
                            delete p;
                        });
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


        pNico = nullptr;            // shared_ptr gives up ownership
        whoMadeCoffee.resize(2);    // all copies of stirng in pNico are destroyed

    }

    void arrays(){
        shared_ptr<int> p1(new int[10]);         // compiles but ERROR 
        shared_ptr<int> p2(new int[10],
                            [](int* p){
                                delete[] p;
                            });                 // OK   
        
        unique_ptr<int[]> p3(new int[10]);       // OK,      note type int[] passed as template arg
        // shared_ptr<int[]> p4(new int[10]);       // ERROR:  no matching constuctor init for shared_ptr<int[]>
        
        // kind of hard to manipulate array with shared pointer, since no [] indexing
        // on the contrary, unique_ptr supports operator[]
        p3[0] = 1;

        // have to specify second template arg for deleter 
        unique_ptr<int, void(*)(int*)> p5(new int[10],
                                            [](int* p){
                                                delete[] p;
                                            });

    }



    void test_filedestructor(){

        class FileDeleter {
            private: 
                string filename;
            public:
                FileDeleter (const string& fn): filename(fn){}
                void operator()(ofstream* fp){
                    cout << "closing file: " << filename << endl;
                    fp->close();                    // close file     
                    remove(filename.c_str());       // delete file
                }
        };

        // create and open temp file 
        shared_ptr<ofstream> fp(new ofstream("tempfile.txt"), FileDeleter("tempfile.txt"));

        // or with lambda?
        string filename = "tempfile.txt";
        shared_ptr<ofstream> fp2(new ofstream(filename),
                [&](ofstream* ptr){
                    cout << "lambda file deleter: " << filename << endl;
                    delete ptr;         // ofstream is proper RAII, so will be closed in ~ofstream 
                    remove(filename.c_str());
                });
    }

    void test_weak_ptr(){

        class Person: public enable_shared_from_this<Person>{
            public:
                string name;
                shared_ptr<Person> mother;
                shared_ptr<Person> father;
                vector<weak_ptr<Person>> kids;          // weak ptr!

                Person(const string& n, 
                        shared_ptr<Person> m = nullptr, 
                        shared_ptr<Person> f = nullptr)
                        : name(n), mother(m), father(f){}
                ~Person(){
                    cout << "delete " << name << endl;
                }


                void setParentAndTheirKids(shared_ptr<Person> m = nullptr,
                        shared_ptr<Person> f = nullptr){
                    mother = m;
                    father = f;

                    if(m != nullptr){
                        m->kids.push_back(shared_from_this());
                    }
                    if(f != nullptr){
                        f->kids.push_back(shared_from_this());
                    }
                    // creating shared_ptr out of this problematic 
                    // since we open a new group of owners
                    // i.e. shared_ptr<Person>(this) and shared_ptr<Person>(kid) 
                    // are different groups of shared pointer referencing the same object kid...
                }
        };
        
        
        auto initFamily = [](const string& name) -> shared_ptr<Person> {
            shared_ptr<Person> mom(new Person(name + "'s mom"));
            shared_ptr<Person> dad(new Person(name + "'s dad"));
            shared_ptr<Person> kid(new Person(name));

            // shared_ptr<Person> kid(new Person(name, mom, dad));
            // mom->kids.push_back(kid);
            // dad->kids.push_back(kid);
            kid->setParentAndTheirKids(mom, dad);
            return kid;
        };

        shared_ptr<Person> p = initFamily("nico");

        cout << "nico's family exists " <<endl;
        cout << "- nico is shared " << p.use_count() << " times" << endl;
        cout << "- name of 1st kid of nico's mom: " << p->mother->kids[0].lock()->name << endl;

        p = initFamily("jim");
        cout << "jim's family exists " << endl;

    }


    void weak_ptr_handle_released_object(){

        try {
            shared_ptr<string> sp(new string("hi"));
            weak_ptr<string> wp = sp;           // create weakptr from sharedptr
            sp.reset();                         // release shared object
            cout << wp.use_count() << endl;     // prints 0
            cout << wp.expired() << endl;         // prints true 
            shared_ptr<string> p(wp);           // throws std::bad_weak_ptr
        } catch (const std::exception& e){
            cerr << "exception: " << e.what() << endl;  // prints: bad_weak_ptr
        }
    }




}

namespace unique_ptr_{

    void basic(){

        unique_ptr<string> up(new string("nico"));
        (*up)[0] = 'N';
        up->append("lai");
        cout << *up << endl;

        // unique_ptr<int> up1 = new int;      // error
        unique_ptr<int> up2(new int);       // ok

        unique_ptr<string> up4;              // empty
        up4 = nullptr;                       // empty
        up4.reset();                         // empty


        if(up){ cout << "up not empty" << endl; }
        string* nptr = up.release();
        cout << "up releases string: " << *nptr << endl;
        if(!up){ cout << "up is empty now" << endl; }
        delete nptr;            // manage yourself now


    }

    void transfer_ownership(){

        // transfer ownership
        unique_ptr<int> up1(new int);
        // unique_ptr<int> up2(up1);            // ERROR, not possible
        unique_ptr<int> up3(move(up1));      // OK, up1 transfer ownership to up3


        unique_ptr<int> up2;
        // up2 = up1;              // ERROR
        up2 = move(up1);        // OK: assigns unique_ptr by move, transfer ownership to up2


        unique_ptr<int> ptr;

        // ptr = new int;               // ERROR
        ptr = unique_ptr<int>(new int);        // OK delete old object and own new
        ptr = nullptr;                    // object deleted
    }


    void dealing_with_arrays(){

        unique_ptr<string> up(string[10]);              // runtime error
        unique_ptr<string[]> up1(new string[10]);        // OK

        // cout << *up1 << endl;            // ERROR: not defined for arrays
        cout << up1[0] << endl;          // OK

    }

}

int main(){


    shared_ptr_::basic();
    shared_ptr_::arrays();
    shared_ptr_::test_filedestructor();
    shared_ptr_::test_weak_ptr();
    shared_ptr_::weak_ptr_handle_released_object();


    unique_ptr_::basic();
    unique_ptr_::transfer_ownership();
    unique_ptr_::dealing_with_arrays();
    

}
