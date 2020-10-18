// Scalar non-uniform quantization
#include "lloyd.h"

namespace SQ { // 8비트 스칼라 분균일 양자화기
	float *D;
	float *R;
	int L = 256; // 2^8 
}

void initialize() {
	SQ::D = new float[SQ::L];
	SQ::R = new float[SQ::L - 1];

	SQ::D[0] = 0;
	SQ::D[SQ::L] = 1024;

	int interval_size = 4;
	for (int i = 0; i < SQ::L; i++) {
		SQ::R[i] = (i + 0.5)*interval_size; 
	}
}

void calculateIntervals() {
	for (int i = 1; i < SQ::L; i++) {
		SQ::D[i] = (SQ::R[i] + SQ::R[i - 1]) / 2;
	}
}

void getNewLevels(unsigned short *P) {
	long int num, den;
	int lower, higher;

	for (int i = 0; i < SQ::L; i++) {
		// upper range for lower
		lower = ceilf(SQ::D[i]);
		// If last value then include it in higher
		if (i == SQ::L - 1)
			higher = ceilf(SQ::D[i + 1]);
		else
			higher = floorf(SQ::D[i + 1]);
		//		cout << i << " " << SQ::D[i] <<" "<< SQ::D[i+1] << endl;
		//		cout << "Lower: " << lower << endl;
		//		cout << "Higher: " << higher << endl;
		num = 0, den = 0;
		for (int j = lower; j <= higher; j++) {
			num += j * P[j];
			den += P[j];
		}
		if (den != 0) {
			SQ::R[i] = (float)num / den; // i 구간 내 모든 샘플의 평균
		}
	}
}

void printD() {
	cout << "Values of the intervals (D) are : " << endl;
	for (int i = 0; i < SQ::L + 1; i++) {
		cout << SQ::D[i] << " ";
	}
	cout << endl << endl;
}

void printR() {
	cout << "Values of the centroid (R) are : " << endl;
	for (int i = 0; i < SQ::L; i++) {
		cout << SQ::R[i] << " ";
	}
	cout << endl << endl;
}

float getQuantizationError(float previousMSE, float currentMSE) {
	return (previousMSE - currentMSE) / currentMSE;
}

//float getMSE(char *memblock) {
//
//	int N = ISIZE;
//	int error;
//	double mse, sum = 0;
//	double psnr[3];
//
//	for (int i = 0; i < 3; i++) { // Y, Cb, Cr의 PSNR을 각각 측정
//		for (int ch = 0; ch < m_iSize[i]; ch++) {
//			error = m_ui16Comp[i][ch] - m_ui16Comp2[i][ch];
//			sum += error * error;
//		}
//		mse = sum / N;
//		psnr[i] = 20 * log10(1023 / sqrt(mse));
//	}
//	
//	double MSE = 0, diff;
//	float QV;
//	for (int i = 0; i < FileSizeinBytes; i++) {
//		unsigned char val = memblock[i];
//		QV = getQuantizedValue(val);
//		diff = pow((val - QV), 2);
//		MSE += diff;
//	}
//	MSE /= FileSizeinBytes;
//	return MSE;
//}
//
//float getQuantizedValue(float key) {
//
//	//Check if it is the MAx Value
//	if (key == MAX_VAL) {
//		//Last position value  of SQ::R
//		return SQ::R[SQ::L - 1];
//	}
//	int i;
//	for (i = 0; i < SQ::L; i++) {
//		if (key >= SQ::T[i] && key < SQ::T[i + 1])
//			break;
//	}
//
//	return SQ::R[i];
//}

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

	for (int ch = 0; ch < 3; ch++)
		fread(&(m_ui16Comp[ch][0]), sizeof(unsigned short), m_iSize[ch], input_image); // 10-bit 영상을 16-bit씩 read

	fclose(input_image);

	////////// Step 2. Input image에 scalar non-uniform quantization //////////
	m_ui8Comp = new unsigned char*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui8Comp[cnt] = new unsigned char[ISIZE];
	}



	////////////////////////////////////////////////////////////////////////////
	float previousMSE, currentMSE, quantization_error = 1;

	unsigned short **P = new unsigned short*[3]; // Counts stored in P
	for (int cnt = 0; cnt < 3; cnt++) {
		P[cnt] = new unsigned short[1024];
		memset(P[cnt], 0, sizeof(unsigned short) * 1024);
	}

	//for (int cnt = 0; cnt < 3; cnt++) {
	//	for (int i = 0; i < 1024; i++) {
	//		cout << i << ": " << P[cnt][i] << " ";
	//	}
	//	cout << endl << endl;
	//}

	unsigned short pixel; // 0부터 1023 사이의 픽셀값 카운트
	for (int i = 0; i < 3; i++) {
		for (int ch = 0; ch < m_iSize[i]; ch++) {
			pixel = m_ui16Comp[i][ch];
			P[i][pixel]++;
		}
	}

	for (int cnt = 0; cnt < 3; cnt++) {
	for (int i = 0; i < 1024; i++) {
		cout << i << ": " << P[cnt][i] << "\t\t\t";
	}
	cout << endl << endl;
	 }

	initialize();

	int iterationNo = 1;
	calculateIntervals();
	for (int ch = 0; ch < 3; ch++) {
		getNewLevels(P[ch]);
	}
	//printD();
	//printR();

	//while (quantization_error > 0.001) { // 양자화 에러가 threshold보다 작아질 때까지 반복적으로 업데이트
	//	cout << "------------------- Iteration No. " << iterationNo++
	//		<< ". ----------------------" << endl;
	//	calculateIntervals();
	//	getNewLevels(P);
	//	printD();
	//	printR();
	//	previousMSE = currentMSE;
	//	currentMSE = getMSE(memblock);
	//	cout << "Current MSE is: " << currentMSE << endl;
	//	quantization_error = getQuantizationError(previousMSE, currentMSE);
	//	cout << "양자화 에러: " << quantization_error << endl;
	//	cout << "xxxxxxxxxxxxxxxxxxxxxx--End of Iteration " << iterationNo - 1
	//		<< ". xxxxxxxxxxxxxxxxxxxxxx" << endl << endl;
	//}





} // end of main


