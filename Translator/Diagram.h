//**********************************************
// Diagram.h - ����� �������������� ��������
//**********************************************
#pragma once
#include "Scanner.h"
#include "Semant.h"

class Diagram {
private:
	Scanner* sc;
	void C(); // ��������
	void V(OBJECT_TYPE ot = TYPE_VAR); // ������
	DATA_TYPE B(); // ���
	void A(OBJECT_TYPE ot); // ���������
	void F(); // �������� � ��������
	void G(); // ��������
	void T(); // ���������
	void Y(); // ���������
	void U(); // ���������
	void I(); // ���������
	void O(); // ������� ��������
	void P(); // ����������� ��������
	void EL(); // ��. ���

	int lookForward(int steps);


public:
	Tree* root;
	void Z(); // ���������
	Diagram(Scanner* sc) { this->sc = sc; root = new Tree(); root->setScanner(sc); root->SetCurr(root); }
	~Diagram() {}
};

