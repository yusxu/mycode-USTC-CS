#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <chrono>
using namespace std;
using namespace chrono;


int input_arr[1 << 17], sort_arr[1 << 17];
string result_out_path = "../output/radix_sort/result_";

long long RadixSort(int arr[], int length) {
	auto start = steady_clock::now();

	int count[16] = { 0 };
	int tmp[1 << 17] = { 0 };
	int radix = 1;

	// 取16为基，[1, 2^16-1]需要进行4次排序
	for (int j = 0; j < 4; j++) {

		// 初始化
		for (int i = 0; i <= 15; i++) {
			count[i] = 0;
		}

		// 计数排序
		for (int i = 0; i <= length - 1; i++) {
			count[(arr[i] / radix) % 16]++;
		}
		for (int i = 1; i <= 15; i++) {
			count[i] += count[i - 1];
		}
		// ps: count[i] contains the number of elements less than or equal to i
		// but array start at 0, so before write to tmp[], --count[i]
		for (int i = length - 1; i >= 0; i--) {
			tmp[--count[(arr[i] / radix) % 16]] = arr[i];
			//count[arr[i]]--;
		}
		// copy tmp to arr
		for (int i = 0; i <= length - 1; i++) {
			arr[i] = tmp[i];
		}
		radix *= 16;
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
	time_out.open("../output/radix_sort/time.txt");
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

		runtime = RadixSort(sort_arr, 1 << n[i]);
		printf("n:%d runtime:%lld nanoseconds\n", n[i], runtime);

		// output time and result
		time_out << runtime << endl;
		output_result(sort_arr, n[i]);
	}
	time_out.close();

	//system("pause");
	return 0;
}