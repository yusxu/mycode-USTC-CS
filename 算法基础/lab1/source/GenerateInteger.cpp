#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main()
{
	ofstream fileout;
	fileout.open("../input/input_integer.txt");

	srand(unsigned(time(0)));                      //初始化随机种子  
	for (int i = 0; i < (1 << 23); i++) {
		fileout << rand() + rand() + 1 << endl;    //rand()函数生成0-32767区间的随机数  
												   //rand() + rand () + 1生成1-65535区间的随机数  
	}

    return 0;
}
