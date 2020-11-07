#include "anti-aliasing.h"

void print_pixels(int ch) {
	cout << "픽셀값 출력 테스트: " << endl;
	for (int i = 0; i < m_iSize1[ch] / 4; i++)
		cout << (int)m_ui8Comp2[ch][i] << "\t";
}

void print_pixels2(unsigned char(*ch)[HEIGHT1 / 2]) {
	cout << "픽셀값 출력 테스트: " << endl;
	for (int i = 0; i < HEIGHT1 / 2; i++) {
		for (int j = 0; j < WIDTH1 / 2; j++)
			cout << (int)ch[i][j] << "\t";
		cout << endl;
	}

}

int main(void) {

	////////// Step 1. YCbCr 10-bit input image를 읽어서 메모리에 저장 //////////
	FILE *original_image = fopen("./TestImage_832x480_yuv420_8bit/BasketballDrill_832x480_yuv420_8bit_frame0.yuv", "rb");
	if (!original_image) {
		cout << "original_image not open" << endl;
		return 0;
	}

	m_ui8Comp1 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		m_ui8Comp1[ch] = new unsigned char[ISIZE1]; // 배열 선언

	for (int ch = 0; ch < 3; ch++)
		fread(&(m_ui8Comp1[ch][0]), sizeof(unsigned char), m_iSize1[ch], original_image); // 8-bit 영상의 픽셀값을 배열에 저장

	fclose(original_image);

	////////// Step 2. 이미지 downsampling //////////
	m_ui8Comp2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		m_ui8Comp2[ch] = new unsigned char[ISIZE1 / 4]; // 배열 선언

	for (int ch = 0; ch < 3; ch++)
		for (int i = 0; i < m_iSize1[ch] / 4; i++)
			m_ui8Comp2[ch][i] = m_ui8Comp1[ch][WIDTH1 * (i / (WIDTH1 / 2)) + (2 * i)];

	FILE* downsample_image_w = fopen("./downsample/BasketballDrill_416x240_yuv420_8bit_frame0.yuv", "wb");
	if (!downsample_image_w) {
		cout << "downsample_image not open" << endl;
		return 0;
	}
	for (int ch = 0; ch < 3; ch++)
		fwrite(&(m_ui8Comp2[ch][0]), sizeof(unsigned char), m_iSize1[ch] / 4, downsample_image_w); // downsample된 8-bit image를 .yuv 포맷 파일로 저장
	fclose(downsample_image_w);

	//////////// Step 3. 이미지 upsampling //////////
	FILE* downsample_image_r = fopen("./downsample/BasketballDrill_416x240_yuv420_8bit_frame0.yuv", "rb");
	if (!downsample_image_r) {
		cout << "downsample_image not open" << endl;
		return 0;
	}
	unsigned char tmpY[HEIGHT1 / 2][WIDTH1 / 2];
	unsigned char tmpCb[HEIGHT1 / 4][WIDTH1 / 4];
	unsigned char tmpCr[HEIGHT1 / 4][WIDTH1 / 4];
	fread(tmpY, sizeof(unsigned char), WIDTH1 / 2 * HEIGHT1 / 2, downsample_image_r);
	fread(tmpCb, sizeof(unsigned char), WIDTH1 / 4 * HEIGHT1 / 4, downsample_image_r);
	fread(tmpCr, sizeof(unsigned char), WIDTH1 / 4 * HEIGHT1 / 4, downsample_image_r);
	fclose(downsample_image_r);

	unsigned char Y[HEIGHT1][WIDTH1];
	unsigned char Cb[HEIGHT1 / 2][WIDTH1 / 2];
	unsigned char Cr[HEIGHT1 / 2][WIDTH1 / 2];
	memset(Y, 0, sizeof(unsigned char) * HEIGHT1 * WIDTH1);
	memset(Cb, 0, sizeof(unsigned char) * HEIGHT1 / 2 * WIDTH1 / 2);
	memset(Cr, 0, sizeof(unsigned char) * HEIGHT1 / 2 * WIDTH1 / 2);

	 for (int i = 0; i < HEIGHT1 / 2; i++) // integer-pel sample (Y)
		for (int j = 0; j < WIDTH1 / 2; j++)
			Y[i * 2][j * 2] = tmpY[i][j];

	for (int i = 0; i < HEIGHT1 / 4; i++) // integer-pel sample (Cb)
		for (int j = 0; j < WIDTH1 / 4; j++)
			Cb[i * 2][j * 2] = tmpCb[i][j];

	for (int i = 0; i < HEIGHT1 / 4; i++) // integer-pel sample (Cr)
		for (int j = 0; j < WIDTH1 / 4; j++)
			Cr[i * 2][j * 2] = tmpCr[i][j];

	for (int k = 0; k < HEIGHT1 - 10; k += 2) { // Y
		for (int j = 0; j < WIDTH1 - 10; j += 2) {
			for (int i = 0; i <= 10; i += 2) { // 수직방향 (half-pel)
				Y[k + i][j + 5] = ((11 * Y[k + i][j]) - (43 * Y[k + i][j + 2]) + (160 * Y[k + i][j + 4])
					+ (160 * Y[k + i][j + 6]) - (43 * Y[k + i][j + 8]) + (11 * Y[k + i][j + 10])) / 256;
			}

			for (int i = 0; i <= 10; i += 2) { // 수평방향 (half-pel)
				Y[k + 5][j + i] = ((11 * Y[k][j + i]) - (43 * Y[k + 2][j + i]) + (160 * Y[k + 4][j + i])
					+ (160 * Y[k + 6][j + i]) - (43 * Y[k + 8][j + i]) + (11 * Y[k + 10][j + i])) / 256;
			}

			Y[k + 5][j + 5] = ((11 * Y[k + 5][j]) - (43 * Y[k + 5][j + 2]) + (160 * Y[k + 5][j + 4])
				+ (160 * Y[k + 5][j + 6]) - (43 * Y[k + 5][j + 8]) + (11 * Y[k + 5][j + 10])) / 256; // j
		}
	}

	for (int k = 0; k < HEIGHT1 / 2 - 10; k += 2) { // Cb
		for (int j = 0; j < WIDTH1 / 2 - 10; j += 2) {
			for (int i = 0; i <= 10; i += 2) { // 수직방향 (half-pel)
				Cb[k + i][j + 5] = ((11 * Cb[k + i][j]) - (43 * Cb[k + i][j + 2]) + (160 * Cb[k + i][j + 4])
					+ (160 * Cb[k + i][j + 6]) - (43 * Cb[k + i][j + 8]) + (11 * Cb[k + i][j + 10])) / 256;
			}

			for (int i = 0; i <= 10; i += 2) { // 수평방향 (half-pel)
				Cb[k + 5][j + i] = ((11 * Cb[k][j + i]) - (43 * Cb[k + 2][j + i]) + (160 * Cb[k + 4][j + i])
					+ (160 * Cb[k + 6][j + i]) - (43 * Cb[k + 8][j + i]) + (11 * Cb[k + 10][j + i])) / 256;
			}

			Cb[k + 5][j + 5] = ((11 * Cb[k + 5][j]) - (43 * Cb[k + 5][j + 2]) + (160 * Cb[k + 5][j + 4])
				+ (160 * Cb[k + 5][j + 6]) - (43 * Cb[k + 5][j + 8]) + (11 * Cb[k + 5][j + 10])) / 256; // j
		}
	}

	for (int k = 0; k < HEIGHT1 / 2 - 10; k += 2) {
		for (int j = 0; j < WIDTH1 / 2 - 10; j += 2) {
			for (int i = 0; i <= 10; i += 2) { // 수직방향 (half-pel)
				Cr[k + i][j + 5] = ((11 * Cr[k + i][j]) - (43 * Cr[k + i][j + 2]) + (160 * Cr[k + i][j + 4])
					+ (160 * Cr[k + i][j + 6]) - (43 * Cr[k + i][j + 8]) + (11 * Cr[k + i][j + 10])) / 256;
			}

			for (int i = 0; i <= 10; i += 2) { // 수평방향 (half-pel)
				Cr[k + 5][j + i] = ((11 * Cr[k][j + i]) - (43 * Cr[k + 2][j + i]) + (160 * Cr[k + 4][j + i])
					+ (160 * Cr[k + 6][j + i]) - (43 * Cr[k + 8][j + i]) + (11 * Cr[k + 10][j + i])) / 256;
			}

			Cr[k + 5][j + 5] = ((11 * Cr[k + 5][j]) - (43 * Cr[k + 5][j + 2]) + (160 * Cr[k + 5][j + 4])
				+ (160 * Cr[k + 5][j + 6]) - (43 * Cr[k + 5][j + 8]) + (11 * Cr[k + 5][j + 10])) / 256; // j
		}
	}

	// 보간되지 않은 외각 채우기
	for (int i = 0; i < WIDTH1; i += 2) {
		Y[1][i] = ((16 * Y[0][i]) + (20 * Y[2][i]) - (5 * Y[4][i])
			+ (1 * Y[6][i])) >> 256;
		Y[3][i] = ((-(4 * Y[0][i])) + (20 * Y[2][i]) + (20 * Y[4][i])
			- (5 * Y[6][i]) + (1 * Y[8][i])) >> 5;

		Y[477][i] = ((16 * Y[478][i]) + (20 * Y[476][i]) - (5 * Y[474][i])
			+ (1 * Y[472][i])) >> 5;
		Y[475][i] = (-(5 * Y[478][i]) + (21 * Y[476][i]) + (20 * Y[474][i])
			- (5 * Y[472][i]) + (1 * Y[470][i])) >> 5;

		Y[i][1] = ((16 * Y[i][0]) + (20 * Y[i][2]) - (5 * Y[i][4]) + (1 * Y[i][6])) >> 5;
		Y[i][3] = (-(4 * Y[i][0]) + (20 * Y[i][2]) + (20 * Y[i][4]) - (5 * Y[i][6]) + (1 * Y[i][8])) >> 5;

		Y[i][829] = ((16 * Y[i][830]) + (20 * Y[i][828]) - (5 * Y[i][826]) + (Y[i][824])) >> 5;
		Y[i][827] = (-(4 * Y[i][830]) + (20 * Y[i][828]) + (20 * Y[i][826]) - (5 * Y[i][824]) + (1 * Y[i][822])) >> 5;
	}

	FILE* upsample_image = fopen("./upsample/BasketballDrill_832x480_yuv420_8bit_frame0.yuv", "wb"); // upsample된 8-bit image를 .yuv 포맷 파일로 저장
	if (!upsample_image) {
		cout << "upsample_image not open" << endl;
		return 0;
	}

	fwrite(Y, sizeof(unsigned char), HEIGHT1 * WIDTH1, upsample_image);
	fwrite(Cb, sizeof(unsigned char), HEIGHT1 /2  * WIDTH1 / 2, upsample_image);
	fwrite(Cr, sizeof(unsigned char), HEIGHT1 / 2 * WIDTH1 / 2, upsample_image);

	fclose(upsample_image);




















	return 0;
} // end of main
