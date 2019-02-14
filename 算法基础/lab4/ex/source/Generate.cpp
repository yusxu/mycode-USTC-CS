#include<fstream>
#include<ctime>

using namespace std;

int n[5] = {5, 8, 11, 14, 17};
int m[5] = {2, 3, 4, 5, 6};

int main(){
	srand(unsigned(time(0)));
	ofstream data;
	data.open("../input/input.txt");
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 1<<n[i]; j++){
			data<< char(rand()%6 + 'A');
		}
		data<<',';
		for(int j = 0; j < m[i]; j++){
			data<< char(rand()%6 + 'A');
		}
		data<<';';
	}
	data.close();
	return 0;
}


