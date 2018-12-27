#include<tuple>
#include <time.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <ctime>
#include <list>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <iomanip> 
using namespace std;
void dat(int &d, int&r);
void dat2(int &d);
void SALBP_Solve(int d);
int main() {
 
		cout << "instance	Status	Obj	Time	Gap%" << endl;
	for(int d=4;d<11;d++){
	
	dat2(d);
	SALBP_Solve(d);

}










	system("pause");
	return 0;




}

