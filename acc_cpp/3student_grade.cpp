#include <iomanip>
#include <ios>
#include <iostream> 
#include <string> 
#include <algorithm>
#include <vector>


using std::cin;     using std::setprecision;
using std::cout;    using std::string;
using std::endl;    using std::streamsize;
using std::vector;


int main(){

    // name
    cout << "Please enter your first name: ";
    string name;
    cin >> name;
    cout << "Hello, " << name << "!" << endl;

    // midterm and final grade 
    cout << "Please enter mideterm and final exam grades: ";
    double midterm, final;
    cin >> midterm >> final;

    // ask for homework grades 
    cout << "enter homework greades followed by EOF: ";

    // a variable into which to read 
    double x;
    vector<double> homework;

    while (cin >> x){
        homework.push_back(x);
    }

    // find median instead 
    typedef vector<double>::size_type vec_sz;
    vec_sz size = homework.size();

    // test empty input 
    if (size == 0){
        cout << "You must enter your grades.  "
                "Please trye again."<< endl;
        return 1;
    }

    // sort the grades 
    // take no worse than nlogn
    sort(homework.begin(), homework.end());

    // COmpute median homework grade 
    vec_sz mid = size / 2;
    double median;
    median = size % 2 == 0 ? (homework[mid] + homework[mid-1]) / 2
                           : homework[mid];

    // write result 
    streamsize prec = cout.precision();
    cout << "Your final grade is " << setprecision(3)
         << 0.2 * midterm + 0.4 * final + 0.4 * median
         << setprecision(prec) << endl;
    return 0;
}
