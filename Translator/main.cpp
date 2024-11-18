#include <iostream>
#include "defs.h"
#include "Scanner.h"
#include "Diagram.h"

using namespace std;

int RunDiagram(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	return RunDiagram(argc, argv);
}

//************************************************************
// Главная программа транслятора - выполняется синтаксический
// анализ методом рекурсивного спуска до первой ошибки
//************************************************************
int RunDiagram(int argc, char* argv[]) {
	Scanner* sc;

	// Choose file
	if (argc <= 1) sc = new Scanner("input.txt");	// default file
	else sc = new Scanner(argv[1]);					// arg file

	Diagram* dg = new Diagram(sc);
	Tree* root = dg->root;
	try
	{
		dg->Z();
		TypeLex l;
		int type;
		type = sc->Scan(l);
		if (type == TEnd) cout << "Синтаксических ошибок не обнаружено" << endl;
		else sc->PrintError("Лишний текст в конце программы.", "");

		root->Print();
		delete sc;
		delete dg;
	}
	catch (const char* err) 
	{
		delete sc;
		delete dg;
		return -1;
	}
	return 0;
}