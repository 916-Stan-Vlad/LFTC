#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

// Define a struct to represent tokens
struct Token {
    string lexeme;
    string type;
    int poz; // Add a position field
};

// Function to read keywords from a file
vector<string> readKeywords(const string& keywordFileName) {
    vector<string> keywords;
    ifstream keywordFile(keywordFileName);
    string keyword;
    while (getline(keywordFile, keyword)) {
        keywords.push_back(keyword);
    }
    return keywords;
}

// Custom hash table with separate chaining
class CustomHashTable {
public:
    CustomHashTable(int size) : table(size) {}

    void insert(const string& key, const string& value) {
        int index = hashFunction(key);
        auto& list = table[index];
        for (auto& entry : list) {
            if (entry.first == key) {
                // Update the value if the key already exists
                return;
            }
        }
        list.emplace_back(key, value);
    }

    string find(const string& key) {
        int index = hashFunction(key);
        for (const auto& entry : table[index]) {
            if (entry.first == key) {
                return entry.second;
            }
        }
        return "";  // Key not found
    }

    const vector<list<pair<string, string>>>& getTable() const {
        return table;
    }

private:
    int hashFunction(const string& key) {
        // A simple hash function for demonstration purposes
        return key.length() % table.size();
    }

    vector<list<pair<string, string>>> table;
};

// Function to tokenize the input code
vector<Token> tokenizeCode(const string& inputFileName, const vector<string>& keywords, CustomHashTable& symbolTable, CustomHashTable& combinedTable, map<string, int>& identifierPositions) {

    vector<Token> tokens;
    ifstream inputFile(inputFileName);
    string line;
    while (getline(inputFile, line)) {
        size_t pos = 0;
        while (pos < line.length()) {
            string token;
            string token_check;
            size_t pos_2 = pos;
            int token_find = 0;
            // if line[pos] == " until you find " in the add to st and pif as string if you do not find " throw error( return error)
            // the while below should be included in the else above
            token_check += line[pos_2];
            if(token_check == "\""){
                cout << "findddddd";
                while (pos_2 < line.length()){
                    token_check += line[pos_2];
                    pos_2 ++;
                    if(token_check == "\""){
                        token = token_check;
                        pos = pos_2;
                        token_find = 1;
                    }
                }
                if(token_find == 0){
                    cout << "Invalid cout";
                    return tokens;
                }
            }
            while (pos < line.length() && isalnum(line[pos]) ) {
                token += line[pos];
                pos++;
            }

            if (!token.empty()) {
                int find = 0;
                Token t;
                // to check if 2er is not a valid variable, check if the first char of token is alphabet
                t.lexeme = token;
                t.type = "identifier"; // Default to "identifier"

                // Check if the token is in the symbol table
                string tokenType = symbolTable.find(token);
                if (!tokenType.empty()) {
                    find = 1;
                } else {
                    for (const string& keyword : keywords) {
                        if (token == keyword) {
                            t.type = "keyword";
                            t.poz = -1;
                            identifierPositions[t.lexeme] = -1;
                            break;
                        }
                    }
                    if (t.type == "identifier" && t.lexeme.length() >=2 && token.find_first_of("0123456789") != string::npos) {
                        // If it's an identifier and contains numbers, it's an error
                        cout << "Error: Invalid identifier with numbers - " << token << endl;
                        return tokens;
                    }
                }
                if (find == 0) {
                    // If it's an identifier, assign the position and update the symbol table
                    if (t.type == "identifier") {
                        t.poz = tokens.size();
                        identifierPositions[t.lexeme] = tokens.size();
                        symbolTable.insert(t.lexeme, t.type);
                        tokens.push_back(t);
                    }

                    combinedTable.insert(t.lexeme, t.type);
                }

            }
            while (pos < line.length() && !isalnum(line[pos])) {
                pos++;
            }
        }
    }
    return tokens;
}

int main() {
    vector<string> keywords = readKeywords("C:\\Users\\vlads\\vlad\\facultate_an_3_1\\LFTC\\Lab3\\keywords.txt");
    CustomHashTable symbolTable(100);
    CustomHashTable combinedTable(100);
    map<string, int> identifierPositions;

    vector<Token> tokens = tokenizeCode("C:\\Users\\vlads\\vlad\\facultate_an_3_1\\LFTC\\Lab3\\input.txt", keywords, symbolTable, combinedTable, identifierPositions);

    // Write the tables to the output file
    ofstream outputFile("C:\\Users\\vlads\\vlad\\facultate_an_3_1\\LFTC\\Lab3\\output.txt");

    outputFile << "Symbol Table:" << endl;
    for (const auto & i : symbolTable.getTable()) {
        for (const auto& entry : i) {
            if (entry.second != "keyword") {
                outputFile << "Lexeme: " << entry.first << " Type: " << entry.second<< " Poz: " << identifierPositions[entry.first]<< endl;
            }
        }
    }

    ofstream outputPIF("C:\\Users\\vlads\\vlad\\facultate_an_3_1\\LFTC\\Lab3\\PIF.out");
    outputPIF << "Combined Table (Keywords and Identifiers):" << endl;
    for (const auto & i : combinedTable.getTable()) {
        for (const auto& entry : i) {
            outputPIF << "Lexeme: " << entry.first << " Type: " << entry.second << " Poz: " << identifierPositions[entry.first] << endl;
        }
    }

    return 0;
}
