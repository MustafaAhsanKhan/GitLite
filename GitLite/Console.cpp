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




		cout << endl;
	}
}