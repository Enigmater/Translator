#define _CRT_SECURE_NO_WARNINGS
#include "Semant.h"
#include <iostream>


Tree* Tree::curr;

Tree::Tree(Tree* l, Tree* r, Tree* u, Node* data)
{
	this->left = l;
	this->right = r;
	this->up = u;
	this->n = data;
}

Tree::Tree()
{
	left = nullptr;
	right = nullptr;
	up = nullptr;
	n = new Node();
}

Tree::~Tree()
{
	delete left;
	delete right;
	delete up;
	delete n;
}


void Tree::setConst(bool flag)
{
	n->flagConst = flag;
}

void Tree::setInit(bool flag)
{
	n->flagInit = flag;
}

char* Tree::getID() {
	return this->n->id;
}

OBJECT_TYPE Tree::getObjType() {
	return n->objectType;
}

void Tree::SetLeft(Node* data)
{
	left = new Tree(nullptr, nullptr, nullptr, data);
	left->up = this;
}

void Tree::SetRight(Node* data)
{
	right = new Tree(nullptr, nullptr, nullptr, data);
	right->up = this;
}

void Tree::SetCurr(Tree* node)
{
	Tree::curr = node;
}

Tree* Tree::GetCurr()
{
	return this->curr;
}

Tree* Tree::FindUp(Tree* from, TypeLex id)
{
	// Поиск идентификатора в родительских узлах дерева
	while (from != nullptr) {
		if (from->n && memcmp(id, from->n->id, std::max(strlen(id), strlen(from->n->id))) == 0)
			return from;
		from = from->up;
	}
	return nullptr;
}

Tree* Tree::FindUp(TypeLex id)
{
	return FindUp(GetCurr(), id);
}

Tree* Tree::FindRightLeft(Tree* from, TypeLex id)
{
	Tree* i = from->right;
	while (i != nullptr) {
		if (!strcmp(id, i->n->id)) return i;
		i = i->left;
	}
	return nullptr;
}

Tree* Tree::FindRightLeft(TypeLex id)
{
	return FindRightLeft(GetCurr(), id);
}

void Tree::Print()
{
	std::cout << "Вершина с данными " << n->id << " -----> ";
	if (left != nullptr) std::cout << " слева данные " << left->n->id;
	if (right != nullptr) std::cout << " справа данные " << right->n->id;
	std::cout << "\n";
	// для устранения зацикливания при рекурсивном выводе
	if (getObjType() != TYPE_STRUCTOBJ && right != nullptr) right->Print();
	if (left != nullptr) left->Print();
}

Tree* Tree::FindUpOneLevel(Tree* from, TypeLex id)
{
	while (from != nullptr && (from->up != nullptr && from->up->right != from)) {
		if (from->n && memcmp(id, from->n->id, std::max(strlen(id), strlen(from->n->id))) == 0)
			return from;
		from = from->up;
	}
	return nullptr;
}


Tree* Tree::SemInclude(TypeLex a, DATA_TYPE dt, OBJECT_TYPE ot)
{
	if (CheckUniqueID(a) == -1) sc->PrintError("Объект с таким именем уже существует!", a);

	Node* b = new Node;
	memcpy(b->id, a, strlen(a) + 1);	// set id
	b->dataType = dt;					// set data type
	b->objectType = ot;					// set obj type
	b->data = nullptr;					// set data
	curr->SetLeft(b);
	curr = curr->left;
	if (dt == TYPE_STRUCTTYPE) return SemIncludeRightBlock();
	return curr;
}

Tree* Tree::SemIncludeStructObj(TypeLex a, Tree* baseStruct) {
	Node* b = new Node;
	memcpy(b->id, a, strlen(a) + 1);	// set id
	b->dataType = TYPE_STRUCTTYPE;		// set data type
	b->objectType = TYPE_STRUCTOBJ;		// set obj type
	b->data = nullptr;					// set data
	curr->SetLeft(b);
	curr = curr->left;
	curr->right = baseStruct;
	return curr;
}

Tree* Tree::SemIncludeRightBlock()
{
	Tree* returnNode = curr;
	Node* blockNode = new Node();
	memcpy(blockNode->id, "Block", 5);
	curr->SetRight(blockNode);
	curr = curr->right;
	return returnNode;
}

Tree* Tree::SemIncludeLeftBlock()
{
	//Tree* returnNode = curr;
	Node* blockNode = new Node();
	memcpy(blockNode->id, "Block", 5);
	curr->SetLeft(blockNode);
	curr = curr->left;
	return curr;
}

void Tree::SemSetType(Tree* addr, DATA_TYPE t)
{
	if (addr && addr->n) addr->n->dataType = t;
}

Tree* Tree::SemGetNode(TypeLex a)
{
	Tree* v = FindUp(a);
	if (v == nullptr) sc->PrintError("Отсутствует описание идентификатора!", a);
	return v;
}

Tree* Tree::SemGetStruct(TypeLex a) 
{	
	Tree* v = FindUp(a);
	if (v == nullptr) sc->PrintError("Такой структуры не существует!", a);
	return v;
}

Tree* Tree::SemGetStructObject(TypeLex a)
{
	Tree* v = FindUp(a);
	if (v == nullptr) {
		v = FindRightLeft(a);
		if (v == nullptr) sc->PrintError("Объект структуры не существует!", a);
	}
	return v;
}

int Tree::CheckUniqueID(TypeLex a)
{
	if (FindUpOneLevel(curr, a)) return -1;
	return 0;
}

int Tree::CheckSaveTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// Преобразование целых типов (Будет доработано в Интерпретации)
	if (t1 == t2) return 0;
	if ((t1 == TYPE_INTEGER && t2 == TYPE_SHORTINT) ||
		(t1 == TYPE_SHORTINT && t2 == TYPE_INTEGER)) {
		return 0; 
	}
	return -1;
}

int Tree::CheckSummandTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// Проверка типов для сложения и вычитания (Будет доработано в Интерпретации)
	if ((t1 == TYPE_INTEGER || t1 == TYPE_SHORTINT || t1 == TYPE_LONGINT) &&
		(t2 == TYPE_INTEGER || t2 == TYPE_SHORTINT || t2 == TYPE_LONGINT)) {
		return 0;
	}
	return -1;
}

int Tree::CheckMultiplierTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// Проверка типов для * / % (Будет доработано в Интерпретации)
	return CheckSummandTypes(t1, t2);
} 

int Tree::CheckEqualityTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// (Будет доработано в Интерпретации)
	return (t1 == t2) ? 0 : -1;
}

int Tree::CheckComparisonTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// (Будет доработано в Интерпретации)
	return (t1 == t2) ? 0 : -1;
}

int Tree::CheckStructAccess(Tree* _struct, TypeLex structField)
{
	if (FindRightLeft(_struct->right, structField) == nullptr) {
		char* str = new char{};
		strcat(strcat(strcat(str, "Структура <"), _struct->getID()), "> не имеет такого поля!");
		sc->PrintError(str, structField);
	}
	return 0;
}

int Tree::CheckVisibility(TypeLex a)
{
	if (FindUp(a)) return 0;
	sc->PrintError("Идентификатор не определен!", a);
}