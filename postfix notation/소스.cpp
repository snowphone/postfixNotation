#include <iostream>
#include <stack>	
#include <string>
#include <algorithm>
using namespace std;
const string PLS = "+", SUB = "-", MUL = "*", DIV = "/";
const string operators = "+-*/";
const string plsSub = "+-";
const string mulDiv = "*/";
char getOperator(stack<char>& lstk, stack<char>& rstk) {
	char ch = 0;
	while (!rstk.empty()) {
		ch = rstk.top();
		rstk.pop();
		if (operators.find(ch) != string::npos)
			return ch;
		else
			lstk.push(ch);
	}
	return 0;
}
string::iterator getLeft(string::iterator it, bool isInTheBracket = false) {
	char ch = *it;
	if (isInTheBracket) {
		if (ch == '(')
			return it;
		else
			return getLeft(it - 1, true);
	}
	if (operators.find(ch) != string::npos)
		return getLeft(it - 1, isInTheBracket);
	if (ch == ')')
		return getLeft(it - 1, true);

	return it;
}
string::iterator getRight(string::iterator it, bool isInTheBracket = false) {
	char ch = *it;

	if (isInTheBracket) {
		if (ch == ')')
			return it;
		else
			return getRight(it + 1, true);
	}
	if (find(operators.begin(), operators.end(), ch) != operators.end())
		return getRight(it + 1, isInTheBracket);
	if (ch == '(')
		return getRight(it + 1, true);
	return it;
}
void setPriority(string&exp) {
	for (auto op = exp.begin(); op != exp.end(); ++op) {
		if (mulDiv.find(*op) == string::npos)
			continue;
		auto lbeg = getLeft(op);
		ptrdiff_t length = op - lbeg;
		lbeg = exp.insert(lbeg, '(');
		op = lbeg + length + 1;

		auto  rlast = getRight(op);
		length = rlast - op;
		rlast = exp.insert(rlast + 1, ')');
		op = rlast - length - 1;
	}
}
string getLeft(stack<char>& lstk, bool inTheBracket = false) {
	if (lstk.empty()) 
		return "";
	char ch = lstk.top();
	lstk.pop();
	if (find(operators.begin(), operators.end(), ch) != operators.end())
		return getLeft(lstk, inTheBracket) + ch;
	if (ch == ')')
		return getLeft(lstk, true) + ch;
	if (inTheBracket) {
		if (ch == '(')
			return "(";
		else
			return getLeft(lstk, true) + ch;
	}
	return string(1, ch);
}
string getRight(stack<char>& rstk, bool inTheBracket = false) {
	if (rstk.empty())
		return "";
	char ch = rstk.top();
	rstk.pop();
	if (find(operators.begin(), operators.end(), ch) != operators.end())
		return ch + getRight(rstk, inTheBracket);
	if (ch == '(')
		return ch + getRight(rstk, true);
	if (inTheBracket) {
		if (ch == ')')
			return ")";
		else
			return ch + getRight(rstk, true);
	}
	return string(1, ch);
}
void Push(stack<char>& stk, const string& str) {
	for (const char& c : str)
		stk.push(c);
}
string deleteBracket(const string& expression) {
	string ret;
	for (auto it = expression.begin(); it != expression.end(); ++it)
		if (*it == '(' || *it == ')')
			continue;
		else
			ret.push_back(*it);
	return ret;
}
string transform(string exp) {
	stack<char> lstk, rstk;
	for_each(exp.rbegin(), exp.rend(), [&](char c) {rstk.push(c); });

	while (!rstk.empty()) {
		char ch = getOperator(lstk, rstk);
		if (ch == 0) break;
		Push(lstk, getLeft(lstk) + transform(getRight(rstk)) + ch);
	}
	string reversed;
	while (!lstk.empty()) {
		reversed.push_back(lstk.top());
		lstk.pop();
	}
	reverse(reversed.begin(), reversed.end());
	return reversed;
}
int main() {
	string expression;
	cin >> expression;
	setPriority(expression);
	cout << expression << endl;
	expression = transform(expression);
	cout << deleteBracket(expression);
}