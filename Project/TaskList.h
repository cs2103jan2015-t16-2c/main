#ifndef TASKLIST_H_
#define TASKLIST_H_

#include <stdio.h>
#include <vector>
#include <string>
#include "Task.h"
#include "Storage.h"
<<<<<<< HEAD
#include "DisplayedTaskList.h"
=======
#include "InterfaceOutput.h"
>>>>>>> 3d743c2abf64de0f8097ab8421c03976bba36abd
using namespace std;

class TaskList
{
private:
	static vector<Task> list;
	static string lastCommandType;
	static int lastChangedTaskIndex;
	static Task lastUnchangedTask;
	static Task lastChangedTask;

	static string getFirstWord(string input);
	static string removeFirstWord(string input);
	static vector<string> splitText(string text);
	static string printVector(vector<string> output);
	static int  findTargetedTaskIndex(int displayedIndex);
public:
	static void copyFromStorage();
	static void copyToStorage();

	static string addTask(string input);
	static string updateTask(string input);
	static string deleteTask(string input);
	static string search(string input);
	static string display();
	static string markAsDone(string input);
	
	static string undo();
	static string redo();
};

#endif