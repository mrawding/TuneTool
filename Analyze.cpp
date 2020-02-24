#include "Analyze.h"
#include "Data.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

// constructor for singly linked list
Analyze::Analyze()
{
	first = NULL;
	last = NULL;
	test_count = 0;
}

// prints List in order
void Analyze::chronPrint(int count)
{
	Data* tmp = first;
	int i = 0;
	while (tmp != NULL && i < count)
	{
		tmp->printAttributes();
		tmp = tmp->next;
		i++;
	}
}

// returns test count	
int Analyze::getTestCt()
{
	return test_count;
}
// inserts first test into list
void Analyze::insertFirst(Data* test)
{
	test_count += 1;
	Data* tmp = test;
	tmp->test_name = "Test " + to_string(test_count);
	first = tmp;
	last = first;
	tmp->test_no = 1;

}

// pushes new test on to end of Analyze list
void Analyze::push(Data* test)
{

	Data* tmp = test;
	test_count++;
	tmp->test_name = "Test " + to_string(test_count);
	last->next = tmp;
	last = tmp;
	tmp->test_no = test_count;

}

// seperate inserFirst for list copying
void Analyze::pushFirst(Data* test)
{
	Data* tmp = new Data;
	test_count += 1;
    tmp = test;
	first = tmp;
	last = first;

}
// serperate push for list copying
void Analyze::easyPush(Data* test)
{

	Data* tmp = new Data();
	tmp = test;
	test_count++;
	last->next = tmp;
	last = tmp;
	last->next = NULL;
	tmp->test_no = test_count;

}

// writes averages and standard deviation to output file
void Analyze::output_avgs(const wchar_t* filename, int subsamples)
{
	Data* tmp = first;
	ofstream output;
	output.open(filename);
	int time = 0;
	output << "Min	Temp(run-avg)	stdv high	stdv low" << endl;
	while (tmp != NULL)
	{
		//	tmp->get_avgs(subsamples);
		//	tmp->get_stvd(tmp->get_top_len());
			//tmp->write_avgs(output, subsamples);

		for (int i = 0; i < subsamples; i++)
		{
			for (int j = i * (tmp->t_glass.size() / subsamples); j < (i + 1) * (tmp->t_glass.size() / subsamples); j++)
			{
				output << tmp->min[j];
				time += 1;
				output << "	";
				output << tmp->moving_avg[j];
				output << "	";
				if (tmp->pos_stdv[j] == 0 || tmp->neg_stdv[j] == 0) {
					output << endl;

				}
				else
				{
					output << tmp->pos_stdv[j];
					output << "	";
					output << tmp->neg_stdv[j];
					output << endl;
				}
			}
		}
		tmp = tmp->next;
	}
}
// outputs data to a text file
void Analyze::output_data(const wchar_t* filename)
{
	Data* tmp = first;
	ofstream output;
	output.open(filename);
	output << "Min	";
	output << "tIntake	";
	output << "tGlass	";
	output << "tERR	";
	output << "tPlate	";
	output << "tSink	";
	output << "tFront	";
	output << "vTEC" << endl;
	int ct = 0;
	while (tmp != NULL)
	{
		//tmp->write_avgs(output, subsamples);

		for (int j = 0; j < tmp->t_glass.size(); j++)
		{
			output << ct;
			ct++;
			output << "	";
			output << tmp->t_intake[j];
			output << "	";
			output << tmp->t_glass[j];
			output << "	";
			output << tmp->tErr[j];
			output << "	";
			output << tmp->tPlate[j];
			output << "	";
			output << tmp->tSink[j];
			output << "	";
			output << tmp->tFront[j];
			output << "	";
			output << tmp->vTec[j];
			output << endl;
		}

		tmp = tmp->next;
	}
}
void Analyze::pop() // deletes last element
{
	Data* tmp = first;
	while (tmp->next != last)
	{
		tmp = tmp->next;

	}
	delete last;
	last = tmp;
	last->next = NULL;
	test_count--;
}

