// Vector non-uniform quantization
#include "VQ.h"

namespace VQ_Y { // Y채널 벡터 분균일 양자화기
	unsigned short** R;
	int group[ISIZE / 4];
	int traslation[K];
	double ** means;
}

namespace VQ_Cb { // Cb채널 벡터 분균일 양자화기
	unsigned short** R;
	int group[ISIZE / 4];
	int traslation[K];
	double ** means;
}

namespace VQ_Cr { // Cr채널 벡터 분균일 양자화기
	unsigned short** R;
	int group[ISIZE / 4];
	int traslation[K];
	double ** means;
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

	// Cb
	vec_Cb = new unsigned short*[numOfVec[1]];
	for (int cnt = 0; cnt < numOfVec[1]; cnt++) {
		vec_Cb[cnt] = new unsigned short[4];
	}

	for (int i = 0; i < numOfVec[1]; i++) {
		vec_Cb[i][0] = m_ui16Comp[1][0 + 480 * (i / (width / 4)) + (2 * i)];
		vec_Cb[i][1] = m_ui16Comp[1][1 + 480 * (i / (width / 4)) + (2 * i)];
		vec_Cb[i][2] = m_ui16Comp[1][480 + 480 * (i / (width / 4)) + (2 * i)];
		vec_Cb[i][3] = m_ui16Comp[1][481 + 480 * (i / (width / 4)) + (2 * i)];
	}

	//for (int i = 0; i < numOfVec[1]; i++) {
	//cout << i << ':' << vec_Cb[i][0] << ',' << vec_Cb[i][1] << ',' << vec_Cb[i][2] << ',' << vec_Cb[i][3] << endl;
	//}

	// Cr
	vec_Cr = new unsigned short*[numOfVec[2]];
	for (int cnt = 0; cnt < numOfVec[2]; cnt++) {
		vec_Cr[cnt] = new unsigned short[4];
	}

	for (int i = 0; i < numOfVec[2]; i++) {
		vec_Cr[i][0] = m_ui16Comp[2][0 + 480 * (i / (width / 4)) + (2 * i)];
		vec_Cr[i][1] = m_ui16Comp[2][1 + 480 * (i / (width / 4)) + (2 * i)];
		vec_Cr[i][2] = m_ui16Comp[2][480 + 480 * (i / (width / 4)) + (2 * i)];
		vec_Cr[i][3] = m_ui16Comp[2][481 + 480 * (i / (width / 4)) + (2 * i)];
	}

	//for (int i = 0; i < numOfVec[2]; i++) {
	//cout << i << ':' << vec_Cr[i][0] << ',' << vec_Cr[i][1] << ',' << vec_Cr[i][2] << ',' << vec_Cr[i][3] << endl;
	//}
}

