#pragma once

#include <set>
#include <utility>
#include <string>
#include <vector>
#include <iostream>

struct State
{
	std::string t;
	std::set<char> dom;
	std::set<std::pair<int, int>> area;
	State();
	void init();
	mutable int c = 0;
	char& operator()(int i, int j);
	const char& operator()(int i, int j) const;
	char& e(int i, int j);
	const char& e(int i, int j) const;
};

bool operator==(const State& lhs, const State& rhs);

std::ostream& operator<<(std::ostream& s, State const& st);

std::vector<State> expand(State const&);

void printWay(const std::vector<State>& way, std::ostream& s = std::cout);

namespace std
{
	template<> struct hash<State>
	{
		std::size_t operator()(State const& state) const
		{
			return std::hash<std::string>{}(state.t);
		}
	};
}


int estim(State const& s);