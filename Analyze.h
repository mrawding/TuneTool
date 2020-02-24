#pragma once
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "Data.h"
using namespace std;
class Analyze

{
	Data* last;
	Data* first;
	int test_count;


public:
	Analyze();
	~Analyze();
	Data* getFirst();
	Data* getLast();
	Data* getTest(int test_no);
	Analyze* copyList();
	Analyze* sort_best();
	Analyze* sort_overshoot();
	Analyze* sort_ringCt();
	Analyze* sort_rise();
	Analyze* sort_settle();
	void push(Data* test);
	void easyPush(Data* test);
	void output_data(const wchar_t* filename);
	void pop();
	void pushFirst(Data *test);
	void insertFirst(Data* test);
	void performAnalysis(int interval_len);
	void output_avgs(const wchar_t* filename, int subsamples);
	void getTests(const wchar_t* filename);
	void chronPrint(int count);
	int getTestCt();

	


	//Data return_best();
	//void graph(Data tester);


};


