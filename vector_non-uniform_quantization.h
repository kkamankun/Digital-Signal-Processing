#pragma once

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <ctime>

typedef unsigned char byte;

using namespace std;

class VectorClusteringCompressor
{
private:
	byte* pData; // Image data

	int width; // Image width
	int height; // Image height
	int depth; // Image depth

	int numOfCluster; // Count of cluster
	int lengthOfVector; // Length of vector

	vector<vector<int>> codewords; // pointer for N vectors
	vector<vector<vector<int>>> clusters; // N-Clusters according to N vectors
	vector<vector<int>> quant; // Result of quantization of input vectors
	vector<pair<bool, int>> isUpdated; // Table which store boolean value when the cluster is changed

	void setImageData(const byte* pData) { this->pData = (byte*)pData; };
	void setWidth(const int width) { this->width = width; };
	void setHeight(const int height) { this->height = height; };
	void setDepth(const int depth) { this->depth = depth; };
	void setNumOfCluster(const int num) { this->numOfCluster = num; };
	void setLenghtOfVector(const int length) { this->lengthOfVector = length; };

	bool InitializeCordwords(vector<vector<int>>& codewords, const int numOfCluster, const int sizeOfVec, const byte* pData, const int w, const int h, const int d);
	bool Clustering(const vector<vector<int>>& codewords, const int sizeOfVec, const byte* pData, const int w, const int h, const int d, vector<vector<vector<int>>>& clusters);
	int ComputeCodeWords(const vector<vector<vector<int>>>& clusters, const int lenghtOfVector, vector<vector<int>>& codewords);
	bool QuantizeVectors(const vector<vector<int>>& codewords, const int sizeOfVec, const byte* pData, const int w, const int h, const int d, vector<vector<int>>& quant);
	bool DecompressImage(const vector<vector<int>>& codewords, const vector<vector<int>>& quant, const int sizeOfVec, const int w, const int h, const int d, byte* pData);


	inline double GetEuclideanDistance(const vector<int>& vert1, const vector<int>& vert2) {
		long sum = 0;
		for (int i = 0; i < vert1.size(); i++) {
			sum += ((vert2[i] - vert1[i] * (vert2[i] - vert1[i])));
		}

		return sqrt(sum);
	}

public:
	// Constructor
	VectorClusteringCompressor();

	// Destructor
	~VectorClusteringCompressor();

	// Compress
	bool Encode(const byte* pData, const int w, const int h, const d, const int numCluster, const int vecSize);
	bool Decode(byte* pData);

};
