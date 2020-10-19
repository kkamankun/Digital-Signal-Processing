#pragma once
#ifndef LLOYD_H_
#define LLOYD_H_
#include <iostream>
#include <math.h>

using namespace std;

#define ISIZE 540*960
#define L 256 // 2^8

unsigned short ** m_ui16Comp; // Input image
unsigned short ** m_ui16Comp2; // Reconstructed image
unsigned char **m_ui8Comp; // Quantized image

int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 }; // YCbCr420 format
unsigned char code[256]; // 8-bit code

void initialize(); // This function uniformly divides the total range into the number of levels
void calculateIntervals(); // Calculate the interval/range values D = (R[i] + R[i - 1]) / 2
void getNewLevels(unsigned short **P); // Recompute the values of R using the probability values
float getQuantizedValue_Y(unsigned short key); // This function gives the quantized value given the key(value)
float getQuantizedValue_Cb(unsigned short key); // This function gives the quantized value given the key(value)
float getQuantizedValue_Cr(unsigned short key); // This function gives the quantized value given the key(value)
void printD();
void printR();


#endif
