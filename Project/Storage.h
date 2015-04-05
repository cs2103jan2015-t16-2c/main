#ifndef STORAGE_H_
#define STORAGE_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include "TaskList.h"

using namespace std;

class storage{
private:
	static vector<string> task;
	static const string FILENAME;

public:
	static string starting();
	static void ending();
	static vector<string> returnTask();
	static string archive(string fileName);
	static string checkEmpty();
};

#endif
