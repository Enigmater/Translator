#include <iostream>
#include "defs.h"
#include "Scanner.h"
#include "Diagram.h"

using namespace std;

int RunScanner(int argc, char* argv[]);
int RunDiagram(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	return RunDiagram(argc, argv);
}

//************************************************************
// Главная программа транслятора - отладочный вариант,
// предназначенный для отладки сканера
//************************************************************
int RunScanner(int argc, char* argv[]) {
	Scanner* sc;

	// Choose file
	if (argc <= 1) sc = new Scanner("input.txt");	// default file
	else sc = new Scanner(argv[1]);					// arg file

	// Scan file
	int type;
	do {
		TypeLex lexeme;
		type = sc->Scan(lexeme);
		cout << lexeme << " - type is " << type << endl;
	} while (type != TEnd);

	return 0;
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
	dg->Z();

	TypeLex l;
	int type;
	type = sc->Scan(l);
	if (type == TEnd) cout << "Синтаксических ошибок не обнаружено" << endl;
	else sc->PrintError("Лишний текст в конце программы.", "");

	return 0;
}