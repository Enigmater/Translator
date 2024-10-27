//**********************************************
// Diagram.h - класс синтаксических диаграмм
//**********************************************
#pragma once
#include "Scanner.h"
class Diagram {
private:
	Scanner* sc;
	void C(); // описания
	void V(); // данные
	void B(); // тип
	void A(); // структура
	void F(); // оператор и описания
	void G(); // оператор
	void T(); // выражения
	void Y(); // сравнение
	void U(); // слагаемое
	void I(); // множитель
	void O(); // унарные операции
	void P(); // постфиксные операции
	void EL(); // эл. выр

	int lookForward(int steps);

public:
	void Z(); // программа
	Diagram(Scanner* sc) { this->sc = sc; }
	~Diagram() {}
};

