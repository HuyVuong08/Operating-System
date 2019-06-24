#include    "findsubstr.h"
#include    <string.h>
#include    <stdio.h>

void computeLPSArray(const char* sub, int M, int* lps);
 
// Prints occurrences of str[] in sub[]
int find_sub_string(const char * str, const char * sub){
    int M = strlen(sub);
    int N = strlen(str);
 
    // create lps[] that will hold the longest prefix suffix
    // values for subtern
    int lps[M];
 
    // Preprocess the subtern (calculate lps[] array)
    computeLPSArray(sub, M, lps);
 
    int i = 0; // index for str[]
    int j = 0; // index for sub[]
    while (i < N) {
        if (sub[j] == str[i]) {
            j++;
            i++;
        }
 
        if (j == M) {
            return i - j;
        }
 
        // mismatch after j matches
        else if (i < N && sub[j] != str[i]) {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
    return -1;
}
 
// Fills lps[] for given subttern sub[0..M-1]
void computeLPSArray(const char* sub, int M, int* lps)
{
    // length of the previous longest prefix suffix
    int len = 0;
 
    lps[0] = 0; // lps[0] is always 0
 
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M) {
        if (sub[i] == sub[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else // (sub[i] != sub[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0) {
                len = lps[len - 1];
 
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}