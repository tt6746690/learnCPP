#include <iomanip>
#include <ios>
#include <iostream> 
#include <string> 
#include <algorithm>
#include <vector>
#include <stdexcept>


using std::cin;     using std::setprecision;
using std::cout;    using std::string;
using std::endl;    using std::streamsize;
using std::vector;  using std::domain_error;
using std::istream;



double median(vector<double> vec);
double grade(double midterm, double final, const vector<double>& hw);
istream& read_hw(istream& in, vector<double>& hw);



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
    vector<double> homework;

    // read homework grades 
    read_hw(cin, homework);


    // compute and generate final grades 
    try {
        double final_grade = grade(midterm, final, homework);
        streamsize prec = cout.precision();
        cout << "Your final grade is " << setprecision(3)
            << final_grade << setprecision(prec) << endl;
    } catch (domain_error){
        cout << endl << " Your must enter your grades. "
                        " Please try again." << endl;
        return 1;
    }

    return 0;
}


// compute median 
double median(vector<double> vec){
    typedef vector<double>::size_type vec_sz;
    vec_sz size = vec.size();
    
    if(size == 0)
        throw domain_error("median of an empty vector");

    sort(vec.begin(), vec.end());

    vec_sz mid = size / 2;
    return size % 2 == 0 ? (vec[mid] = vec[mid - 1]) / 2 : vec[mid];
}

// compute overall grade 
double grade(double midterm, double final, double homework){
    return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}

double grade(double midterm, double final, const vector<double>& hw){       // read-only pass by reference
    if(hw.size() == 0)
        throw domain_error("student has done no homework");

    return grade(midterm, final, median(hw));
}

// read homework greads from input stream into vector<double>
istream& read_hw(istream& in, vector<double>& hw){                          // writable pass by reference 

    if(in){
        // remove previous state 
        hw.clear();

        double x;
        while(in >> x)
            hw.push_back(x);

        // clear stream error state for next student input
        in.clear();
    }
    
    return in;
}
