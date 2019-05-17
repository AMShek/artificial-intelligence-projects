#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Eigen\Dense>
//#include "tdidf.h"
#pragma once
using namespace std;
using namespace Eigen;

/*
map <string,unsigned int> emotion2num;
emotion2num["anger"] = 0;
emotion2num["disgust"] = 1;
emotion2num["fear"] = 2;
emotion2num["joy"] = 3;
emotion2num["sad"] = 4;
emotion2num["surprise"] = 5;
*/
struct neighbour {
	double distance;
	Eigen::Array<double, 1, 6> emotion;
	neighbour(double d, Eigen::Array<double, 1, 6> e) {
		distance = d;
		emotion = e;
	}
};

inline double CalDis(int r, Eigen::ArrayXd target, Eigen::ArrayXd neighbour) {
	if (target.size() == neighbour.size()) {
		Eigen::ArrayXd distance = target - neighbour;

		distance = distance.abs();

		distance = distance.pow(r);
		//cout << "distance array=" << endl << distance << endl;
		//cout << "sum=" << distance.sum() << endl;
		//cout << "1/r=" << (double)1 / r << endl;
		double result = pow(distance.sum(), (double)1 / r);
		//cout << "result=" << result << endl;
		return result;
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
std::vector<Eigen::Array<double, 1, 6>> knn
(unsigned int r,
	unsigned int k,
	std::vector<Eigen::Array<double, 1, 6>>labels,
	std::vector<Eigen::ArrayXd> weightMat,
	std::vector<Eigen::ArrayXd> target) {

	vector<neighbour> neighbours;
	vector<Eigen::Array<double, 1, 6>> returnEmotion;
	Eigen::Array<double, 1,6> emotionPredict;

	//for each document in target data set
	for (int i = 0; i<target.size(); i++) {
		cout << i << "\r";//test
		std::cout.flush();//test

		neighbours.clear();

		//compute the distance between target document and each neighbour
		for (int j = 0; j < weightMat.size(); j++) {
			neighbours.push_back(neighbour(CalDis(r, target[i], weightMat[j]), labels[j]));
		}

		sort(neighbours.begin(), neighbours.end(), ascDis);

		//get the vector of k nearest neighbours
		emotionPredict.setZero();
		for (int i = 0; i < k; i++) {
			emotionPredict += neighbours[i].emotion / pow(neighbours[i].distance,7);
			
		}

		//normalization
		emotionPredict /= emotionPredict.sum();
		returnEmotion.push_back(emotionPredict);
	}

	return returnEmotion;
}

