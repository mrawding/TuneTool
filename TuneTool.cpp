#include <Windows.h>;
#include <iostream>
#include <string.h>
#include "Analyze.h";
#include "Data.h";
using namespace std;
LRESULT WINAPI WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_DESTROY)
		PostQuitMessage(0);
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

Analyze* prevList;
Analyze* curList;
bool test_loaded = false;
enum mode {
	prompt, simple_parse, smooth_parse, run_analysis, print_test
};

void printStars();

void printCommands()
{
	printStars();
	cout << endl;
	cout << "SIP " << " -> simple parse" << endl;
	cout << endl;
	cout << "SMP " << " -> filter data with moving average" << endl;
	cout << endl;
	cout << "ANYL " << "-> computes analysis on curves" << endl;
	cout << endl;
	cout << "PRNT " << "-> prints ranked list of analyses on curves" << endl;
	cout << endl;
	cout << "CMD " << " -> show commands" << endl;
	cout << endl;
	printStars();
}
void printStars()
{
	cout << "********************************************************************************************" << endl;
	cout << endl;
}
int main()
{

	WCHAR filename[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	printStars();
	cout << "Welcome to Tune Tool! please select a command" << endl;
	cout << endl;
	cout << "SIP " << " -> simple parse" << endl;
	cout << endl;
	cout << "SMP " << " -> filter data with moving average" << endl;
	cout << endl;
	cout << "ANYL " << "-> computes analysis on curves" << endl;
	cout << endl;
	cout << "PRNT " << "-> prints ranked list of analyses on curves" << endl;
	cout << endl;
	cout << "CMD " << " -> show commands" << endl;
	cout << endl;
	printStars();
	cout << endl;
	int cmd = prompt;
	string input;
	while (1)
	{
		switch (cmd) {
		case prompt:
		{
			
			cout << "Please enter a command: ";
			cin >> input;
			cout << endl;
			if (input == "SIP")
			{
				cmd = simple_parse;
			}
			else if (input == "SMP")
			{
				cmd = smooth_parse;
			}
			else if (input == "ANYL")
			{
				cmd = run_analysis;

			}
			else if (input == "PRNT")
			{
				cmd = print_test;
			}
			else if (input == "CMD")
			{
				printCommands();
				cmd = prompt;

			}
			else {
				cout << "Please enter a valid command!, type 'CMD' to view commands";
				cmd = prompt;
			}

			break;

		case simple_parse:
			if (1)
			{
				printStars();
				cout << "Please select input data file. " << endl;
				cout << endl;
				GetOpenFileName(&ofn);
				cout << "Parsing..." << endl;
				prevList = curList;
				Analyze* test_list = new Analyze();
				curList = test_list;
				curList->getTests(ofn.lpstrFile);
				cout << "Please select output data file " << endl;
				cout << endl;
				OPENFILENAME tmp = ofn;
				GetSaveFileName(&ofn);
				cout << "Outputing data to file " << endl;
				cout << endl;
				test_list->output_data(ofn.lpstrFile);
				cout << "Finished!, please specify different output file to complete other parsing functions." << endl << endl << "Data will be overwritten " << endl;
				cout << endl;
				printStars();
				test_loaded = true;

			}
			cmd = prompt;
			break;

		case smooth_parse:
			if (1)
			{
				printStars();
				cout << endl;
				cout << "Enter int value for moving_avg interval length " << endl << "3 is recommended for best results" << endl << "length: ";
				int len;
				cin >> len;
				cout << endl;
				cout << "Please select input data file. " << endl;
				cout << endl;
				GetOpenFileName(&ofn);
				cout << "Filtering..." << endl;
				prevList = curList;
				Analyze* test_list1 = new Analyze();
				curList = test_list1;
				curList->getTests(ofn.lpstrFile);
				cout << "Please select output data file " << endl;
				cout << endl;
				OPENFILENAME tmp = ofn;
				GetSaveFileName(&ofn);
		
				cout << "outputing data to file..." << endl;
				cout << endl;
				curList->performAnalysis(len);
				curList->output_avgs(ofn.lpstrFile, len); 
				cout << "Finished!, please specify different output file to complete other parsing functions." << endl << endl << "Data will be overwritten " << endl;
				cout << endl;
				test_loaded = true;
				cmd = prompt;
			}
			break;
		case run_analysis:
			if (test_loaded)
			{
				printStars();
				cout << endl;
				cout << "set moving_avg interval length: ";
				int len;
				cin >>  len;
				cout << "Analyzing..." << endl;
				cout << endl;
				curList->performAnalysis(len);
				cout << "Analysis done! Run print command to see results" << endl;
				cout << endl;
				printStars();
				cout << endl;
				cmd = prompt;
			}
			else
			{
				printStars();
				cout << endl;
				cout << "No tests loaded, please run 'SIP' or 'SMP' commands first" << endl;
				cmd = prompt;
				printStars();
				cout << endl;
			}
			break;
		case print_test:
			if (test_loaded)
			{
				printStars();
				cout << endl;
				cout << "Please enter a print command" << endl;
				cout << endl;
				cout << "-b -> prints from best PID rating to worst" << endl;
				cout << endl;
				cout << "-t -> prints in chronological order" << endl;
				cout << endl;
				cout << "-rt -> prints from shortest to longest rise times" << endl;
				cout << endl;
				cout << "-st -> prints from shortest to longest settling times" << endl;
				cout << endl;
				cout << "-rng -> prints from lowest to highest ring count" << endl;
				cout << endl;
				cout << "-ovr -> prints from largest to smallest overshoot" << endl;
				string print;
				cin >> print;
				if (print == "-b")
				{
					cout << curList->getTestCt() << " tests detected in this run" << endl;
					cout << "enter # of tests you wish to display: " << endl;
					int ct;
					cin >> ct;
					Analyze* tmp = curList->copyList();
					tmp->sort_best()->chronPrint(ct);
					cmd = prompt;
				}
				else if (print == "-t")
				{

					cout << curList->getTestCt() << " tests detected in this run" << endl;
					cout << "enter # of tests you wish to see" << endl;
					int ct;
					cin >> ct;
					cout << endl;
					curList->chronPrint(ct);
					cmd = prompt;

				}
				else if (print == "-rt")
				{
					cout << curList->getTestCt() << " tests detected in this run" << endl;
					cout << "enter # of tests you wish to see" << endl;
					int ct;
					cin >> ct;
					cout << endl;
					Analyze* tmp = curList->copyList();
					tmp->sort_rise()->chronPrint(ct);
					cmd = prompt;

				}
				else if (print == "-st")
				{
					cout << curList->getTestCt() << " tests detected in this run" << endl;
					cout << "enter # of tests you wish to see" << endl;
					int ct;
					cin >> ct;
					cout << endl;
					Analyze* tmp = curList->copyList();
					tmp->sort_settle()->chronPrint(ct);
					cmd = prompt;
				}
				else if (print == "-rng")
				{
					cout << "enter # of tests you wish to see" << endl;
					int ct;
					cin >> ct;
					cout << endl;
					Analyze* tmp = curList->copyList();
					tmp->sort_ringCt()->chronPrint(ct);
					cmd = prompt;

				}
				else if (print == "-ovr")
				{
					cout << "enter # of tests you wish to see" << endl;
					int ct;
					cin >> ct;
					cout << endl;
					Analyze* tmp = curList->copyList();
					tmp->sort_overshoot()->chronPrint(ct);
					cmd = prompt;

				}
				else {
					printStars();
					cout << endl;
					cout << "invalid print command!" << endl;
					cout << endl;
					cmd = print_test;
				}
			}
			else
			{
				cout << "No tests loaded, please run 'SIP' or 'SMP' commands first" << endl;
				cout << endl;
				printStars();
				cmd = prompt;
			}
			break;


		}
		}
	}
}



