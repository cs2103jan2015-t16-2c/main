// Source File
// Interface.cpp

#include "InterfaceInput.h"

string InterfaceInput::getUserCommand() {
	string userCommand;

	cout << "command: ";
	getline(cin, userCommand);

	return userCommand;
}

string InterfaceInput::executeUserCommand(string userCommand) {
	string commandTypeString;
	string taskString;

	commandTypeString = getFirstWord(userCommand);
	taskString = removeFirstWord(userCommand);

	transform(commandTypeString.begin(), commandTypeString.end(), commandTypeString.begin(), ::tolower);
	
	COMMAND_TYPE commandType;
	commandType = determineCommandType(commandTypeString, taskString);

	string str;

	switch (commandType) {
	case HELP:
		return InterfaceOutput::displayHelp();
	case ADD_TASK:
		return TaskList::addTask(taskString);
	case SEARCH:
		return TaskList::search(taskString);
	case UPDATE:
		return TaskList::updateTask(taskString);
	case DELETE_TASK:
		return TaskList::deleteTask(taskString);
	case DISPLAY_TASKS:
		return TaskList::display(taskString);
	case MARK_DONE:
		return TaskList::markAsDone(taskString);
	case ARCHIVE:
		return storage::archive(taskString);
	case UNDO:
		return TaskList::undo();
	case REDO:
		return TaskList::redo();
	case EXIT:
		storage::ending();
		cout << InterfaceOutput::MESSAGE_GOODBYE << endl;;
		exit(0);
	case OTHERS:
	default:
		return InterfaceOutput::ERROR_INVALID_COMMAND;
	}
}

InterfaceInput::COMMAND_TYPE InterfaceInput::determineCommandType(string commandTypeString, string taskString) {
	if (commandTypeString == "help") {
		return COMMAND_TYPE::HELP;
	}
	else if (commandTypeString == "add") {
		return COMMAND_TYPE::ADD_TASK;
	}
	else if (commandTypeString == "update") {
		return COMMAND_TYPE::UPDATE;
	}
	else if (commandTypeString == "delete") {
		return COMMAND_TYPE::DELETE_TASK;
	}
	else if (commandTypeString == "search") {
		return COMMAND_TYPE::SEARCH;
	}
	else if (commandTypeString == "display" && InterfaceInput::isValidDisplayChoice(taskString)) {
		return COMMAND_TYPE::DISPLAY_TASKS;
	}
	else if (commandTypeString == "done") {
		return COMMAND_TYPE::MARK_DONE;
	}
	else if (commandTypeString == "archive") {
		return COMMAND_TYPE::ARCHIVE;
	}
	else if (commandTypeString == "undo") {
		return COMMAND_TYPE::UNDO;
	}
	else if (commandTypeString == "redo") {
		return COMMAND_TYPE::REDO;
	}
	else if (commandTypeString == "exit") {
		return COMMAND_TYPE::EXIT;
	}
	else{
		return COMMAND_TYPE::OTHERS;
	}
}

string InterfaceInput::getFirstWord(string userCommand) {
	return userCommand.substr(0, userCommand.find(' '));
}

string InterfaceInput::removeFirstWord(string userCommand) {
	return userCommand.substr(userCommand.find_first_of(" ") + 1);
}

bool InterfaceInput::isValidDisplayChoice(string taskString) {
	if (taskString == "all") {
		return true;
	}
	else if (taskString == "timed") {
		return true;
	}
	else if (taskString == "deadline") {
		return true;
	}
	else if (taskString == "floating") {
		return true;
	}
	else {
		return false;
	}
}

// for unit test
string InterfaceInput::testGetFirstWord(string testString) {
	return InterfaceInput::getFirstWord(testString);
}

string InterfaceInput::testRemoveFirstWord(string testString){
	return InterfaceInput::testRemoveFirstWord(testString);
}
