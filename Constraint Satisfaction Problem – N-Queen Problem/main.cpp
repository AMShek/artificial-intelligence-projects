#include <iostream>
#include <Windows.h>
#include "queen.h"

using namespace std;

int main() {
	int size;
	string algorithm;
	
	cout<<"Please choose the size of board."<<endl;
	cin>>size;
	Game queen(size);
	
	cout << "Please choose the search alogorithm:" << endl;
	cout << "BT-Backtracking" << endl;
	cout << "FC-Forward Checking" << endl;
	cin>>algorithm;
	int times=10;
	while(times--){
	if(algorithm=="bt"){
		LARGE_INTEGER t1,t2,tc;
		QueryPerformanceFrequency(&tc);
		QueryPerformanceCounter(&t1);
		
		queen.Backtracking(0);
		
		QueryPerformanceCounter(&t2);
		//printf("Time Elapsed:%f\n",(t2.QuadPart - t1.QuadPart)*1.0/tc.QuadPart);		
		printf("%f\n",(t2.QuadPart - t1.QuadPart)*1.0/tc.QuadPart);		
		//queen.printSolutions();
	}	
	else if (algorithm=="fc"){
		LARGE_INTEGER t1,t2,tc;
		QueryPerformanceFrequency(&tc);
		QueryPerformanceCounter(&t1);
		
		queen.ForwardChecking(0);

		QueryPerformanceCounter(&t2);
		//printf("Time Elapsed:%f\n",(t2.QuadPart - t1.QuadPart)*1.0/tc.QuadPart);		
		printf("%f\n",(t2.QuadPart - t1.QuadPart)*1.0/tc.QuadPart);		
		//queen.printSolutions();
	}
	}
	//system("pause");
	return 0;
}
