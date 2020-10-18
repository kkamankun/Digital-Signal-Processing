#pragma once
#ifndef LLOYD_H_
#define LLOYD_H_
#include <iostream>
#include <math.h>

using namespace std;

#define ISIZE 540*960
#define L 256 // 2^8

unsigned short ** m_ui16Comp; // input image
unsigned short ** m_ui16Comp2; // reconstructed image
unsigned char **m_ui8Comp; // quantized image

int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 };

void initialize();
void printD();
void printR();


#endif
