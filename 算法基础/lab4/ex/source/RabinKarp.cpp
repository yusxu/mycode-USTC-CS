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


long long RabinKarp() {
	auto start = steady_clock::now();

	/* in this complement, s = s(in book) - 1 */

	int n = strlen(T);
	int m = strlen(P);
	int d = 10;
	int h = (int)pow((double)d, m - 1);
	int p = 0;
	int t = 0;
	
	for (int i = 0; i < m; i++) {
		p = d * p + P[i];
		t = d * t + T[i];
	}
	for (int s = 0; s <= n - m; s++) {
		if (p == t) {
			if (memcmp(P, T + s, m) == 0) {
				cout << "pattern occurs with T+" << s << endl;
				if (match_start_at == -1) {
					match_start_at = s;
				}
			}
		}
		if (s < n - m) {
			t = d * (t - T[s] * h) + T[s + m];
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
	ofstream output("../output/RK/output.txt");

	int i;
	long long runtime;
	for (i = 0; i < 5; i++) {
		DoInput(i);

		cout << "n=" << (1 << n[i]) << " m=" << m[i] << endl;
		match_start_at = -1;
		runtime = RabinKarp();
		cout << "match_start_at " << match_start_at << endl << endl;
		output << (1 << n[i]) << ' ' << m[i] << ' ' << match_start_at << ' ' << runtime << endl;
	}

	input.close();
	output.close();
	//system("pause");
    return 0;
}