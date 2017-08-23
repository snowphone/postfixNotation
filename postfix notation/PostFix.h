#pragma once
#include <ios>
#include <string>
#include <algorithm>
using std::find;	using std::ostream;
class PostFix {
	typedef std::string Str;
	typedef Str::iterator Iter;
	const Str opers = "+-*/",
		mulDiv = "*/";
	Str expression;
	char getOperator(Str& output, Str& exp) {
		if (exp.empty())
			return 0;
		for (auto it = exp.begin(); it != exp.end(); ++it) {
			auto opIt = find(opers.begin(), opers.end(), *it);
			if (opIt != opers.end()) {
				output.append(Str(exp.begin(), it));
				char oper = *it;
				exp = Str(it + 1, exp.end());
				return oper;
			}
		}
		return 0;
	}
	Iter getLeft(Iter it, int pairs = 0) {
		char ch = *it;
		if (pairs) {
			if (ch == '(')
				return pairs == 1 ? it : getLeft(it - 1, pairs - 1);
			else if (ch == ')')
				return getLeft(it - 1, pairs + 1);
			else
				return getLeft(it - 1, pairs);
		}
		if (opers.find(ch) != Str::npos)
			return getLeft(it - 1, pairs);
		if (ch == ')')
			return getLeft(it - 1, pairs + 1);

		return it;
	}
	Iter getRight(Iter it, int pairs = 0) {
		char ch = *it;

		if (pairs) {
			if (ch == ')')
				return pairs == 1 ? it : getRight(it + 1, pairs - 1);
			else if (ch == '(')
				return getRight(it + 1, pairs + 1);
			else
				return getRight(it + 1, pairs);
		}
		if (find(opers.begin(), opers.end(), ch) != opers.end())
			return getRight(it + 1, pairs);
		if (ch == '(')
			return getRight(it + 1, pairs + 1);
		return it;
	}
	void setPriority(Str&exp) {
		for (auto op = exp.begin(); op != exp.end(); ++op) {
			if (mulDiv.find(*op) == Str::npos)
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

	void deleteBracket(Str& exp) {
		auto it = exp.begin();
		while (it != exp.end())
			if (*it == '(' || *it == ')')
				it = exp.erase(it);
			else
				++it;
	}
	Str transform(Str exp) {
		Str output;
		if (exp.size() == 1) return exp;
		while (!exp.empty()) {
			char op = getOperator(output, exp);
			if (op == 0) break;
			auto rlastIter = getRight(exp.begin());
			Str right = transform(Str(exp.begin(), rlastIter + 1));
			output.append(right + op);
			exp = Str(rlastIter + 1, exp.end());
		}
		deleteBracket(output);
		return output;
	}
public:
	PostFix(const Str& exp) : expression(exp) {}
	Str transform() { setPriority(expression); return transform(expression); }
};
ostream& operator<<(ostream& os, PostFix& postfix) {
	os << postfix.transform();
	return os;
}