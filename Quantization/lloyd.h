#pragma once
#ifndef LLOYD_H_
#define LLOYD_H_
#include <iostream>
#include <math.h>

using namespace std;

#define ISIZE 540*960

unsigned short ** m_ui16Comp;
unsigned char **m_ui8Comp;


int m_iSize[3] = { ISIZE, ISIZE / 4, ISIZE / 4 };

void initialize();
void printD();
void printR();


#endif
