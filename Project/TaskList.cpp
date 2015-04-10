#include <sstream>
#include <assert.h>
#include "TaskList.h"

vector<Task> TaskList::list;
vector<string> TaskList::taskGroup;
vector<string> TaskList::taskPlace;
bool TaskList::isLastCommandUndo = false;
string TaskList::lastCommandType;
int TaskList::lastChangedTaskIndex;
Task TaskList::lastChangedTask;
Task TaskList::lastUnchangedTask;

//@Ma Zhuoyuan A0113624R
void TaskList::copyFromStorage(){
	vector<string> taskList = storage::returnTask();

	for (unsigned int i = 0; i < taskList.size(); i++){
		string task = removeFirstWord(taskList[i]);
		Task newTask(task, "copy");
		
		list.push_back(newTask);
		addTaskGroup(newTask);
		addPlace(newTask);
	}
}

void TaskList::copyToStorage(){	
	storage::deleteTemp();
	storage::ending();
}

string TaskList::addTask(string input){	
	std::size_t repeat = input.find("-every");
	if (repeat != std::string::npos){
		string taskInfo = input.substr(0, repeat - 1);
		Task newTask(taskInfo);
		if (newTask.getTaskType() == "floating"){
			return MagicString::FLOATING_CANNOT_RECUR;
		}
		else{
			string repeatInfo = input.substr(repeat + 6);
			string repeat_type = getFirstWord(repeatInfo);
			int repeat_time = atoi((removeFirstWord(repeatInfo)).c_str());
			addRepeatTask(taskInfo, repeat_type, repeat_time);
			storage::tempFile();
			return MagicString::RECURRING_TASK_ADDED;
		}
	}
	else{
		lastCommandType = "add";

		Task newTask(input);
		list.push_back(newTask);

		lastChangedTask = newTask;

		addTaskGroup(newTask);
		addPlace(newTask);
		storage::tempFile();
		return MagicString::TASK_ADDED;
	}
}

void TaskList::addRepeatTask(Task newTask, string repeat_type, int repeat_time){
	Task *newPtr = new Task;
	*newPtr = newTask;
	list.push_back(*newPtr);

	for (int i = 1; i < repeat_time; i++){
		Task *ptr = new Task;
		newTask.recurringAdd(repeat_type);
		*ptr = newTask;
		list.push_back(*ptr);
	}
}

string TaskList::updateTask(string input){
	string taskIndex = getFirstWord(input);
	string taskInfo = removeFirstWord(input);

	int index;
	istringstream in(taskIndex);
	in >> index;

	string output;
	int size = DisplayedTaskList::returnListSize();
	if (index > size|| index <= 0){
		output = "Task " + taskIndex + " does not exit"; //change to magic string
		return output;
	}
	else{
		lastCommandType = "update";
		lastChangedTaskIndex = findTargetedTaskIndex(index);
		lastUnchangedTask = list[lastChangedTaskIndex];

		list[lastChangedTaskIndex].UpdateTask(taskInfo);

		lastChangedTask = list[lastChangedTaskIndex];
		output = "Task " + taskIndex + " updated"; //change to magic string

		storage::tempFile();
		return output;
	}
}

string TaskList::deleteTask(string input){
	int index;
	istringstream in(input);
	in >> index;

	string output;
	int size = DisplayedTaskList::returnListSize();
	if (index > size || index <= 0){
		output = "Task " + input + " does not exit"; //change to magic string
		return output;
	}
	else{
		lastCommandType = "delete";
		lastChangedTaskIndex = findTargetedTaskIndex(index);
		lastUnchangedTask = list[lastChangedTaskIndex];

		list.erase(list.begin() + lastChangedTaskIndex);
		string output = "Task " + input + " deleted"; //change to magic string

		storage::tempFile();
		return output;
	}
}