void kmeans()
{
	////  Y: set new means  ////
	VQ_Y::means = new double*[K];

	for (int i = 0; i < K; i++)
		VQ_Y::means[i] = new double[4];

	for (int i = 0; i < K; i++)
		for (int j = 0; j < 4; j++)
			VQ_Y::means[i][j] = vec_Y[i * 50 + 40000][j];

	////  Cb: set new means  ////
	VQ_Cb::means = new double*[K];

	for (int i = 0; i < K; i++)
		VQ_Cb::means[i] = new double[4];

	for (int i = 0; i < K; i++)
		for (int j = 0; j < 4; j++)
			VQ_Cb::means[i][j] = vec_Cb[i * 50 + 10000][j];

	////  Cr: set new means  ////
	VQ_Cr::means = new double*[K];

	for (int i = 0; i < K; i++)
		VQ_Cr::means[i] = new double[4];

	for (int i = 0; i < K; i++)
		for (int j = 0; j < 4; j++)
			VQ_Cr::means[i][j] = vec_Cr[i * 50 + 10000][j];

	//// Y: clustering  ////
	for (int i = 0; i < 2; i++) {
		for (int i = 0; i < numOfVec[0]; i++) {
			double min_dis = 9999999;
			int min = -1;

			// calculating distances //
			for (int j = 0; j < K; j++) {
				double dis = 0;
				for (int l = 0; l < 4; l++) {
					dis += pow(float(vec_Y[i][l] - VQ_Y::means[j][l]), 2);
				}
				// cout << dis << endl;
				if (dis < min_dis) {
					min_dis = dis;
					min = j;
				}
			}

			// set groups //
			VQ_Y::group[i] = min;
		}

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
				if (fabs(temp[i][j] - VQ_Y::means[i][j]) > 0.01) {
					VQ_Y::means[i][j] = temp[i][j];
				}
			}
	}

	//// Cb: clustering  ////
	for (int i = 0; i < 2; i++) {
		for (int i = 0; i < numOfVec[1]; i++) {
			double min_dis = 9999999;
			int min = -1;

			// calculating distances //
			for (int j = 0; j < K; j++) {
				double dis = 0;
				for (int l = 0; l < 4; l++) {
					dis += pow(float(vec_Cb[i][l] - VQ_Cb::means[j][l]), 2);
				}
				// cout << dis << endl;
				if (dis < min_dis) {
					min_dis = dis;
					min = j;
				}
			}

			// set groups //
			VQ_Cb::group[i] = min;
		}

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
		for (int i = 0; i < numOfVec[1]; i++) {
			count[VQ_Cb::group[i]]++;
			for (int j = 0; j < 4; j++)
				temp[VQ_Cb::group[i]][j] += vec_Cb[i][j];
		}

		for (int i = 0; i < K; i++)
			for (int j = 0; j < 4; j++) {
				temp[i][j] /= count[i];
				if (fabs(temp[i][j] - VQ_Cb::means[i][j]) > 0.01) {
					VQ_Cb::means[i][j] = temp[i][j];
				}
			}
	}

	//// Cr: clustering  ////
	for (int i = 0; i < 2; i++) {
		for (int i = 0; i < numOfVec[2]; i++) {
			double min_dis = 9999999;
			int min = -1;

			// calculating distances //
			for (int j = 0; j < K; j++) {
				double dis = 0;
				for (int l = 0; l < 4; l++) {
					dis += pow(float(vec_Cr[i][l] - VQ_Cr::means[j][l]), 2);
				}
				// cout << dis << endl;
				if (dis < min_dis) {
					min_dis = dis;
					min = j;
				}
			}

			// set groups //
			VQ_Cr::group[i] = min;
		}

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
		for (int i = 0; i < numOfVec[2]; i++) {
			count[VQ_Cr::group[i]]++;
			for (int j = 0; j < 4; j++)
				temp[VQ_Cr::group[i]][j] += vec_Cr[i][j];
		}

		for (int i = 0; i < K; i++)
			for (int j = 0; j < 4; j++) {
				temp[i][j] /= count[i];
				if (fabs(temp[i][j] - VQ_Cr::means[i][j]) > 0.01) {
					VQ_Cr::means[i][j] = temp[i][j];
				}
			}
	}

	//for (int i = 0; i < K; i++) {
	//	for (int j = 0; j < 4; j++)
	//		cout << VQ_Cb::means[i][j] << "\t";
	//	cout << endl;
	//}



}

void InitializeCodebook()
{
	//// Y: Codebook ////
	vector<pair<float, int>> v1;
	for (int i = 0; i < K; i++) {
		float sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += VQ_Y::means[i][j];
		}
		pair<float, int> p1 = make_pair(sum, i);
		v1.push_back(p1);
	}
	sort(v1.begin(), v1.end());

	//for (int i = 0; i < K; i++) {
	//	cout << v1[i].first << "\t" << v1[i].second << endl;
	//}

	VQ_Y::traslation[K];
	for (int i = 0; i < K; i++) {
		VQ_Y::traslation[i] = v1[i].second;
	}

	VQ_Y::R = new unsigned short*[K];

	for (int i = 0; i < K; i++)
		VQ_Y::R[i] = new unsigned short[4];


	for (int i = 0; i < K; i++) {
		for (int j = 0; j < 4; j++) {
			VQ_Y::R[i][j] = VQ_Y::means[v1[i].second][j];
		}
	}

	//// Cb: Codebook ////
	vector<pair<float, int>> v2;
	for (int i = 0; i < K; i++) {
		float sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += VQ_Cb::means[i][j];
		}
		pair<float, int> p2 = make_pair(sum, i);
		v2.push_back(p2);
	}
	sort(v2.begin(), v2.end());

	//for (int i = 0; i < K; i++) {
	//cout << v2[i].first << "\t" << v2[i].second << endl;
	//}

	VQ_Cb::traslation[K];
	for (int i = 0; i < K; i++) {
		VQ_Cb::traslation[i] = v2[i].second;
	}

	VQ_Cb::R = new unsigned short*[K];

	for (int i = 0; i < K; i++)
		VQ_Cb::R[i] = new unsigned short[4];


	for (int i = 0; i < K; i++) {
		for (int j = 0; j < 4; j++) {
			VQ_Cb::R[i][j] = VQ_Cb::means[v2[i].second][j];
		}
	}

	//// Cr: Codebook ////
	vector<pair<float, int>> v3;
	for (int i = 0; i < K; i++) {
		float sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += VQ_Cr::means[i][j];
		}
		pair<float, int> p3 = make_pair(sum, i);
		v3.push_back(p3);
	}
	sort(v3.begin(), v3.end());

	//for (int i = 0; i < K; i++) {
	//	cout << v3[i].first << "\t" << v3[i].second << endl;
	//}

	VQ_Cr::traslation[K];
	for (int i = 0; i < K; i++) {
		VQ_Cr::traslation[i] = v3[i].second;
	}

	VQ_Cr::R = new unsigned short*[K];

	for (int i = 0; i < K; i++)
		VQ_Cr::R[i] = new unsigned short[4];


	for (int i = 0; i < K; i++) {
		for (int j = 0; j < 4; j++) {
			VQ_Cr::R[i][j] = VQ_Cr::means[v3[i].second][j];
		}
	}
}

