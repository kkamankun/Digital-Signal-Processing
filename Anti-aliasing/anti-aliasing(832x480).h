#pragma once
#pragma warning(disable: 4996)
#ifndef HEADER_H_
#define HEADER_H_
#include <iostream>
#include <string>
#define ISIZE 832 * 480
#define WIDTH 832
#define HEIGHT 480


using namespace std;

unsigned char** m_ui8Comp1; // Original image
unsigned char** m_ui8Comp2; // Downsample image
unsigned char** m_ui8Comp3; // Reconstructed image

int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 }; // 832*480 

void print_pixels(int ch);

#endif