string TaskList::search(string input){
	if (list.empty()){
		return "Task list is empty"; //change to magic string
	}
	else{
		DisplayedTaskList::emptyList();
		for (unsigned int i = 0; i < list.size(); i++){
			string taskName = (list[i]).getTaskname();
			vector<string> contents = splitText(taskName);

			for (unsigned int j = 0; j < contents.size(); j++){
				std::size_t search = contents[j].find(input);
				if (search!=std::string::npos){
					DisplayedTaskList::addTask(list[i]);
					break;
				}
			}
		}

		string output = DisplayedTaskList::display();
		if (output == "-1"){
			return "No task containes the searched word"; //change to magic string
		}
		else{
			return output;
		}
	}
}

string TaskList::display(string displayType){
	DisplayedTaskList::emptyList();
	
	if (addToDisplayedTaskList(displayType) == MagicString::INVALID_DISPLAY){
		return MagicString::INVALID_DISPLAY;
	}
	else if (DisplayedTaskList::isEmpty()){
		return MagicString::TASK_EMPTY2; //change to magic string
	}
	else{
		return DisplayedTaskList::display();
	}
}

string TaskList::addToDisplayedTaskList(string displayType){
	if (displayType == "all"){
		for (unsigned int i = 0; i < list.size(); i++){
			if (!list[i].taskDone()){
				DisplayedTaskList::addTask(list[i]);
			}
		}
		return "";
	}
	else if (displayType == "done"){
		for (unsigned int i = 0; i < list.size(); i++){
			if (list[i].taskDone()){
				DisplayedTaskList::addTask(list[i]);
			}
		}
		return "";
	}
	else if (displayType == "timed" || displayType == "deadline" || displayType == "floating"){
		for (unsigned int i = 0; i < list.size(); i++){
			if ((list[i]).getTaskType() == displayType){
				DisplayedTaskList::addTask(list[i]);
			}
		}
		return "";
	}
	else if (displayType == "today"){
		for (unsigned int i = 0; i < list.size(); i++){
			if (list[i].getDate() == list[i].getDate(0)){
				DisplayedTaskList::addTask(list[i]);
			}
		}
		return "";
	}
	else if (isExist(taskGroup, displayType)){
		for (unsigned int i = 0; i < list.size(); i++){
			if ((list[i]).getTaskGroup() == displayType){
				DisplayedTaskList::addTask(list[i]);
			}
		}
		return "";
	}
	else if (isExist(taskPlace, displayType)){
		for (unsigned int i = 0; i < list.size(); i++){
			if ((list[i]).getPlace() == displayType){
				DisplayedTaskList::addTask(list[i]);
			}
		}
		return "";
	}
	else{
		return MagicString::INVALID_DISPLAY;
	}
}

string TaskList::markAsDone(string input){
	int index;
	istringstream in(input);
	in >> index;

	lastCommandType = "done";
	lastChangedTaskIndex = findTargetedTaskIndex(index);

	list[lastChangedTaskIndex].markAsDone();
	string output = "Task " + input + " marked as done"; //change to magic string

	storage::tempFile();
	return output;
}

string TaskList::setPriority(string input){
	string taskIndex = getFirstWord(input);
	string taskInfo = removeFirstWord(input);

	int index;
	istringstream in(taskIndex);
	in >> index;

	string output;
	int size = DisplayedTaskList::returnListSize();
	if (index > size || index <= 0){
		output = "Task " + taskIndex + " does not exit"; //change to magic string
		return output;
	}
	else{
		lastCommandType = "setPriority";
		lastChangedTaskIndex = findTargetedTaskIndex(index);
		lastUnchangedTask = list[lastChangedTaskIndex];

		list[lastChangedTaskIndex].setPriority(taskInfo);

		lastChangedTask = list[lastChangedTaskIndex];
		output = "Task " + taskIndex + " is prioritised"; //change to magic string

		storage::tempFile();
		return output;
	}	
}

