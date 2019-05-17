//#include <iostream>
//#include <cstdio>
//#include <fstream>
//#include <vector>
//#include <sstream>
//#include <omp.h>
//#include <Eigen\Dense>

#include "tfidf.h"
//#include "file_related.h"
#include "knn.h"

using namespace std;
using namespace Eigen;


int main() {

	std::vector<std::vector<std::string>> trainData = loadTrain();
	tfidf train(trainData);
	train.createVocabList();


	/*TEST
	train.writeDataNoZero();
	for (int i=0; i<labels.size(); i++)
	cout << labels[i] << endl;
	*/

	int r, k;

	cout << "Please select distance parameter r and knn parameter k.\n";
	cin >> r;
	cin >> k;

	std::vector<std::vector<std::string>> testData = loadTest();
	tfidf test(testData);
	test.createVocabList();
	test.calMat();
	train.calMat();
	knn(r, k, labels, train.weightMat, test.weightMat);//test
													   //writeResult(knn(r, k, labels, train.weightMat, test.weightMat));

	system("pause");
	return 0;
}


/*
int main() {

Eigen::Array3d a, b;

a << 1.0, 2.0, 3.0;
b << 2.0, 2.0, 1.0;
cout << "test" << endl;
cout << "distance="<<CalDis(2, a, b) << endl;
system("pause");
return 0;
}
*/