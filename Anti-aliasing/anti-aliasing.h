#pragma once
#pragma warning(disable: 4996)
#ifndef HEADER_H_
#define HEADER_H_
#include <iostream>
#include <string>
#define ISIZE1 832 * 480
#define WIDTH1 832
#define HEIGHT1 480
#define ISIZE2 1920 * 1080


using namespace std;

unsigned char** m_ui8Comp1; // Original image
unsigned char** m_ui8Comp2; // Downsample image
unsigned char*** m_ui8Comp3; // Reconstructed image

int m_iSize1[3] = { ISIZE1, ISIZE1 / 4, ISIZE1 / 4 }; // 832*480 
int m_iSize2[3] = { ISIZE2, ISIZE2 / 4, ISIZE2 / 4 }; // 1920*1080

void print_pixels(int ch);

#endif
