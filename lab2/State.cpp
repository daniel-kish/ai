#include "State.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <tuple>

State::State()
	: dom{'1','2','3','4','5','6','7','8','9'}, str(9, '_')
{
}

range State::line(int i) const
{
	using std::make_pair;

	auto b{ str.cbegin() };
	switch (i) {
	case 0: return make_pair(b, b+1); break;
	case 1: return make_pair(b+1, b+3); break;
	case 2: return make_pair(b+3, b+6); break;
	case 3: return make_pair(b+6, b+8); break;
	case 4: return make_pair(b+8, str.cend()); break;
	}
}

bool operator==(State const & l, State const & r)
{
	return l.str == r.str;
}

std::ostream& operator<< (std::ostream& os, State const& s)
{
	os << "  " << s.str[0] << '\n';
	os << ' ' << s.str[1] << ' ' << s.str[2] << '\n';
	os << s.str[3] << ' ' << s.str[4] << ' ' << s.str[5] << '\n';
	os << ' ' << s.str[6] << ' ' << s.str[7] << '\n';
	os << "  " << s.str[8] << '\n';
	
	return os;
}

bool isPerfectSquare(long long n) {
	long long squareRootN = (long long)round((sqrt(n)));

	if (squareRootN*squareRootN == n) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

bool notComplete(range r)
{
	if (std::find(r.first, r.second, '_') != r.second)
		return true;
	return false;
}

bool isSquareStr(citer b, citer e)
{
	std::ostringstream ss;
	std::ostream_iterator<char> out{ ss };
	
	for (auto p = b; p != e; ++p) {
		if (*p == '_')
			throw std::runtime_error{R"( '_' found)"};
		*out = *p;
	}
	int sq = std::stoi(ss.str());
	return isPerfectSquare(sq);
}

bool isSquareStr(range r)
{
	return isSquareStr(r.first, r.second);
}

int countSquareStr(State const& s)
{
	int n{0};
	for (int i : {2,1,3,0,4}) {
		if (notComplete(s.line(i)))
			continue;
		if (isSquareStr(s.line(i)))
			n++;
	}
	return n;
}

bool accept(State const& s)
{
	for (int i = 0; i < 5; ++i)
		if (notComplete(s.line(i)) || !isSquareStr(s.line(i)))
			return false;
	return true;
}

bool reject(State const& s)
{
	for (int i = 0; i < 5; i++) 
	{
		if (notComplete(s.line(i)))
			continue;
		if (!isSquareStr(s.line(i)))
			return true;
	}
	return false;
}

std::vector<State> expand(State const& s) 
{
	//std::vector<int> order{0,1,2,3,4,5,6,7,8};
	std::vector<int> order{3,4,5,1,2,6,7,0,8};
	int i = 0;

	while (i < s.str.size() && s.str[order[i]] != '_')
		i++;

	if (i >= s.str.size())
		return {};

	std::vector<State> next;
	i = order[i];
	for (const auto& e : s.dom) {
		auto copy = s;
		copy.str[i] = e;
		copy.dom.erase(e);
		next.push_back(copy);
	}
	next.shrink_to_fit();
	return next;
}