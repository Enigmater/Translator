//************************************************************
// модуль Scaner.h --- класс сканера
//************************************************************
#pragma once
#include "defs.h"
class Scanner
{
private:
	TypeMod moduleText; // исходный текст
	int uk; // указатель текущей позиции в исходном тексте
public:
	void SetUK(int uk);
	int GetUK();
	void PrintError(const char* err, const char* a);
	int Scan(TypeLex lexeme);
	void GetData(const char* filename);
	Scanner(const char* filename);
	~Scanner();

};

