// Seamus Tynan Project #5
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <cctype>
#include <sstream>
#include <cassert>

using namespace std;

bool isoperator(char ch); // Returns true if a char is an operator
bool isparenthesis(char ch);  // Returns true if a char is a '(' or a ')'
int priority(char ch); // Checks the priority of an operator (for part 3)

int main() { // Part 1: Write Expression
	string expression;
	cout << "Enter mathematical expression : ";
	getline(cin, expression);					// Read and store expression up to the eol character EXPRESSION IS AN ARRAY OF CHARACTERS!
	int len = expression.length();              // len will be the number of characters in expression

	queue <string> infix, cinfix; // Part 2: Turn Expression into a queue Infix, then copy into cinfix for output
	string token = " "; // token variable

	string variables[26]; // <== A*A (should be the same A, so use an array implementation) 
	for (int i = 0; i < 26; i++){
		variables[i] = "?"; 
	}

	int left_p_counter = 0; // Must be the same #
	int right_p_counter = 0;

	int operator_counter = 0; // Operator # cannot be >= number count
	int number_counter = 0;

	if (isoperator(expression[0])){
		cout << "EXPRESSION INVALID! Cannot start with an operator!" << endl;
		system("pause");
		exit(-1); // Exit the program early
	}

	int counter = 0;
	while (counter != len) {
		char ch = expression[counter]; // Saves part of string into a char ch

		if (isdigit(ch)) { // is ch a digit?
			token = ch;
			number_counter++;
			while (isdigit(expression[counter + 1]) || expression[counter + 1] == '.') { 
				token += expression[counter + 1]; // keep adding to token until full number is recovered
				counter++; // update counter so next char is read-in
			}
		}

		else if (isalpha(ch)) {  // is ch alphabetical?
			if (counter != 0) { // if it's not the first ch being passed in, then check for implied multiplication
				if (isalpha(expression[counter - 1]) || isdigit(expression[counter - 1])) {
					cout << "EXPRESSION INVALID! Cannot have implied multiplication" << endl;
					system("pause");
					exit(-1); // Exit the program early
				}
				else{
 					if (variables[toupper(ch) - 65] == "?") { // if (the variable at index 'A' is == '?'){ set its value }
						cout << "What is the value of : " << ch << "? : ";
						cin >> token;
						variables[toupper(ch) - 65] = token;
						number_counter++;
					}
					else { // else use value already entered
						token = variables[toupper(ch) - 65];
						number_counter++;
					}
				}
			}
			else{
					if (variables[toupper(ch) - 65] == "?") { // if (the variable at index 'A' is == '?'){ set its value }
						cout << "What is the value of : " << ch << "? : ";
						cin >> token;
						variables[toupper(ch) - 65] = token;
						number_counter++;
					}
					else {
						token = variables[toupper(ch) - 65]; // else use value already entered
						number_counter++;
					}
				}
			}

		else if (isoperator(ch)) { // is ch an operator?
			if (isoperator(expression[counter + 1])) {
				cout << "EXPRESSION INVALID! No two types of operators may be next to each other!" << endl; // +*, +-, ++, etc.
				system("pause");
				exit(-1); // Exit the program early
			}
			operator_counter++;
			token = ch;
		}

		else if (isparenthesis(ch)) { // is ch '(' or ')'
			token = ch;
			if (ch == '(') { // update counters accordingly
				left_p_counter++;
			}
			else if (ch == ')'){
				right_p_counter++;
			}
		}

		else if (isspace(ch)) { // is ch a space?
			if (isoperator(expression[counter + 1]) && isoperator(expression[counter - 1])) { 
				cout << "EXPRESSION INVALID! No two types of operators may be next to each other!" << endl; // + *, + -, + +, etc.
				system("pause");
				exit(-1); // Exit the program early
			}
				token = ch;
		}

		else if (expression[counter + 1] == '\0'){ // if ch is end of string character, then do nothing
		}

		else{ // if ch isn't a valid character, then exit
			cout << "The character " << ch << " is invalid!" << endl;
				system("pause");
				exit(-1); // Exit program early
		}

		if (!(token == " ")){ // if ch is " ", then don't add it to the queue
			infix.push(token);
		}
		counter++; // update counter
	}

	if (left_p_counter != right_p_counter) { // is there an excess of parenthesis?
		cout << "EXPRESSION INVALID! Left parentheses ammount : " << left_p_counter << " != " 
			<< "Right parentheses ammount : " << right_p_counter << " " << endl;
		system("pause");
		exit(-1);
	}

	if (operator_counter >= number_counter) { // is the # of operators greater-than or equal to the amount of numbers?
		cout << "EXPRESSION INVALID! Too many operators : " << endl;
		system("pause");
		exit(-1);
	}

	if (isoperator(expression[len])){ // is the last ch an operator?
		cout << "EXPRESSION INVALID! Cannot end with an operator!" << endl;
		system("pause");
		exit(-1); // Exit the program early
	}

	cinfix = infix; // copy = infix

	cout << endl << "Infix Queue: " << endl;

	while (!cinfix.empty()){ // Display's queue
		cout << cinfix.front() << endl;
		cinfix.pop();
	}

	cout << endl;

	queue <string> postfix, cpostfix; // Part 3: Infix ==> Postfix
	stack <string> operators; // Holds operators and left parentheses

	do{
		if (infix.front() == "("){
			operators.push(infix.front());
			infix.pop();
		}
		else if (isdigit(infix.front()[0]) /*|| infix.front() == "."*/){ // 22.5 <== this case is number
			postfix.push(infix.front());
			infix.pop();
		}
		else if (infix.front() == ")") {
			infix.pop();
			while (operators.top() != "(") {
				postfix.push(operators.top());
				operators.pop();
			}
			operators.pop();
		}
		else if (isoperator(infix.front()[0])) {
			if (operators.empty()) {
				operators.push(infix.front());
				infix.pop();
			}
			else if (priority(operators.top()[0]) < priority(infix.front()[0])) { 
				operators.push(infix.front());
				infix.pop();
			}
			else {
				do {
					postfix.push(operators.top());
					operators.pop();
				} while(!(operators.empty()) && (priority(operators.top()[0]) >= priority(infix.front()[0]))); 
				operators.push(infix.front());
				infix.pop();
			}
		}
	} while (!infix.empty());

	while (!operators.empty()){
		postfix.push(operators.top());
		operators.pop();
	}

	cpostfix = postfix; // copy = postfixfix

	cout << "Postfix Queue" << endl;

	while (!cpostfix.empty()){ // Display's queue
		cout << cpostfix.front() << endl;
		cpostfix.pop();
	}

	stack <double> results; // Part 4: Evaluation 
	double first, second;

	do{
		if (isdigit(postfix.front()[0])){
			results.push(atof(postfix.front().c_str()));
			postfix.pop();
		}
		else if (isoperator(postfix.front()[0])){
			second = results.top();
			results.pop();

			first = results.top();
			results.pop();
			
			if (postfix.front() == "+"){
				results.push(first + second);
			}
			else if (postfix.front() == "-"){
				results.push(first - second);
			}
			else if (postfix.front() == "*"){
				results.push(first * second);
			}
			else if (postfix.front() == "/"){
				results.push(first / second);
			}
			else{ // "^"
				results.push(pow(first, second));
			}
			postfix.pop();
		}
	} while (!(postfix.empty()));

	cout << endl << "The RESULT = " << results.top() << endl << endl; // displays the result

	system("pause");
	return 0;
}

bool isoperator(char ch){ // isoperator function
	if (ch == '+'||ch == '-'|| ch == '*' || ch == '/' || ch == '^') {
		return true;
	}
	else
		return false;
}

bool isparenthesis(char ch){ // isparenthesis function
	if (ch == '(' || ch == ')') {
		return true;
	}
	else 
		return false;
}

int priority(char ch){ // returns priority value
	switch(ch){
		case '(':return 0;
			break;
		case '+': return 1;
			break;
		case '-': return 1;
			break;
		case '*': return 2;
			break;
		case '/': return 2;
			break;
		case '^': return 3;
			break;
	}
}
