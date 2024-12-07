#include "string2.h"
#include "AVL-Tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

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

int main() {
    AVL tree;
    String csvFileName;
    cout << "Enter the CSV file name (with extension): ";
    cin >> csvFileName;

    String directoryName = "temp";
    //cout << "Enter the directory name to store node files: ";
    //cin >> directoryName;

    tree.initialize(directoryName);

    ifstream csvFile(csvFileName.getData());
    if (!csvFile.is_open()) {
        cout << "Unable to open the CSV file." << endl;
        return 1;
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
            cout << header;
            cout << endl;
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
    cin >> columnChoice;
    --columnChoice; // Adjust for zero-based index

    if (columnChoice < 0 || columnChoice >= headers.size()) {
        cout << "Invalid column choice." << endl;
        return 1;
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
            tree.insert(key, dataRow);
        }
        else {
            cout << "Row has fewer columns than expected." << endl;
        }
    }

	// In-order traversal to write the tree to a file
	tree.inOrder();

    csvFile.close();

    // Optionally, perform in-order traversal to display the tree
    // tree.inOrder();

    return 0;
}
