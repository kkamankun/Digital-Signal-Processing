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

char ori_path[] = "./TestImage_832x480_yuv420_8bit/BasketballDrill_832x480_yuv420_8bit_frame0.yuv";
char coeffi1_path[] = "./transform_coefficients/BasketballDrill_832x480_yuv420_8bit_coefficient1.yuv";
char coeffi2_path[] = "./transform_coefficients/BasketballDrill_832x480_yuv420_8bit_coefficient2.yuv";
char coeffi3_path[] = "./transform_coefficients/BasketballDrill_832x480_yuv420_8bit_coefficient3.yuv";
