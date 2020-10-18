// Scalar non-uniform quantization
#include "lloyd.h"

namespace SQ_Y { // Y채널 스칼라 분균일 양자화기
	float *D;
	float *R;
}

namespace SQ_Cb { // Cb채널 스칼라 분균일 양자화기
	float *D;
	float *R;
}

namespace SQ_Cr { // Cr채널 스칼라 분균일 양자화기
	float *D;
	float *R;
}

void initialize() {
	int interval_size = 4;

	// Y
	SQ_Y::D = new float[L + 1];
	SQ_Y::R = new float[L];

	SQ_Y::D[0] = 0;
	SQ_Y::D[L] = 1024;

	for (int i = 0; i < L; i++) {
		SQ_Y::R[i] = (i + 0.5)*interval_size;
	}

	// Cb
	SQ_Cb::D = new float[L + 1];
	SQ_Cb::R = new float[L];

	SQ_Cb::D[0] = 0;
	SQ_Cb::D[L] = 1024;

	for (int i = 0; i < L; i++) {
		SQ_Cb::R[i] = (i + 0.5)*interval_size;
	}

	// Cr
	SQ_Cr::D = new float[L + 1];
	SQ_Cr::R = new float[L];

	SQ_Cr::D[0] = 0;
	SQ_Cr::D[L] = 1024;

	for (int i = 0; i < L; i++) {
		SQ_Cr::R[i] = (i + 0.5)*interval_size;
	}
}

void calculateIntervals() {
	for (int i = 1; i < L; i++) { // Y
		SQ_Y::D[i] = (SQ_Y::R[i] + SQ_Y::R[i - 1]) / 2;
	}

	for (int i = 1; i < L; i++) { // Cb
		SQ_Cb::D[i] = (SQ_Cb::R[i] + SQ_Cb::R[i - 1]) / 2;
	}

	for (int i = 1; i < L; i++) { // Cr
		SQ_Cr::D[i] = (SQ_Cr::R[i] + SQ_Cr::R[i - 1]) / 2;
	}
}

void getNewLevels(unsigned short **P) {
	long int num, den;
	int lower, higher;

	for (int i = 0; i < L; i++) { // Y
		lower = ceilf(SQ_Y::D[i]);
		if (i == L - 1)
			higher = ceilf(SQ_Y::D[i + 1]);
		else
			higher = floorf(SQ_Y::D[i + 1]);
		// cout << i << " " << SQ_Y::D[i] <<" "<< SQ_Y::D[i+1] << endl;
		// cout << "Lower: " << lower << endl;
		// cout << "Higher: " << higher << endl;
		num = 0, den = 0;
		for (int j = lower; j <= higher; j++) {
			num += j * P[0][j];
			den += P[0][j];
		}
		if (den != 0) {
			SQ_Y::R[i] = (float)num / den; // i 구간 내 모든 샘플의 평균
		}
	}

	for (int i = 0; i < L; i++) { // Cb
		lower = ceilf(SQ_Cb::D[i]);
		if (i == L - 1)
			higher = ceilf(SQ_Cb::D[i + 1]);
		else
			higher = floorf(SQ_Cb::D[i + 1]);
		// cout << i << " " << SQ_Cb::D[i] <<" "<< SQ_Cb::D[i+1] << endl;
		// cout << "Lower: " << lower << endl;
		// cout << "Higher: " << higher << endl;
		num = 0, den = 0;
		for (int j = lower; j <= higher; j++) {
			num += j * P[1][j];
			den += P[1][j];
		}
		if (den != 0) {
			SQ_Cb::R[i] = (float)num / den; // i 구간 내 모든 샘플의 평균
		}
	}

	for (int i = 0; i < L; i++) { // Cr
		lower = ceilf(SQ_Cr::D[i]);
		if (i == L - 1)
			higher = ceilf(SQ_Cr::D[i + 1]);
		else
			higher = floorf(SQ_Cr::D[i + 1]);
		// cout << i << " " << SQ_Cr::D[i] <<" "<< SQ_Cr::D[i+1] << endl;
		// cout << "Lower: " << lower << endl;
		// cout << "Higher: " << higher << endl;
		num = 0, den = 0;
		for (int j = lower; j <= higher; j++) {
			num += j * P[2][j];
			den += P[2][j];
		}
		if (den != 0) {
			SQ_Cr::R[i] = (float)num / den; // i 구간 내 모든 샘플의 평균
		}
	}
}

