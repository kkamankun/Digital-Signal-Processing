// 832 * 480
#include "anti-aliasing(832x480).h"

bool convolve2D(unsigned char* in, unsigned char* out, int dataSizeX, int dataSizeY,
	float* kernel, int kernelSizeX, int kernelSizeY)
{
	int i, j, m, n, mm, nn;
	int kCenterX, kCenterY;                         // center index of kernel
	float sum;                                      // temp accumulation buffer
	int rowIndex, colIndex;

	// check validity of params
	if (!in || !out || !kernel) return false;
	if (dataSizeX <= 0 || kernelSizeX <= 0) return false;

	// find center position of kernel (half of kernel size)
	kCenterX = kernelSizeX / 2;
	kCenterY = kernelSizeY / 2;

	for (i = 0; i < dataSizeY; ++i)                // rows
	{
		for (j = 0; j < dataSizeX; ++j)            // columns
		{
			sum = 0;                            // init to 0 before sum
			for (m = 0; m < kernelSizeY; ++m)      // kernel rows
			{
				mm = kernelSizeY - 1 - m;       // row index of flipped kernel

				for (n = 0; n < kernelSizeX; ++n)  // kernel columns
				{
					nn = kernelSizeX - 1 - n;   // column index of flipped kernel

					// index of input signal, used for checking boundary
					rowIndex = i + (kCenterY - m);
					colIndex = j + (kCenterX - n);

					// ignore input samples which are out of bound
					if (rowIndex >= 0 && rowIndex < dataSizeY && colIndex >= 0 && colIndex < dataSizeX)
						sum += in[dataSizeX * rowIndex + colIndex] * kernel[kernelSizeX * m + n];
				}
			}
			out[dataSizeX * i + j] = (unsigned char)((float)fabs(sum) + 0.5f);
		}
	}

	return true;
}

void print_pixels(int ch) {
	cout << "픽셀값 출력 테스트: " << endl;
	for (int i = 0; i < m_iSize[ch] / 4; i++)
		cout << (int)m_ui8Comp2[ch][i] << "\t";
}

void print_pixels2(unsigned char(*ch)[HEIGHT / 2]) {
	cout << "픽셀값 출력 테스트: " << endl;
	for (int i = 0; i < HEIGHT / 2; i++) {
		for (int j = 0; j < WIDTH / 2; j++)
			cout << (int)ch[i][j] << "\t";
		cout << endl;
	}
}

