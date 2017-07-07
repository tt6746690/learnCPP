#include <ratio>
#include <iostream>
#include <chrono>


using namespace std;



namespace ratio_ {

    void basic(){
        typedef ratio<5,3> FiveThird;
        cout << FiveThird::num << "/" << FiveThird::den << endl;
        typedef ratio<25,15> alsoFiveThird;
        cout << alsoFiveThird::num << "/" << alsoFiveThird::den << endl;
        typedef ratio<0> zero;
        cout << zero::num << "/" << zero::den << endl;
        using neg = ratio<7, -3>;
        cout << neg::num << "/" << neg::den << endl;
    }



    void ratio_arithmetics(){

        using sum = ratio_add<ratio<2,7>, ratio<2,6>>::type;
        cout << sum::num << "/" << sum::den << endl;


        if(ratio_equal<ratio<5,3>, ratio<25,15>>::value){
            cout << "5/3 and 25/15 is same" << endl;
        }
    }


}


namespace chrono_ {
    void basics(){
        chrono::seconds twentySeconds(20);  // 20 seconds
        chrono::hours aDay(24);             // 24 hours
        chrono::milliseconds ms;            // 0 millisecoinds

        ms += twentySeconds + aDay;         // 86400000 milliseconds
        --ms;                               // 86399999 milliseconds
        ms *= 2;                            // 1728398998 milliseconds
        cout << ms.count() << " ms" << endl;
        cout << chrono::nanoseconds(ms).count() << " ns" << endl;
    }

    template<typename V, typename R>
    ostream& operator<<(ostream& s, const chrono::duration<V, R>& d){
        s << "[" << d.count() << " of" << R::num << "/" << R::den << "]";
        return s;
    }

    void print_duration(){
        chrono::seconds s(25);
        cout << s << endl;
    }
}


int main(){
    ratio_::basic();
    ratio_::ratio_arithmetics();


    chrono_::basics();
    chrono_::print_duration();
}
