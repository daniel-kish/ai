#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <set>

using namespace std;
struct State;
std::ostream& operator<<(std::ostream& s, State const& st);
struct State
{
	char t[6][6];
	std::set<char> dom;
	std::set<pair<int,int>> area;
	State()
	{
		area.insert({0,0});
		area.insert({0,1});
		area.insert({0,2});
		area.insert({0,3});
		area.insert({0,4});
		area.insert({0,5});

		area.insert({2,3});
		area.insert({2,4});
		area.insert({2,5});

		area.insert({5,1});
		area.insert({5,2});
		area.insert({5,3});

		for(int i=0; i < 6; ++i)
			for(int j=0; j < 6; ++j)
				t[i][j]='_';
		char word1 [7] = "brusok";
		for(int i=0; i < 6; ++i)
			t[0][i] = word1[i];

		char word2 [4] = "bur";
		for(int i=3; i < 6; ++i)
			t[2][i] = word2[i-3];

		char word3 [4] = "bor";
		for(int i=1; i < 4; ++i)
			t[5][i] = word3[i-1];

		for(int i = 0; i < 6; ++i)
			dom.insert(word1[i]);
		for(int i = 0; i < 3; ++i)
			dom.insert(word2[i]);
		for(int i = 0; i < 3; ++i)
			dom.insert(word3[i]);
	}
	void init()
	{
		for (int col = 0; col < 6; col++)
		{
			std::set<char> colDom = dom;
			// erase all present in this col
			for(auto& pos : area)
			{
				if (pos.second != col)
					continue;
				colDom.erase(t[pos.first][pos.second]);
			}
			std::string str(colDom.begin(), colDom.end());
			std::mt19937 m;
			std::shuffle(str.begin(), str.end(), m);

			for (int i=0; i < 6; ++i)
			{
				if(area.find(make_pair(i,col)) == area.end())
				{
					t[i][col] = str.back();
					str.pop_back();
				}
			}
		}
	}

	int estim(State const& s)
	{
		int total=0;

		for (int r=0; r < 6; r++)
		{
			std::set<char> rowchars;
			for (int c=0; c < 6; c++)
				rowchars.insert(s.t[r][c]);
			total += 6 - rowchars.size();
		}
		return total;
	}
	State generation()
	{
		string col; col.reserve(6);
		int c=0;
		for(int r=0; r < 6; r++)
		{
			if (area.find({r,c}) == area.end())
				col.push_back(t[r][c]);
		}
		std::sort(begin(col), end(col));
		int min = 100;
		State minState, cur(*this);
		do {
			for(int r=0, n=0; r < 6; r++)
			{
				if (area.find({r,c}) == area.end())
					cur.t[r][c] = col[n++];
			}
			int est = estim(cur);
			std::cout << cur << '\n'<< est << '\n' ;
			if (est < min)
			{
				min = est;
				minState = cur;
			}
		} while(next_permutation(col.begin(), col.end()));

		return minState;
	}
};

std::ostream& operator<<(std::ostream& s, State const& st)
{
	for(int i=0; i < 6; ++i)
	{
		for(int j=0; j < 6; ++j)
			s << st.t[i][j] << ' ';
		s << '\n';
	}
	return s;
}



int main()
{
	State s;

	std::cout << s;

	s.init();

	std::cout << '\n' << s << '\n';

	auto g = s.generation();
	std::cout << '\n' << g << '\n';

//	system("PAUSE");
}
