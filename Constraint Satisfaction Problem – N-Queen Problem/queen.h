#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <cstring>
#include <string>
#include <cstdlib>

using namespace std;


class Game {
private:
	//the size of board
	int size;
	//whether a variable has been assigned
	bool* assigned;
	//valid values of each variable
	//Each variable in N-queen has same domain, so only needs one array
	//seems unnecessary in this problem actually
	//used in backtracking algorithm
	int* domain;
	//current domain of each variable
	//used in forward checking
	vector<vector<int>> curDomains;
	//assigned value of each variable
	int* values;
	//the ORDER in which variables are assigned
	vector<int> variables;
	set<vector<int>> solutions;
	
public:
	Game(int s):size(8){
		this->size = s;
		assigned = (bool*)malloc(sizeof(bool)*s);
		for (int i = 0; i < s; i++) { assigned[i] = false; }

		domain = (int*)malloc(sizeof(int)*s);
		for (int i = 0; i < s; i++) { domain[i] = i; }
		
		for (int i=0; i<s; i++){
			vector<int> curDomain;
			curDomain.clear();
			for (int j=0; j<s; j++){curDomain.push_back(j);}
			curDomains.push_back(curDomain);
		}
		
		values = (int*)malloc(sizeof(int)*s);
		
		variables.clear();
	}
	
	~Game(){
		delete(assigned);
		delete(domain);
		delete(values);
	}
	
	int PickUnassignedVariable(){
		for (int i = 0; i < this->size; i++) {
			if (assigned[i] == false) return i;
		}
		//test
		//cout << "All variables have already been assigned!" << endl;
		return -1;
	}
	
	bool allAssigned() {
		for (int i = 0; i < size; i++) {
			if (assigned[i] == false) return false;
		}
		return true;
	}
	
	void printSolutions() {
		cout<<"The search has found "<<solutions.size()<<" solution(s)."<<endl;
		set<vector<int>>::iterator it;
		for (it=solutions.begin(); it!=solutions.end(); it++){
			for(int i=0; i<this->size; i++){
				cout<<" "<<(*it)[i];
			}
			cout<<endl;
		}
	}
	
	//undo the last assignment
	void Undo(int level) {
		if (variables.empty()) return ;
		/*
		while (variables.size()>level){
			int v=variables[variables.size()-1];
			assigned[v]=false;
			variables.pop_back();
		}
		*/
		//test
		//cout<<"Undo Row#"<<variables[variables.size()-1]<<endl;
		
		assigned[variables[variables.size()-1]]=false;
		variables.pop_back();
	}
	
	void assign(int level, int variable, int value) {
		variables.push_back(variable);
		assigned[variable] = true;
		values[variable] = value;
		return ;
	}
	
	
	bool columnConstraint() {
		int* columns;
		columns = (int*)malloc(sizeof(int)*this->size);
		for (int i = 0; i < this->size; i++) columns[i] = 0;
		//how many queens in each column
		for (int i = 0; i < this->size; i++) {
			if (assigned[i] == true) {
				columns[values[i]]++;
			}
		}
		for (int i = 0; i < this->size; i++) {
			//more than 1 queens in this column
			//violate column constraint
			if (columns[i] > 1) {
				//cout << "columns " <<i<<" "<<columns[i]<< endl;
				//cout << "Violate column constraint!" << endl;
				return false;
			}
		}
		//satisfy constraint
		return true;
	}
	
	bool diagonalConstraint() {
		vector<int> assignedrow;
		vector<int> assignedcol;
		assignedcol.clear();
		assignedcol.clear();
		//TODO - ERROR HERE!
		for (int i = 0; i < this->size; i++) {
			if (assigned[i] == true) { 
				assignedrow.push_back(i);
				assignedcol.push_back(values[i]);
			}
		}
		for (unsigned int i = 0; i < assignedrow.size(); i++) {
			for (unsigned int j = i + 1; j < assignedcol.size(); j++) {
				if (abs(assignedrow[i] - assignedrow[j]) == abs(assignedcol[i] - assignedcol[j])) {
					//cout << "Violate diagonal constraint!" << endl;
					return false;
				}
			}
		}
		return true;
	}
	
	//combine column constraint and diagonal constraint
	bool satisfyConstraint() {
		return (this->columnConstraint() && this->diagonalConstraint());
	}
	
