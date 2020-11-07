#pragma once
#pragma warning(disable: 4996)
#ifndef VQ_H_
#define VQ_H_
#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <utility>
#define ISIZE 540*960
#define K 256 // 2^8

using namespace std;

unsigned short** m_ui16Comp; // Input image
unsigned short** m_ui16Comp2; // Reconstructed image
unsigned char** m_ui8Comp; // Quantized image

unsigned short** vec_Y;
unsigned short** vec_Cb;
unsigned short** vec_Cr;

int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 }; // YCbCr420 format
unsigned char code[256]; // 8-bit code

int width = 960; // Image width
int height = 540; // Image height
int numOfVec[3] = { ISIZE / 4, ISIZE / 16, ISIZE / 16 };

#endif
