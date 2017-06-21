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


struct Student_info {
    string name;
    double midterm, final;
    vector<double> homework;
};



double median(vector<double> vec);

double grade(const Student_info& s);
double grade(double midterm, double final, const vector<double>& hw);
double grade(double midterm, double final, double homework);

istream& read_hw(istream& in, vector<double>& hw);
istream& read(istream& is, Student_info& s);

bool compare(const Student_info& x, const Student_info& y);


int main(){

    vector<Student_info> students;
    Student_info record;
    string::size_type maxlen = 0;


    // read and store all records, and fine length of longest name
    while(read(cin, record)){
        maxlen = std::max(maxlen, record.name.size());
        students.push_back(record);
    }

    // alphabetize records 
    sort(students.begin(), students.end(), compare);

    for(vector<Student_info>::size_type i = 0; i != students.size(); ++i){
        // write name, padded on right to maxlen + 1
        cout << students[i].name
             << string(maxlen + 1 - students[i].name.size(), ' ');
        
        // compute and generate final grades 
        try {
            double final_grade = grade(students[i]);
            streamsize prec = cout.precision();
            cout << setprecision(3)
                << final_grade << setprecision(prec);
        } catch (domain_error e){
            cout << e.what();
        }
        cout << endl;
    }



    return 0;
}



bool compare(const Student_info& x, const Student_info& y){
    return x.name < y.name;
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

// overloading grade again with struct 
double grade(const Student_info& s){
    return grade(s.midterm, s.final, s.homework);
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


istream& read(istream& is, Student_info& s){

    // read ans store student midterm and final grades 
    is >> s.name >> s.midterm >> s.final;

    read_hw(is, s.homework);
    return is;

}
