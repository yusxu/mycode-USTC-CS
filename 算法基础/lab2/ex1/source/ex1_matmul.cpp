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
int n[4] = { 5,10,20,30 };
int input_arr[31];
int m[31][31], s[31][31];

long long MatrixChainOrder(int p[], int length) {
	auto start = steady_clock::now();

	int n = length;
	for (int i = 1; i <= n; i++) {
		m[i][i] = 0;
	}
	for (int l = 2; l <= n; l++) {
		for (int i = 1; i <= n - l + 1; i++) {
			int j = i + l - 1;
			m[i][j] = 0xfffffff;
			for (int k = i; k <= j - 1; k++) {
				int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (q < m[i][j]) {
					m[i][j] = q;
					s[i][j] = k;
				}
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



void DoPrint(int i, int j) {
	if (i == j) {
		result_out << "A" << i;
	}
	else {
		result_out << "(";
		DoPrint(i, s[i][j]);
		DoPrint(s[i][j] + 1, j);
		result_out << ")";
	}
}

void PrintOptimalParens(int i, int j) {
	result_out << "矩阵链规模为" << int_to_string(j) << "时，最优乘法顺序为：";
	DoPrint(i, j);
	result_out << endl;
}



int main()
{
	ifstream input;
	input.open("../input/input.txt");
	if (!input) {
		cout << "fail to open input.txt!" << endl;
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

	for (int i = 0; i < 31; i++) {
		input >> input_arr[i];
	}

	long long runtime;

	for (int i = 0; i < 4; i++) {

		runtime = MatrixChainOrder(input_arr, n[i]);
		printf("n:%d runtime:%lld nanoseconds\n", n[i], runtime);

		// output time and result
		time_out << runtime << endl;
		PrintOptimalParens(1, n[i]);
	}
	input.close();
	time_out.close();
	result_out.close();
	//system("pause");
	return 0;
}
