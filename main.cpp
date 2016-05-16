#include <iostream>
#include <fstream>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>

#include "AI_search.h"
#include "SquareWord.h"

State greedy(State s)
{
	while (s.c <= 5)
	{
		auto ch = expand(s);
		s = *std::min_element(begin(ch), end(ch), [](State const& l, State const& r) {
			return estim(l) < estim(r);
		});
	}
	return s;
}

std::pair<State,State> greedyMultiStartInfo(int n = 100)
{
	State mins;
	mins.init();
	int min = 100;
	while (n--)
	{
		State st;
		st.init();

		State sol = greedy(st);
		int est = estim(sol);
		if (est == 0) {
			return{ sol, sol };
		}
		if (est < min) {
			min = est;
			mins = sol;
		}
	}
	return{ mins,mins };
}

State greedyMultiStart(int n = 100)
{
	State mins;
	mins.init();
	int min = 100;
	while (n--)
	{
		State st;
		st.init();

		State sol = greedy(st);
		int est = estim(sol);
		if (est < min) {
			min = est;
			mins = sol;
		}
	}
	return mins;
}

template <class StateT>
void showWay(std::vector<StateT> const& way)
{
	using namespace std::chrono_literals;
	if (way.empty()) {
		std::cout << "beznadega.\n";
		return;
	}
	int n = 0;
	std::cout << "a - autopresent; p - present by promt;\nq - exit\n";
	char c;
	std::cin >> c;
	if (c == 'q') return;
	for (auto p = way.crbegin(); p != way.crend(); ++p)
	{
		system("cls");
		std::cout << n++ << '\n';
		std::cout << *p << '\n';
		if (c == 'a')
			std::this_thread::sleep_for(250ms);
		else
			system("pause");
	}
}

int main()
{
	using namespace std;
	using namespace std::chrono_literals;

	//State st, sol;
	//std::tie(st,sol) = greedyMultiStartInfo();
	//std::cout << st <<'\n' << sol << ' ' << estim(st) << '\n';

	State st; State sol;
	int est = 1;
	while (est != 0) {
		st.init();
		sol = greedy(st);
		est = estim(sol);
	}
	std::cout << "greedy soln\n" << sol << '\n';

	auto heu = [](const ais::Node<State>& a) {
		return estim(a.state);
	};
	auto isTerm = [](const State& w) { return estim(w) == 0; };

	ais::Search<State, decltype(heu), decltype(isTerm)> s(st, heu, isTerm);

	//std::thread t{ [&]() {
	//	while (!s.done()) {
	//		std::this_thread::sleep_for(500ms);
	//		auto cur = s.head;
	//		std::cout << s.i << ' ' << s.opened.size() << ' ' << s.closed.size() << ' '
	//			 << heu(cur) << ' ' << cur.treeLvl << '\n';
	//		std::cout << cur.state << '\n';
	//	}
	//}
	//};

	try {
		while (!s.done())
		{
			//auto cur = s.head;
			//int estim = heu(cur);
			//std::cout << s.i << ' ' << s.opened.size() << ' ' << s.closed.size() << '\n'
			//	<< estim << ' ' << cur.treeLvl
			//	<< ' ' << std::hex << std::hash<State>{}(cur.state) << std::dec << '\n';
			//std::cout << cur.state << '\n';
			//system("pause");
			//system("cls");
			s.core();
		}
		
	}
	catch (std::exception& e)
	{
		// попробуем обработать икслючение как std::bad_alloc - ошибка выделения памяти
		std::bad_alloc* ba = dynamic_cast<std::bad_alloc*>(&e);
		if (ba) {
			std::cerr << "Beda.\n";
			std::cout << s.head.state << '\n';
			system("pause");
		}
		else // в противном случае проблема не ясна
			std::cerr << "Ne ponimayu, chto sluchilos.\n";
		std::exit(1);
	}

	//t.join();
	std::cout << "A* sol\n" << s.head.state << '\n';

	std::cout << s.status << '\n' << "elapsed " << s.i << '\n';
	std::cout << std::boolalpha << (sol == s.head.state) << '\n';
	auto way = s.buildWay();

	std::ofstream os(R"(C:\Users\Daniel\Documents\visual studio 2015\Projects\AI_search\man_way.txt)");
	printWay(way, os);
	showWay(way);
}