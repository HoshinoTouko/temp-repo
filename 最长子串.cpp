#include <iostream>
using namespace std;

int main() {
	int a[] = { -6, 2, 4, -7, 5, 3, 2, -1, 6, -9, 10, -2 };
	int n = sizeof(a)/sizeof(int);
	int p1, p2, start, end, max=1<<(sizeof(int)-1), t;
	
	for (p1 = 0; p1 < n; p1++) {
	    while(a[p1] < 0 && p1 < n) 
            p1++;
	    if (p1 == n) 
            break;
        t = 0;
        for (p2 = p1; p2 < n; p2++){
            t += a[p2];
            if (t > max){
                start = p1;
                end = p2;
                max = t;
            }
        }
    }
	
	cout<<"Max Value: "<<max<<" Start at: "<<start<<" End at: "<<end;
	
	return 0;
}
