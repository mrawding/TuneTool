/*Welcome to Tune tool version 0.0.1

Description: Console Aplication that allows users to parse
thermal control text files into vectors. These vectors can
be used to output to another text file so no editing is needed,
and can also perform filtering and different analysis.

Simple Parse -> parses all data to a clean file
Smooth Parse-> filters noise, calculates upper and lower standard deviation in the top half of the step,
				and outputs to a file so that the tests can easily be graphed in excel

There are also other analyses that can be performed to give each test an overall PID rating.

Author: Mike Rawding

Date: Summer 2019

*/


#include "Data.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>


using namespace std;


Data::Data(string filename)
{

	test_name = filename;
}

int Data::getTestNo()
{
	return test_no;
}

Data::Data()
{
	pgain = 0.0;
	igain = 0.0;
	dgain = 0.0;
	cycle_ct = 0;
	test_name = "no name yet";
	test_no = NULL;
}

string Data::getName()
{
	return test_name;
}

float Data::getD()
{
	return dgain;
}
int Data::getCycleSecs()
{
	return cycle_time;
}

void Data::setData(Data* copy)
{
	pidRating = copy->pidRating;
	settle_time = copy->settle_time;
	setpoint = copy->setpoint;
	rise_time = copy->rise_time;
	pgain = copy->pgain;
	igain = copy->igain;
	dgain = copy->dgain;
	cycle_ct = copy->cycle_ct;
	cycle_time = copy->cycle_time;
	overshoot = copy->overshoot;
	undershoot = copy->undershoot;
	ring_ct = copy->ring_ct;
	sampleTime = copy->sampleTime;
	t_glass = copy->t_glass;
	t_intake = copy->t_intake;
	tErr = copy->tErr;
	tPlate = copy->tPlate;
	tSink = copy->tSink;
	tFront = copy->tFront;
	vTec = copy->vTec;
	moving_avg = copy->moving_avg;
	pos_stdv = copy->pos_stdv;
	neg_stdv = copy->neg_stdv;
	min = copy->min;
	test_name = copy->test_name;
	test_no = copy->test_no;
	next = NULL;




}

// console function to print tests
void Data::printAttributes()
{
	cout << "*************************************************************************************" << endl;
	cout << "Cycle Time: " << cycle_time ;
	cout << "		Cycle Count: " << cycle_ct;
	cout << "		P Gain: " << pgain;
	cout << "		I Gain: " << igain;
	cout << "		D Gain: " << dgain << endl;
	cout << "Overshoot:	" << overshoot;
	cout << "		Undershoot:	 " << undershoot;
	cout << "		Rise Time: " << rise_time << " Min" << endl;;
	cout << "Settle Time: " << settle_time << " Min";
	cout << "		Ring Ct: " << ring_ct;
	cout << "		PID Rating: " << pidRating << endl;
	cout << endl;
	cout << "*************************************************************************************" << endl;


}
void Data::getRiseTime()
{
	int i = 0;
	while (i < t_glass.size())
	{
		if (igain == 0) {
			
			break;
			
		}
		else
		{
			if (t_glass[i] > setpoint)
			{
				break;
			}
		}
		i++;
	}
	if (i == 0)
	{
		rise_time = 0;
	}
	else
	{
		rise_time = (i * sampleTime) - soakTime;
	}
}

