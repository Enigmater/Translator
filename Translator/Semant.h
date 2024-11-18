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
	TypeLex id; 			// идентификатор объекта
	DATA_TYPE dataType; 	// тип значения
	OBJECT_TYPE objectType;	// тип объекта
	char* data; 			// ссылка на значение или NULL
	bool flagConst;			// флаг константы (true, если константа)
	bool flagInit;			// флаг нач. инициализации (true, если иниц.) 
};

class Tree {
private:
	Node* n;					// данные таблицы
	Tree* up, * left, * right; 	// родитель, левый и правый потомок
	static Tree* curr; 			// текущий элемент дерева
	Scanner* sc;
public:
	Tree(Tree* l, Tree* r, Tree* u, Node* data);
	Tree();
	~Tree();

	void setScanner(Scanner* sc) { this->sc = sc; }
	// ФУНКЦИИ РАБОТЫ С Node
	void setConst(bool flag);
	void setInit(bool flag);

	char* getID();
	OBJECT_TYPE getObjType();

	// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА
	void SetLeft(Node* data);
	void SetRight(Node* data);
	void SetCurr(Tree* node);									// Установить текущий узел дерева
	Tree* GetCurr();											// Получить текущий уровень дерева
	Tree* FindUp(Tree* from, TypeLex id);
	Tree* FindUp(TypeLex id);
	Tree* FindRightLeft(Tree* from, TypeLex id);
	Tree* FindRightLeft(TypeLex id);
	Tree* FindUpOneLevel(Tree* from, TypeLex id);
	void Print();

	// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
	Tree* SemInclude(TypeLex a, DATA_TYPE dt = TYPE_UNKNOWN, OBJECT_TYPE ot = TYPE_EMPTY);					// Занесение идентификатора a в таблицу с типом t
	Tree* SemIncludeStructObj(TypeLex a, Tree* baseStruct);
	Tree* SemIncludeRightBlock();
	Tree* SemIncludeLeftBlock();
public:
	void SemSetType(Tree* Addr, DATA_TYPE t);					// Установить тип t для той переменной, которая хранится в таблице по адресу Addr
	Tree * SemGetNode(TypeLex a);								// Найти в таблице переменную с именем a и вернуть ссылку на соответствующий элемент дерева
	Tree* SemGetStruct(TypeLex a);
	Tree* SemGetStructObject(TypeLex a);
	int CheckUniqueID(TypeLex a);								// Проверка на уникальность имен переменных в одной области видимости
	int CheckSaveTypes(DATA_TYPE t1, DATA_TYPE t2);				// Проверка совместимости типов при присваивании
	int CheckSummandTypes(DATA_TYPE t1, DATA_TYPE t2);			// Проверка типов операндов для арифметических операций(сложение, вычитание)
	int CheckMultiplierTypes(DATA_TYPE t1, DATA_TYPE t2);		// Проверка типов операндов для арифметических операций(умножение, деление, остаток от деления)
	int CheckEqualityTypes(DATA_TYPE t1, DATA_TYPE t2);			// Проверка типов для операторов сравнения(== , != )
	int CheckComparisonTypes(DATA_TYPE t1, DATA_TYPE t2);		// Проверка типов для операторов сравнения(> , >= , < , <= )
	int CheckStructAccess(Tree* _struct, TypeLex structField);// Проверка корректности доступа к элементам структуры
	int CheckVisibility(TypeLex a);								// Проверка области видимости переменных
	int CheckInit(TypeLex a);									// Проверка на использования переменных без инициализации

};
