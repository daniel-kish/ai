#pragma once
#include <vector>
#include <unordered_set>
#include <utility>
#include <queue>
#include <stack>
#include <utility>
#include <tuple>
#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <sstream>

#include "SquareWord.h"

namespace ais {


	// весь поиск построен в изол€ции от реального типа данных позиций

	// обертка вокруг позиции/состо€ни€ State, превращающа€ ее в элемент дерева Node<State>
	template <class State>
	struct Node {
		Node(State s, const Node* p, int t)
			:state{ s }, parent{ p }, treeLvl{ t }
		{}
		State state; // собственно игровое состо€ние
		const Node* parent; // родитель; равен null_ptr, если это корень дерева
		int treeLvl; // уровень дерева
	};

	template <class State>
	bool operator==(Node<State> const& l, Node<State> const& r)
	{
		return l.state == r.state;
	}

	// хеширование элемента дерева сводитс€ к хешированию самого состо€ни€
	template <class State>
	struct NodeHash
	{
		typedef Node<State> argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const
		{
			return std::hash<State>()(s.state);
		}
	};
	template <class State>
	std::ostream& operator<< (std::ostream& os, Node<State>const& s) // печать статуса дл€ отладочной печати
	{
		std::cout << s.treeLvl << ' ' << std::hex << std::hash<State>{}(s.state) << std::dec << '\n';
		std::cout << s.state;
		if (s.parent != nullptr)
			std::cout << std::hex << std::hash<State>{}(s.parent->state) << std::dec;
		return os;
	}

	// оператор сравнени€, обеспечивающий работы очереди с приоритетом (списка "открыт")
	// с его помощью проводитс€ упор€дочение элементов
	template <class State, class Heu>
	struct greater {
		Heu heu;
		greater(Heu _heu) : heu{ _heu }
		{}
		bool operator()(const Node<State>& a, const Node<State>& b) {
			auto fa = a.treeLvl + heu(a); // суммарна€ оценка f(x) = g(x) + h(x)
			auto fb = b.treeLvl + heu(b);
			if (fa == fb)
				return a.treeLvl > b.treeLvl;
			return fa > fb;
		}
	};

	template <class State, class Heu>
	class priority_queue : public std::priority_queue<Node<State>, std::vector<Node<State>>, greater<State, Heu>>
	{
	public:
		priority_queue(Heu heu)
			: std::priority_queue<Node<State>, std::vector<Node<State>>, greater<State, Heu>>(heu)
		{}
		typename container_type::iterator begin() { return this->c.begin(); }
		typename container_type::iterator end() { return this->c.end(); }
	};

	enum class Status { notYet, success, failure }; // статус выполнени€ поиска
	std::ostream& operator<< (std::ostream& str, Status s) // печать статуса дл€ отладочной печати
	{
		if (s == Status::failure)
			str << 'f';
		else if (s == Status::notYet)
			str << 'n';
		else if (s == Status::success)
			str << 's';
		return str;
	}

	template <class State, class Heu, class Term>
	struct Search
	{
		// дл€ краткости переименуем тип очереди с приоритетом и хеш-таблицы
		using OpenedLst = ais::priority_queue<State, Heu>;
		using ClosedLst = std::unordered_set<Node<State>, NodeHash<State>>;

		OpenedLst opened;
		ClosedLst closed;
		Status status{ Status::notYet };
		Node<State> head;
		typename ClosedLst::iterator parent; // текуща€ позици€ вершины 'head' в хеш-таблице
		Heu heu;
		Term isTerm;
		int i{ 0 };
	public:
		Search(State const& _start, Heu _heu, Term _term)
			: opened(_heu), head(_start, nullptr, 0), heu{ _heu }, isTerm{_term}
		{
			opened.push(head); // инициализаци€
		}

		bool done() {
			return (status == Status::failure || status == Status::success);
		}

		void core()
		{
			using namespace std;

			++i;
			if (opened.empty()) {
				status = Status::failure;
				return;
			}
			head = opened.top();
			opened.pop();
			if ( isTerm(head.state) ) {
				auto p = closed.insert(head);
				parent = p.first;
				status = Status::success;
				return;
			}

			auto res = closed.insert(head); // попытка вставить в список "закрыт"

			parent = res.first;

			auto children = expand(head.state); // порождение потомков

			// в цикле по всем потомкам
			for (auto p = begin(children); p != end(children); ++p) {
				const Node<State>& n = *parent;
				Node<State> childNode(*p, std::addressof(n), head.treeLvl + 1); // формируем элемент дерева
				if (closed.find(childNode) != closed.end())
					continue;
				opened.push(childNode); // "открываем"
			}
		}

		std::vector<State> buildWay() // расшифровка пути по списку "закрыт"
		{
			if (status != ais::Status::success)
				return{};

			const Node<State>& n = *parent;
			const Node<State>* next = std::addressof(n);
			std::vector<State> way;

			while (next != nullptr) { // идем в обратную сторону от последнего элемента по указателю на родител€
				way.push_back(next->state);
				next = next->parent;
			}
			return way;
		}
	};
}