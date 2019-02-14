#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main()
{
	ofstream fileoutA, fileoutB;
	fileoutA.open("../input/inputA.txt");
	fileoutB.open("../input/inputB.txt");

	srand(unsigned(time(0)));                      //初始化随机种子 
	// inputA
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 15; j++) {
			fileoutA << (char)('A' + rand() % 26);
		}
		fileoutA << endl;
		for (int j = 0; j < (i + 1) * 10; j++) {
			fileoutA << (char)('A' + rand() % 26);
		}
		fileoutA << endl;
	}
	// inputB
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < (i + 1) * 15; j++) {
			fileoutB << (char)('A' + rand() % 26);
		}
		fileoutB << endl;
		for (int j = 0; j < 25; j++) {
			fileoutB << (char)('A' + rand() % 26);
		}
		fileoutB << endl;
	}
	return 0;
}
