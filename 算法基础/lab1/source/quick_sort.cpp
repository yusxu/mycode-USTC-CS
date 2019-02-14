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
string result_out_path = "../output/quick_sort/result_";

void swap(int &a, int &b) {
	int t = a;
	a = b;
	b = t;
}

int partition(int arr[], int start, int end) {
	int lastnum = arr[end];
	int i = start - 1;
	for (int j = start; j < end; j++) {
		if (arr[j] <= lastnum) {
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[end]);
	return i + 1;
}

// [start, length]
void quick(int arr[], int start, int end) {
	if (start < end) {
		int q = partition(arr, start, end);
		quick(arr, start, q - 1);
		quick(arr, q + 1, end);
	}
}

long long QuickSort(int arr[], int length) {
	auto start = steady_clock::now();

	quick(arr, 0, length - 1);

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
	time_out.open("../output/quick_sort/time.txt");
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

		runtime = QuickSort(sort_arr, 1 << n[i]);
		printf("n:%d runtime:%lld nanoseconds\n", n[i], runtime);

		// output time and result
		time_out << runtime << endl;
		output_result(sort_arr, n[i]);
	}
	time_out.close();

	//system("pause");
	return 0;
}
