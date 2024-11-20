#include "Console.h"

Console::Console()
{

}

void Console::run()  // Program Loop
{
	while (true)
	{
		// The user can either input a single word command (init)
		// Or a command that has an additional part seperated by a space (commit "message")
		// The additional part can be enclosed in quotes or angle brackets
		// The command and second strings allow us to split these into two
		// Further allowing us to use them where needed

		cout << '>';  // Ready for user input
		cin >> command;
		command.to_lower();  // Convert to lowercase to make everything consistent
		// For all commands that only have one part
		if (!(command.compare("log") || command.compare("branches") || command.compare("current-branch") || command.compare("save")))
		{
			cin >> second;
			String temp;
			for (int i = 0; i < second.getlen(); ++i)
			{
				char c = second[i];
				if (c == '<' || c == '>' || c == '"')  // Ignoring enclosing characters
				{
					continue;
				}
				temp = temp + c;
			}
			second = temp;  // Removed enclosing characters
		}

		// All inputs

		if (command.compare("init"))
		{
			// Create the repo using the filename provided
			cout << "Which tree would you like to use?" << endl;
			cout << "1: AVL" << endl;
			cout << "2: B-Tree" << endl;
			cout << "3: Red-Black Tree" << endl;

			int x = 0;
			while (1)
			{
				cin >> x;
				if (x == 1)
				{
					cout << "Selected AVL tree.";
					break;
				}
				else if (x == 2)
				{
					cout << "Selected B-Tree.";
					break;
				}
				else if (x == 3)
				{
					cout << "Selected Red-Black Tree.";
					break;
				}
			}
		}
		else if (command.compare("branch"))
		{
			// second stores branch name
			cout  << "Branch " << '\'' << second << '\'' << " created successfully.";
		}
		else if (command.compare("checkout"))
		{
			// second stores branch name
			cout << "Switched to branch " << '\'' << second << '\'' << '.';
		}
		else if (command.compare("commit"))
		{
			// second stores the commit message
			cout << "Changes committed with message: " << '"' << second << '"' << '.';
		}
		else if (command.compare("branches"))
		{
			// Display all branches
			cout << "Temp1" << endl << "Temp2";
		}




		cout << endl;
	}
}