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
string result_out_path = "../output/heap_sort/result_";

void swap(int &a, int &b) {
	int t = a;
	a = b;
	b = t;
}

void max_heapify(int arr[], int start, int end) {
	int current = start;
	int num = arr[start];
	// lc = 2*i+1  rc = 2*i+2
	int child = 2 * start + 1;
	for (; child <= end; current = child, child = 2 * child + 1) {
		// if rc > lc
		if (child < end && arr[child] < arr[child + 1]) {
			child += 1;
		}
		if (num > arr[child]) {
			// num at its right position
			break;
		}
		else {
			arr[current] = arr[child];
			arr[child] = num;
		}
	}
}

void build_max_heap(int arr[], int end) {
	// father = (i-1)/2
	for (int i = (end - 1) / 2; i >= 0; i--) {
		max_heapify(arr, i, end);
	}
}

long long HeapSort(int arr[], int length) {
	auto start = steady_clock::now();

	build_max_heap(arr, length - 1);
	for (int i = length - 1; i > 0; i--) {
		swap(arr[0], arr[i]);
		max_heapify(arr, 0, i - 1);
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
	time_out.open("../output/heap_sort/time.txt");
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

		runtime = HeapSort(sort_arr, 1 << n[i]);
		printf("n:%d runtime:%lld nanoseconds\n", n[i], runtime);

		// output time and result
		time_out << runtime << endl;
		output_result(sort_arr, n[i]);
	}
	time_out.close();

	//system("pause");
	return 0;
}
