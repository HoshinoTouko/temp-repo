#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
using namespace std;

int hashConflictTime = 0;
/*
 Rabin-Karp
*/
int rkHash(string find, string text, int p, int dicLen);
int KMPIndex(string find, string text);
int KMPIndexNextval(string find, string text);
int bf(string find, string text);
int randInt(int a, int b){
    return (rand() / double(RAND_MAX)) * (b-a+1) + a;
}

int main(){
    string find = "", text = "";
    //Generate a random string
    srand(time(NULL));
    for (int i = 0; i < 10000000; i++){
        text += randInt(97, 122);
    }
    find = text.substr(9999000, 500);
    //cout<<"Text: "<<text<<endl;
    cout<<"Find: "<<find<<endl;
    
    int turn = 0, times, result;
    double startTime, endTime, useTime;
    double rkTotal = 0, kmpTotal = 0, kmpNextvalTotal = 0, bfTotal = 0;
    cout<<"Times: ";
    cin>>times;
    cout<<endl;
    for (int i = 0; i < times; i++){
        startTime = clock();
        switch (turn){
            case 0:
                //printf("Rabin-Karp: ");
                result = rkHash(find, text, 101, 26);
                break;
            case 1:
                //printf("KMP: ");
                result = KMPIndex(find, text);
                break;
            case 2:
                //printf("KMP-nextval: ");
                result = KMPIndexNextval(find, text);
                break;
            case 3:
                //printf("BF: ");
                result = KMPIndexNextval(find, text);
                break;
            default:
                break;
        }
        endTime = clock();
        useTime = endTime - startTime;
        //printf("Result: %d. Time: %.2f ms\n", result, useTime);
        switch (turn){
            case 0:
                rkTotal += useTime;
                break;
            case 1:
                kmpTotal += useTime;
                break;
            case 2:
                kmpNextvalTotal += useTime;
                break;
            case 3:
                bfTotal += useTime;
                break;
            default:
                break;
        }
        
        turn = (turn + 1) % 4;
    }
        
    cout<<KMPIndexNextval(find, text)<<endl;
    cout<<"rkTotal: "<<rkTotal<<endl;
    cout<<"hashConflictTime: "<<hashConflictTime<<endl;
    cout<<"kmpTotal: "<<kmpTotal<<endl;
    cout<<"kmpNextvalTotal: "<<kmpNextvalTotal<<endl;
    cout<<"bfTotal: "<<bfTotal<<endl;
    
    return 0;
}

/* p must be a prime number */
int rkHash(string find, string text, int p, int dicLen){
    int find_len = find.length();
    int text_len = text.length();
    int find_hash = 0, text_hash = 0;
    
    int j = 0;
    
    /* Pre resolve the hash */
    int h = 1;
    for (int i = 0; i < find_len - 1; i++){
        h = (h * dicLen) % p;
    }
    
    /* Pre resolve the find string and the first block of text string */
    for (int i = 0; i < find_len; i++){
        find_hash = (dicLen * find_hash + find[i]) % p;
        text_hash = (dicLen * text_hash + text[i]) % p;
        //cout<<find[i]<<" "<<text[i]<<endl;
    }
    //cout<<"find_hash "<<find_hash<<endl;
    //cout<<"text_hash "<<text_hash<<endl;
    
    /* Check them */
    for (int i = 0; i <= text_len - find_len; i++){
        if (find_hash == text_hash){
            /* If their hash match, check the characters one by one */
            for (j = 0; j < find_len; j++){
                if (find[j] != text[i + j]){
                    //cout<<find[j]<<" "<<text[i + j]<<endl;
                    //cout<<"Hash conflict"<<endl;
                    hashConflictTime++;
                    break;
                }
            }
            if (j == find_len){
                return i;
                //cout<<"Find at index: "<<i<<endl;
                //j = 0;
            }
        } 
        /* Calculate the next block of the original text */
        if (i < text_len - find_len){
            text_hash = (dicLen * (text_hash - text[i] * h) + text[i + find_len]) % p;
            text_hash = (text_hash + p) % p;
            //cout<<"text_hash "<<text_hash<<endl;
        }
    }
    return -1;
}

int KMPIndex(string find, string text){
    int find_len = find.length();
    int text_len = text.length();
    
    int next[find_len];
    int i = 0;
    
    /* Pre resolve the find string */
    /* KMP */
    int j, k;
    j = 0;
    k = -1;
    next[0] = -1;
    
    while (j < find_len - 1){
        if (k == -1 || find[j] == find[k]){
            j++;
            k++;
            next[j] = k;
        }
        else {
            k = next[k];
        }
    }
    
    /* KMP */
    while (i < text_len and j < find_len){
        if (j == -1 || text[i] == find[j]){
            i++;
            j++;
        }
        else{
            j = next[j];
        }
    }
    if (j >= find_len){
        return (i - find_len);
    }
    else {
        return -1;
    }
}

int KMPIndexNextval(string find, string text){
    int find_len = find.length();
    int text_len = text.length();
    
    int nextval[find_len];
    int i = 0;
    
    /* Pre resolve the find string */
    /* KMP nextval */
    int j, k;
    j = 0;
    k = -1;
    nextval[0] = -1;
    
    while (j < find_len - 1){
        if (k == -1 || find[j] == find[k]){
            j++;
            k++;
            if (find[j] != find[k]){
                nextval[j] = k;
            }
            else {
                nextval[j] = nextval[k];
            }
        }
        else {
            k = nextval[k];
        }
    }
    
    /* KMP */
    while (i < text_len and j < find_len){
        if (j == -1 || text[i] == find[j]){
            i++;
            j++;
        }
        else{
            j = nextval[j];
        }
    }
    if (j >= find_len){
        return (i - find_len);
    }
    else {
        return -1;
    }
}

int BFMatch(string find, string text){
    int i, j;
    int find_len = find.length();
    int text_len = text.length();
    i =0;

    while(i < text_len){
       j = 0;
       while(text[i] == find[j] && j < text_len){
           i++;
           j++;
       }
        if(find_len == j){
           return i - find_len;
       }
       i = i - j + 1;
    }
    return -1;
}
