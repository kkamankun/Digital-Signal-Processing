#pragma once
#pragma warning(disable: 4996)
#ifndef HEADER_H_
#define HEADER_H_
#include <iostream>
#include <string.h>
#define ISIZE 832 * 480
#define WIDTH 832
#define HEIGHT 480

using namespace std;

unsigned char** m_ui8Comp1; // Original image
unsigned char** m_ui8Comp2; // Downsample image
unsigned char** m_ui8Comp3; // Reconstructed image
unsigned char** m_ui8Comp4; // Anti-aliasing filtering

int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 }; // 832*480
unsigned char Y[HEIGHT][WIDTH];
unsigned char Cb[HEIGHT / 2][WIDTH / 2];
unsigned char Cr[HEIGHT / 2][WIDTH / 2];

void print_pixels(int ch);

// define 5x5 Gaussian kernel
float kernel[25] = { 1 / 256.0f,  4 / 256.0f,  6 / 256.0f,  4 / 256.0f, 1 / 256.0f,
					 4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f,
					 6 / 256.0f, 24 / 256.0f, 36 / 256.0f, 24 / 256.0f, 6 / 256.0f,
					 4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f,
					 1 / 256.0f,  4 / 256.0f,  6 / 256.0f,  4 / 256.0f, 1 / 256.0f };

char ori_path[] = "./TestImage_832x480_yuv420_8bit/BasketballDrill_832x480_yuv420_8bit_frame360.yuv";
char down_path[] = "./downsample/anti-aliasing(x)/BasketballDrill_416x240_yuv420_8bit_frame360.yuv";
char recon_path[] = "./upsample/anti-aliasing(x)/BasketballDrill_832x480_yuv420_8bit_frame360.yuv";

bool convolve2D(unsigned char* in, unsigned char* out, int sizeX, int sizeY, float* kernel, int kSizeX, int kSizeY);
void six_tap();


#endif
