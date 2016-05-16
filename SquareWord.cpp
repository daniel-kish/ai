#include "SquareWord.h"
#include <algorithm>
#include <random>


State::State()
{
	area.insert({ 0,0 });
	area.insert({ 0,1 });
	area.insert({ 0,2 });
	area.insert({ 0,3 });
	area.insert({ 0,4 });
	area.insert({ 0,5 });

	area.insert({ 2,3 });
	area.insert({ 2,4 });
	area.insert({ 2,5 });

	area.insert({ 5,1 });
	area.insert({ 5,2 });
	area.insert({ 5,3 });
	t.resize(36);
	for (int i = 0; i < 6; ++i)
		for (int j = 0; j < 6; ++j)
			e(i,j) = '_';
	char word1[7] = "brusok";
	for (int i = 0; i < 6; ++i)
		e(0,i) = word1[i];

	char word2[4] = "bur";
	for (int i = 3; i < 6; ++i)
		e(2,i) = word2[i - 3];

	char word3[4] = "bor";
	for (int i = 1; i < 4; ++i)
		e(5,i) = word3[i - 1];

	for (int i = 0; i < 6; ++i)
		dom.insert(word1[i]);
	for (int i = 0; i < 3; ++i)
		dom.insert(word2[i]);
	for (int i = 0; i < 3; ++i)
		dom.insert(word3[i]);
}

void State::init()
{
	using namespace std;
	for (int col = 0; col < 6; col++)
	{
		std::set<char> colDom = dom;
		// erase all present in this col
		for (auto& pos : area)
		{
			if (pos.second != col)
				continue;
			colDom.erase(e(pos.first, pos.second));
		}
		std::string str(colDom.begin(), colDom.end());

		std::random_device rd{};
		std::mt19937 m{rd()};
		std::shuffle(str.begin(), str.end(), m);
		//std::sort(begin(str), end(str));
		
		for (int i = 0, n=0; n < str.size(); ++i) {
			if (area.find({ i, col }) == area.end()) {
				e(i, col) = str[n++];
			}
		}
	}
}

char& State::operator()(int i, int j)
{
	return t[i * 6 + j];
}

const char & State::operator()(int i, int j) const
{
	return t[i * 6 + j];
}

const char & State::e(int i, int j) const
{
	return t[i * 6 + j];
}

char& State::e(int i, int j)
{
	return t[i * 6 + j];
}

bool operator==(const State & lhs, const State & rhs)
{
	return lhs.t == rhs.t;
}

std::ostream& operator<<(std::ostream& s, State const& st)
{
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 6; ++j)
			s << st(i,j) << ' ';
		s << '\n';
	}
	return s;
}

std::vector<State> expand(State const& st)
{
	using namespace std;

	const auto& area = st.area;

	if (st.c > 5)
		return {};

	int c = st.c;

	string col; 
	col.reserve(6);
	for (int r = 0; r < 6; r++)
	{
		if (area.find({ r,c }) == area.end())
			col.push_back(st(r,c));
	}
	col.shrink_to_fit();

	std::sort(begin(col), end(col));

	std::vector<State> next;

	do {
		State copy{st};
		for (int r = 0, n = 0; n < col.size(); r++) {
			if (area.find({ r,c }) == area.end())
				copy(r,c) = col[n++];
		}
		copy.c = c + 1;
		next.push_back(copy);
	} while (next_permutation(begin(col), end(col)));

	st.c++;
	return next;
}

void printWay(const std::vector<State>& way, std::ostream& s)
{
	using namespace std;

	if (way.empty()) {
		s << "no way\n";
		return;
	}
	int n = 0;

	for (auto p = way.crbegin(); p != way.crend(); ++p)
	{
		s << "pos " << n++ << '\n';
		s << *p << '\n';
	}
}

int estim(State const& s)
{
	int total = 0;

	for (int r = 0; r < 6; r++)
	{
		std::set<char> rowchars;
		for (int c = 0; c < 6; c++)
			rowchars.insert(s(r,c));
		total += 6 - rowchars.size();
	}

	std::set<char> diagchars;
	for (int r = 0; r <= 5; r++)
	{
		diagchars.insert(s(r,r));
	}
	total += 6 - diagchars.size();

	return total;
}