string TaskList::undo(){
	if (lastCommandType == "add"){
		list.pop_back();
		isLastCommandUndo = true;
		storage::tempFile();
		return "Adding command is undone"; //change to magic string
	}
	else if (lastCommandType == "update"){
		list[lastChangedTaskIndex] = lastUnchangedTask;
		isLastCommandUndo = true;
		storage::tempFile();
		return "Updating command is undone"; //change to magic string
	}
	else if (lastCommandType == "delete"){
		list.insert(list.begin() + lastChangedTaskIndex, lastUnchangedTask);
		isLastCommandUndo = true;
		storage::tempFile();
		return "Deleting command is undone"; //change to magic string
	}
	else if (lastCommandType == "done"){
		list[lastChangedTaskIndex].markAsUndone();
		isLastCommandUndo = true;
		storage::tempFile();
		return "MarkasDone command is undone"; //change to magic string
	}
	else if (lastCommandType == "setPriority"){
		list[lastChangedTaskIndex] = lastUnchangedTask;
		isLastCommandUndo = true;
		storage::tempFile();
		return "SetPriority command is undone";
	}
	else{
		return "Previous action cannot be undo"; //change to magic string
	}
}

string TaskList::redo(){
	if (isLastCommandUndo){
		isLastCommandUndo = false;
		if (lastCommandType == "add"){
			list.push_back(lastChangedTask);
			storage::tempFile();
			return "Adding command is redone"; //change to magic string
		}
		else if (lastCommandType == "update"){
			list[lastChangedTaskIndex] = lastChangedTask;
			storage::tempFile();
			return "Updating command is redone"; //change to magic string
		}
		else if (lastCommandType == "delete"){
			list.erase(list.begin() + lastChangedTaskIndex);
			storage::tempFile();
			return "Deleting command is redone"; //change to magic string
		}
		else if (lastCommandType == "done"){
			list[lastChangedTaskIndex].markAsDone();
			storage::tempFile();
			return "MarkasDone command is redone"; //change to magic string
		}
		else{
			list[lastChangedTaskIndex] = lastChangedTask;
			storage::tempFile();
			return "SetPriority command is redone";
		}	
	}
	else{
		return "No undo action is done previously"; //change to magic string
	}
}

string TaskList::getFirstWord(string input)
{
	return input.substr(0, input.find(' '));
}

string TaskList::removeFirstWord(string input)
{
	return input.substr(input.find_first_of(" ") + 1);
}

vector<string> TaskList::splitText(string text){
	vector<string> words;
	string word;
	istringstream in(text);
	copy(istream_iterator<string>(in),
		istream_iterator<string>(),
		back_inserter<vector<string>>(words));

	return words;
}

string TaskList::printVector(vector<string> output){
	ostringstream oss;
	for (unsigned int i = 0; i < output.size(); i++){
		oss << output[i];
	}
	return oss.str();
}

int TaskList::findTargetedTaskIndex(int displayedIndex){
	int size = list.size();
	for (int i = 0; i < size; i++){
		if (list[i].ToString()==(DisplayedTaskList::resturnTask(displayedIndex)).ToString()){
			return i;
		}
	}
	return -1;
}

void TaskList::empty(){
	while (!list.empty()){
		list.pop_back();
	}
}

void TaskList::addTaskGroup(Task newTask){
	string group = newTask.getTaskGroup();
	if (group != ""){
		if (!isExist(taskGroup, group)){
			taskGroup.push_back(group);
		}
	}
}

void TaskList::addPlace(Task newTask){
	string place = newTask.getPlace();
	if (place != ""){
		if (!isExist(taskPlace, place)){
			taskPlace.push_back(place);
		}
	}
}

bool TaskList::isExist(vector<string> list, string input){
	if (list.empty()){
		return false;
	}
	
	for (unsigned int i = 0; i < list.size(); i++){
		if (input == list[i]){
			return true;
		}
	}

	return false;
}