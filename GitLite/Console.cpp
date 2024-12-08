#include "Console.h"
#include <iostream>
#include <fstream>
using namespace std;
Console::Console()
{
	command = "";
	second = "";
	third = "";
	treeType = 0;
}

bool customGetline(std::ifstream& file, String& line, char delimiter = '\n') {
	line.clear();
	char ch;
	while (file.get(ch)) {
		if (ch == delimiter) {
			return true; // Successfully read a line
		}
		line += ch;
	}
	return !line.empty(); // Return true if there was any content read before EOF
}

void Console::parseCsv(int selectedTree, String second, path treeDir)
{
	String csvFileName = second;  // csv file

	String directoryName = "tree";

	// Create the tree using a switch based on the selected tree
	switch (selectedTree)
	{
		case(1):
		{
			avl.initialize(treeDir);
			break;
		}
		case(2):
		{
			//Btree.initialize(directoryName);
			break;
		}
		case(3):
		{
			rb.initialize(treeDir);
			break;
		}
	}


	ifstream csvFile(csvFileName.getData());
	if (!csvFile.is_open()) {
		cout << "Unable to open the CSV file." << endl;
		return;
	}

	// Read the header line to get column names
	String headerLine;
	customGetline(csvFile, headerLine);
	string headerLineStr;
	vector<String> headers;
	String header;
	int pos = 0;
	for (int i = 0; headerLine[i] != '\0'; i++) {
		if (headerLine[i] == ',') {
			headers.push_back(header);
			header.clear();
		}
		else {
			header += headerLine[i];
		}
		pos = i;
	}
	// Add the last header after the loop
	header += headerLine[pos + 1];
	headers.push_back(header);

	// Display available columns
	cout << "Available columns:" << endl;
	for (size_t i = 0; i < headers.size(); ++i) {
		cout << i + 1 << ": " << headers[i].getData() << endl;
	}

	int columnChoice;
	cout << "Select a column number to use as the key: ";
	cin.ignore(); // Ignore the newline character left by previous input
	cin >> columnChoice;
	--columnChoice; // Adjust for zero-based index

	if (columnChoice < 0 || columnChoice >= headers.size()) {
		cout << "Invalid column choice." << endl;
		return;
	}

	// Read and process each row
	String line;
	while (customGetline(csvFile, line)) {
		vector<String> rowData;
		String cell;
		for (int i = 0; line[i] != '\0'; ++i) {
			if (line[i] == ',') {
				rowData.push_back(cell);
				cell.clear();
			}
			else {
				cell += line[i];
			}
		}
		// Add the last cell after the loop
		rowData.push_back(cell);

		if (columnChoice < rowData.size()) {
			String key = rowData[columnChoice];
			// Reconstruct the data row as a single string
			String dataRow;
			for (size_t i = 0; i < rowData.size(); ++i) {
				dataRow += rowData[i];
				if (i != rowData.size() - 1) {
					dataRow += ',';
				}
			}
			switch (selectedTree)
			{
				case(1):
				{
					avl.insert(key, dataRow);
					break;
				}
				case(2):
				{
					//bt.insert(key, dataRow);
					break;
				}
				case(3):
				{
					rb.insert(key, dataRow);
					break;
				}
			}
		}
		else
		{
			cout << "Row has fewer columns than expected." << endl;
		}
	}

	csvFile.close();

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
		command.clear();
		second.clear();
		third.clear();
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
		if (command == "merge" || command == "select")  // This command needs two further inputs  // So taking the third input from the user
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
		if (command == "select")
		{
			// Read the lower and upper bounds

			// Ensure that both bounds are provided
			if (second.empty() || third.empty())
			{
				cout << "\033[91mUsage: select <lower_bound> <upper_bound>\033[0m" << endl;
				continue;
			}

			// Convert bounds to integers
			int lowerBound = stoi(second.getData());
			int upperBound = stoi(third.getData());

			// Check if repository is initialized
			if (treeType == 0)
			{
				cout << "\033[91mNo tree initialized. Please initialize a repository first.\033[0m" << endl;
				continue;
			}

			cout << "\033[33mFetching records where keys are between "
				<< lowerBound << " and " << upperBound << ".\033[0m" << endl;
			cout << endl;

			// Query the selected tree for records within the range
			switch (treeType)
			{
			case 1: // AVL Tree
				avl.queryRange(lowerBound, upperBound);
				break;
			case 3: // Red-Black Tree
				// rb.queryRange(lowerBound, upperBound);
				break;
			default:
				cout << "\033[91mTree type not supported for querying.\033[0m" << endl;
			}
		}
		// All inputs
		if (command == "init")
		{
			String folderName;
			folderName.readTill(second, '.');


			repoFolder = current_path() / folderName.getData();
			if (exists(repoFolder))
			{
				cout << "\033[91mRepository already exists. Please load the repository.\033[0m";
				continue;
			}
			create_directory(repoFolder);
			// Create the repo using the filename provided
			cout << "\033[33mWhich tree would you like to use?\033[0m" << endl;
			cout << "1: AVL" << endl;
			cout << "2: B-Tree" << endl;
			cout << "3: Red-Black Tree" << endl;
			ofstream file(repoFolder / "repo_data.txt");
			int x = 0;
			while (1)
			{
				cin >> x;
				if (x == 1)
				{
					treeType = 1;
					cout << "\033[33mSelected AVL tree.\033[0m";
					file << x << endl;
					break;
				}
				else if (x == 2)
				{
					treeType = 2;
					cout << "\033[33mSelected B-Tree.\033[0m";
					file << x << endl;
					break;
				}
				else if (x == 3)
				{
					treeType = 3;
					cout << "\033[33mSelected Red-Black Tree.\033[0m";
					file << x << endl;
					break;
				}
			}
			cout << endl;
			file << "master";
			create_directory(repoFolder / "master");
			currentBranchFolder = repoFolder / "master";
			create_directory(currentBranchFolder / "tree");
			parseCsv(treeType, second, currentBranchFolder/"tree");

			file.close();
		}
		else if (command == "branch")
		{

			create_directory(repoFolder / second.getData());
			copy(currentBranchFolder, repoFolder / second.getData());
			currentBranchFolder = repoFolder / second.getData();
			ofstream file(repoFolder / "repo_data.txt");
			file << treeType << endl << second;
			// second stores branch name
			cout << "\033[33mBranch " << '\'' << second << '\'' << " created successfully.\033[0m";
			file.close();
		}
		else if (command == "checkout")
		{
			if (exists(repoFolder / second.getData())) {
				currentBranchFolder = repoFolder / second.getData();
				ofstream file(repoFolder / "repo_data.txt");
				file << treeType << endl << second;
				file.close();
				cout << "\033[33mSwitched to branch " << '\'' << second << '\'' << ".\033[0m";
			}
			else {
				cout << "\033[91mBranch " << '\'' << second << '\'' << " does not exist.\033[0m";
			}
			// second stores branch name
		}
		else if (command == "commit")
		{
			// second stores commit message
			cout << "\033[33mChanges committed with message: " << '"' << second << '"' << ".\033[0m";
		}
		else if (command == "branches")
		{
			for (const auto& folder : directory_iterator(repoFolder)) {
				if (is_directory(folder)) {
					cout << "\033[33m" << folder.path().filename() << "\033[0m" << endl;
				}
			}
		}
		else if (command == "delete-branch")
		{
			if (exists(repoFolder / second.getData())) {
				remove_all(repoFolder / second.getData());
				cout << "\033[33mBranch " << '\'' << second << '\'' << " deleted successfully.\033[0m";
			}
			else {
				cout << "\033[91mBranch " << '\'' << second << '\'' << " does not exist.\033[0m";
			}
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

			switch (treeType)
			{
				case(1):
				{
					avl.inOrder();
					break;
				}
				case(2):
				{
					//Btree.initialize(directoryName);
					break;
				}
				case(3):
				{
					rb.inOrder();
					break;
				}
			}
		}
		else if (command == "log")
		{
			cout << "\033[33mCommit History for " << '\'' << "master" << '\'' << ":\033[0m" << endl;  // Replace with current branch
			cout << "\033[33mCommit #3: \"deleted 46 key\".\033[0m" << endl;  // Replace with the real message
			cout << "\033[33mCommit #2: \"Added 11 key\".\033[0m" << endl;
			cout << "\033[33mCommit #1: \"Initialized branch\".\033[0m";
		}
		else if (command == "current-branch")
		{
			cout << "\033[33m" << currentBranchFolder.filename() << "\033[0m";
			//cout << "\033[33mYou are on branch: 'main'.\033[0m";  // Replace with current branch
		}
		else if (command == "save")
		{
			cout << "\033[33mRepository saved successfully to 'repo_data.txt'.\033[0m";  // Replace with the real filename
		}
		else if (command == "load")
		{
			if (repoFolder == "") {
				if (exists(current_path() / second.getData())){
					repoFolder = current_path() / second.getData();
					ifstream file(repoFolder / "repo_data.txt");
					file >> treeType;
					String Branch;
					file >> Branch;
					currentBranchFolder = repoFolder / Branch.getData();
					file.close();

					cout << "\033[33mRepository loaded successfully from '" << second << "'.\033[0m";
				}

				else {
					cout << "\033[91mNo repository found. Please initialize a repository first.\033[0m";
				}
			}
			else {

				cout << "\033[91mAlready in a repository. Close this repo first.\033[0m";
			}

			cout << "\033[33mRepository loaded successfully from '" << second << "'.\033[0m";
		}
		else
		{
			cout << "\033[91mInvalid command. Please try again.\033[0m";
		}

		cout << endl;
		//cin.ignore();
	}
}
