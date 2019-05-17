#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Eigen\Dense>
//#include "tdidf.h"
#pragma once
using namespace std;
using namespace Eigen;


struct neighbour {
	double distance;
	unsigned int emotion;
	neighbour(double d, unsigned int e){
		distance = d;
		emotion = e;
	}
};
inline double CalDisCos(int r, Eigen::ArrayXd target, Eigen::ArrayXd neighbour) {
	if (target.size() == neighbour.size()) {
		double result = target.cwiseProduct(neighbour).sum();
		result /= target.matrix().norm();
		result /= neighbour.matrix().norm();
		return result;
	}
	else {
		cout << "different size!!" << endl;
		return -1;
	}
}
inline double CalDis (int r,Eigen::ArrayXd target, Eigen::ArrayXd neighbour){
	if (target.size() == neighbour.size()) {
		Eigen::ArrayXd distance = target - neighbour;
		
		distance = distance.abs();
		
		distance = distance.pow(r);
		double result = pow(distance.sum(),(double)1/r);
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
bool desDis(neighbour a, neighbour b) {
	return a.distance > b.distance;
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
std::vector<unsigned int> knn
	(unsigned int r,
	unsigned int k,
	std::vector<unsigned int>labels,
	std::vector<Eigen::ArrayXd> weightMat,
	std::vector<Eigen::ArrayXd> target){

	unsigned int emotionCount[6];
	vector<neighbour> neighbours;
	vector<unsigned int> returnEmotion;

	//for (Eigen::ArrayXd document : target) {
	for (int i=0; i<target.size();i++){
		cout << i << "\r";//test
		std::cout.flush();//test

		neighbours.clear();
		memset(emotionCount, 0, sizeof(emotionCount));

		for (int j = 0; j < weightMat.size();j++) {
			neighbours.push_back(neighbour(CalDis(r, target[i], weightMat[j]), labels[j]));
		}

		sort(neighbours.begin(), neighbours.end(), ascDis);

		//get the vector of k nearest neighbours
		for (int i = 0; i < k; i++) {
			emotionCount[neighbours[i].emotion]++;
		}
		/*
		cout << "the nearest neighbours' emotionCount are" << endl;//test
		for (int i = 0; i < 6; i++) {
			cout << emotionCount[i] << " ";
		}
		cout << endl;
		*/


		unsigned int maxCount = 0;
		unsigned int emotion;
		for (int i = 0; i <6 ; i++) {
			if (emotionCount[i] >= maxCount) {
				maxCount = emotionCount[i];
				emotion = i;
			}
		}
		returnEmotion.push_back(emotion);
	}
	/*
	for (int i = 0; i < returnEmotion.size(); i++) {
		//cout << "Line" << i << " of validation/test set's predicted emotion.\n";
		//cout << num2emotion[returnEmotion[i]]<<endl;
	} 
	*/

	return returnEmotion;
}

