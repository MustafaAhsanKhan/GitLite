#pragma once
#include <iostream>
using namespace std;
class Console
{
private:
	string userInput;
	string command;
	string second;

public:
	Console()
	{

	}

	void run()  // Program Loop
	{
		while (true)
		{
			cout << '>';  // Ready for user input
			cin >> command;
			command = tolower(command);
			command.
			if (command != "init");
			cin >> second;

			// The user can either input a single word command (init)
			// Or a command that has an additional part seperated by a space (commit "message")
			// The additional part can be enclosed in quotes or angle brackets
			// Spliting these two into seperate strings
			//cout << userInput;

			//bool part = 0;  // First message
			//for (int i = 0; i < userInput.size() - 1; ++i)  // Looping up till size - 1 to exclude the bracket or quote
			//{
			//	if (userInput[i] == ' ')
			//		part = 1; // second message started
			//	if (part == 0)
			//	{
			//		command += userInput[i];
			//	}
			//	else
			//	{
			//		second += userInput[i];
			//	}
			//}


			cout << endl;
			cout << command << ' ' << second;

			if (command == "init")
			{
				cout << "Init command called";
			}
		}
	}
};