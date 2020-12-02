#pragma once
#include <iostream>
#define ISIZE 832 * 480
#define WIDTH 832
#define HEIGHT 480

using namespace std;

unsigned char** m_ui8Comp0; // Original image
unsigned char** coeffiecient; // Coefficient
unsigned char** m_ui8Comp4; // Reconstructed image

int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 }; // 832*480
int width[3] = { WIDTH, WIDTH / 2, WIDTH / 2 };
int height[3] = { HEIGHT, HEIGHT / 2, HEIGHT / 2 };

unsigned char Y[HEIGHT][WIDTH];
unsigned char Cb[HEIGHT / 2][WIDTH / 2];
unsigned char Cr[HEIGHT / 2][WIDTH / 2];

char test_path[] = "./test/RaceHorsesC_832x480_yuv420_8bit_frame120.yuv";
char ori_path[] = "./TestImage_832x480_yuv420_8bit/RaceHorsesC_832x480_yuv420_8bit_frame120.yuv";
