#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

#define k 256
#define num_data 20

int data[num_data][4];
int group[num_data];

void readfile();
void kmeans();
void printData();

void main() {
	readfile();
	kmeans();
	printData();
	getchar();
	return;
}


//////  file i/o  //////

void readfile() {
	int d;

	ifstream indata;
	indata.open("data.txt");

	for (int i = 0; i < num_data; i++) {
		for (int j = 0; j < 4; j++) {
			indata >> d;
			::data[i][j] = d;
		}
	}

	indata.close();
}

//////  k_means clustering  //////

void kmeans() {
	double ** means = new double*[k];

	for (int i = 0; i < k; i++)
		means[i] = new double[4];

	////  set new means  ////
	for (int i = 0; i < k; i++)
		for (int j = 0; j < 4; j++)
			means[i][j] = ::data[i + 4][j];


	////  clustering  ////
	for (;;) {
		int flag = 0;

		for (int i = 0; i < num_data; i++) {
			double min_dis = 9999;
			int min = -1;

			// calculating distances //
			for (int j = 0; j < k; j++) {
				double dis = 0;
				for (int l = 0; l < 4; l++)
					dis += pow(float(::data[i][l] - means[j][l]), 2);
				if (dis < min_dis) {
					min_dis = dis;
					min = j;
				}
			}

			// set groups //
			group[i] = min;
		}

		// allocate new memory for calculate //
		double **temp = new double*[k];
		int *count = new int[k];

		for (int i = 0; i < k; i++) {
			count[i] = 0;
			temp[i] = new double[4];
			for (int j = 0; j < 4; j++)
				temp[i][j] = 0;
		}

		// calculating new means //
		for (int i = 0; i < num_data; i++) {
			count[group[i]]++;
			for (int j = 0; j < 4; j++)
				temp[group[i]][j] += ::data[i][j];
		}

		for (int i = 0; i < k; i++)
			for (int j = 0; j < 4; j++) {
				temp[i][j] /= count[i];
				if (fabs(temp[i][j] - means[i][j]) > 0.000001) {
					flag++;
					means[i][j] = temp[i][j];
				}
			}
		cout << "flag: " << flag << endl;
		// break point //
		if (flag == 0)
			break;
	}

	////  freeing memory  ////
	for (int i = 0; i < k; i++)
		delete means[i];
	delete means;
}


//////  display function  //////

void printData() {
	for (int i = 0; i < num_data; i++) {
		cout << "data" << i + 1 << " : ";
		for (int j = 0; j < 4; j++)
			cout << ::data[i][j] << " ";
		cout << "group : " << group[i] << endl;
	}
}
