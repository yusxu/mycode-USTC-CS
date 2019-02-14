#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <chrono>
using namespace std;
using namespace chrono;

ofstream result_out;
string X, Y;
int b[91][91], c[91][91];

long long LCS_Length() {
	auto start = steady_clock::now();

	int m = X.length();
	int n = Y.length();
	for (int i = 1; i <= m; i++) {
		c[i][0] = 0;
	}
	for (int j = 0; j <= n; j++) {
		c[0][j] = 0;
	}
	// b[i][j] = 0 represent ↖ upleft
	// 1 represent ↑ up
	// 2 represent ← left
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (X[i - 1] == Y[j - 1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 0;					// upleft
			}
			else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
				b[i][j] = 1;					// up
			}
			else {
				c[i][j] = c[i][j - 1];
				b[i][j] = 2;					// left
			}
		}
	}

	auto end = steady_clock::now();
	auto duration = duration_cast<nanoseconds>(end - start);
	return duration.count();
}

string int_to_string(int a) {
	string res;
	stringstream ss;
	ss << a;
	ss >> res;
	return res;
}



void DoPrint(int xlen, int ylen) {
	if (xlen == 0 || ylen == 0) {
		return;
	}
	if (b[xlen][ylen] == 0) {		// upleft
		DoPrint(xlen - 1, ylen - 1);
		result_out << X[xlen - 1];
	}
	else if (b[xlen][ylen] == 1) {  // up
		DoPrint(xlen - 1, ylen);
	}
	else {                          // left
		DoPrint(xlen, ylen - 1);
	}
}

void PrintLCS(int i, int j) {
	if (i == 1) {
		result_out << "规模为(15," << j * 10 << ")的字符串组LCS长度为:" << c[X.length()][Y.length()] << endl;
		
	}
	else if (i == 2) {
		result_out << "规模为(" << j * 15 << ",25)的字符串组LCS长度为:" << c[X.length()][Y.length()] << endl;
	}
	result_out << "其中一个解为: '";
	DoPrint(X.length(), Y.length());
	result_out << "'" << endl;
}



int main()
{
	ifstream inputA, inputB;
	inputA.open("../input/inputA.txt");
	if (!inputA) {
		cout << "fail to open inputA.txt!" << endl;
	}
	inputB.open("../input/inputB.txt");
	if (!inputB) {
		cout << "fail to open inputB.txt!" << endl;
	}
	ofstream time_out;
	time_out.open("../output/time.txt");
	if (!time_out) {
		cout << "fail to open output time.txt!" << endl;
	}
	result_out.open("../output/result.txt");
	if (!result_out) {
		cout << "fail to open output result.txt!" << endl;
	}

	

	long long runtime;

	for (int i = 1; i <= 2; i++) {
		result_out << "第" << i << "组" << endl;
		for (int j = 1; j <= 6; j++) {
			if (i == 1) {
				inputA >> X;
				inputA >> Y;
			}
			else if (i == 2) {
				inputB >> X;
				inputB >> Y;
			}

			runtime = LCS_Length();
			printf("i:%d j:%d runtime:%lld nanoseconds\n", i, j, runtime);

			// output time and result
			time_out << runtime << endl;
			PrintLCS(i, j);
		}
	}
	inputA.close();
	inputB.close();
	time_out.close();
	result_out.close();
	//system("pause");
	return 0;
}
