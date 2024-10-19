#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
using namespace std;

unordered_map<string, string> memory;

void error(string msg) {
  cout << "Error: " << msg << endl;
  exit(1);
}

// Extract first token from string
// and remove it from the string
// paratemer is a reference to the string
// return the first token
string chomp(string &str) {
  // Find the first space or newline character in the string.
  int pos = 0;
  while (pos < str.size() && str[pos] != ' ' && str[pos] != '\n') {
      pos++;
  }

  // Extract the part of the string up to the space or newline.
  string token = str.substr(0, pos);

  // Skip over the space or newline and any additional whitespace.
  while (pos < str.size() && (str[pos] == ' ' || str[pos] == '\n')) {
    pos++;
  }

  // Update the input string by removing the processed part.
  str = str.substr(pos);

  return token;
}


string chomp_reverse(string &str) {
    // Find the last space in the string.
    int pos = str.size() - 1;
    while (pos >= 0 && str[pos] != ' ') {
        pos--;
    }

    // Extract the part of the string between space and end.
    string token = str.substr(pos + 1);

    // Update the input string by removing the processed part.
    if (pos >= 0) {
        str = str.substr(0, pos);
    } else {
        str.clear(); // If no space was found, clear the string.
    }

    return token;
}

bool is_digits(const string &str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

void process_equals(int line, string l, string r) {
  cout << "Processing equals: " << l << " = " << r << endl;
  // validate tokens
  if (is_digits(l)) {
    error("Compiler error: invalid left token (" + l + ") at line (" + to_string(line) + ")");
  }
  // Note: this decision here is interesting, bring it up with Kaz
  // save only values, or tokens & values in memory
  // memory[l] = get_data(r);
  memory[l] = r;
}

// Recursivley fetch data from memory until we get a int value
int get_data(string identifier) {
  if (is_digits(identifier)) {
    return stoi(identifier);
  }
  if (memory.find(identifier) == memory.end()) {
    error("Compiler error: identifier not found in memory");
  }
  return get_data(memory[identifier]);
}

string process_plus(int line, string l, string r) {
  return to_string(get_data(l) + get_data(r));
}

string process_minus(int line, string l, string r) {
  return to_string(get_data(l) - get_data(r));
}

string process(int line, string l, string op, string r) {

  // validate parameters
  if (l == "" || r == "" || op == "") {
    error("Compiler error: invalid token chain (" + l + " " + op + " " + r + ") at line (" + to_string(line) + ")");
  }
  // TODO: do more validations n shit

  
  cout << "Processing: " << l << " " << op << " " << r << endl;

  if (op == "=") {
    process_equals(line, l, r);
    return "";
  } else if (op == "+") {
    return process_plus(line, l, r);
  } else if (op == "-") {
    return process_minus(line, l, r);
  } else {
    error("Compiler error: invalid operator at line (" + to_string(line) + ")");
    return "";
  }
}

void print_memory(bool get_value = false) {
  cout << "------------Memory-----------" << endl;
  if (get_value) {
    cout << "------------As Values-----------" << endl;
  }
  for (const auto& pair : memory) {
    string key = pair.first;
    string value = get_value ? to_string(get_data(pair.second)) : pair.second;
    cout << key << "->" << value << endl;
  }
}

int main() {

  // read fine line by line
  ifstream file("input.txt");
  string line;
  // parse file for lines
  int line_counter = 0;
  while (getline(file, line)) {
    line_counter++;

    string tok_identifier_l = "";
    string tok_identifier_r = "";
    string tok_operator = "";
    // parse line for tokens
    while (line.size() > 0 && line[0] != '\n') {
      string token = chomp_reverse(line);
      cout << "token: " << token;
      // NOTE: we assume we always have valid chains of tokens
      // defined as (left_identifier operator right_identifier)
      if (tok_identifier_r == "") {
        cout << " is 'right'" << endl;
        tok_identifier_r = token;
      } else if (tok_operator == "") {
        cout << " is 'operator'" << endl;
        tok_operator = token;
      } else if (tok_identifier_l == "") {
        cout << " is 'left'" << endl;
        tok_identifier_l = token;
      } 
      
      if (tok_identifier_l != "" && tok_operator != "" && tok_identifier_r != "") { 
        tok_identifier_r = process(line_counter, tok_identifier_l, tok_operator, tok_identifier_r);
        tok_operator = "";
        tok_identifier_l = "";
      }
    }
  }

  print_memory();
  print_memory(true);

  return 0;
}