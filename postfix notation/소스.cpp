#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
const string operators = "+-*/",
	mulDiv = "*/";
char getOperator(string& output, string& exp) {
	if (exp.empty())
		return 0;
	for(auto it = exp.begin(); it != exp.end(); ++it){
		auto opIt = find(operators.begin(), operators.end(), *it);
		if (opIt != operators.end()) {
			output.append(string(exp.begin(), it));
			char oper = *it;
			exp = string(it + 1, exp.end());
			return oper;
		}
	}
	return 0;
}
string::iterator getLeft(string::iterator it, int pairs = 0) {
	char ch = *it;
	if (pairs) {
		if (ch == '(')
			return pairs == 1 ? it : getLeft(it - 1, pairs - 1);
		else if (ch == ')')
			return getLeft(it - 1, pairs + 1);
		else
			return getLeft(it - 1, pairs);
	}
	if (operators.find(ch) != string::npos)
		return getLeft(it - 1, pairs);
	if (ch == ')')
		return getLeft(it - 1, pairs+1);

	return it;
}
string::iterator getRight(string::iterator it, int pairs = 0) {
	char ch = *it;

	if (pairs) {
		if (ch == ')')
			return pairs == 1 ? it : getRight(it + 1, pairs - 1);
		else if (ch == '(')
			return getRight(it + 1, pairs + 1);
		else
			return getRight(it + 1, pairs);
	}
	if (find(operators.begin(), operators.end(), ch) != operators.end())
		return getRight(it + 1, pairs);
	if (ch == '(')
		return getRight(it + 1, pairs + 1);
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

void deleteBracket(string& expression) {
	auto it = expression.begin();
	while (it != expression.end())
		if (*it == '(' || *it == ')')
			it = expression.erase(it);
		else
			++it;
}
string transform(string exp) {
	string output;
	if (exp.size() == 1) return exp;
	while (!exp.empty()) {
		char op = getOperator(output, exp);
		if (op == 0) break;
		auto rlastIter = getRight(exp.begin());
		string right = transform(string(exp.begin(), rlastIter + 1));
		output.append(right + op);
		exp = string(rlastIter + 1, exp.end());
	}
	deleteBracket(output);
	return output;
}
int main() {
	string expression;
	cin >> expression;
	setPriority(expression);
	cout << transform(expression) << endl;
}