float getQuantizedValue_Y(unsigned short key) {

	// Y
	if (key == 1023) { //Check if it is the MAX Value
		return SQ_Y::R[L - 1]; //Last position value of SQ_Y::R
	}
	int i;
	for (i = 0; i < L; i++) {
		if (key >= SQ_Y::D[i] && key < SQ_Y::D[i + 1])
			break;
	}

	return SQ_Y::R[i] / 4;
}

float getQuantizedValue_Cb(unsigned short key) {

	// Cb
	if (key == 1023) { //Check if it is the MAX Value
		return SQ_Cb::R[L - 1]; //Last position value of SQ_Cb::R
	}
	int i;
	for (i = 0; i < L; i++) {
		if (key >= SQ_Cb::D[i] && key < SQ_Cb::D[i + 1])
			break;
	}

	return SQ_Cb::R[i] / 4;
}

float getQuantizedValue_Cr(unsigned short key) {

	// Cr
	if (key == 1023) { //Check if it is the MAX Value
		return SQ_Cr::R[L - 1]; //Last position value of SQ_Cr::R
	}
	int i;
	for (i = 0; i < L; i++) {
		if (key >= SQ_Cr::D[i] && key < SQ_Cr::D[i + 1])
			break;
	}

	return SQ_Cr::R[i] / 4;
}

void printD() {
	cout << "[Y] Values of the intervals (D) are : " << endl; // Y
	for (int i = 0; i < L + 1; i++) {
		cout << SQ_Y::D[i] << " ";
	}
	cout << endl << endl;

	cout << "[Cb] Values of the intervals (D) are : " << endl; // Cb
	for (int i = 0; i < L + 1; i++) {
		cout << SQ_Cb::D[i] << " ";
	}
	cout << endl << endl;

	cout << "[Cr] Values of the intervals (D) are : " << endl; // Cr
	for (int i = 0; i < L + 1; i++) {
		cout << SQ_Cr::D[i] << " ";
	}
	cout << endl << endl;
}

void printR() {
	cout << "[Y] Values of the centroid (R) are : " << endl; // Y
	for (int i = 0; i < L; i++) {
		cout << SQ_Y::R[i] << " ";
	}
	cout << endl << endl;

	cout << "[Cb] Values of the centroid (R) are : " << endl; // Cb
	for (int i = 0; i < L; i++) {
		cout << SQ_Cb::R[i] << " ";
	}
	cout << endl << endl;

	cout << "[Cr] Values of the centroid (R) are : " << endl; // Cr
	for (int i = 0; i < L; i++) {
		cout << SQ_Cr::R[i] << " ";
	}
	cout << endl << endl;

}

