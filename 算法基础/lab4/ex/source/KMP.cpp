#include<cstdio>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<chrono>
#include<ctime>
#include<cmath>
using namespace std;
using namespace chrono;

char T[(1 << 17) + 1];
char P[7];

int n[5] = { 5, 8, 11, 14, 17 };
int m[5] = { 2, 3, 4, 5, 6 };

int match_start_at;

int pi[6];

void ComputePrefixFunction() {
	int m = strlen(P);
	pi[0] = -1;
	int k = -1;
	for (int q = 1; q < m; q++) {
		while (k > -1 && P[k + 1] != P[q]) {
			k = pi[k];
		}
		if (P[k + 1] == P[q]) {
			k = k + 1;
		}
		pi[q] = k;
	}
}

long long KMP() {
	auto start = steady_clock::now();

	int n = strlen(T);
	int m = strlen(P);
	ComputePrefixFunction();
	int q = -1;			// index of characters matched
	for (int i = 0; i < n; i++) {
		while (q > -1 && P[q + 1] != T[i]) {
			q = pi[q];
		}
		if (P[q + 1] == T[i]) {
			q = q + 1;
		}
		if (q == m - 1) {
			if (match_start_at == -1) {
				match_start_at = i - m + 1;
			}
			cout << "pattern occurs with T+" << i - m + 1 << endl;

			q = pi[q];
		}
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
	ofstream output("../output/KMP/output.txt");

	int i;
	long long runtime;
	for (i = 0; i < 5; i++) {
		DoInput(i);

		cout << "n=" << (1 << n[i]) << " m=" << m[i] << endl;
		match_start_at = -1;
		runtime = KMP();
		cout << "match_start_at " << match_start_at << endl << endl;
		output << (1 << n[i]) << ' ' << m[i] << ' ' << match_start_at << ' ' << runtime << endl;
	}

	input.close();
	output.close();
	//system("pause");
	return 0;
}