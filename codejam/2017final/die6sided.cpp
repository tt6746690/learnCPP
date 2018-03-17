#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;  

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

int brute_force(vvi D, int N) {

    int max_len = 0;
    int dec_num, inc_num;

    vi row(N);
    iota(row.begin(), row.end(), 0);
    do {
        for (int i = 0; i < 6; ++i) 
        {
            int dec_len = 0, inc_len = 0;
            for (int j = 0; j < N; ++j) {
                int r = row[j];
                // assign first value; 
                if (j == 0) {
                    dec_num = D[r][i];
                    inc_num = D[r][i];
                    inc_len += 1; dec_len +=1;
                    continue;
                }
                // see if there is the next number in 
                // sequence in this (row[j]-th) dice
                for (int k = 0; k < 6; ++k) {
                    if (dec_num == (D[r][k] + 1)) {
                        dec_num -= 1;
                        dec_len += 1;
                        break;
                    } else if (inc_num == (D[r][k] - 1)) {
                        inc_num += 1;
                        inc_len += 1;
                        break;
                    }
                    
                }
            }

            // update
            max_len = (dec_len > max_len) ? dec_len : max_len;
            max_len = (inc_len > max_len) ? inc_len : max_len;
        }
        
    } while (next_permutation(row.begin(), row.end()));

    return max_len;
}


int main() {

    int T; 
    cin >> T; 
    // read in m x 6 matrix into D
    for (int i = 0; i < T; ++i) {
        int N;
        cin >> N; 

        vvi D;
        D.resize(N);
        for (int j = 0; j < N; ++j) {
            D[j].resize(6);
        }

        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < 6; ++k) {
                cin >> D[j][k];
            }
        }
        // find all ordering of N dies
        cout << "Case #" << to_string(i) << ": " << to_string(brute_force(D, N)) << '\n';
    }

}
