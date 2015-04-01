#ifndef TASKLIST_H_
#define TASKLIST_H_

#include <stdio.h>
#include <vector>
#include <string>
#include "Task.h"
#include "Storage.h"
#include "DisplayedTaskList.h"
#include "InterfaceOutput.h"
using namespace std;

class TaskList
{
private:
	static vector<Task> list;
	static vector<string> taskGroup;
	static vector<string> taskPlace;
	static string lastCommandType;
	static int lastChangedTaskIndex;
	static Task lastUnchangedTask;
	static Task lastChangedTask;

	static string getFirstWord(string input);
	static string removeFirstWord(string input);
	static vector<string> splitText(string text);
	static string printVector(vector<string> output);
	static int  findTargetedTaskIndex(int displayedIndex);
	static void addToDisplayedTaskList(string displayType);
	static void addTaskGroup(Task newtask);
	static void addPlace(Task newtask);
	static bool isExist(vector<string> checkingList, string input);
public:
	static void copyFromStorage();
	static void copyToStorage();

	static string addTask(string input);
	static string updateTask(string input);
	static string deleteTask(string input);
	static string search(string input);
	static string display(string displayType);
	static string markAsDone(string input);
	static string setPriority(string input);
	static void empty();
	
	static string undo();
	static string redo();
};

#endif