void six_tap()
{
	for (int k = 0; k < HEIGHT - 10; k += 2) { // Y
		for (int j = 0; j < WIDTH - 10; j += 2) {
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

	for (int k = 0; k < HEIGHT / 2 - 10; k += 2) { // Cb
		for (int j = 0; j < WIDTH / 2 - 10; j += 2) {
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

	for (int k = 0; k < HEIGHT / 2 - 10; k += 2) {
		for (int j = 0; j < WIDTH / 2 - 10; j += 2) {
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
	for (int i = 0; i < WIDTH; i += 2) {
		Y[1][i] = ((128 * Y[0][i]) + (160 * Y[2][i]) - (43 * Y[4][i]) + (11 * Y[6][i])) / 256;
		Y[3][i] = ((-(43 * Y[0][i])) + (171 * Y[2][i]) + (160 * Y[4][i]) - (43 * Y[6][i]) + (11 * Y[8][i])) / 256;

		Y[HEIGHT - 3][i] = ((128 * Y[HEIGHT - 2][i]) + (160 * Y[HEIGHT - 4][i]) - (43 * Y[HEIGHT - 6][i]) + (11 * Y[HEIGHT - 8][i])) / 256;
		Y[HEIGHT - 5][i] = (-(43 * Y[HEIGHT - 2][i]) + (171 * Y[HEIGHT - 4][i]) + (160 * Y[HEIGHT - 6][i]) - (43 * Y[HEIGHT - 8][i]) + (11 * Y[HEIGHT - 10][i])) / 256;
	}

	for (int i = 0; i < WIDTH / 2; i += 2) {
		Cb[1][i] = ((128 * Cb[0][i]) + (160 * Cb[2][i]) - (43 * Cb[4][i]) + (11 * Cb[6][i])) / 256;
		Cb[3][i] = ((-(43 * Cb[0][i])) + (171 * Cb[2][i]) + (160 * Cb[4][i]) - (43 * Cb[6][i]) + (11 * Cb[8][i])) / 256;

		Cb[HEIGHT / 2 - 3][i] = ((128 * Cb[HEIGHT / 2 - 2][i]) + (160 * Cb[HEIGHT / 2 - 4][i]) - (43 * Cb[HEIGHT / 2 - 6][i]) + (11 * Cb[HEIGHT / 2 - 8][i])) / 256;
		Cb[HEIGHT / 2 - 5][i] = (-(43 * Cb[HEIGHT / 2 - 2][i]) + (171 * Cb[HEIGHT / 2 - 4][i]) + (160 * Cb[HEIGHT / 2 - 6][i]) - (43 * Cb[HEIGHT / 2 - 8][i]) + (11 * Cb[HEIGHT / 2 - 10][i])) / 256;

		Cr[1][i] = ((128 * Cr[0][i]) + (160 * Cr[2][i]) - (43 * Cr[4][i]) + (11 * Cr[6][i])) / 256;
		Cr[3][i] = ((-(43 * Cr[0][i])) + (171 * Cr[2][i]) + (160 * Cr[4][i]) - (43 * Cr[6][i]) + (11 * Cr[8][i])) / 256;

		Cr[HEIGHT / 2 - 3][i] = ((128 * Cr[HEIGHT / 2 - 2][i]) + (160 * Cr[HEIGHT / 2 - 4][i]) - (43 * Cr[HEIGHT / 2 - 6][i]) + (11 * Cr[HEIGHT / 2 - 8][i])) / 256;
		Cr[HEIGHT / 2 - 5][i] = (-(43 * Cr[HEIGHT / 2 - 2][i]) + (171 * Cr[HEIGHT / 2 - 4][i]) + (160 * Cr[HEIGHT / 2 - 6][i]) - (43 * Cr[HEIGHT / 2 - 8][i]) + (11 * Cr[HEIGHT / 2 - 10][i])) / 256;
	}

	for (int i = 0; i < HEIGHT; i++) {
		Y[i][1] = ((128 * Y[i][0]) + (160 * Y[i][2]) - (43 * Y[i][4]) + (11 * Y[i][6])) / 256;
		Y[i][3] = (-(43 * Y[i][0]) + (171 * Y[i][2]) + (160 * Y[i][4]) - (43 * Y[i][6]) + (11 * Y[i][8])) / 256;

		Y[i][WIDTH - 3] = ((128 * Y[i][WIDTH - 2]) + (160 * Y[i][WIDTH - 4]) - (43 * Y[i][WIDTH - 6]) + (11 * Y[i][WIDTH - 8])) / 256;
		Y[i][WIDTH - 5] = (-(43 * Y[i][WIDTH - 2]) + (171 * Y[i][WIDTH - 4]) + (160 * Y[i][WIDTH - 6]) - (43 * Y[i][WIDTH - 8]) + (11 * Y[i][WIDTH - 10])) / 256;
	}

	for (int i = 0; i < HEIGHT / 2; i++) {
		Cb[i][1] = ((128 * Cb[i][0]) + (160 * Cb[i][2]) - (43 * Cb[i][4]) + (11 * Cb[i][6])) / 256;
		Cb[i][3] = (-(43 * Cb[i][0]) + (171 * Cb[i][2]) + (160 * Cb[i][4]) - (43 * Cb[i][6]) + (11 * Cb[i][8])) / 256;

		Cb[i][WIDTH / 2 - 3] = ((128 * Cb[i][WIDTH / 2 - 2]) + (160 * Cb[i][WIDTH / 2 - 4]) - (43 * Cb[i][WIDTH / 2 - 6]) + (11 * Cb[i][WIDTH / 2 - 8])) / 256;
		Cb[i][WIDTH / 2 - 5] = (-(43 * Cb[i][WIDTH / 2 - 2]) + (171 * Cb[i][WIDTH / 2 - 4]) + (160 * Cb[i][WIDTH / 2 - 6]) - (43 * Cb[i][WIDTH / 2 - 8]) + (11 * Cb[i][WIDTH / 2 - 10])) / 256;

		Cr[i][1] = ((128 * Cr[i][0]) + (160 * Cr[i][2]) - (43 * Cr[i][4]) + (11 * Cr[i][6])) / 256;
		Cr[i][3] = (-(43 * Cr[i][0]) + (171 * Cr[i][2]) + (160 * Cr[i][4]) - (43 * Cr[i][6]) + (11 * Cr[i][8])) / 256;

		Cr[i][WIDTH / 2 - 3] = ((128 * Cr[i][WIDTH / 2 - 2]) + (160 * Cr[i][WIDTH / 2 - 4]) - (43 * Cr[i][WIDTH / 2 - 6]) + (11 * Cr[i][WIDTH / 2 - 8])) / 256;
		Cr[i][WIDTH / 2 - 5] = (-(43 * Cr[i][WIDTH / 2 - 2]) + (171 * Cr[i][WIDTH / 2 - 4]) + (160 * Cr[i][WIDTH / 2 - 6]) - (43 * Cr[i][WIDTH / 2 - 8]) + (11 * Cr[i][WIDTH / 2 - 10])) / 256;
	}

	for (int i = 1; i < WIDTH; i += 2) {
		Y[1][i] = (Y[1][i - 1] + Y[1][i + 1]) / 2;
		Y[3][i] = (Y[3][i - 1] + Y[3][i + 1]) / 2;
		Y[HEIGHT - 5][i] = (Y[HEIGHT - 5][i - 1] + Y[HEIGHT - 5][i + 1]) / 2;
		Y[HEIGHT - 3][i] = (Y[HEIGHT - 3][i - 1] + Y[HEIGHT - 3][i + 1]) / 2;
	}

	for (int i = 1; i < WIDTH / 2; i += 2) {
		Cb[1][i] = (Cb[1][i - 1] + Cb[1][i + 1]) / 2;
		Cb[3][i] = (Cb[3][i - 1] + Cb[3][i + 1]) / 2;
		Cb[HEIGHT / 2 - 3][i] = (Cb[HEIGHT / 2 - 3][i - 1] + Cb[HEIGHT / 2 - 3][i + 1]) / 2;
		Cb[HEIGHT / 2 - 5][i] = (Cb[HEIGHT / 2 - 5][i - 1] + Cb[HEIGHT / 2 - 5][i + 1]) / 2;
	}

	for (int i = 1; i < WIDTH / 2; i += 2) {
		Cr[1][i] = (Cr[1][i - 1] + Cr[1][i + 1]) / 2;
		Cr[3][i] = (Cr[3][i - 1] + Cr[3][i + 1]) / 2;
		Cr[HEIGHT / 2 - 3][i] = (Cr[HEIGHT / 2 - 3][i - 1] + Cr[HEIGHT / 2 - 3][i + 1]) / 2;
		Cr[HEIGHT / 2 - 5][i] = (Cr[HEIGHT / 2 - 5][i - 1] + Cr[HEIGHT / 2 - 5][i + 1]) / 2;
	}

	for (int i = 0; i < HEIGHT; i++)
		Y[i][WIDTH - 1] = (288 * Y[i][WIDTH - 2] - 43 * Y[i][WIDTH - 4] + 11 * Y[i][WIDTH - 8]) / 256;

	for (int i = 0; i < WIDTH; i++)
		Y[HEIGHT - 1][i] = (288 * Y[HEIGHT - 2][i] - 43 * Y[HEIGHT - 4][i] + 11 * Y[HEIGHT - 8][i]) / 256;

	for (int i = 0; i < HEIGHT / 2; i++)
		Cb[i][WIDTH / 2 - 1] = (288 * Cb[i][WIDTH / 2 - 2] - 43 * Cb[i][WIDTH / 2 - 4] + 11 * Cb[i][WIDTH / 2 - 8]) / 256;

	for (int i = 0; i < WIDTH / 2; i++)
		Cb[HEIGHT / 2 - 1][i] = (288 * Cb[HEIGHT / 2 - 2][i] - 43 * Cb[HEIGHT / 2 - 4][i] + 11 * Cb[HEIGHT / 2 - 8][i]) / 256;

	for (int i = 0; i < HEIGHT / 2; i++)
		Cr[i][WIDTH / 2 - 1] = (288 * Cr[i][WIDTH / 2 - 2] - 43 * Cr[i][WIDTH / 2 - 4] + 11 * Cr[i][WIDTH / 2 - 8]) / 256;

	for (int i = 0; i < WIDTH / 2; i++)
		Cr[HEIGHT / 2 - 1][i] = (288 * Cr[HEIGHT / 2 - 2][i] - 43 * Cr[HEIGHT / 2 - 4][i] + 11 * Cr[HEIGHT / 2 - 8][i]) / 256;
}

int main(void) {
	////////// Step 1. YCbCr 10-bit input image를 읽어서 메모리에 저장 //////////
	m_ui8Comp1 = new unsigned char*[3];
	m_ui8Comp1[0] = new unsigned char[ISIZE]; // 배열 선언
	m_ui8Comp1[1] = new unsigned char[ISIZE / 4];
	m_ui8Comp1[2] = new unsigned char[ISIZE / 4];

	FILE *original_image = fopen(ori_path, "rb");
	if (!original_image) {
		cout << "original_image not open" << endl;
		return 0;
	}

	for (int ch = 0; ch < 3; ch++)
		fread(&(m_ui8Comp1[ch][0]), sizeof(unsigned char), m_iSize[ch], original_image); // original image의 픽셀값을 배열에 저장

	fclose(original_image);

	int n;
	cout << "Anti-Aliasing filter를 적용할까요?:" << endl << "1. Yes" << endl << "2. No" << endl;
	cin >> n;
	if (n == 1) {
		m_ui8Comp4 = new unsigned char*[3];
		m_ui8Comp4[0] = new unsigned char[ISIZE]; // 배열 선언
		m_ui8Comp4[1] = new unsigned char[ISIZE / 4];
		m_ui8Comp4[2] = new unsigned char[ISIZE / 4];

		convolve2D(m_ui8Comp1[0], m_ui8Comp4[0], WIDTH, HEIGHT, kernel, 5, 5);
		convolve2D(m_ui8Comp1[1], m_ui8Comp4[1], WIDTH / 2, HEIGHT / 2, kernel, 5, 5);
		convolve2D(m_ui8Comp1[2], m_ui8Comp4[2], WIDTH / 2, HEIGHT / 2, kernel, 5, 5);

		for (int i = 0; i < m_iSize[0]; i++) {
			if (i >= WIDTH || i %WIDTH != 0 || i % (WIDTH-1) != 0 || i <= ISIZE-1-WIDTH)
			m_ui8Comp1[0][i] = m_ui8Comp4[0][i];
		}

		strcpy(down_path, "./downsample/anti-aliasing(o)/RaceHorsesC_416x240_yuv420_8bit_frame120.yuv");
		strcpy(recon_path, "./upsample/anti-aliasing(o)/RaceHorsesC_832x480_yuv420_8bit_frame120.yuv");

		//FILE* filtered_image = fopen("./filtered/BasketballDrill_832x480_yuv420_8bit_frame360.yuv", "wb");
		//if (!filtered_image) {
		//	cout << "filtered_image not open" << endl;
		//	return 0;
		//}
		//for (int ch = 0; ch < 3; ch++)
		//	fwrite(&(m_ui8Comp1[ch][0]), sizeof(unsigned char), m_iSize[ch], filtered_image); // 필터링된 8-bit image를 .yuv 포맷 파일로 저장
		//fclose(filtered_image);
	}

	////////// Step 2. 이미지 downsampling //////////
	m_ui8Comp2 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		m_ui8Comp2[ch] = new unsigned char[ISIZE / 4]; // 배열 선언

	for (int ch = 0; ch < 3; ch++) {
		for (int i = 0; i < m_iSize[ch] / 4; i++)
			m_ui8Comp2[ch][i] = m_ui8Comp1[ch][1 + WIDTH * (i / (WIDTH / 2)) + (2 * i)];
	}

	FILE* downsample_image_w = fopen(down_path, "wb");
	if (!downsample_image_w) {
		cout << "downsample_image not open" << endl;
		return 0;
	}
	for (int ch = 0; ch < 3; ch++)
		fwrite(&(m_ui8Comp2[ch][0]), sizeof(unsigned char), m_iSize[ch] / 4, downsample_image_w); // downsample된 8-bit image를 .yuv 포맷 파일로 저장
	fclose(downsample_image_w);

	//////////// Step 3. 이미지 upsampling //////////
	FILE* downsample_image_r = fopen(down_path, "rb"); // downsample된 8-bit image의 픽셀값을 배열에 저장
	if (!downsample_image_r) {
		cout << "downsample_image not open" << endl;
		return 0;
	}
	unsigned char tmpY[HEIGHT / 2][WIDTH / 2];
	unsigned char tmpCb[HEIGHT / 4][WIDTH / 4];
	unsigned char tmpCr[HEIGHT / 4][WIDTH / 4];
	fread(tmpY, sizeof(unsigned char), WIDTH / 2 * HEIGHT / 2, downsample_image_r);
	fread(tmpCb, sizeof(unsigned char), WIDTH / 4 * HEIGHT / 4, downsample_image_r);
	fread(tmpCr, sizeof(unsigned char), WIDTH / 4 * HEIGHT / 4, downsample_image_r);
	fclose(downsample_image_r);

	memset(Y, 0, sizeof(unsigned char) * HEIGHT * WIDTH);
	memset(Cb, 0, sizeof(unsigned char) * HEIGHT / 2 * WIDTH / 2);
	memset(Cr, 0, sizeof(unsigned char) * HEIGHT / 2 * WIDTH / 2);

	for (int i = 0; i < HEIGHT / 2; i++) // integer-pel sample (Y)
		for (int j = 0; j < WIDTH / 2; j++)
			Y[i * 2][j * 2] = tmpY[i][j];

	for (int i = 0; i < HEIGHT / 4; i++) // integer-pel sample (Cb)
		for (int j = 0; j < WIDTH / 4; j++)
			Cb[i * 2][j * 2] = tmpCb[i][j];

	for (int i = 0; i < HEIGHT / 4; i++) // integer-pel sample (Cr)
		for (int j = 0; j < WIDTH / 4; j++)
			Cr[i * 2][j * 2] = tmpCr[i][j];

	six_tap();

	FILE* reconstructed_image_w = fopen(recon_path, "wb"); // upsample된 8-bit image를 .yuv 포맷 파일로 저장
	if (!reconstructed_image_w) {
		cout << "upsample_image not open" << endl;
		return 0;
	}

	fwrite(Y, sizeof(unsigned char), HEIGHT * WIDTH, reconstructed_image_w);
	fwrite(Cb, sizeof(unsigned char), HEIGHT / 2 * WIDTH / 2, reconstructed_image_w);
	fwrite(Cr, sizeof(unsigned char), HEIGHT / 2 * WIDTH / 2, reconstructed_image_w);
	fclose(reconstructed_image_w);

	////////// Step 4. Reconstructed image와 Original image 간 PSNR 측정 //////////
	m_ui8Comp3 = new unsigned char*[3];
	for (int ch = 0; ch < 3; ch++)
		m_ui8Comp3[ch] = new unsigned char[ISIZE]; // 배열 선언

	FILE* reconstructed_image_r = fopen(recon_path, "rb");
	if (!reconstructed_image_r) {
		cout << "reconstruced_image not open" << endl;
		return 0;
	}
	for (int ch = 0; ch < 3; ch++)
		fread(&(m_ui8Comp3[ch][0]), sizeof(unsigned char), m_iSize[ch], reconstructed_image_r); // upsample된 8-bit image의 픽셀값을 배열에 저장
	fclose(reconstructed_image_r);

	int N = ISIZE;
	int MAX = 255;
	int error;
	double mse, sum = 0;
	double psnr[3];

	for (int i = 0; i < 3; i++) { // Y, Cb, Cr의 PSNR을 각각 측정
		for (int ch = 0; ch < m_iSize[i]; ch++) {
			error = m_ui8Comp1[i][ch] - m_ui8Comp3[i][ch];
			sum += error * error;
		}
		mse = sum / N;
		psnr[i] = 20 * log10(MAX / sqrt(mse));
	}

	cout << "Y의 PSNR은 " << psnr[0] << "입니다." << endl;
	cout << "Cb의 PSNR은 " << psnr[1] << "입니다." << endl;
	cout << "Cr의 PSNR은 " << psnr[2] << "입니다." << endl;

	return 0;
} // end of main
