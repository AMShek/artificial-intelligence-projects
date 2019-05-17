#pragma once
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <Eigen\Dense>
#include <vector>
#include <set>
#include <omp.h>
#include <Windows.h>
using namespace std;
using namespace Eigen;

std::vector<std::string> vocabList; // all terms

class tfidf {
private:
	unsigned int nrow = 0; // matrix row number, namely the number of documents
	unsigned int ncol = 0; // matrix column number
	std::vector<Eigen::ArrayXd> dataMat; // converted bag of words matrix
	std::vector<std::vector<std::string>> rawDataSet; // raw data
	std::vector<int> numOfTerms; // used in tf calculation
	Eigen::ArrayXd IDFVector;//IDF, used in calculating TF-IDF
							 //Eigen::ArrayXd termCount;

	inline Eigen::VectorXd bagOfWords2VecMN(const std::vector<std::string> & inputSet);
	void vec2mat();


public:
	tfidf(std::vector<std::vector<std::string>> & input) :rawDataSet(input) {
	}
	std::vector<Eigen::ArrayXd> weightMat; // TF-IDF weighting matrix
	void createVocabList();
	void calMat();
	void writeData();
	void writeDataNoZero();
};

/*
OBJECTIVE:
create private variable VocalList (A "word bag")
VocalList is the collection of terms
VocalList's terms are all distinct
*/
void tfidf::createVocabList() {
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);

	for (std::vector<std::string> document : rawDataSet) {
		for (std::string word : document)
			if (std::find(vocabList.begin(), vocabList.end(), word) == vocabList.end())
				vocabList.push_back(word);
	}

	QueryPerformanceCounter(&t2);
	printf("Create the set of terms.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);

	nrow = rawDataSet.size();
	ncol = vocabList.size();

	//this->termCount.resize(ncol);
	//this->termCount.fill(1);
	//memset(&(this->termCount[0]),1,this->termCount.size());

	this->IDFVector.resize(ncol);
	this->IDFVector.fill(1);
	return;
}

/*
OBJECTIVE:
For each term in VocalLIst, calculate the number of it.
The numbers of terms are in returnVect.
INPUT:
inputSet is a vector of all terms in a document
Terms may have duplicates
OUTPUT:
returnVec is a vector of double containing the number of each term in this document
returnVec[i] shows the number of the i_th term
*/
inline Eigen::VectorXd tfidf::bagOfWords2VecMN(const std::vector<std::string> & inputSet) {
	Eigen::VectorXd returnVec;
	returnVec.resize(ncol);
	returnVec.fill(0);

	for (std::string word : inputSet) {
		size_t idx = std::find(vocabList.begin(), vocabList.end(), word) - vocabList.begin();
		if (idx == vocabList.size())
			cout << "word: " << word << "not found" << endl;
		else
			returnVec(idx) += 1;
	}
	//error!
	//这里测试输出的returnVec是一个值全部为一个很奇怪的小数的向量
	//可能是没有初始化？ -> 是的，错误点在这里
	//cout << "eigen vector return is " << endl;
	//cout << returnVec << "\r";
	//std::cout.flush();
	return returnVec;
}

void tfidf::vec2mat() {
	int cnt = 0;

	//for each string(document) in raw DataSet, add a row to matrix
	for (auto it = rawDataSet.begin(); it != rawDataSet.end(); ++it) {
		cnt++;
		cout << cnt << "\r";
		std::cout.flush();
		Eigen::VectorXd dataVec = bagOfWords2VecMN(*it);
		dataMat.push_back(dataVec);
		numOfTerms.push_back(it->size());
		it->clear();
	}
	//ncol = dataMat[0].size();
	nrow = dataMat.size();
	rawDataSet.clear(); // release memory
}

/*
OBJECTIVE:
Calculate the TF_IDF matrix
dataMat: A matrix consisted of (vectors recording the number of each terms in this docoment)
termCount: A vector got by adding entries in the same column in dataMat
numOfTerms
*/
void tfidf::calMat() {
	LARGE_INTEGER t1, t2, tc;

	createVocabList();
	vec2mat();
	//error:删除发现dataMat是空的！？
	//错误已解决：问题出在vector没有初始化
	for (unsigned int i = 0; i != nrow; ++i) {
		for (unsigned int j = 0; j != ncol; ++j) {
			if (this->dataMat[i][j] >= 1) // only keep 1 and 0
				this->IDFVector(j)++;
		}
	}

	IDFVector = IDFVector.inverse();

	this->IDFVector *= this->nrow;

	IDFVector = IDFVector.log();
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);

	for (unsigned int i = 0; i<nrow; i++) {
		Eigen::ArrayXd tf = dataMat[i] / numOfTerms[i];
		weightMat.push_back(tf*this->IDFVector);
	}


	QueryPerformanceCounter(&t2);
	printf("Calulate TF-IDF.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);
}

void tfidf::writeData() {
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);

	unsigned int i, j;
	std::ofstream out("eigen_tf_idf.txt");
	if (out.is_open()) {
		cout << "Start to write TF_IDF matrix!\n";
#pragma omp parallel for num_threads(8)
		for (i = 0; i < this->nrow; i++) {
#pragma omp parallel for num_threads(8)
			for (j = 0; j < this->ncol; j++) {
				out << this->weightMat[i][j] << "\t";
			}
			out << "\n";
		}
	}

	QueryPerformanceCounter(&t2);
	printf("Write TF-IDF Matrix to output file.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);
}


void tfidf::writeDataNoZero() {
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);

	unsigned int i, j;
	std::ofstream out("eigen_tf_idf_no_zero.txt");
	if (out.is_open()) {
		cout << "Start to write TF_IDF matrix!\n";
		for (i = 0; i < this->nrow; i++) {
			for (j = 0; j < this->ncol; j++) {
				if (this->weightMat[i][j])
					out << this->weightMat[i][j] << "\t";
			}
			out << "\n";
		}
	}

	QueryPerformanceCounter(&t2);
	printf("Write TF-IDF Matrix (without zero) to output file.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);

}