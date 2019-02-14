#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <chrono>
using namespace std;
using namespace chrono;



int input_arr[1 << 17], sort_arr[1 << 17];
string result_out_path = "../output/counting_sort/result_";

/*void swap(int &a, int &b) {
int t = a;
a = b;
b = t;
}*/


long long CountingSort(int A[], int B[], int length) {
	auto start = steady_clock::now();

	int count[65536] = {0};
	for (int i = 0; i <= length - 1; i++) {
		count[A[i]]++;
	}
	for (int i = 2; i <= 65535; i++) {
		count[i] += count[i - 1];
	}
	// ps: count[i] contains the number of elements less than or equal to i
	// but array start at 0, so before write to B, --count[i]
	for (int i = length - 1; i >= 0; i--) {
		B[--count[A[i]]] = A[i];
		//count[A[i]]--;
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

void output_result(int arr[], int n) {
	ofstream result_out;
	string output_path = result_out_path + int_to_string(n) + ".txt";
	result_out.open(output_path.c_str());

	for (int i = 0; i < 1 << n; i++) {
		result_out << arr[i] << endl;
	}

	result_out.close();
}

int main()
{
	int n[6] = { 2,5,8,11,14,17 };

	ifstream input;
	input.open("../input/input_integer.txt");
	ofstream time_out;
	time_out.open("../output/counting_sort/time.txt");
	if (!time_out) {
		cout << "fail to open file!" << endl;
	}

	for (int i = 0; i < 1 << 17; i++) {
		input >> input_arr[i];
	}

	long long runtime;

	for (int i = 0; i < 6; i++) {
		// copy array
		for (int j = 0; j < 1 << n[i]; j++) {
			sort_arr[j] = input_arr[j];
		}

		runtime = CountingSort(input_arr ,sort_arr, 1 << n[i]);
		printf("n:%d runtime:%lld nanoseconds\n", n[i], runtime);

		// output time and result
		time_out << runtime << endl;
		output_result(sort_arr, n[i]);
	}
	time_out.close();

	//system("pause");
	return 0;
}
