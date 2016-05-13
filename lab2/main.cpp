#include <iostream>
#include "State.h"
#include <list>
#include <thread>
#include <chrono>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <random>
using namespace std::chrono_literals;

std::vector<State> sols; // ���������� ������ �������
int n = 0;				 // ������� ����������� �������

// �������� ��������� ��� ������������ ��������
struct less {
	bool operator()(State const& l, State const& r)
	{
		return countSquareStr(l) < countSquareStr(r);
	}
};

// ������������� ������ �������� (��� ��������������� ������������)
void shuffle(std::vector<State> & v)
{
	static std::random_device rd{}; // ��������� �������� ��������� ����� ("��������")

	// ������������� � ������� ����� ��������, ������������������� � ������� 'rd'
	std::shuffle(begin(v), end(v), std::mt19937{ rd() });
}

// ���������� �����������
bool bt(State const& s) {
	n++;

	if (reject(s))	return false;
	if (accept(s)) {
		sols.push_back(s);
		if (sols.size() == 4) // ����� �������� � �������� 4 �������
			return true;
		return false;
	}
	auto ch = expand(s); // ���������� ��������

	//shuffle(ch);							 // ��� ����. ������������
	//std::sort(begin(ch), end(ch), less{}); // ��� �������� � �����������

	std::make_heap(begin(ch), end(ch), less{}); // ��������� ������������
	while (!ch.empty()) {
		// ���������� ����. ������� � ����� � ��������� ����. ������� ����������� ������
		std::pop_heap(begin(ch), end(ch), less{}); 
		auto best = ch.back(); // ��������� ������� ����. �������
		ch.pop_back();		   // ������� ��� �� ������
		if (bt(best))		   // ���������� �������� BT
			return true;
	}
	return false;
}

void stat()
{
	const int times = 500;
	double sum = 0.0;

	for (int i = 0; i < times; ++i) {
		State s;
		bt(s);
		sols.clear();
		sum += n;
		//std::cout << n << '\n';
		n = 0;
	}
	std::cout << "mean " << sum / double(times) << '\n';
}

int main()
{
	//stat(); // ������ ��������������� ������������
	State s;
	std::cout << bt(s) << '\n';
	for (const auto& e : sols)
		std::cout << e << '\n';
	
	std::cout << n << '\n';
}