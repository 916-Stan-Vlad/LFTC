#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

// Define a struct to represent tokens
struct Token {
    string lexeme;
    string type;
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
vector<Token> tokenizeCode(const string& inputFileName, const vector<string>& keywords, CustomHashTable& symbolTable) {
    vector<Token> tokens;
    ifstream inputFile(inputFileName);
    string line;
    while (getline(inputFile, line)) {
        size_t pos = 0;
        while (pos < line.length()) {
            string token;
            while (pos < line.length() && isalnum(line[pos])) {
                token += line[pos];
                pos++;
            }
            if (!token.empty()) {
                int find = 0;
                Token t;
                t.lexeme = token;
                t.type = "identifier"; // Default to "identifier"

                // Check if the token is in the symbol table
                string tokenType = symbolTable.find(token);
                if (!tokenType.empty()) {
                    find = 1;
                    ///t.type = tokenType;
                } else {
                    for (const string& keyword : keywords) {
                        if (token == keyword) {
                            t.type = "keyword";
                            break;
                        }
                    }
                }
                if(find == 0){
                    ///symbolTable.insert(t.lexeme, t.type);
                    tokens.push_back(t);
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

    vector<Token> tokens = tokenizeCode("C:\\Users\\vlads\\vlad\\facultate_an_3_1\\LFTC\\Lab3\\input.txt", keywords, symbolTable);

    for (const Token& token : tokens) {
        if (symbolTable.find(token.lexeme) == "") {
            symbolTable.insert(token.lexeme, token.type);
        }
    }

    // Write the hash table to the output file
    ofstream outputFile("C:\\Users\\vlads\\vlad\\facultate_an_3_1\\LFTC\\Lab3\\output.txt");

    // Print the entire symbol table
    for (const auto & i : symbolTable.getTable()) {
        for (const auto& entry : i) {
            if (entry.second != "keyword"){
                outputFile << "Lexeme: " << entry.first << " Type: " << entry.second << endl;
            }
        }
    }


    return 0;
}
