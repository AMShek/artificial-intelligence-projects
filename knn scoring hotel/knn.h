#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Eigen\Dense>
#include "file_related.h"

#pragma once
using namespace std;
using namespace Eigen;


struct neighbour {
	double distance;
	double label;
	neighbour(double d, double l) {
		distance = d;
		label = l;
	}
};
//先不开方
//把另外3个属性的次方加到TF-IDF的次方和上，再一起开方
inline double CalDis_NoRooting(int r, Eigen::ArrayXd target, Eigen::ArrayXd neighbour) {
	if (target.size() == neighbour.size()) {
		Eigen::ArrayXd distance = target - neighbour;

		distance = distance.abs();

		distance = distance.pow(r);
		//cout << "distance array=" << endl << distance << endl;
		//cout << "sum=" << distance.sum() << endl;
		//cout << "1/r=" << (double)1 / r << endl;

		//double result = pow(distance.sum(), (double)1 / r);
		//cout << "result=" << result << endl;

		double result_NoRooting;
		result_NoRooting = distance.sum();
		return result_NoRooting;
	}
	else {
		cout << "different size!!" << endl;
		return -1;
	}
}

bool ascDis(neighbour a, neighbour b) {
	return a.distance < b.distance;
}

/*
INPUT:
r:parameter of distance
k:parameter of knn method
wightMat: td-idf matrix of train set
labels: lables of train set
target:td-idf matrix of target set
OUTPUT:
a vector of unsigned integer showing the emotion.
use arrar num2emotion to get the corresponding emotion string.
*/
void knn
(unsigned int r,
	unsigned int k,
	std::vector<double>labels,
	std::vector<Eigen::ArrayXd> weightMat,
	std::vector<Eigen::ArrayXd> target) {
	//weightMat是训练集的TF-IDF矩阵
	//target是测试集的TF-IDF矩阵

	vector<neighbour> neighbours;
	vector<double> returnReview;
	double reviewPredict;
	double weightSum;

	//for each document in target data set

	//returnReview.resize(target.size());
	//#pragma omp parallel for num_threads(8)
	for (int i = 0; i<target.size(); i++) {
		cout << i << "\r";//test
		std::cout.flush();//test

		neighbours.clear();


		//compute the distance between target document and each neighbour
		for (int j = 0; j < weightMat.size(); j++) {
			double distance = CalDis_NoRooting(r, target[i], weightMat[j])*0.01;
			distance += pow(abs(Additional_Number_of_Scoring[j] - Additional_Number_of_Scoring_Test[i]), r)*0.01;
			distance += pow(abs(Average_Score[j] - Average_Score_Test[i]), r) * 6;
			distance += pow(abs(Total_Negative[j] - Total_Negative_Test[i]), r) * 4;
			distance += pow(abs(Total_Positive[j] - Total_Positive_Test[i]), r) * 4;
			distance = pow(distance, (1 / r));
			neighbours.push_back(neighbour(distance, labels[j]));
		}

		sort(neighbours.begin(), neighbours.end(), ascDis);

		//get the vector of k nearest neighbours
		reviewPredict = 0;
		weightSum = 0;
		for (int i = 0; i < k; i++) {
			double weight;
			weight = pow(Total_Reviews[i], 1 / 4)*pow(Total_Reviews_Given[i], 1 / 4) / pow(neighbours[i].distance, 1 / 4);
			//weight = 1/ pow(neighbours[i].distance, 1);
			weightSum += weight;
			reviewPredict += neighbours[i].label*weight;
		}

		//normalization
		reviewPredict /= weightSum;
		//cout << reviewPredict << endl;//test
		returnReview.push_back(reviewPredict);
	}

	std::ofstream out("regression_2.txt");
	if (out.is_open()) {
		cout << "Start to write predicted reviews!\n";
		for (int counter = 0; counter < returnReview.size(); counter++) {
			out << returnReview[counter] << "\n";
		}
	}
	//return returnReview;
}

