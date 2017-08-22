#pragma once
#include <string>
#include <algorithm>
using namespace std;
class PostFix {
	const std::string operators = "+-*/";
	const std::string mulDiv = "*/";
	std::string expression;
	string output;

	char getOperator() {
		if (expression.empty())
			return 0;

		for (auto it = expression.begin(); it != expression.end(); ++it) {
			auto  op = find(operators.begin(), operators.end(), *it);
			if (op != operators.end()) {
				output.append(string(expression.cbegin(), op));
				expression = string(op + 1, expression.cend());
				return *op;
			}
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
		if (ch == ')') {
			auto jt = getLeft(it - 1, true);
			return jt != output.begin
		}

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
		if (ch == '(') {
			auto jt = getRight(it + 1, true);
			return (jt != expression.end() && it[1] == ')' ? ++jt : jt);
		}
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
public:
	void setExpression(string exp) { expression = exp; }
};
