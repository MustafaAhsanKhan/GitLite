#include "Console.h"
#include <iomanip>
Console::Console()
{
}

void Console::run()  // Program Loop
{
	cin >> std::noskipws;  // To read spaces as well
	while (true)
	{
		// The user can either input a single word command (init)
		// Or a command that has an additional part seperated by a space (commit "message"
		// The additional part can be enclosed in quotes or angle brackets
		// The command and second strings allow us to split these into two
		// Further allowing us to use them where needed

		cout << '>';  // Ready for user input
		cin >> command;
		command.toLower();  // Convert to lowercase to make everything consistent

		if (command == "exit")
		{
			cout << "\033[33mExiting GitLite...\033[0m";
			break;
		}
		// For all commands that only have one part
		if (!(command == "log" || command == "branches" || command == "current-branch" || command == "save"))
		{
			cin >> second;
			String temp;
			for (int i = 0; i < second.size(); ++i)
			{
				char c = second[i];
				if (c == '<' || c == '>' || c == '"')  // Ignoring enclosing characters
				{
					continue;
				}
				temp += c;
			}
			second = temp;  // Removed enclosing characters
		}
		if (command == "merge")  // This command needs two further inputs  // So taking the third input from the user
		{
			cin >> third;
			String temp;
			for (int i = 0; i < third.size(); ++i)
			{
				char c = third[i];
				if (c == '<' || c == '>' || c == '"')  // Ignoring enclosing characters
				{
					continue;
				}
				temp += c;
			}
			third = temp;  // Removed enclosing characters
		}

		// All inputs
		if (command == "init")
		{
			// Create the repo using the filename provided
			cout << "\033[33mWhich tree would you like to use?\033[0m" << endl;
			cout << "1: AVL" << endl;
			cout << "2: B-Tree" << endl;
			cout << "3: Red-Black Tree" << endl;

			int x = 0;
			while (1)
			{
				cin >> x;
				if (x == 1)
				{
					cout << "\033[33mSelected AVL tree.\033[0m";
					break;
				}
				else if (x == 2)
				{
					cout << "\033[33mSelected B-Tree.\033[0m";
					break;
				}
				else if (x == 3)
				{
					cout << "\033[33mSelected Red-Black Tree.\033[0m";
					break;
				}
			}
		}
		else if (command == "branch")
		{
			// second stores branch name
			cout << "\033[33mBranch " << '\'' << second << '\'' << " created successfully.\033[0m";
		}
		else if (command == "checkout")
		{
			// second stores branch name
			cout << "\033[33mSwitched to branch " << '\'' << second << '\'' << ".\033[0m";
		}
		else if (command == "commit")
		{
			// second stores commit message
			cout << "\033[33mChanges committed with message: " << '"' << second << '"' << ".\033[0m";
		}
		else if (command == "branches")
		{
			// List all branches
			cout << "\033[33mBranches:\033[0m" << endl << "\033[33mmain\033[0m" << endl << "\033[33mfeature-1\033[0m";
			cout << "\033[33mTemp1\033[0m" << endl << "\033[33mTemp2\033[0m";
		}
		else if (command == "delete-branch")
		{
			// second stores branch name
			cout << "\033[33mBranch " << '\'' << second << '\'' << " deleted successfully.\033[0m";
		}
		else if (command == "merge")
		{
			// second stores the source branch
			// third stores the target branch
			cout << "\033[33mMerged " << '\'' << second << '\'' << " into " << '\'' << third << '\'' << " successfully.\033[0m";
		}
		else if (command == "visualize-tree")
		{
			// BONUS //
			// DO AT THE END //
			// second stores the branch the visualize
			// Call the respective display functions
		}
		else if (command == "log")
		{
			cout << "\033[33mCommit History for " << '\'' << "feature-1" << '\'' << ":\033[0m" << endl;  // Replace with current branch
			cout << "\033[33mCommit #3: \"Refactored feature implementation\".\033[0m" << endl;  // Replace with the real message
			cout << "\033[33mCommit #2: \"Added new feature to branch\".\033[0m" << endl;
			cout << "\033[33mCommit #1: \"Initialized branch\".\033[0m";
		}
		else if (command == "current-branch")
		{
			cout << "\033[33mYou are on branch: 'main'.\033[0m";  // Replace with current branch
		}
		else if (command == "save")
		{
			cout << "\033[33mRepository saved successfully to 'repo_data.txt'.\033[0m";  // Replace with the real filename
		}
		else if (command == "load")
		{
			cout << "\033[33mRepository loaded successfully from '" << second << "'.\033[0m";
		}
		else
		{
			cout << "\033[91mInvalid command. Please try again.\033[0m";
		}

		cout << endl;
	}
}
