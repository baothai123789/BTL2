#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include "restaurant.cpp"

using namespace std;




void comparefile(int start, int end)
{
	string folder_solution_you = "/Users/thaibao/CLionProjects/DSA/test/solution_you/output";
	string folder_solution = "/Users/thaibao/CLionProjects/DSA/test/solution/output";
	vector<int> result;
	for (int i = start; i <= end; i++)
	{
		string file_solution_you = folder_solution_you + to_string(i) + ".txt";
		string file_solution = folder_solution + to_string(i) + ".txt";

		ifstream read_solution_you(file_solution_you);
		ifstream read_solution(file_solution);
		string s1, s2;
		while (read_solution_you >> s1 && read_solution >> s2)
		{
			if (s1 != s2)
			{
				result.push_back(i);
				break;
			}
		}
		if (read_solution_you >> s1 || read_solution >> s2)
		{
			if (result.size() == 0 || result.size() > 0 && result[result.size() - 1] != i)
				result.push_back(i);
		}
	}

	if (result.size() == 0)
	{
		cout << "Success: test " << start << " to " << end << endl;
	}
	else
	{
		cout << "percent success : " << (1 - result.size() * 1.0 / (end - start + 1)) * 100 << "%" << endl;
		cout << "Fail : test [";
		for (int i = 0; i < result.size() - 1; i++)
		{
			cout << result[i] << ", ";
		}
		cout << result[result.size() - 1] << "]\n";
	}
}

int main(int argc, char *argv[])
{
	clock_t start, end;
	start = clock();
	cout << "Start program" << endl;
	string folder_input = "/Users/thaibao/CLionProjects/DSA/test/input/input";
	string folder_solution_you = "/Users/thaibao/CLionProjects/DSA/test/solution_you/output";
	string folder_solution = "/Users/thaibao/CLionProjects/DSA/test/solution/output";

	if (argc < 2)
	{
		cout << "checking : ";
		for (int i = 1; i <= 1000; i++)
		{
			cout << i << " ";
			//open file not using freopen
			streambuf *backup=cout.rdbuf();
			ofstream ofs("/Users/thaibao/CLionProjects/DSA/test/solution_you/output"+to_string(i)+".txt");
			cout.rdbuf(ofs.rdbuf());
			string file_input = folder_input + to_string(i) + ".txt";
			simulate(file_input);
			cout.rdbuf(backup);
			

		}
		cout << "\nOK: runs without errors\n"<< endl;

		comparefile(1, 1000);
	}
	else if (argc == 2)
	{
		string s = argv[1];
		cout << "checking : ";
		int i = stoi(s);
		cout << i << " ";
		streambuf *backup=cout.rdbuf();
		ofstream ofs("/Users/thaibao/CLionProjects/DSA/test/solution_you/output"+to_string(i)+".txt");
		cout.rdbuf(ofs.rdbuf());
		string file_input = folder_input + to_string(i) + ".txt";
		simulate(file_input);
		cout.rdbuf(backup);
		
		cout << "\nOK: runs without errors\n" << endl;
		comparefile(stoi(s), stoi(s));
	}
	else if (argc == 3)
	{
		cout << "checking : ";
		string s1 = argv[1], s2 = argv[2];
		int start = stoi(s1);
		int end = stoi(s2);
		for (int i = start; i <= end; i++)
		{
			cout << i << " ";
			streambuf *backup=cout.rdbuf();
			ofstream ofs("/Users/thaibao/CLionProjects/DSA/test/solution_you/output"+to_string(i)+".txt");
			cout.rdbuf(ofs.rdbuf());
			string file_input = folder_input + to_string(i) + ".txt";
			simulate(file_input);
			cout.rdbuf(backup);
		}
		cout << "\nOK: runs without errors\n" << endl;
		comparefile(start, end);
	}
	cout<<"Time run: "<<(clock() - start) / (double)(CLOCKS_PER_SEC / 1000)<<" ms"<<endl;
}