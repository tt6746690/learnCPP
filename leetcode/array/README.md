

#### 2 pointer algorithm [link](https://tp-iiita.quora.com/The-Two-Pointer-Algorithm)


>  Given a sorted array A, with size N. Find any pair (i,j) having sum as given number X

+ since sorted, use 2 pointer 
    + one at start, one at end
    + sum up value pointered by 2 pointers, 
        + if value greater than required, then shift right pointer to left (to decrease the sum)
        + if value less than required, then shift left pointer to right (to increase the sum)
        + if equal, we found the solution!

```cpp 
// O(n)
#define  lli long long 

bool f(lli sum) {
    int l = 0, r = n - 1; // 2 pointers

    while( l < r) {
        if (A[l] + A[r] == sum) return 1;
        else if (A[l] + A[r] > sum) r--;
        else l++;
    }
    return 0;
}
```


> Given 2 sorted array A and B, each having length N and M. Form a new sorted merged array having values of both arrays in sorted format 


+ since 2 array sorted in order, use 2 pointers
    + use `l1` and `l2` to traverse `A` and `B`, and 
    + another write-index `cnt` to store position of first free cell in resulting array 
    + if both indices in range `l1 < N` or `l2 < M` choose minimum of `(A_i, B_j)` to write to `C[cnt]` and increase respective pointers


```cpp 
// O(m+n)
#define MAX 100005

lli C[2*MAX];

void merge(lli A[], lli B[], int m, int n) 
{
    int l1 = 0, l2 = 0, cnt = 0;
    while (l1 < n || l2 < m) {
        if (ll < n && l2 < m) {
            if (A[l1] < B[l2]) {
                C[cnt++] = A[l1++];
            } else if (A[l1] > B[l2]) {
                C[cnt++] = B[l2++];
            }
        } 
        else if (l1 < n) {
            C[cnt++] = A[l1++];
        } 
        else if (l2 < m) {
            C[cnt++] = B[l2++];
        }
    }
}
```

> Given an array containing N integers, you need to find the length of the smallest contiguous subarray that contains at least K distinct elements in it. output -1 if no such subarray exists 

+ 2 pointers `l` and `r` with both at start of array
    + keep shifting right pointer unless we have K elements into the set
    + as soon as we had K distinct elements in the set, we shift left pointer unless size of set becomes < K.
    + update length of minimum contiguous subarray as soon as we are ready to shift the left pointer

```cpp 
int l = 0, r = 0, ans = INF; 
map<int, int> cnt;  // a non-duplicate set, mapping values in subarray to their occurrences

while(l < n) {
    while(r < n && cnt.size() < K) {
        cnt.insert(A[r]);
        cnt[A[r]]++;
        r++;
    } 

    if(cnt.size() > K) {
        ans = min(ans, r-l);
    }

    cnt[A[l]]--;
    if(cnt[A[l]] == 0) cnt.erase(A[l]);
    l++;
}
```
