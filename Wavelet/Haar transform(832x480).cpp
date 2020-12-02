// 832 * 480
#include "Haar transform(832x480).h"

int main(void)
{
	////////// Step 1. YCbCr 8-bit input image를 읽어서 메모리에 저장 //////////
	m_ui8Comp0 = new unsigned char*[3];
	for (int i = 0; i < 3; i++)
		m_ui8Comp0[i] = new unsigned char[m_iSize[i]];

	FILE *original_image = fopen(ori_path, "rb");
	if (!original_image) {
		cout << "original image not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++)
		fread(&(m_ui8Comp0[ch][0]), sizeof(unsigned char), m_iSize[ch], original_image); // original image의 픽셀값을 배열에 저장
	fclose(original_image);

	////////// Step 2. haar wavelet transform //////////
	/* 
	1단계
	*/

	/* 수평방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	unsigned char** L1 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		L1[ch] = new unsigned char[m_iSize[ch] / 2];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 2; i++)
			L1[ch][i] = m_ui8Comp0[ch][2 * i] / 2 + m_ui8Comp0[ch][2 * i + 1] / 2;

	unsigned char** H1 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		H1[ch] = new unsigned char[m_iSize[ch] / 2];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 2; i++)
			H1[ch][i] = m_ui8Comp0[ch][2 * i] / 2 - m_ui8Comp0[ch][2 * i + 1] / 2;

	/* 수직방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	unsigned char** LL1 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		LL1[ch] = new unsigned char[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 4; i++)
			LL1[ch][i] = L1[ch][i] / 2 + L1[ch][WIDTH / 2 + i] / 2;

	unsigned char** LH1 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		LH1[ch] = new unsigned char[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 4; i++)
			LH1[ch][i] = L1[ch][i] / 2 - L1[ch][WIDTH / 2 + i] / 2;

	unsigned char** HL1 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		HL1[ch] = new unsigned char[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 4; i++)
			HL1[ch][i] = H1[ch][i] / 2 + H1[ch][WIDTH / 2 + i] / 2;

	unsigned char** HH1 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		HH1[ch] = new unsigned char[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 4; i++)
			HH1[ch][i] = H1[ch][i] / 2 - H1[ch][WIDTH / 2 + i] / 2;

	coeffiecient = new unsigned char*[3];
	for (int i = 0; i < 3; i++)
		coeffiecient[i] = new unsigned char[m_iSize[i]];

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL1[ch][width[ch] / 2 * i + j];
				coeffiecient[ch][width[ch] * i + j + width[ch] / 2] = HL1[ch][(width[ch] / 2) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j] = LH1[ch][(width[ch] / 2) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j + width[ch] / 2] = HH1[ch][(width[ch] / 2) * i + j];
			}

	for (int ch = 0; ch < 3; ch++)
		delete[] L1[ch];
	delete[] L1;

	for (int ch = 0; ch < 3; ch++)
		delete[] H1[ch];
	delete[] H1;

	for (int ch = 0; ch < 3; ch++)
		delete[] HL1[ch];
	delete[] HL1;

	for (int ch = 0; ch < 3; ch++)
		delete[] LH1[ch];
	delete[] LH1;

	for (int ch = 0; ch < 3; ch++)
		delete[] HH1[ch];
	delete[] HH1;

	FILE* coefficient1 = fopen("./test/RaceHorsesC_832x480_yuv420_8bit_coefficient1.yuv", "wb");
	if (!coefficient1) {
		cout << "coefficient1 not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++) {
		fwrite(&(coeffiecient[ch][0]), sizeof(unsigned char), m_iSize[ch], coefficient1);
	}
	fclose(coefficient1);

	/*
	2단계
	*/

	/* 수평방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	unsigned char** L2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		L2[ch] = new unsigned char[m_iSize[ch] / 8];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 8; i++)
			L2[ch][i] = LL1[ch][2 * i] / 2 + LL1[ch][2 * i + 1] / 2;

	unsigned char** H2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		H2[ch] = new unsigned char[m_iSize[ch] / 8];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 8; i++)
			H2[ch][i] = LL1[ch][2 * i] / 2 - LL1[ch][2 * i + 1] / 2;

	/* 수직방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	unsigned char** LL2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		LL2[ch] = new unsigned char[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 16; i++)
			LL2[ch][i] = L2[ch][i] / 2 + L2[ch][WIDTH / 2 + i] / 2;

	unsigned char** LH2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		LH2[ch] = new unsigned char[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 16; i++)
			LH2[ch][i] = L2[ch][i] / 2 - L2[ch][WIDTH / 2 + i] / 2;

	unsigned char** HL2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		HL2[ch] = new unsigned char[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 16; i++)
			HL2[ch][i] = H2[ch][i] / 2 + H2[ch][WIDTH / 2 + i] / 2;

	unsigned char** HH2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		HH2[ch] = new unsigned char[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 16; i++)
			HH2[ch][i] = H2[ch][i] / 2 - H2[ch][WIDTH / 2 + i] / 2;

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL2[ch][width[ch] / 4 * i + j];
				coeffiecient[ch][width[ch] * i + j + width[ch] / 4] = HL2[ch][(width[ch] / 4) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j] = LH2[ch][(width[ch] / 4) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j + width[ch] / 4] = HH2[ch][(width[ch] / 4) * i + j];
			}

	for (int ch = 0; ch < 3; ch++)
		delete[] L2[ch];
	delete[] L2;

	for (int ch = 0; ch < 3; ch++)
		delete[] H2[ch];
	delete[] H2;

	for (int ch = 0; ch < 3; ch++)
		delete[] HL2[ch];
	delete[] HL2;

	for (int ch = 0; ch < 3; ch++)
		delete[] LH2[ch];
	delete[] LH2;

	for (int ch = 0; ch < 3; ch++)
		delete[] HH2[ch];
	delete[] HH2;

	FILE* coefficient2 = fopen("./test/RaceHorsesC_832x480_yuv420_8bit_coefficient2.yuv", "wb");
	if (!coefficient2) {
		cout << "coefficient2 not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++) {
		fwrite(&(coeffiecient[ch][0]), sizeof(unsigned char), m_iSize[ch], coefficient2);
	}
	fclose(coefficient2);

	/*
	3단계
	*/

	/* 수평방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	unsigned char** L3 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		L3[ch] = new unsigned char[m_iSize[ch] / 32];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 32; i++)
			L3[ch][i] = LL2[ch][2 * i] / 2 + LL2[ch][2 * i + 1] / 2;

	unsigned char** H3 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		H3[ch] = new unsigned char[m_iSize[ch] / 32];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 32; i++)
			H3[ch][i] = LL2[ch][2 * i] / 2 - LL2[ch][2 * i + 1] / 2;

	/* 수직방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	unsigned char** LL3 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		LL3[ch] = new unsigned char[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 64; i++)
			LL3[ch][i] = L3[ch][i] / 2 + L3[ch][WIDTH / 2 + i] / 2;

	unsigned char** LH3 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		LH3[ch] = new unsigned char[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 64; i++)
			LH3[ch][i] = L3[ch][i] / 2 - L3[ch][WIDTH / 2 + i] / 2;

	unsigned char** HL3 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		HL3[ch] = new unsigned char[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 64; i++)
			HL3[ch][i] = H3[ch][i] / 2 + H3[ch][WIDTH / 2 + i] / 2;

	unsigned char** HH3 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		HH3[ch] = new unsigned char[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < m_iSize[ch] / 64; i++)
			HH3[ch][i] = H3[ch][i] / 2 - H3[ch][WIDTH / 2 + i] / 2;

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 8; i++)
			for (int j = 0; j < width[ch] / 8; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL3[ch][width[ch] / 8 * i + j];
				coeffiecient[ch][width[ch] * i + j + width[ch] / 8] = HL3[ch][(width[ch] / 8) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / + 8) + j] = LH3[ch][(width[ch] / 8) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 8) + j + width[ch] / 8] = HH3[ch][(width[ch] / 8) * i + j];
			}

	for (int ch = 0; ch < 3; ch++)
		delete[] L3[ch];
	delete[] L3;

	for (int ch = 0; ch < 3; ch++)
		delete[] H3[ch];
	delete[] H3;

	for (int ch = 0; ch < 3; ch++)
		delete[] HL3[ch];
	delete[] HL3;

	for (int ch = 0; ch < 3; ch++)
		delete[] LH3[ch];
	delete[] LH3;

	for (int ch = 0; ch < 3; ch++)
		delete[] HH3[ch];
	delete[] HH3;

	FILE* coefficient3 = fopen("./test/RaceHorsesC_832x480_yuv420_8bit_coefficient3.yuv", "wb");
	if (!coefficient3) {
		cout << "coefficient3 not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++) {
		fwrite(&(coeffiecient[ch][0]), sizeof(unsigned char), m_iSize[ch], coefficient3);
	}
	fclose(coefficient3);


	//FILE* test = fopen("./test/RaceHorsesC_832x480_yuv420_8bit_coefficient1.yuv", "wb");
	//if (!test) {
	//	cout << "test not open" << endl;
	//	return 0;
	//}

	//for (int ch = 0; ch < 3; ch++) {
	//	fwrite(&(coeffiecient1[ch][0]), sizeof(unsigned char), m_iSize[ch], test);
	//}
	//fclose(test);

	return 0;
}
