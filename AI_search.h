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


	// ���� ����� �������� � �������� �� ��������� ���� ������ �������

	// ������� ������ �������/��������� State, ������������ �� � ������� ������ Node<State>
	template <class State>
	struct Node {
		Node(State s, const Node* p, int t)
			:state{ s }, parent{ p }, treeLvl{ t }
		{}
		State state; // ���������� ������� ���������
		const Node* parent; // ��������; ����� null_ptr, ���� ��� ������ ������
		int treeLvl; // ������� ������
	};

	template <class State>
	bool operator==(Node<State> const& l, Node<State> const& r)
	{
		return l.state == r.state;
	}

	// ����������� �������� ������ �������� � ����������� ������ ���������
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
	std::ostream& operator<< (std::ostream& os, Node<State>const& s) // ������ ������� ��� ���������� ������
	{
		std::cout << s.treeLvl << ' ' << std::hex << std::hash<State>{}(s.state) << std::dec << '\n';
		std::cout << s.state;
		if (s.parent != nullptr)
			std::cout << std::hex << std::hash<State>{}(s.parent->state) << std::dec;
		return os;
	}

	// �������� ���������, �������������� ������ ������� � ����������� (������ "������")
	// � ��� ������� ���������� ������������ ���������
	template <class State, class Heu>
	struct greater {
		Heu heu;
		greater(Heu _heu) : heu{ _heu }
		{}
		bool operator()(const Node<State>& a, const Node<State>& b) {
			auto fa = a.treeLvl + heu(a); // ��������� ������ f(x) = g(x) + h(x)
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

	enum class Status { notYet, success, failure }; // ������ ���������� ������
	std::ostream& operator<< (std::ostream& str, Status s) // ������ ������� ��� ���������� ������
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
		// ��� ��������� ����������� ��� ������� � ����������� � ���-�������
		using OpenedLst = ais::priority_queue<State, Heu>;
		using ClosedLst = std::unordered_set<Node<State>, NodeHash<State>>;

		OpenedLst opened;
		ClosedLst closed;
		Status status{ Status::notYet };
		Node<State> head;
		typename ClosedLst::iterator parent; // ������� ������� ������� 'head' � ���-�������
		Heu heu;
		Term isTerm;
		int i{ 0 };
	public:
		Search(State const& _start, Heu _heu, Term _term)
			: opened(_heu), head(_start, nullptr, 0), heu{ _heu }, isTerm{_term}
		{
			opened.push(head); // �������������
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

			auto res = closed.insert(head); // ������� �������� � ������ "������"

			parent = res.first;

			auto children = expand(head.state); // ���������� ��������

			// � ����� �� ���� ��������
			for (auto p = begin(children); p != end(children); ++p) {
				const Node<State>& n = *parent;
				Node<State> childNode(*p, std::addressof(n), head.treeLvl + 1); // ��������� ������� ������
				if (closed.find(childNode) != closed.end())
					continue;
				opened.push(childNode); // "���������"
			}
		}

		std::vector<State> buildWay() // ����������� ���� �� ������ "������"
		{
			if (status != ais::Status::success)
				return{};

			const Node<State>& n = *parent;
			const Node<State>* next = std::addressof(n);
			std::vector<State> way;

			while (next != nullptr) { // ���� � �������� ������� �� ���������� �������� �� ��������� �� ��������
				way.push_back(next->state);
				next = next->parent;
			}
			return way;
		}
	};
}