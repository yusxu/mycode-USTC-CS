#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main()
{
	ofstream fileout;
	fileout.open("../input/input.txt");

	srand(unsigned(time(0)));                      //初始化随机种子  
	for (int i = 0; i < 31; i++) {
		fileout << rand() % 30 + 1 << endl;
	}
	return 0;
}