void Encode()
{
	// Y
	//for (int i = 0; i < K; i++) {
	//	for (int j = 0; j < 4; j++)
	//		cout << VQ_Y::means[i][j] << "\t";
	//	cout << endl;
	//}
	
	for (int i = 0; i < numOfVec[0]; i++) {
		for (int j = 0; j < K; j++) {
			if (VQ_Y::group[i] == VQ_Y::traslation[j]) {
				m_ui8Comp[0][i] = (unsigned int)VQ_Y::means[j][0] / 4;
				// cout << "j:" << j << " " << VQ_Y::means[j][0] << "\t";
				break;
			}
			// m_ui8Comp[0][i] = (unsigned char)j;
		}
		// cout << (int)m_ui8Comp[0][i] << " ";
	}


	// Cb
	for (int i = 0; i < numOfVec[1]; i++) {
		for (int j = 0; j < K; j++) {
			if (VQ_Cb::group[i] == VQ_Cb::traslation[j]) {
				m_ui8Comp[1][i] = (unsigned int)VQ_Cb::means[j][0] / 4;
				break;
			}
			// m_ui8Comp[1][i] = (unsigned char)j;
			// cout << (int)m_ui8Comp[1][i] << " ";
		}
	}

	// Cr
	for (int i = 0; i < numOfVec[2]; i++) {
		for (int j = 0; j < K; j++) {
			if (VQ_Cr::group[i] == VQ_Cr::traslation[j]) {
				m_ui8Comp[2][i] = (unsigned int)VQ_Cr::means[j][0] / 4;
				break;
			}
			// m_ui8Comp[2][i] = (unsigned char)j;
			// cout << (int)m_ui8Comp[2][i] << " ";
		}
	}

}

void printData_Y() {
	
	cout << "Y:" << endl;
	for (int i = 0; i < numOfVec[0]; i++) {
		cout << "vector" << i << " : ";
		for (int j = 0; j < 4; j++)
			cout << vec_Y[i][j] << " ";
		cout << "group : " << VQ_Y::group[i] << endl;
	}
	cout << endl;
}

void printData_Cb() {

	cout << "Cb:" << endl;
	for (int i = 0; i < numOfVec[1]; i++) {
		cout << "vector" << i << " : ";
		for (int j = 0; j < 4; j++)
			cout << vec_Cb[i][j] << " ";
		cout << "group : " << VQ_Cb::group[i] << endl;
	}
	cout << endl;
}

void printData_Cr() {

	cout << "Cr:" << endl;
	for (int i = 0; i < numOfVec[2]; i++) {
		cout << "vector" << i << " : ";
		for (int j = 0; j < 4; j++)
			cout << vec_Cr[i][j] << " ";
		cout << "group : " << VQ_Cr::group[i] << endl;
	}
	cout << endl;
}

