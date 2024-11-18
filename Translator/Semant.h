#pragma once
#include "defs.h"
#include "Scanner.h"

enum DATA_TYPE {
	TYPE_UNKNOWN = 0, TYPE_INTEGER,
	TYPE_SHORTINT, TYPE_LONGINT, TYPE_CHAR, TYPE_STRUCTTYPE
};
enum OBJECT_TYPE {
	TYPE_EMPTY = 0, TYPE_VAR, TYPE_STRUCT, TYPE_STRUCTOBJ, TYPE_STRUCTFIELD, TYPE_FUNC
};

struct Node {
	TypeLex id; 			// ������������� �������
	DATA_TYPE dataType; 	// ��� ��������
	OBJECT_TYPE objectType;	// ��� �������
	char* data; 			// ������ �� �������� ��� NULL
	bool flagConst;			// ���� ��������� (true, ���� ���������)
	bool flagInit;			// ���� ���. ������������� (true, ���� ����.) 
};

class Tree {
private:
	Node* n;					// ������ �������
	Tree* up, * left, * right; 	// ��������, ����� � ������ �������
	static Tree* curr; 			// ������� ������� ������
	Scanner* sc;
public:
	Tree(Tree* l, Tree* r, Tree* u, Node* data);
	Tree();
	~Tree();

	void setScanner(Scanner* sc) { this->sc = sc; }
	// ������� ������ � Node
	void setConst(bool flag);
	void setInit(bool flag);

	char* getID();
	OBJECT_TYPE getObjType();

	// ������� ��������� ��������� ������
	void SetLeft(Node* data);
	void SetRight(Node* data);
	void SetCurr(Tree* node);									// ���������� ������� ���� ������
	Tree* GetCurr();											// �������� ������� ������� ������
	Tree* FindUp(Tree* from, TypeLex id);
	Tree* FindUp(TypeLex id);
	Tree* FindRightLeft(Tree* from, TypeLex id);
	Tree* FindRightLeft(TypeLex id);
	Tree* FindUpOneLevel(Tree* from, TypeLex id);
	void Print();

	// ������������� ������������
	Tree* SemInclude(TypeLex a, DATA_TYPE dt = TYPE_UNKNOWN, OBJECT_TYPE ot = TYPE_EMPTY);					// ��������� �������������� a � ������� � ����� t
	Tree* SemIncludeStructObj(TypeLex a, Tree* baseStruct);
	Tree* SemIncludeRightBlock();
	Tree* SemIncludeLeftBlock();
public:
	void SemSetType(Tree* Addr, DATA_TYPE t);					// ���������� ��� t ��� ��� ����������, ������� �������� � ������� �� ������ Addr
	Tree * SemGetNode(TypeLex a);								// ����� � ������� ���������� � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* SemGetStruct(TypeLex a);
	Tree* SemGetStructObject(TypeLex a);
	int CheckUniqueID(TypeLex a);								// �������� �� ������������ ���� ���������� � ����� ������� ���������
	int CheckSaveTypes(DATA_TYPE t1, DATA_TYPE t2);				// �������� ������������� ����� ��� ������������
	int CheckSummandTypes(DATA_TYPE t1, DATA_TYPE t2);			// �������� ����� ��������� ��� �������������� ��������(��������, ���������)
	int CheckMultiplierTypes(DATA_TYPE t1, DATA_TYPE t2);		// �������� ����� ��������� ��� �������������� ��������(���������, �������, ������� �� �������)
	int CheckEqualityTypes(DATA_TYPE t1, DATA_TYPE t2);			// �������� ����� ��� ���������� ���������(== , != )
	int CheckComparisonTypes(DATA_TYPE t1, DATA_TYPE t2);		// �������� ����� ��� ���������� ���������(> , >= , < , <= )
	int CheckStructAccess(Tree* _struct, TypeLex structField);// �������� ������������ ������� � ��������� ���������
	int CheckVisibility(TypeLex a);								// �������� ������� ��������� ����������
	int CheckInit(TypeLex a);									// �������� �� ������������� ���������� ��� �������������

};
