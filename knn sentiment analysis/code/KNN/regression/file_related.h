#pragma once
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <boost/tokenizer.hpp>
#include <Eigen\Dense>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <omp.h>
#include<Windows.h>
using namespace std;

//string num2emotion[6] = { "anger","disgust","fear","joy","sad","surprise" };
std::vector<Eigen::Array<double, 1, 6>> labels;//store labels of train set

/*
OBJECTIVE:
	Seperate a sentence into a vector of words.
	Reqire tokenizer in boost library.
*/
std::vector<std::string> textParse(const std::string & bigString) {
	std::vector<std::string> vec;
	boost::tokenizer<> tok(bigString);
	for (boost::tokenizer<>::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
		vec.push_back(*beg);
	}
	return vec;
}

std::vector<std::vector<std::string>> loadTrain() {

	/*LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);*/
	/*
	map <string, unsigned int> emotion2num;
	emotion2num["anger"] = 0;
	emotion2num["disgust"] = 1;
	emotion2num["fear"] = 2;
	emotion2num["joy"] = 3;
	emotion2num["sad"] = 4;
	emotion2num["surprise"] = 5;
	*/
	std::vector<std::vector<std::string>>  data;
	std::string sentence;
	int counter = 0;
	labels.clear();

	ifstream filename("C:\\Users\\mibyb\\Documents\\课程\\大三上\\AI\\lab1\\lab1_data\\lab1_data\\regression_dataset\\train_set.csv");
	//the first line of train set is the header
	//dont want it in data and lables
	//cout << "Ignore the header." << endl;//test
	getline(filename, sentence);
	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		if (counter%7==0) {
			//cout << "This is a document" << sentence << endl;//test
			std::vector<std::string> wordList = textParse(sentence);
			data.push_back(wordList);
			counter++;
			seperate = ',';
		}
		else if (counter%7==1){
			//cout << "This is a label" << sentence << endl;//test
			Eigen::Array<double, 1, 6> emotionVec;
			emotionVec[0]= atof(sentence.c_str());
			labels.push_back(emotionVec);
			counter++;
			seperate = ',';
		}
		else if (counter % 7 != 5) {
			labels[(int)counter/7][counter%7-1] = atof(sentence.c_str());
			counter++;
			seperate = ',';
		}
		else {
			labels[(int)counter / 7][counter % 7 - 1] =atof(sentence.c_str());
			counter++;
			seperate = '\n';
		}
	}
	/*QueryPerformanceCounter(&t2);
	printf("Load documents.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);*/
	return data;
}

std::vector<std::vector<std::string>> loadValidation() {
	std::vector<std::vector<std::string>>  data;
	std::string sentence;

	ifstream filename("C:\\Users\\mibyb\\Documents\\课程\\大三上\\AI\\lab1\\lab1_data\\lab1_data\\regression_dataset\\validation_set.csv");
	//the first line of train set is the header
	//dont want it in data and labels
	getline(filename, sentence);
	int counter = 0;
	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		//cout << "counter=" << counter << endl;//test
		if (counter%7==0) {
			//cout << "This is a document" << sentence << endl;//test
			std::vector<std::string> wordList = textParse(sentence);
			data.push_back(wordList);
			counter++;
			seperate = ',';
		}
		else if (counter%7!=5){
			counter++;
			seperate = ',';
		}
		else {
			counter++;
			seperate = '\n';
		}
	}
	return data;
}

std::vector<std::vector<std::string>> loadTest() {
	std::vector<std::vector<std::string>>  data;
	std::string sentence;
	int counter = 0;

	ifstream filename("C:\\Users\\mibyb\\Documents\\课程\\大三上\\AI\\lab1\\lab1_data\\lab1_data\\regression_dataset\\test_set.csv");
	//the first line of train set is the header
	//dont want it in data and lables
	getline(filename, sentence);

	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		//cout << "counter=" << counter << endl;//test
		if (counter % 8 == 1) {
			//cout << "This is a document" << sentence << endl;//test
			std::vector<std::string> wordList = textParse(sentence);
			data.push_back(wordList);
			seperate = ',';
			counter++;

		}
		else if (counter % 8 != 6) {
			counter++;
			seperate = ',';
		}
		else {
			counter++;
			seperate = '\n';
		}

	}

	return data;
}

void writeResult(std::vector<Eigen::Array<double, 1, 6>>emotionPredict) {
	unsigned int i, j;
	std::ofstream out("regression.txt");
	if (out.is_open()) {
		cout << "Start to write predicted emotions!\n";
		for (i = 0; i < emotionPredict.size(); i++) {
			out << emotionPredict[i][0];
			for (j = 1; j < 6; j++) {
				out << "\t" << emotionPredict[i][j];
			}
			out << "\n";
		}
	}
	
	/*
	cout << "Start to write predicted emotions!\n";
	for (i = 0; i < emotionPredict.size(); i++) {
		cout << emotionPredict[i][0];
		for (j = 1; j < 6; j++) {
			cout << "\t" << emotionPredict[i][j];
		}
		cout << "\n";
	}
	*/
}