void Decode()
{
	// Y
	for (int i = 0; i < numOfVec[0]; i++) {
		m_ui16Comp2[0][0 + 960 * (i / (width / 2)) + (2 * i)] = VQ_Y::R[m_ui8Comp[0][i]][0];
		m_ui16Comp2[0][1 + 960 * (i / (width / 2)) + (2 * i)] = VQ_Y::R[m_ui8Comp[0][i]][1];
		m_ui16Comp2[0][0 + 960 * (i / (width / 2)) + (2 * i)] = VQ_Y::R[m_ui8Comp[0][i]][2];
		m_ui16Comp2[0][1 + 960 * (i / (width / 2)) + (2 * i)] = VQ_Y::R[m_ui8Comp[0][i]][3];
	}

	//for (int i = 0; i < numOfVec[0]; i++) {
	//	cout << i << ':' << vec_Y[i][0] << ',' << vec_Y[i][1] << ',' << vec_Y[i][2] << ',' << vec_Y[i][3] << endl;
	//}

	// Cb
	for (int i = 0; i < numOfVec[1]; i++) {
		m_ui16Comp2[1][0 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cb::R[m_ui8Comp[1][i]][0];
		m_ui16Comp2[1][1 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cb::R[m_ui8Comp[1][i]][1];
		m_ui16Comp2[1][0 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cb::R[m_ui8Comp[1][i]][2];
		m_ui16Comp2[1][1 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cb::R[m_ui8Comp[1][i]][3];
	}

	//for (int i = 0; i < numOfVec[1]; i++) {
	//cout << i << ':' << vec_Cb[i][0] << ',' << vec_Cb[i][1] << ',' << vec_Cb[i][2] << ',' << vec_Cb[i][3] << endl;
	//}

	// Cr
	for (int i = 0; i < numOfVec[2]; i++) {
		m_ui16Comp2[2][0 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cr::R[m_ui8Comp[2][i]][0];
		m_ui16Comp2[2][1 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cr::R[m_ui8Comp[2][i]][1];
		m_ui16Comp2[2][0 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cr::R[m_ui8Comp[2][i]][2];
		m_ui16Comp2[2][1 + 480 * (i / (width / 4)) + (2 * i)] = VQ_Cr::R[m_ui8Comp[2][i]][3];
	}


	//for (int i = 0; i < numOfVec[2]; i++) {
	//cout << i << ':' << vec_Cr[i][0] << ',' << vec_Cr[i][1] << ',' << vec_Cr[i][2] << ',' << vec_Cr[i][3] << endl;
	//}
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
	// printData_Y();
	// printData_Cb();
	// printData_Cr();
	InitializeCodebook();
	Encode();

	FILE* quantized_image = fopen("./output/RitualDance_480x270_8bit_420_frame100.yuv", "wb");
	//for (int ch = 0; ch < 3; ch++)
	//	fwrite(&(m_ui8Comp[ch][0]), sizeof(unsigned char), numOfVec[ch], quantized_image); // 양자화된 8-bit image를 .yuv 포맷 파일로 저장
	fwrite(&(m_ui8Comp[0][0]), sizeof(unsigned char), numOfVec[0], quantized_image); // 양자화된 8-bit image를 .yuv 포맷 파일로 저장
	fwrite(&(m_ui8Comp[1][0]), sizeof(unsigned char), numOfVec[1], quantized_image); // 양자화된 8-bit image를 .yuv 포맷 파일로 저장
	fwrite(&(m_ui8Comp[2][0]), sizeof(unsigned char), numOfVec[2], quantized_image); // 양자화된 8-bit image를 .yuv 포맷 파일로 저장

	fclose(quantized_image);

	////////// Step 3. Quantized image에 역양자화 수행 //////////
	m_ui16Comp2 = new unsigned short*[3];
	for (int cnt = 0; cnt < 3; cnt++) {
		m_ui16Comp2[cnt] = new unsigned short[ISIZE];
	}

	Decode();

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

	FILE* reconstructed_image = fopen("./output/RitualDance_960x540_10bit_420_frame100.yuv", "wb");
	for (int ch = 0; ch < 3; ch++)
		fwrite(&(m_ui16Comp2[ch][0]), sizeof(unsigned short), m_iSize[ch], reconstructed_image); // 역양자화된 10-bit image를 .yuv 포맷 파일로 저장

	fclose(quantized_image);

	return 0;


}