int main()
{
	////////// Step 1. YCbCr 10-bit input image를 읽어서 메모리에 저장 //////////
	FILE *input_image = fopen("./input/RitualDance_960x540_10bit_420_frame350.yuv", "rb");

	if (!input_image) {
		cout << "Error: file not open" << endl;
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
	unsigned short **P = new unsigned short*[3]; // Counts stored in P
	for (int cnt = 0; cnt < 3; cnt++) {
		P[cnt] = new unsigned short[1024];
		memset(P[cnt], 0, sizeof(unsigned short) * 1024);
	}

	unsigned short pixel; // 0부터 1023 사이의 픽셀값 카운트
	for (int i = 0; i < 3; i++) {
		for (int ch = 0; ch < m_iSize[i]; ch++) {
			pixel = m_ui16Comp[i][ch];
			P[i][pixel]++;
		}
	}

	initialize();

	int iterationNo = 1;
	calculateIntervals();
	getNewLevels(P);

	for (int i = 0; i < 10000; i++) { // 정해진 횟수를 수행할 때 까지, decision level과 reconstruction level을 반복적으로 업데이트
		calculateIntervals();
		getNewLevels(P);
	}

	m_ui8Comp = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++) {
		m_ui8Comp[ch] = new unsigned char[ISIZE];
		memset(m_ui8Comp[ch], 0, sizeof(unsigned char)*ISIZE);
	}

	for (int i = 0; i < m_iSize[0]; i++) { // Y
		unsigned short origvalue = m_ui16Comp[0][i];
		unsigned char val = floor(getQuantizedValue_Y(origvalue) + 0.5);
		// cout << i << " " << (int)val << endl;
		m_ui8Comp[0][i] = val;
	}

	for (int i = 0; i < m_iSize[1]; i++) { // Cb
		unsigned short origvalue = m_ui16Comp[1][i];
		unsigned char val = floor(getQuantizedValue_Cb(origvalue) + 0.5);
		// cout << i << " " << (int)val << endl;
		m_ui8Comp[1][i] = val;
	}

	for (int i = 0; i < m_iSize[2]; i++) { // Cr
		unsigned short origvalue = m_ui16Comp[2][i];
		unsigned char val = floor(getQuantizedValue_Cr(origvalue) + 0.5);
		// cout << i << " " << (int)val << endl;
		m_ui8Comp[2][i] = val;
	}

	FILE* quantized_image = fopen("./output/RitualDance_960x540_8bit_420_frame350.yuv", "wb");
	for (int ch = 0; ch < 3; ch++)
		fwrite(&(m_ui8Comp[ch][0]), sizeof(unsigned char), m_iSize[ch], quantized_image); // 양자화된 8-bit image를 .yuv 포맷 파일로 저장

	fclose(quantized_image);

	////////// Step 3. Quantized image에 역양자화 수행 //////////
	m_ui16Comp2 = new unsigned short*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui16Comp2[cnt] = new unsigned short[ISIZE];
	}

	for (int ch = 0; ch < 3; ch++) {
		for (int i = 0; i < m_iSize[ch]; i++) {
			m_ui16Comp2[ch][i] = (unsigned short)(m_ui8Comp[ch][i] << 2); // 8-bit quantized image의 Y, Cb, Cr 성분 각각 역양자화를 수행하여 reconstruction image 복원
		}
	}

	////////// Step 4. Reconstructed image와 Input image 간 PSNR 측정 //////////
	int N = ISIZE;
	int error;
	double mse, sum = 0;
	double psnr[3];

	for (int i = 0; i < 3; i++) { // Y, Cb, Cr의 PSNR을 각각 측정
		for (int ch = 0; ch < m_iSize[i]; ch++) {
			error = m_ui16Comp[i][ch] - m_ui16Comp2[i][ch];
			sum += error * error;
		}
		mse = sum / N;
		psnr[i] = 20 * log10(1023 / sqrt(mse));
	}

	cout << "Y의 PSNR은 " << psnr[0] << "입니다." << endl;
	cout << "Cb의 PSNR은 " << psnr[1] << "입니다." << endl;
	cout << "Cr의 PSNR은 " << psnr[2] << "입니다." << endl;

	//FILE* reconstructed_image = fopen("./output/RitualDance_960x540_10bit_420_frame100.yuv", "wb");
	//for (int ch = 0; ch < 3; ch++)
	//	fwrite(&(m_ui16Comp2[ch][0]), sizeof(unsigned short), m_iSize[ch], reconstructed_image); // 역양자화된 10-bit image를 .yuv 포맷 파일로 저장

	//fclose(quantized_image);

	return 0;

} // end of main
