#pragma once
#include <iostream>
#include <cstdio>
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

string num2emotion[6] = { "anger","disgust","fear","joy","sad","surprise" };
std::vector<unsigned int> labels;//store labels of train set

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

	map <string, unsigned int> emotion2num;
	emotion2num["anger"] = 0;
	emotion2num["disgust"] = 1;
	emotion2num["fear"] = 2;
	emotion2num["joy"] = 3;
	emotion2num["sad"] = 4;
	emotion2num["surprise"] = 5;
	std::vector<std::vector<std::string>>  data;
	std::string sentence;
	//int counter = 0;
	labels.clear();

	ifstream filename("C:\\Users\\mibyb\\Documents\\课程\\大三上\\AI\\lab1\\lab1_data\\lab1_data\\classification_dataset\\train_set.csv");
	//the first line of train set is the header
	//dont want it in data and lables
	//cout << "Ignore the header." << endl;//test
	getline(filename, sentence);
	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		//cout << "counter=" << counter << endl;//test
		if (seperate==',') {
			//cout << "This is a document" << sentence << endl;//test
			std::vector<std::string> wordList = textParse(sentence);
			data.push_back(wordList);
			seperate = '\n';
			
		}
		else {
			//cout << "This is a label" << sentence << endl;//test
			labels.push_back(emotion2num[sentence]);
			seperate = ',';
		}

	}
	/*QueryPerformanceCounter(&t2);
	printf("Load documents.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);*/
	return data;
}

std::vector<std::vector<std::string>> loadValidation() {
	std::vector<std::vector<std::string>>  data;
	std::string sentence;

	ifstream filename("C:\\Users\\mibyb\\Documents\\课程\\大三上\\AI\\lab1\\lab1_data\\lab1_data\\classification_dataset\\validation_set.csv");
	//the first line of train set is the header
	//dont want it in data and labels
	getline(filename, sentence);
	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		//cout << "counter=" << counter << endl;//test
		if (seperate == ',') {
			//cout << "This is a document" << sentence << endl;//test
			std::vector<std::string> wordList = textParse(sentence);
			data.push_back(wordList);
			seperate = '\n';

		}
		else {
			seperate = ',';
		}

	}
	return data;
}

std::vector<std::vector<std::string>> loadTest() {
	std::vector<std::vector<std::string>>  data;
	std::string sentence;
	int counter = 0;

	ifstream filename("C:\\Users\\mibyb\\Documents\\课程\\大三上\\AI\\lab1\\lab1_data\\lab1_data\\classification_dataset\\test_set.csv");
	//the first line of train set is the header
	//dont want it in data and lables

	getline(filename, sentence);

	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		//cout << "counter=" << counter << endl;//test
		if (counter%3==1) {
			//cout << "This is a document" << sentence << endl;//test
			std::vector<std::string> wordList = textParse(sentence);
			data.push_back(wordList);
			seperate = '\n';
			counter++;

		}
		else if (counter % 3 == 0) {
			counter++;
		}
		else{
			seperate = ',';
			counter++;
		}

	}

	return data;
}

void writeResult(vector<unsigned int> emotionnum) {
	unsigned int i, j;
	std::ofstream out("classification.txt");
	if (out.is_open()) {
		cout << "Start to write predicted emotions!\n";
		for (i = 0; i < emotionnum.size(); i++) {
			out << num2emotion[emotionnum[i]] << "\n";
			//out << emotionnum[i] << "\n";
		}
	}
}