void Data::getSettlingTime()
{
	int consecutive_min = 0;
	for (int i = 0; i < moving_avg.size(); i++)
	{
		if (moving_avg[i] > neg_stdv[std_start + 1] && moving_avg[i] < pos_stdv[std_start +1])
		{
			consecutive_min += 1 * sampleTime;
		}
		else
		{
			consecutive_min = 0;
		}
		if (consecutive_min > 10)
		{
			settle_time = (i * sampleTime) - soakTime;
			break;
		}
	}
}
void Data::getRelativeOvershoots()
{
	float relative_max = 0;
	float relative_min = setpoint;
	for (int i = std_start; i < t_glass.size(); i++)
	{
		if (t_glass[i] < relative_min)
		{
			relative_min = t_glass[i];
			if (relative_min > neg_stdv[std_start])
			{
				ring_ct++;
			}
		}
	}

	for (int i = 0; i < t_glass.size(); i++)
	{
		if (t_glass[i] > relative_max)
		{
			relative_max = t_glass[i];
			if (relative_max > pos_stdv[0])
			{
				ring_ct++;
			}
		}
	}
	if (igain == 0)
	{
		overshoot = relative_max - pos_stdv[std_start];
	}
	else
	{
		overshoot = relative_max - setpoint;
	}
	undershoot = neg_stdv[std_start] - relative_min;
}
float Data::get_stvd(int start_point)
{
	double sum = 0;
	double avg;
	int count = 0;
	for (int n = start_point - 1; n < moving_avg.size(); n++)
	{
		sum += moving_avg[n];
		count++;
	}
	avg = sum / count;
	double var = 0;
	count = 0;
	for (int i = start_point - 1; i < moving_avg.size(); i++)
	{
		var += (double(moving_avg[i] - float(avg)) * double(moving_avg[i] - float(avg)));
		count++;
	}
	var /= double(count);
	double std = sqrt(var);
	pos_stdv.resize(moving_avg.size());
	neg_stdv.resize(moving_avg.size());

	for (int j = start_point - 1; j < moving_avg.size(); j++)
	{
		pos_stdv[j] = avg + std;
		neg_stdv[j] = avg - std;
	}

	return float(sqrt(var));




}
void Data::write_avgs(string filename, int subsamples)
{

	ofstream output;
	output.open(filename);

	for (int i = 0; i < subsamples; i++)
	{
		for (int j = i * (t_glass.size() / subsamples); j < (i + 1) * (t_glass.size() / subsamples); j++)
		{
			output << j;
			output << "	";
			output << moving_avg[i];
			output << endl;
		}
	}
}
void Data::get_avgs(int samplesize)
{
	int subsamples = t_glass.size() / samplesize;
	for (int i = 0; i < subsamples; i++)
	{

		for (int j = i * (t_glass.size() / subsamples); j < (i + 1) * (t_glass.size() / subsamples); j++)
		{
			float sum = 0;
			if (j > (t_glass.size() - (t_glass.size() / subsamples)))
			{
				int ct = 0;
				for (int z = t_glass.size(); z > j; z--)
				{
					sum += t_glass[z - 1];
					ct++;
				}
				moving_avg.push_back(sum / ct);
			}

			else
			{
				for (int k = 0; k < samplesize; k++)
				{
					sum += t_glass[j + k];

				}
				moving_avg.push_back(sum / (t_glass.size() / subsamples));

			}

		}

	}
}

void Data::setPidRating()
{
	float overShootWt = 1;
	float underShootWt = 1;
	float riseTimeWt = 1;
	float ringCtWeight = 1;
	float setTimeWt = 0.8;
	pidRating = float((ringCtWeight * ring_ct) + (overShootWt * overshoot) + (underShootWt * undershoot) + (riseTimeWt * rise_time) + (settle_time*setTimeWt));
	
}
int Data::get_top_len()
{

	float last_val = moving_avg[(cycle_time / 60) - 1];
	float pres_val = 0;
	float last_slope = 0, slope = 0, slope_factor = 0;
	int end_pos = 0;
	for (int i = t_glass.size(); i >= 0; i--)
	{
		if (i > 5)
		{
			last_slope = slope;
			slope = float(((moving_avg[i - 1] - moving_avg[i - 2]) / 2));
			slope_factor = last_slope / slope;
			if (slope_factor < 0.5 && slope_factor > -0.5 && slope_factor != 0 && slope_factor != -0 && slope > 0)
			{
				if (i < (cycle_time / 60) * 0.63)
				{
					end_pos = i;
					break;
				}
			}
		}
	}
	std_start = end_pos;
	return end_pos;

}



// not important, used for debugging purposes
void Data::getData()
{
	ifstream input;
	input.open(test_name);
	bool data_stream = false;
	while (input)
	{
		string line;
		float value;
		int test;
		string line_edit;
		string edit;


		getline(input, line, '\n');
		size_t find = line.find(':');
		stringstream ss(line);
		getline(ss, line_edit, '=');
		//cout << "--------" << endl;
		//cout << line_edit << endl;
		stringstream st(line_edit);
		getline(st, edit, ',');


		//	getline(ss, line, '=');

		input >> ws;
		//cout << "********" << endl;
	//	cout << line << endl;
		if (line_edit == "cycle_seconds  ")
		{
			ss >> cycle_time;
			ss >> ws;

		}
		if (line_edit == "cycle_count    ")
		{
			ss >> cycle_ct;
			ss >> ws;
		}
		else if (line_edit == "p_gain_start   ")
		{
			ss >> pgain;
			ss >> ws;
		}
		else if (line_edit == "i_gain_start   ")
		{
			ss >> igain;
			ss >> ws;
		}
		else if (line_edit == "d_gain_start   ")
		{
			ss >> dgain;
			ss >> ws;
		}
		else if (edit == "Cycle 1")
		{
			data_stream = true;
			//cout << edit << endl;


		}
		else if (data_stream == true)
		{
			stringstream data(line);
			float data_array[8];
			for (int i = 0; i < 8; i++)
			{
				data >> data_array[i];
			}

			t_glass.push_back(data_array[2]);

		}
		if (!input) {
			break;
		}


	}
	input.close();
	//t_glass.pop_back();

}