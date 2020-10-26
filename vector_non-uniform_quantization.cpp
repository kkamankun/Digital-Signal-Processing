#include "stdafx.h"
#include "vector_non-uniform_quantization.h"

#define MIN_UNUPDATED 5
#define MAX_KLUSTERING_PROCESS 1000

VectorClusteringCompressor::VectorClusteringCompressor() {
	pData = NULL; // Image data

	width = 0; // Image width
	height = 0; // Image height
	depth = 0; // Image color depth;

	numOfCluster = 0; // Count of cluster
	lengthOfVector = 0; // Length of bector
}

VectorClusteringCompressor::~VectorClusteringCompressor() {}

bool VectorClusteringCompressor::InitializeCordwords(vector<vector<int>>& codewords, const int numOfCluster, const int sizeOfVec, const byte* pData, const int w, const int h, const int d)
bool VectorClusteringCompressor::Clustering(const vector<vector<int>>& codewords, const int sizeOfVec, const byte* pData, const int w, const int h, const int d, vector<vector<vector<int>>>& clusters)
int VectorClusteringCompressor::ComputeCodeWords(const vector<vector<vector<int>>>& clusters, const int lenghtOfVector, vector<vector<int>>& codewords)
bool VectorClusteringCompressor::VectorClusteringCompressor::QuantizeVectors(const vector<vector<int>>& codewords, const int sizeOfVec, const byte* pData, const int w, const int h, const int d, vector<vector<int>>& quant)
bool VectorClusteringCompressor::DecompressImage(const vector<vector<int>>& codewords, const vector<vector<int>>& quant, const int sizeOfVec, const int w, const int h, const int d, byte* pData)