//retrieves first element of list
Data* Analyze::getFirst()
{
	return first;
}

// retrieves last element of list
Data* Analyze::getLast()
{
	return last;
}

// returns a specific test
Data* Analyze::getTest(int test_no)
{
	int t = 1;
	Data* tmp = first;
	while (t < test_no)
	{
		t++;
		tmp = tmp->next;

	}
	return tmp;
}

// destructor
Analyze::~Analyze()
{
	Data* current = first;
	while (current != NULL)
	{
		Data* tmp = current->next;
		delete current;
		current = tmp;
	}
}
// runs different calculations and sets analytic measurements on each tests
void Analyze::performAnalysis(int interval_len)
{
	Data* tmp = first;
	while (tmp != NULL)
	{
		tmp->soakTime = (tmp->cycle_time) / 60 / 2;
		tmp->sampleTime = tmp->min[1] - tmp->min[0];
		tmp->get_avgs(interval_len);
		tmp->get_stvd(tmp->get_top_len());
		tmp->getRelativeOvershoots();
		tmp->getRiseTime();
		tmp->getSettlingTime();
		tmp->setPidRating();
		tmp = tmp->next;


	}
}


// Parses input text file
void Analyze::getTests(const wchar_t* filename)
{
	ifstream input;
	input.open(filename);
	bool data_stream = false;
	bool data_flag = false;
	Data* tmp = new Data();
	insertFirst(tmp);
	int test_ct = 0;
	int cycle_count = 0;
	float setpoint_temp;
	int cycle_t = 0;
	int ct = 0;
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
		stringstream st(line_edit);
		getline(st, edit, ',');
		stringstream sd(edit);
		string edit_;
		getline(sd, edit_, ' ');
		input >> ws;

		if (line_edit == "cycle_seconds  ")
		{

			ss >> cycle_t;
			ss >> ws;

		}

		if (line_edit == "setpoint_temp             ")
		{
			ss >> setpoint_temp;
			ss >> ws;
			tmp->setpoint = setpoint_temp;
		}
		if (line_edit == "cycle_count    ")
		{
			ss >> cycle_count;
			ss >> ws;
		}
		else if (line_edit == "p_gain                    ")
		{

			ss >> tmp->pgain;
			tmp->cycle_time = cycle_t;
			tmp->cycle_ct = cycle_count;
			ss >> ws;


		}
		else if (line_edit == "i_gain                    ")
		{
			ss >> tmp->igain;
			ss >> ws;
		}
		else if (line_edit == "d_gain                    ")
		{
			ss >> tmp->dgain;
			ss >> ws;

		}
		else if (edit_ == "heatsink_max_temp" && data_flag)
		{
			data_stream = true;
		}
		else if (edit_ == "Exiting")
		{
			break;
		}
		else if (edit_ == "Cycle")
		{
			ct++;
			if (ct % 2 != 0 && ct > 1)
			{
				tmp = new Data();
				push(tmp);
				tmp->cycle_ct = cycle_count;
				tmp->cycle_time = cycle_t;

				data_stream = true;

			}
			else if (ct % 2 == 0)
			{
				data_stream = false;
			}
			else {
				data_stream = true;
			}


		}
		else if (data_stream == true)
		{
			data_flag = true;
			stringstream data(line);
			float data_array[8];
			for (int i = 0; i < 8; i++)
			{
				data >> data_array[i];
			}
			tmp->min.push_back(data_array[0]);
			tmp->t_intake.push_back(data_array[1]);
			tmp->t_glass.push_back(data_array[2]);
			tmp->tErr.push_back(data_array[3]);
			tmp->tPlate.push_back(data_array[4]);
			tmp->tSink.push_back(data_array[5]);
			tmp->tFront.push_back(data_array[6]);
			tmp->vTec.push_back(data_array[7]);


		}

		if (!input) {
			break;
		}


	}
	input.close();
}

