// 832 * 480
#include "Haar transform(832x480).h"

int main(void)
{
	////////// Step 1. YCbCr 8-bit input image를 읽어서 메모리에 저장 //////////
	m_ui8Comp0 = new unsigned char*[3];
	for (int i = 0; i < 3; i++)
		m_ui8Comp0[i] = new unsigned char[m_iSize[i]];

	m_ui8Comp1 = new unsigned char*[3];
	for (int i = 0; i < 3; i++)
		m_ui8Comp1[i] = new unsigned char[m_iSize[i]];

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
	1단계 변환
	*/

	/* 수평방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	short** L1 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		L1[ch] = new short[m_iSize[ch] / 2];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 2; i++)
			L1[ch][i] = m_ui8Comp0[ch][2 * i] + m_ui8Comp0[ch][2 * i + 1];

	short** H1 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		H1[ch] = new short[m_iSize[ch] / 2];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 2; i++)
			H1[ch][i] = m_ui8Comp0[ch][2 * i] - m_ui8Comp0[ch][2 * i + 1];

	/* 수직방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	short** LL1 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		LL1[ch] = new short[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++)
				LL1[ch][((width[ch] / 2) * i) + j] = L1[ch][((width[ch] / 2) * (2 * i)) + j] + L1[ch][((width[ch] / 2) * (2 * i + 1)) + j];

	short** LH1 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		LH1[ch] = new short[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++)
				LH1[ch][((width[ch] / 2) * i) + j] = L1[ch][((width[ch] / 2) * (2 * i)) + j] - L1[ch][((width[ch] / 2) * (2 * i + 1)) + j];

	short** HL1 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		HL1[ch] = new short[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++)
				HL1[ch][((width[ch] / 2) * i) + j] = H1[ch][((width[ch] / 2) * (2 * i)) + j] + H1[ch][((width[ch] / 2) * (2 * i + 1)) + j];

	short** HH1 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		HH1[ch] = new short[m_iSize[ch] / 4];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++)
				HH1[ch][((width[ch] / 2) * i) + j] = H1[ch][((width[ch] / 2) * (2 * i)) + j] + H1[ch][((width[ch] / 2) * (2 * i + 1)) + j];

	coeffiecient = new short*[3];
	for (int i = 0; i < 3; i++)
		coeffiecient[i] = new short[m_iSize[i]];

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL1[ch][width[ch] / 2 * i + j];
				coeffiecient[ch][width[ch] * i + j + width[ch] / 2] = HL1[ch][(width[ch] / 2) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j] = LH1[ch][(width[ch] / 2) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j + width[ch] / 2] = HH1[ch][(width[ch] / 2) * i + j];
			}

	FILE* coefficient1 = fopen(coeffi1_path, "wb");
	if (!coefficient1) {
		cout << "coefficient1 not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++) {
		fwrite(&(coeffiecient[ch][0]), sizeof(short), m_iSize[ch], coefficient1);
	}
	fclose(coefficient1);

	/*
	2단계 변환
	*/

	/* 수평방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	short** L2 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		L2[ch] = new short[m_iSize[ch] / 8];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 8; i++)
			L2[ch][i] = LL1[ch][2 * i] + LL1[ch][2 * i + 1];

	short** H2 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		H2[ch] = new short[m_iSize[ch] / 8];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 8; i++)
			H2[ch][i] = LL1[ch][2 * i] - LL1[ch][2 * i + 1];

	/* 수직방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	short** LL2 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		LL2[ch] = new short[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++)
				LL2[ch][((width[ch] / 4) * i) + j] = L2[ch][((width[ch] / 4) * (2 * i)) + j] + L2[ch][((width[ch] /4) * (2 * i + 1)) + j];

	short** LH2 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		LH2[ch] = new short[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++)
				LH2[ch][((width[ch] / 4) * i) + j] = L2[ch][((width[ch] / 4) * (2 * i)) + j] - L2[ch][((width[ch] / 4) * (2 * i + 1)) + j];

	short** HL2 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		HL2[ch] = new short[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++)
				HL2[ch][((width[ch] / 4) * i) + j] = H2[ch][((width[ch] / 4) * (2 * i)) + j] + H2[ch][((width[ch] / 4) * (2 * i + 1)) + j];

	short** HH2 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		HH2[ch] = new short[m_iSize[ch] / 16];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++)
				HH2[ch][((width[ch] / 4) * i) + j] = H2[ch][((width[ch] / 4) * (2 * i)) + j] - H2[ch][((width[ch] / 4) * (2 * i + 1)) + j];

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL2[ch][width[ch] / 4 * i + j];
				coeffiecient[ch][width[ch] * i + j + width[ch] / 4] = HL2[ch][(width[ch] / 4) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j] = LH2[ch][(width[ch] / 4) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j + width[ch] / 4] = HH2[ch][(width[ch] / 4) * i + j];
			}

	FILE* coefficient2 = fopen(coeffi2_path, "wb");
	if (!coefficient2) {
		cout << "coefficient2 not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++) {
		fwrite(&(coeffiecient[ch][0]), sizeof(short), m_iSize[ch], coefficient2);
	}
	fclose(coefficient2);

	/*
	3단계 변환
	*/

	/* 수평방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	short** L3 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		L3[ch] = new short[m_iSize[ch] / 32];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 32; i++)
			L3[ch][i] = LL2[ch][2 * i] + LL2[ch][2 * i + 1];

	short** H3 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		H3[ch] = new short[m_iSize[ch] / 32];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Horizontal)
		for (int i = 0; i < m_iSize[ch] / 32; i++)
			H3[ch][i] = LL2[ch][2 * i] - LL2[ch][2 * i + 1];

	/* 수직방향으로 오버랩하지 않은 인접한 두 픽셀에 대하여 low-pass, high-pass filtering 수행 */
	short** LL3 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		LL3[ch] = new short[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 8; i++)
			for (int j = 0; j < width[ch] / 8; j++)
				LL3[ch][((width[ch] / 8) * i) + j] = L3[ch][((width[ch] / 8) * (2 * i)) + j] + L3[ch][((width[ch] / 8) * (2 * i + 1)) + j];

	short** LH3 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		LH3[ch] = new short[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 8; i++)
			for (int j = 0; j < width[ch] / 8; j++)
				LH3[ch][((width[ch] / 8) * i) + j] = L3[ch][((width[ch] / 8) * (2 * i)) + j] - L3[ch][((width[ch] / 8) * (2 * i + 1)) + j];

	short** HL3 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		HL3[ch] = new short[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // low-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 8; i++)
			for (int j = 0; j < width[ch] / 8; j++)
				HL3[ch][((width[ch] / 8) * i) + j] = H3[ch][((width[ch] / 8) * (2 * i)) + j] + H3[ch][((width[ch] / 8) * (2 * i + 1)) + j];

	short** HH3 = new short*[3];
	for (int ch = 0; ch < 3; ch++)
		HH3[ch] = new short[m_iSize[ch] / 64];

	for (int ch = 0; ch < 3; ch++)  // high-pass filtering (Vertical)
		for (int i = 0; i < height[ch] / 8; i++)
			for (int j = 0; j < width[ch] / 8; j++)
				HH3[ch][((width[ch] / 8) * i) + j] = H3[ch][((width[ch] / 8) * (2 * i)) + j] - H3[ch][((width[ch] / 8) * (2 * i + 1)) + j];

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 8; i++)
			for (int j = 0; j < width[ch] / 8; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL3[ch][width[ch] / 8 * i + j];
				coeffiecient[ch][width[ch] * i + j + width[ch] / 8] = HL3[ch][(width[ch] / 8) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / + 8) + j] = LH3[ch][(width[ch] / 8) * i + j];
				coeffiecient[ch][width[ch] * (i + height[ch] / 8) + j + width[ch] / 8] = HH3[ch][(width[ch] / 8) * i + j];
			}

	FILE* coefficient3 = fopen(coeffi3_path, "wb");
	if (!coefficient3) {
		cout << "coefficient3 not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++) {
		fwrite(&(coeffiecient[ch][0]), sizeof(short), m_iSize[ch], coefficient3);
	}
	fclose(coefficient3);

	////////// Step 3. Quantization & Inverse Quantization //////////
	int c;
	cout << "Case1. 양자화 x" << endl << "Case2. 각 영역에 대하여 8bit로 양자화" << endl << "Case3. 각 영역에 대하여 8bit 미만으로 양자화" << endl << endl;
	cout << "Select: ";
	cin >> c;

	if (c == 1)  // Case1. 양자화 x
		;
	else if (c == 2)  // Case2. 각 영역에 대하여 8bit로 양자화
	{
		for (int ch = 0; ch < 3; ch++)
			for (int i = 0; i < m_iSize[ch]; i++)
				coeffiecient[ch][i] = coeffiecient[ch][i] >> 6;
	}
	else if (c == 3) // Case3. 각 영역에 대하여 8bit 미만으로 양자화
	{
		for (int ch = 0; ch < 3; ch++)
			for (int i = 0; i < height[ch] / 8; i++)
				for (int j = 0; j < width[ch] / 8; j++) {
					coeffiecient[ch][width[ch] * i + j] = coeffiecient[ch][width[ch] * i + j] >> 7;  // LL3
					coeffiecient[ch][width[ch] * i + j + width[ch] / 8] = coeffiecient[ch][width[ch] * i + j + width[ch] / 8] >> 7;  // HL3
					coeffiecient[ch][width[ch] * (i + height[ch] / +8) + j] = coeffiecient[ch][width[ch] * (i + height[ch] / +8) + j] >> 7;  // LH3
					coeffiecient[ch][width[ch] * (i + height[ch] / 8) + j + width[ch] / 8] = coeffiecient[ch][width[ch] * (i + height[ch] / 8) + j + width[ch] / 8] >> 7;  // HH3
				}

		for (int ch = 0; ch < 3; ch++)
			for (int i = 0; i < height[ch] / 4; i++)
				for (int j = 0; j < width[ch] / 4; j++) {
					coeffiecient[ch][width[ch] * i + j + width[ch] / 4] = coeffiecient[ch][width[ch] * i + j + width[ch] / 4] >> 8;  // HL2
					coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j] = coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j] >> 8;  // LH2
					coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j + width[ch] / 4] = coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j + width[ch] / 4] >> 8;  // HH2
				}

		for (int ch = 0; ch < 3; ch++)
			for (int i = 0; i < height[ch] / 2; i++)
				for (int j = 0; j < width[ch] / 2; j++) {
					coeffiecient[ch][width[ch] * i + j + width[ch] / 2] = coeffiecient[ch][width[ch] * i + j + width[ch] / 2] >> 9;  // HL1
					coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j] = coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j] >> 9;  // LH1
					coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j + width[ch] / 2] = coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j + width[ch] / 2] >> 9;  // HH1
				}
	}
	else  // exception handling
		;

	////////// Step 4. haar inverse transform //////////

	/*
	1단계 역변환
	*/

	for (int ch = 0; ch < 3; ch++)  // LL3, LH3, HL3, HH3 -> L3, H3
		for (int i = 0; i < height[ch] / 8; i++)
			for (int j = 0; j < width[ch] / 8; j++) {
				L3[ch][((width[ch] / 8) * (2 * i)) + j] = (coeffiecient[ch][width[ch] * i + j] + coeffiecient[ch][width[ch] * (i + height[ch] / +8) + j]) / 2;
				L3[ch][((width[ch] / 8) * (2 * i + 1)) + j] = (coeffiecient[ch][width[ch] * i + j] - coeffiecient[ch][width[ch] * (i + height[ch] / +8) + j]) / 2;
				H3[ch][((width[ch] / 8) * (2 * i)) + j] = (coeffiecient[ch][width[ch] * i + j + width[ch] / 8] + coeffiecient[ch][width[ch] * (i + height[ch] / 8) + j + width[ch] / 8]) / 2;
				H3[ch][((width[ch] / 8) * (2 * i + 1)) + j] = (coeffiecient[ch][width[ch] * i + j + width[ch] / 8] - coeffiecient[ch][width[ch] * (i + height[ch] / 8) + j + width[ch] / 8]) / 2;
			}

	for (int ch = 0; ch < 3; ch++)  // L3, H3 -> LL2
		for (int i = 0; i < m_iSize[ch] / 32; i++) {
			LL2[ch][2 * i] = (L3[ch][i] + H3[ch][i]) / 2;
			LL2[ch][2 * i + 1] = (L3[ch][i] - H3[ch][i]) / 2;
		}

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL2[ch][width[ch] / 4 * i + j];
			}

	//FILE* inv_coefficient1 = fopen("./transform_coefficients/BasketballDrill_832x480_yuv420_12bit_inv_coefficient1.yuv", "wb");
	//if (!inv_coefficient1) {
	//	cout << "inv_coefficient1 not open" << endl;
	//	return 0;
	//}

	//for (int ch = 0; ch < 3; ch++) {
	//	fwrite(&(coeffiecient[ch][0]), sizeof(short), m_iSize[ch], inv_coefficient1);
	//}
	//fclose(inv_coefficient1);


	/*
	2단계 역변환
	*/

	for (int ch = 0; ch < 3; ch++)  // LL2, LH2, HL2, HH2 -> L2, H2
		for (int i = 0; i < height[ch] / 4; i++)
			for (int j = 0; j < width[ch] / 4; j++) {
				L2[ch][((width[ch] / 4) * (2 * i)) + j] = (coeffiecient[ch][width[ch] * i + j] + coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j]) / 2;
				L2[ch][((width[ch] / 4) * (2 * i + 1)) + j] = (coeffiecient[ch][width[ch] * i + j] - coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j]) / 2;
				H2[ch][((width[ch] / 4) * (2 * i)) + j] = (coeffiecient[ch][width[ch] * i + j + width[ch] / 4] + coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j + width[ch] / 4]) / 2;
				H2[ch][((width[ch] / 4) * (2 * i + 1)) + j] = (coeffiecient[ch][width[ch] * i + j + width[ch] / 4] - coeffiecient[ch][width[ch] * (i + height[ch] / 4) + j + width[ch] / 4]) / 2;
			}

	for (int ch = 0; ch < 3; ch++)  // L2, H2 -> LL1
		for (int i = 0; i < m_iSize[ch] / 8; i++) {
			LL1[ch][2 * i] = (L2[ch][i] + H2[ch][i]) / 2;
			LL1[ch][2 * i + 1] = (L2[ch][i] - H2[ch][i]) / 2;
		}

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++) {
				coeffiecient[ch][width[ch] * i + j] = LL1[ch][width[ch] / 2 * i + j];
			}

	//FILE* inv_coefficient2 = fopen("./transform_coefficients/BasketballDrill_832x480_yuv420_10bit_inv_coefficient2.yuv", "wb");
	//if (!inv_coefficient2) {
	//	cout << "inv_coefficient2 not open" << endl;
	//	return 0;
	//}

	//for (int ch = 0; ch < 3; ch++) {
	//	fwrite(&(coeffiecient[ch][0]), sizeof(short), m_iSize[ch], inv_coefficient2);
	//}
	//fclose(inv_coefficient2);

	/*
	3단계 역변환
	*/

	for (int ch = 0; ch < 3; ch++)  // LL1, LH1, HL1, HH1 -> L1, H1
		for (int i = 0; i < height[ch] / 2; i++)
			for (int j = 0; j < width[ch] / 2; j++) {
				L1[ch][((width[ch] / 2) * (2 * i)) + j] = (coeffiecient[ch][width[ch] * i + j] + coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j]) / 2;
				L1[ch][((width[ch] / 2) * (2 * i + 1)) + j] = (coeffiecient[ch][width[ch] * i + j] - coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j]) / 2;
				H1[ch][((width[ch] / 2) * (2 * i)) + j] = (coeffiecient[ch][width[ch] * i + j + width[ch] / 2] + coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j + width[ch] / 2]) / 2;
				H1[ch][((width[ch] / 2) * (2 * i + 1)) + j] = (coeffiecient[ch][width[ch] * i + j + width[ch] / 2] - coeffiecient[ch][width[ch] * (i + height[ch] / 2) + j + width[ch] / 2]) / 2;
			}

	for (int ch = 0; ch < 3; ch++)  // L1, H1 -> Reconstructed image
		for (int i = 0; i < m_iSize[ch] / 2; i++) {
			m_ui8Comp1[ch][2 * i] = (L1[ch][i] + H1[ch][i]) / 2;
			m_ui8Comp1[ch][2 * i + 1] = (L1[ch][i] - H1[ch][i]) / 2;
		}
	
	FILE* reconstructed_image = fopen("./transform_coefficients/BasketballDrill_832x480_yuv420_8bit_reconstructed_image.yuv", "wb");
	if (!reconstructed_image) {
		cout << "reconstructed_image not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++) {
		fwrite(&(m_ui8Comp1[ch][0]), sizeof(unsigned char), m_iSize[ch], reconstructed_image);
	}
	fclose(reconstructed_image);



	return 0;
} // end of main()