	void refreshCurDomains(){
		//for each curDomain in curDomains
		for (unsigned int variable=0; variable<curDomains.size(); variable++){
			vector<int> refreshedDomain;
			refreshedDomain.clear();
			//assigned variable has empty domain
			if (assigned[variable]==true){
				refreshedDomain.clear();
			}
			//un-assigned variable
			else
				for (unsigned int j=0; j<this->size; j++){
					//try assigning this variable with this value
					int value=this->domain[j];
					variables.push_back(variable);
					assigned[variable]=true;
					values[variable]=value;
					
					if (this->satisfyConstraint()){refreshedDomain.push_back(value);}
					
					//undo this attempt
					assigned[variable]=false;
					variables.pop_back();
				}
			curDomains[variable]=refreshedDomain;
			//test
			//cout<<"Current domain of Row #"<<variable<<endl;
			//for (int k=0; k<refreshedDomain.size();k++) cout<<" "<<refreshedDomain[k];
			//cout<<endl<<endl;
		}
	}
	
	//refresh the current domain of selected variable
	void refreshCurDomain(int variable){
		if (assigned[variable]==true){
			this->curDomains[variable].clear();
		}
		else{
			vector<int> refreshedDomain;
			refreshedDomain.clear();
			
			assigned[variable]=true;
			variables.push_back(variable);
			for(int value=0; value<this->size; value++){
				values[variable]=value;
				if (this->satisfyConstraint()){
					refreshedDomain.push_back(value);
				}
			}
			assigned[variable]=false;
			variables.pop_back();
			
			this->curDomains[variable]=refreshedDomain;
			//test
			//cout<<"Current domain of Row #"<<variable<<endl;
			//for (int k=0; k<refreshedDomain.size();k++) cout<<" "<<refreshedDomain[k];
		}
		//cout<<endl<<endl;
	}
	
	
	bool reachDWO(){
		for (unsigned int variable=0; variable<this->size; variable++){
			if (assigned[variable]==false && curDomains[variable].empty()) return true;
		}
		return false;
	}
	
	
	void Backtracking(int level) {
		if (this->allAssigned()) {
			vector<int>solution;
			for (int i=0; i<this->size; i++){
				solution.push_back(values[i]);
			}
			this->solutions.insert(solution);
		}
		
		else {
			int variable = this->PickUnassignedVariable();
			//test
			//cout << "Assigning the queen on ROW #" << variable << endl;
			int d;
			for (int i = 0; i < this->size; i++) {
				d = this->domain[i];
				//cout << "ROW" << variable<<" = "<< d<< endl;
				//test
				/*
				if (variable==0&&d==2){
					cout << "ROW" << variable<<" = "<< d<< endl;
					for (int i=0; i<this->size; i++){
						cout<<"assigned["<<i<<"] = "<<assigned[i]<<endl;
						cout<<"values["<<i<<"] = "<<values[i]<<endl;
					}
				}
				*/				
				
				this->assign(level, variable, d);

				//violate constraint
				//cancel this assignment and try another value
				if (!this->satisfyConstraint()) {
					//cout<<"Cancel!"<<endl;
					this->Undo(level);
					continue;
				}

				//keep assigning for other variables
				else {
					//cout<<"Deepthen!"<<endl;
					this->Backtracking(level + 1);
				}

			}
		}
		return;
	}
	
	void ForwardChecking(int level) {
		if (this->allAssigned()) {
			vector<int>solution;
			for (int i=0; i<this->size; i++){
				solution.push_back(values[i]);
			}
			this->solutions.insert(solution);
			//return ;
		}
		else{
			int variable = this->PickUnassignedVariable();
			//test
			//cout << "Assigning the queen on ROW #" << variable << endl;

			//test
			//cout<<"Refresh current domains!"<<endl;
			//refreshCurDomains();
			
			//variables.push_back(variable);
			//assigned[variable]=true;
			
			for (int i=0; i<(curDomains[variable]).size(); i++){
				int d=(curDomains[variable]).at(i);
				//values[variable]=d;
				assign(level,variable,d);
				//cout << "ROW" << variable<<" = "<< d<< endl;
				
				for (int v=0; v<this->size; v++){
					if (assigned[v]==false && v!=variable){
						this->refreshCurDomain(v);
					}
				}
				
				//Refresh_1
				//Refresh all current domains
				//refreshCurDomains();
				
				if (reachDWO()||!satisfyConstraint()){
					this->Undo(level);
					//!!!!ERROR SOLVED
					//should NOT use return here!
					//return;				
					
					//Refresh_2
					for (int v=0; v<this->size; v++){
						if (assigned[v]==false && v!=variable){
							this->refreshCurDomain(v);
						}
					}
					
					continue;
				}
				else {
					ForwardChecking(level+1);
				}
								
			}
			return;
		}	
	}
	
	
};//Game class
