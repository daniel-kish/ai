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

std::vector<State> sols; // глобальный список решений
int n = 0;				 // счетчик рекурсивных вызовов

// оператор сравнения для упорядочения потомков
struct less {
	bool operator()(State const& l, State const& r)
	{
		return countSquareStr(l) < countSquareStr(r);
	}
};

// перемешивание списка потомков (для статитисческого тестирования)
void shuffle(std::vector<State> & v)
{
	static std::random_device rd{}; // системный источник случайных чисел ("энтропии")

	// перемешивание с помощью вихря Мерсенна, инициализированного с помощью 'rd'
	std::shuffle(begin(v), end(v), std::mt19937{ rd() });
}

// реализация бэктрекинга
bool bt(State const& s) {
	n++;

	if (reject(s))	return false;
	if (accept(s)) {
		sols.push_back(s);
		if (sols.size() == 4) // всего возможно в принципе 4 решения
			return true;
		return false;
	}
	auto ch = expand(s); // порождение потомков

	//shuffle(ch);							 // для стат. тестирования
	//std::sort(begin(ch), end(ch), less{}); // для варианта с сортировкой

	std::make_heap(begin(ch), end(ch), less{}); // частичное упорядочение
	while (!ch.empty()) {
		// перемещает макс. элемент в конец и следующий макс. элемент оказывается первым
		std::pop_heap(begin(ch), end(ch), less{}); 
		auto best = ch.back(); // извлекаем текущий макс. элемент
		ch.pop_back();		   // удаляем его из списка
		if (bt(best))		   // рекурсивно вызываем BT
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
	//stat(); // запуск статистического тестирования
	State s;
	std::cout << bt(s) << '\n';
	for (const auto& e : sols)
		std::cout << e << '\n';
	
	std::cout << n << '\n';
}