#pragma once
#include <set>
#include <string>
#include <vector>

struct State {
	std::set<char> dom; // множество исп букв
	std::string str;
	State();
	std::pair<std::string::const_iterator, std::string::const_iterator> line(int i) const;
};
using citer = std::string::const_iterator;
using range = std::pair<citer, citer>;

bool operator==(State const& l, State const& r);
std::ostream& operator<< (std::ostream& os, State const& s);
bool isPerfectSquare(long long n);
bool notComplete(range);
bool isSquareStr(range);
bool isSquareStr(citer b, citer e);
int countSquareStr(State const& s);
bool accept(State const& s);
bool reject(State const& s);
std::vector<State> expand(State const& s);
