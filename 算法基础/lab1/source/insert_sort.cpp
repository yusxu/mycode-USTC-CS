#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <string>
#include <sstream>
#include <chrono>
using namespace std;
using namespace chrono;

int input_arr[1 << 17], sort_arr[1 << 17];
string result_out_path = "../output/insert_sort/result_";

long long InsertSort(int arr[], int length) {
	auto start = system_clock::now();

	int i, j;
	int tmp;


	for (i = 1; i < length; i++) {                    //从1位置开始遍历数组,0位置视为已排序
		tmp = arr[i];                                 //记录待插入的元素
		for (j = i - 1; j >= 0 && arr[j]>tmp; j--) {
			arr[j + 1] = arr[j];                      //待插入元素前面有元素比tmp大，较大的元素向后移动一个位置
		}
		arr[j + 1] = tmp;
	}

	auto end = system_clock::now();
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
	time_out.open("../output/insert_sort/time.txt");
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

		runtime = InsertSort(sort_arr, 1 << n[i]);
		printf("n:%d runtime:%lld nanoseconds\n", n[i], runtime);
		
		// output time and result
		time_out << runtime << endl;
		output_result(sort_arr, n[i]);
	}
	time_out.close();

	//system("pause");
    return 0;
}
