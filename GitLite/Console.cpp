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
		if (command.compare("merge"))  // This command needs two further inputs  // So taking the third input from the user
		{
			cin >> third;
			String temp;
			for (int i = 0; i < third.getlen(); ++i)
			{
				char c = third[i];
				if (c == '<' || c == '>' || c == '"')  // Ignoring enclosing characters
				{
					continue;
				}
				temp = temp + c;
			}
			third = temp;  // Removed enclosing characters
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
		else if (command.compare("delete-branch"))
		{
			// second stores branch name
			cout << "Branch" << '\'' << second << '\'' << " deleted successfully.";
		}
		else if (command.compare("merge"))
		{
			// second stores the source branch
			// third stores the target branch
			cout << "Merged " << '\'' << second << '\'' << " into " << '\'' << third << '\'' << " successfully.";
		}
		else if (command.compare("visualize-tree"))
		{
			// BONUS //
			// DO AT THE END //
			// second stores the branch the visualize
			// Call the respective display functions
		}
		else if (command.compare("log"))
		{
			cout << "Commit History for " << '\'' << "feature-1" << '\'' << ':' << endl;  // Replace with current branch
			cout << "Commit #3: " << '"' << "Refactored feature implementation" << '"' << '.' << endl;  // Replace with the real message
			cout << "Commit #2: " << '"' << "Added new feature to branch" << '"' << '.' << endl;
			cout << "Commit #1: " << '"' << "Initialized branch" << '"' << '.';
		}
		else if (command.compare("current-branch"))
		{
			cout << "You are on branch: " << '\'' << "main" << '\'' << '.';  // Replace with current branch
		}
		else if (command.compare("save"))
		{
			cout << "Repository saved successfully to " << '\'' << "repo_data.txt" << '\'' << '.';  // Replace with the real filename
		}
		else if (command.compare("load"))
		{
			// second stores file name
			cout << "Repository loaded successfully from " << '\'' << second << '\'' << '.';
		}
		else if (command.compare("exit"))
		{
			cout << "Exiting GitLite...";
			break;
		}
		else
		{
			cout << "Invalid command. Please try again.";
		}



		cout << endl;
	}
}