#include <iostream>
#include <string>
using namespace std;

class Task
{
public:
	//vector<string> task;
	Task(string input);
	string ToString(int time);
	string getTaskname();
	string UpdateTask(string input);
	void markAsDone();
	void markAsUndone();
	string taskname;
	string task_type;
	string start_time;
	string end_time;
	string deadline_time;
	string scheduled_date;
	string deadline_date;
	string status;
};