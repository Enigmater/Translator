//**********************************************
// Diagram.h - ����� �������������� ��������
//**********************************************
#pragma once
#include "Scanner.h"
class Diagram {
private:
	Scanner* sc;
	void C(); // ��������
	void V(); // ������
	void B(); // ���
	void A(); // ���������
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
	void Z(); // ���������
	Diagram(Scanner* sc) { this->sc = sc; }
	~Diagram() {}
};

