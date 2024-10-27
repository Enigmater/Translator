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
// ������� ��������� ����������� - ���������� �������,
// ��������������� ��� ������� �������
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
// ������� ��������� ����������� - ����������� ��������������
// ������ ������� ������������ ������ �� ������ ������
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
	if (type == TEnd) cout << "�������������� ������ �� ����������" << endl;
	else sc->PrintError("������ ����� � ����� ���������.", "");

	return 0;
}