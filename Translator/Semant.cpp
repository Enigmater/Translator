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

void Tree::setConst(bool flag)
{
	n->flagConst = flag;
}

void Tree::setInit(bool flag)
{
	n->flagInit = flag;
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
	// Поиск в правом поддереве и затем в левом
	if (from->right) {
		Tree* res = FindUp(from->right, id);
		if (res) return res;
	}
	return FindUp(from->left, id);
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
	if (right != nullptr) right->Print();
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
	if (dt == TYPE_STRUCTTYPE) return SemIncludeBlock();
	return curr;
}

Tree* Tree::SemIncludeBlock()
{
	Tree* returnNode = curr;
	Node* blockNode = new Node();
	memcpy(blockNode->id, "Block", 5);
	curr->SetRight(blockNode);
	curr = curr->right;
	return returnNode;
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

int Tree::CheckUniqueID(TypeLex a)
{
	if (FindUpOneLevel(curr, a)) return -1;
	return 0;
}

int Tree::CheckSaveTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// Преобразование целых типов (Будет доработано в ЛР 9)
	if (t1 == t2) return 0;
	if ((t1 == TYPE_INTEGER && t2 == TYPE_SHORTINT) ||
		(t1 == TYPE_SHORTINT && t2 == TYPE_INTEGER)) {
		return 0; 
	}
	return -1;
}

int Tree::CheckSummandTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// Проверка типов для сложения и вычитания (Будет доработано в ЛР 9)
	if ((t1 == TYPE_INTEGER || t1 == TYPE_SHORTINT || t1 == TYPE_LONGINT) &&
		(t2 == TYPE_INTEGER || t2 == TYPE_SHORTINT || t2 == TYPE_LONGINT)) {
		return 0;
	}
	return -1;
}

int Tree::CheckMultiplierTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// Проверка типов для * / % (Будет доработано в ЛР 9)
	return CheckSummandTypes(t1, t2);
}

int Tree::CheckEqualityTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// (Будет доработано в ЛР 9)
	return (t1 == t2) ? 0 : -1;
}

int Tree::CheckComparisonTypes(DATA_TYPE t1, DATA_TYPE t2)
{
	// (Будет доработано в ЛР 9)
	return (t1 == t2) ? 0 : -1;
}

int Tree::CheckStructAccess(TypeLex structField)
{
	// (Будет доработано в ЛР 9)
	return 0;
}

int Tree::CheckVisibility(TypeLex a)
{
	if (FindUp(a)) return 0;
	sc->PrintError("Идентификатор не определен!", a);
}

int Tree::CheckInit(TypeLex a)
{
	Tree* node = SemGetNode(a);
	if (node && node->n && !node->n->flagInit) return -1;
	return 0;
}
