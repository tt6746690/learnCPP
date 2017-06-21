#include <vector> 
#include <string> 
#include <iostream> 
#include <cctype> // for isspace

using namespace std;

// function for taking in a line and separate into strings based on space words in [i, j)
vector<string> split(const string& s){
    vector<string> ret;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()){
        // ignore leading blanks
        // invariant: [i_prev, i_cur] are all spaces
        while( i != s.size() && isspace(s[i]) )
            ++i;

        // find end of next word
        string_size j = i;

        //invariant: none of chars in [j_prev, j_cur] is a space
        while( j != s.size() && !isspace(s[j]) )
            ++j;

        // if found none white space chars
        if (i != j){
            // copy from s[i, j-i] to ret
            ret.push_back(s.substr(i, j-i));
            i = j;

        }

    }

    return ret;

}


// int main(int argc, char **argv){
//
//     vector<string> v_str;
//     v_str = split(argv[1]);
//
//     for(vector<string>::iterator iter = v_str.begin(); iter != v_str.end(); iter++){
//         cout << *iter << endl;
//     }
//     return 0;
// }
//
//
//


int main() {
    string s;
    // read and split each line of input
    while(getline(cin, s)){
        vector<string> v = split(s);

        // write each word in v
        for (vector<string>::size_type i = 0; i != v.size(); i++)
            cout << v[i] << endl;

    }

}

