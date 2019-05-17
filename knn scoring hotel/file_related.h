#pragma once
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

std::vector<double> labels;//store labels of train set
						   //train set 要用的容器
						   //这四个属性用来计算距离
std::vector<double> Additional_Number_of_Scoring;//the value of attribute "Additional_Number_of_Scoring"
std::vector<double> Average_Score;//the value of attribute "Average_Score"
std::vector<double> Total_Negative;//the value of attribute "Review_Total_Negative_Word_Counts"
std::vector<double> Total_Positive;//the value of attribute "Review_Total_Positive_Word_Counts"

								   //这两个属性用来调参
std::vector<double> Total_Reviews;//the value of attribute "Total_Number_of_Reviews"
std::vector<double> Total_Reviews_Given;//the value of attribute "Total_Number_of_Reviews_Reviewer_Has_Given"

										//test set要用的容器
std::vector<double> Additional_Number_of_Scoring_Test;//the value of attribute "Additional_Number_of_Scoring"
std::vector<double> Average_Score_Test;//the value of attribute "Average_Score"
std::vector<double> Total_Negative_Test;//the value of attribute "Review_Total_Negative_Word_Counts"
std::vector<double> Total_Positive_Test;//the value of attribute "Review_Total_Positive_Word_Counts"
										//这两个属性用来调参
std::vector<double> Total_Reviews_Test;//the value of attribute "Total_Number_of_Reviews"
std::vector<double> Total_Reviews_Given_Test;//the value of attribute "Total_Number_of_Reviews_Reviewer_Has_Given"


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
	std::vector<std::vector<std::string>>  data;
	std::string sentence;
	std::vector<std::string> wordList;
	int counter = 0;
	labels.clear();

	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	ifstream filename("C:\\Users\\mibyb\\Desktop\\train_sample.csv");
	//the first line of train set is the header
	//dont want it in data and lables
	//cout << "Ignore the header." << endl;//test

	//我已经手动把表头去掉了，所以不t需要事先读一行了
	//getline(filename, sentence);

	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		//cout << sentence << endl;//test
		switch (counter % 8) {
		case 0:
			//cout << "Additional_Number_of_Scoring=" << sentence << endl;//test
			Additional_Number_of_Scoring.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 1:
			//cout << "Average_Score=" << atof(sentence.c_str()) << endl;//test
			Average_Score.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 2:
			//cout << "Total_Negative=" << atof(sentence.c_str()) << endl;//test
			Total_Negative.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 3:
			//cout << "Total_Reviews=" << atof(sentence.c_str()) << endl;//test
			Total_Reviews.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 4:
			//cout << "Total_Positive=" << atof(sentence.c_str()) << endl;//test
			Total_Positive.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 5:
			//cout << "Total_Reviews_Given=" << atof(sentence.c_str()) << endl;//test
			Total_Reviews_Given.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 6:
			//cout << "This is a document: " << sentence << endl;//test
			wordList.clear();
			wordList = textParse(sentence);
			data.push_back(wordList);
			counter++;
			seperate = '\n';
			break;
		case 7:
			//cout << "labels=" << atof(sentence.c_str()) << endl;//test
			labels.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		default:
			//cout << "default" << endl;//test
			break;
		}
	}
	QueryPerformanceCounter(&t2);
	printf("Load documents.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);
	cout << "The number of train samples: " << counter / 8 << endl;
	return data;
}

std::vector<std::vector<std::string>> loadTest() {
	std::vector<std::vector<std::string>>  data;
	std::string sentence;
	std::vector<std::string> wordList;

	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	ifstream filename("C:\\Users\\mibyb\\Desktop\\validation_sample.csv");
	//the first line of train set is the header
	//dont want it in data and labels

	//我已经手动把表头去掉了，所以不需要事先读一行了
	//getline(filename, sentence);
	int counter = 0;
	char seperate = ',';
	while (getline(filename, sentence, seperate)) {
		switch (counter % 7) {
		case 0:
			//cout << "Additional_Number_of_Scoring=" << sentence << endl;//test
			Additional_Number_of_Scoring_Test.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 1:
			//cout << "Average_Score=" << atof(sentence.c_str()) << endl;//test
			Average_Score_Test.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 2:
			//cout << "Total_Negative=" << atof(sentence.c_str()) << endl;//test
			Total_Negative_Test.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 3:
			//cout << "Total_Reviews=" << atof(sentence.c_str()) << endl;//test
			Total_Reviews_Test.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 4:
			//cout << "Total_Positive=" << atof(sentence.c_str()) << endl;//test
			Total_Positive_Test.push_back(atof(sentence.c_str()));
			counter++;
			seperate = ',';
			break;
		case 5:
			//cout << "Total_Reviews_Given=" << atof(sentence.c_str()) << endl;//test
			Total_Reviews_Given_Test.push_back(atof(sentence.c_str()));
			counter++;
			seperate = '\n';
			break;
		case 6:
			//cout << "This is a document: " << sentence << endl;//test
			wordList.clear();
			wordList = textParse(sentence);
			data.push_back(wordList);
			counter++;
			seperate = ',';
			break;
		default:
			//cout << "default" << endl;//test
			break;
		}
	}
	QueryPerformanceCounter(&t2);
	printf("Load documents.\nTime Esclaped:%f\n\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);
	cout << "The number of test samples: " << counter / 7 << endl;
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