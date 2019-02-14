#include<cstdio>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<chrono>
#include<ctime>
#include<cmath>

#define ASIZE 6

using namespace std;
using namespace chrono;

char T[(1 << 17) + 1];
char P[7];

int n[5] = { 5, 8, 11, 14, 17 };
int m[5] = { 2, 3, 4, 5, 6 };

int match_start_at;

int Bc[ASIZE];

void PreBc(int i) {
	for (int j = 0; j < ASIZE; j++) {
		Bc[j] = m[i];
	}
	for (int j = 0; j < m[i] - 1; j++) {
		Bc[P[j] - 'A'] = m[i] - j - 1;
	}
}

long long BMH(int i) {
	auto start = steady_clock::now();

	int n = strlen(T);
	int m = strlen(P);
	int j;
	char c;
	PreBc(i);
	j = 0;
	while (j < n - m + 1) {
		c = T[j + m - 1];
		if (P[m - 1] == c && memcmp(P, T + j, m) == 0) {
			cout << "pattern occurs with T+" << j << endl;
			if (match_start_at == -1) {
				match_start_at = j;
			}
		}
		j += Bc[c - 'A'];
	}


	auto end = steady_clock::now();
	auto duration = duration_cast<nanoseconds>(end - start);
	return duration.count();
}

ifstream input;

void DoInput(int i) {
	char ch;
	int j;

	for (j = 0; j < (1 << n[i]); j++) {
		input >> ch;
		if (ch != ',') {
			T[j] = ch;
		}
		else {
			cout << "txt error" << endl;
		}
	}
	T[j] = '\0';
	input >> ch;
	if (ch != ',') {
		cout << "missing ','. please check out your input" << endl;
	}
	for (j = 0; j < m[i]; j++) {
		input >> ch;
		if (ch != ';') {
			P[j] = ch;
		}
		else {
			cout << "pattern error" << endl;
		}
	}
	P[j] = '\0';
	input >> ch;
	if (ch != ';') {
		cout << "missing ';'. please check out your input" << endl;
	}
	/*cout << T << endl;
	cout << P << endl;*/
}

int main()
{
	//FILE *input;
	//input = fopen("../input/input.txt", "r");
	//fopen_s(&input, "../input/input.txt", "r");

	input.open("../input/input.txt");
	ofstream output("../output/BMH/output.txt");

	int i;
	long long runtime;
	for (i = 0; i < 5; i++) {
		DoInput(i);

		cout << "n=" << (1 << n[i]) << " m=" << m[i] << endl;
		match_start_at = -1;
		runtime = BMH(i);
		cout << "match_start_at " << match_start_at << endl << endl;
		output << (1 << n[i]) << ' ' << m[i] << ' ' << match_start_at << ' ' << runtime << endl;
	}

	input.close();
	output.close();
	//system("pause");
	return 0;
}