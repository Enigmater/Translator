//**********************************************
// Diagram.h - класс синтаксических диаграмм
//**********************************************
#pragma once
#include "Scanner.h"
#include "Semant.h"

class Diagram {
private:
	Scanner* sc;
	void C(); // описания
	void V(OBJECT_TYPE ot = TYPE_VAR); // данные
	DATA_TYPE B(); // тип
	void A(OBJECT_TYPE ot); // структура
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
	Tree* root;
	void Z(); // программа
	Diagram(Scanner* sc) { this->sc = sc; root = new Tree(); root->setScanner(sc); root->SetCurr(root); }
	~Diagram() {}
};

