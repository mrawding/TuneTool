#pragma once
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;
class Data
{
	friend class Analyze;
	int cycle_ct;
	int cycle_time;
	string test_name;
	float setpoint;
	float soakTime;
	float settle_time;
	float sampleTime;
	int std_start;
	int test_no;
	float dgain;
	float pgain;
	float igain;
	float overshoot;
	float undershoot;
	int ring_ct;
	int rise_time;
	float pidRating;
	vector <float> min;
	vector <float> t_glass;
	vector <float> t_intake;
	vector <float> tErr;
	vector <float> tPlate;
	vector <float> tSink;
	vector <float> tFront;
	vector <float> vTec;
	vector <float> moving_avg;
	vector <float> pos_stdv;
	vector <float> neg_stdv;


public:
	// "Data" object type represents 1 cycle of a test run
	// constructor from a file, Only used for debugging;
	Data(string filename);
	// constructor, useful
	Data();

	void setData(Data* copy);
	// pointer to next cycle in a test run
	Data* next;
	// returns the name of the test, i.e "Test No: 3"
	string getName();
	//writes running average of interval: "subsamples" to a text file. Only used for debugging.
	void write_avgs(string filename, int subsamples);
	//returns the starting point for the top part of the step response.
	int get_top_len();
	// returns the test #
	int getTestNo();

	
	// iterates over top step and computes standard deviation for that top interval
	float get_stvd(int start_point);
	// Counts the number of ringing that occurs, " > or < stdv." and writes into the overshoot, and undershoot fields of each test
	void getRelativeOvershoots();
	// Amount of time taken for it to settle, used 10 minutes for the required length of time to be detrmined as "settled"
	void getSettlingTime();
	// Returns time taken to reach the setpoint or upper limit for stdv in P only tests
	void getRiseTime();
	void getData();
	// smoothes data buy doing a running average
	void get_avgs(int subsamples);
	// returns D gain
	float getD();
	// returns Cycle seconds
	int getCycleSecs();
	// prints attributes to each test in console window
	void printAttributes();
	void setPidRating();


};

