// Scalar uniform quantization
#pragma warning(disable: 4996)
#include <iostream>
#define ISIZE 540*960
using namespace std;

int main()
{
	////////// Step 1. YCbCr 10-bit input image를 읽어서 메모리에 저장 //////////
	FILE *input_image = fopen("./input/RitualDance_960x540_10bit_420_frame350.yuv", "rb");

	if (!input_image) {
		cout << "file not open" << endl;
		return 0;
	}

	unsigned short ** m_ui16Comp = new unsigned short*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui16Comp[cnt] = new unsigned short[ISIZE];
	}

	int m_iBit = 10;
	int bitFactor = 2;

	int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 };

	for (int ch = 0; ch < 3; ch++)
		fread(&(m_ui16Comp[ch][0]), sizeof(unsigned short), m_iSize[ch], input_image); // 10-bit 영상을 16-bit씩 read

	fclose(input_image);

	////////// Step 2. Input image에 scalar uniform quantization //////////
	unsigned char **m_ui8Comp = new unsigned char*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui8Comp[cnt] = new unsigned char[ISIZE];
	}

	for (int i = 0; i < 3; i++) {
		for (int ch = 0; ch < m_iSize[i]; ch++) {
			m_ui8Comp[i][ch] = (unsigned char)(m_ui16Comp[i][ch] >> 2); // 10-bit input image의 Y, Cb, Cr 성분 단위로 8-bit로 양자화 수행
		}
	}

	FILE* quantized_image = fopen("./output/RitualDance_960x540_8bit_420_frame350.yuv", "wb");
	for (int ch = 0; ch < 3; ch++)
		fwrite(&(m_ui8Comp[ch][0]), sizeof(unsigned char), m_iSize[ch], quantized_image); // 양자화된 8-bit image를 .yuv 포맷 파일로 저장

	fclose(quantized_image);

	////////// Step 3. Quantized image에 역양자화 수행 //////////
	unsigned short ** m_ui16Comp2 = new unsigned short*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui16Comp2[cnt] = new unsigned short[ISIZE];
	}

	for (int i = 0; i < 3; i++) {
		for (int ch = 0; ch < m_iSize[i]; ch++) {
			m_ui16Comp2[i][ch] = (unsigned short)(m_ui8Comp[][ch] << 2); // 8-bit quantized image의 Y, Cb, Cr 성분 각각 역양자화를 수행하여 reconstruction image 복원
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

	return 0;

} // end of main