// sorts from lowest to highest PID rating, lowest = GOOD, best = BAD, PID rating variable could use some looking into for correct calculation
Analyze* Analyze::sort_best()
{
	Analyze* sorted = new Analyze();
	Data* curr = first;
	while (curr != NULL)
	{
		Data* next1 = curr->next;
		if (sorted->first == NULL || sorted->first->pidRating >= curr->pidRating)
		{
				curr->next = sorted->first;
				sorted->first = curr;
			
		}
		else
		{
			Data *tmp = sorted->first;
			while (tmp->next != NULL && tmp->next->pidRating < curr->pidRating)
			{
				tmp = tmp->next;
			}
			curr->next = tmp->next;
			tmp->next = curr;
		}
		curr = next1;

	}

	return sorted;

}


Analyze* Analyze::sort_overshoot()
{
	Analyze* sorted = new Analyze();
	Data* curr = first;
	while (curr != NULL)
	{
		Data* next1 = curr->next;
		if (sorted->first == NULL || sorted->first->overshoot >= curr->overshoot)
		{
			curr->next = sorted->first;
			sorted->first = curr;
		}
		else
		{
			Data* tmp = sorted->first;
			while (tmp->next != NULL && tmp->next->overshoot< curr->overshoot)
			{
				tmp = tmp->next;
			}
			curr->next = tmp->next;
			tmp->next = curr;
		}
		curr = next1;

	}
	return sorted;

}


Analyze* Analyze::sort_ringCt()
{
	Analyze* sorted = new Analyze();
	Data* curr = first;
	while (curr != NULL)
	{
		Data* next1 = curr->next;
		if (sorted->first == NULL || sorted->first->ring_ct >= curr->ring_ct)
		{
			curr->next = sorted->first;
			sorted->first = curr;
		}
		else
		{
			Data* tmp = sorted->first;
			while (tmp->next != NULL && tmp->next->ring_ct< curr->ring_ct)
			{
				tmp = tmp->next;
			}
			curr->next = tmp->next;
			tmp->next = curr;
		}
		curr = next1;

	}
	return sorted;

}
Analyze* Analyze::sort_rise()
{
	Analyze* sorted = new Analyze();
	Data* curr = first;
	while (curr != NULL)
	{
		Data* next1 = curr->next;
		if (sorted->first == NULL || sorted->first->rise_time >= curr->rise_time)
		{
			curr->next = sorted->first;
			sorted->first = curr;
		}
		else
		{
			Data* tmp = sorted->first;
			while (tmp->next != NULL && tmp->next->rise_time < curr->rise_time)
			{
				tmp = tmp->next;
			}
			//curr->next = tmp->next;
			tmp->next = curr;
		}
		curr = next1;
		sorted->last = curr;
	}
	return sorted;

}

Analyze* Analyze::sort_settle()
{
	Analyze* sorted = new Analyze();
	Data* curr = first;
	while (curr != NULL)
	{
		Data* next1 = curr->next;
		if (sorted->first == NULL || sorted->first->settle_time >= curr->settle_time)
		{
			curr->next = sorted->first;
			sorted->first = curr;
		}
		else
		{
			Data* tmp = sorted->first;
			while (tmp->next != NULL && tmp->next->settle_time < curr->settle_time)
			{
				tmp = tmp->next;
			}
			curr->next = tmp->next;
			tmp->next = curr;
		}
		curr = next1;

	}
	return sorted;

}

Analyze* Analyze::copyList()
{
	Data* tmp = first;
	Analyze* copy = new Analyze();
	while (tmp != NULL)
	{
		Data* tmp2 = new Data();
		tmp2->setData(tmp);
		if (copy->first == NULL)
		{
			copy->pushFirst(tmp2);

		}
		else
		{
			copy->easyPush(tmp2);
		}
		tmp = tmp->next;
	}

	return copy;

}