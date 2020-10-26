// Vector non-uniform quantization
#include "VQ.h"

namespace VQ_Y { // Y채널 벡터 분균일 양자화기
	float *D;
	float *R;
	int group[ISIZE/4];
}

void InitializeCodewords()
{
	// Y
	vec_Y = new unsigned short*[numOfVec[0]];
	for (int cnt = 0; cnt < numOfVec[0]; cnt++) {
		vec_Y[cnt] = new unsigned short[4];
	}

	for (int i = 0; i < numOfVec[0]; i++) {
		vec_Y[i][0] = m_ui16Comp[0][0 + 960 * (i / (width / 2)) + (2 * i)];
		vec_Y[i][1] = m_ui16Comp[0][1 + 960 * (i / (width / 2)) + (2 * i)];
		vec_Y[i][2] = m_ui16Comp[0][960 + 960 * (i / (width / 2)) + (2 * i)];
		vec_Y[i][3] = m_ui16Comp[0][961 + 960 * (i / (width / 2)) + (2 * i)];
	}

	//for (int i = 0; i < numOfVec[0]; i++) {
	//	cout << i << ':' << vec_Y[i][0] << ',' << vec_Y[i][1] << ',' << vec_Y[i][2] << ',' << vec_Y[i][3] << endl;
	//}
}

void kmeans()
{
	double ** means = new double*[K];

	for (int i = 0; i < K; i++)
		means[i] = new double[4];

	////  set new means  ////
	for (int i = 0; i < K; i++)
		for (int j = 0; j < 4; j++)
			means[i][j] = vec_Y[i + 4][j];

	//for (int i = 0; i < K; i++) {
	//	for (int j = 0; j < 4; j++)
	//		cout << means[i][j] << " ";
	//	cout << endl;
	//}

	////  clustering  ////
	for (;;) {
		int flag = 0;

		for (int i = 0; i < numOfVec[0]; i++) {
			double min_dis = 9999999;
			int min = -1;

			// calculating distances //
			for (int j = 0; j < K; j++) {
				double dis = 0;
				for (int l = 0; l < 4; l++) {
					dis += pow(float(vec_Y[i][l] - means[j][l]), 2);
				}
				cout << dis << endl;
				if (dis < min_dis) {
					min_dis = dis;
					min = j;
				}
			}

			// set groups //
			VQ_Y::group[i] = min;
		}

		//for (int i = 0; i < numOfVec[0]; i++) {
		//	cout << VQ_Y::group[i] << ' ';
		//}

		// allocate new memory for calculate //
		double **temp = new double*[K];
		int *count = new int[K];

		for (int i = 0; i < K; i++) {
			count[i] = 0;
			temp[i] = new double[4];
			for (int j = 0; j < 4; j++)
				temp[i][j] = 0;
		}

		// calculating new means //
		for (int i = 0; i < numOfVec[0]; i++) {
			count[VQ_Y::group[i]]++;
			for (int j = 0; j < 4; j++)
				temp[VQ_Y::group[i]][j] += vec_Y[i][j];
		}

		for (int i = 0; i < K; i++)
			for (int j = 0; j < 4; j++) {
				temp[i][j] /= count[i];
				if (fabs(temp[i][j] - means[i][j]) > 0.01) {
					flag++;
					means[i][j] = temp[i][j];
				}
			}

		cout << "flag: " << flag << endl;

		// break point //
		if (flag == 0)
			break;
	}

	cout << "hello";

	////  freeing memory  ////
	for (int i = 0; i < K; i++)
		delete means[i];
	delete means;
}

void printData() {
	for (int i = 0; i < K; i++) {
		cout << "vector" << i + 1 << " : ";
		for (int j = 0; j < 4; j++)
			cout << vec_Y[i][j] << " ";
		cout << "group : " << VQ_Y::group[i] << endl;
	}
}

int main()
{
	////////// Step 1. YCbCr 10-bit input image를 읽어서 메모리에 저장 //////////
	FILE *input_image = fopen("./input/RitualDance_960x540_10bit_420_frame100.yuv", "rb");

	if (!input_image) {
		cout << "file not open" << endl;
		return 0;
	}

	m_ui16Comp = new unsigned short*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui16Comp[cnt] = new unsigned short[ISIZE];
	}

	int m_iBit = 10;
	int bitFactor = 2;

	int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 };

	for (int ch = 0; ch < 3; ch++)
		fread(&(m_ui16Comp[ch][0]), sizeof(unsigned short), m_iSize[ch], input_image); // 10-bit 영상을 16-bit씩 read

	fclose(input_image);

	////////// Step 2. Input image에 vector non-uniform quantization //////////
	m_ui8Comp = new unsigned char*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui8Comp[cnt] = new unsigned char[ISIZE];
	}

	InitializeCodewords();
	kmeans();
	printData();
}
