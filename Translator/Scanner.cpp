//************************************************************
//модуль Scaner.cpp --- реализация методов класса сканера
//************************************************************
#include "Scanner.h"
#include <iostream>
#include <fstream>

using namespace std;

TypeLex keyword[MAX_KEYW] = {
	"if", "else", "while", "for", "const", "return", "struct", "main", "int", "short", "long", "char"
};

int indexKeyword[MAX_KEYW] = { 
	TIF, TElse, TWhile, TFor, TConst, TReturn, TStruct, TMain, TInt, TShort, TLong, TChar
};

void Scanner::SetUK(int uk)
{
	this->uk = uk;
}

int Scanner::GetUK()
{
	return this->uk;
}

void Scanner::PrintError(const char* err, const char* a)
{
	cout << "Error: " << err;
	if (a[0] != '\0') cout << " -> Wrong character: " << a << endl;
	throw("Fatal error!");
	exit(0);
}

int Scanner::Scan(TypeLex lexeme)
{
	int i = 0; // current lexeme length

	// clear lexeme
	for (i; i < MAX_LEX; i++) lexeme[i] = 0; 

	i = 0;
start: 
	/* skip ignored characters */
	while (moduleText[uk] == ' ' || moduleText[uk] == '\n' || moduleText[uk] == '\t' || moduleText[uk] == '\r') uk++;

	/* skip comments */
	if (moduleText[uk] == '/' && moduleText[uk + 1] == '/') { 
		uk = uk + 2; // first char after "//"
		while (moduleText[uk] != '\n' || moduleText[uk] == '#') uk++;
		goto start;
	}
	if ((moduleText[uk] == '/') && (moduleText[uk + 1] == '*')) {
		uk = uk + 2; // first char after "/*"
		while ((moduleText[uk] != '*' && moduleText[uk + 1] != '/') || moduleText[uk] == '#') uk++;
		goto start;
	}

	/* end of module */
	if (moduleText[uk] == '\0') { 
		lexeme[0] = '#'; 
		return TEnd; 
	}

	/* if digit (const int) */
	if (moduleText[uk] >= '0' && moduleText[uk] <= '9') {
		lexeme[i++] = moduleText[uk++];
		while (moduleText[uk] >= '0' && moduleText[uk] <= '9')
			if (i < MAX_LEX - 1) lexeme[i++] = moduleText[uk++]; 
			else uk++;
		return TConstInt;
	}
	/* else if character [a-z] || [A-Z] */
	else if ((moduleText[uk] >= 'a' && moduleText[uk] <= 'z') || (moduleText[uk] >= 'A' && moduleText[uk] <= 'Z')) { 
		/* identificator */
		lexeme[i++] = moduleText[uk++];
		while ((moduleText[uk] >= '0' && moduleText[uk] <= '9') || (moduleText[uk] >= 'a' && moduleText[uk] <= 'z') || (moduleText[uk] >= 'A' && moduleText[uk] <= 'Z'))
			if (i < MAX_LEX - 1) lexeme[i++] = moduleText[uk++]; 
			else uk++; // cut too long ident

		/* keyword */
		for (int j = 0; j < MAX_KEYW; j++)
			if (strcmp(lexeme, keyword[j]) == 0) return indexKeyword[j];

		return TIdent;
	}
	/* else if const char */
	else if (moduleText[uk] == '\"') { 
		uk++; // first char after '"'
		while (moduleText[uk] != '\"' && moduleText[uk] != '#' && moduleText[uk] != '\n')
		{
			if (i < MAX_LEX - 1) lexeme[i++] = moduleText[uk++]; 
			else uk++;
		}
		// if not const char
		if (moduleText[uk] != '\"')
		{
			PrintError("Wrong const char", lexeme);
			return TErr;
		}

		uk++; // last char before '"'
		return TConstChar; 
	}
	else if (moduleText[uk] == '+') {
		lexeme[i++] = moduleText[uk++];
		/* increment */
		if (moduleText[uk] == '+') {
			lexeme[i++] = moduleText[uk++];
			return TIncr;
		}
		return TPlus;
	}
	else if (moduleText[uk] == '-') {
		lexeme[i++] = moduleText[uk++];
		/* decrement */
		if (moduleText[uk] == '-')
		{
			lexeme[i++] = moduleText[uk++];
			return TDecr;
		}
		return TMinus;
	}
	else if (moduleText[uk] == '<') {
		lexeme[i++] = moduleText[uk++];
		if (moduleText[uk] == '=') {
			lexeme[i++] = moduleText[uk++];
			return TLE;
		}
		return TLT;
	}
	else if (moduleText[uk] == '>') {
		lexeme[i++] = moduleText[uk++];
		if (moduleText[uk] == '=') {
			lexeme[i++] = moduleText[uk++];
			return TGE;
		}
		else return TGT;
	}
	else if (moduleText[uk] == '=') {
		lexeme[i++] = moduleText[uk++];
		if (moduleText[uk] == '=') {
			lexeme[i++] = moduleText[uk++];
			return TEQ;
		}
		else return TSave;
	}
	else if (moduleText[uk] == '!') {
		lexeme[i++] = moduleText[uk++];
		if (moduleText[uk] == '=') {
			lexeme[i++] = moduleText[uk++];
			return TNEQ;
		}
		else return TNOT;
	}
	else if (moduleText[uk] == '*') {
		lexeme[i++] = moduleText[uk++];
		return TMult;
	}
	else if (moduleText[uk] == '/') {
		lexeme[i++] = moduleText[uk++];
		return TDiv;
	}
	else if (moduleText[uk] == '%') {
		lexeme[i++] = moduleText[uk++];
		return TMod;
	}
	else if (moduleText[uk] == '&') {
		lexeme[i++] = moduleText[uk++];
		if (moduleText[uk] == '&') {
			lexeme[i++] = moduleText[uk++];
			return TAND;
		}
		else return TAmper;
	}
	else if (moduleText[uk] == '|') {
		lexeme[i++] = moduleText[uk++];
		if (moduleText[uk] == '|') {
			lexeme[i++] = moduleText[uk++];
			return TOR;
		}
		else {
			PrintError("Wrong operator", lexeme);
			exit(1);
		}
	}
	else if (moduleText[uk] == '(') {
		lexeme[i++] = moduleText[uk++];
		return TLS;
	}
	else if (moduleText[uk] == ')') {
		lexeme[i++] = moduleText[uk++];
		return TRS;
	}
	else if (moduleText[uk] == '{') {
		lexeme[i++] = moduleText[uk++];
		return TFLS;
	}
	else if (moduleText[uk] == '}') {
		lexeme[i++] = moduleText[uk++];
		return TFRS;
	}
	else if (moduleText[uk] == ',') {
		lexeme[i++] = moduleText[uk++]; 
		return TZpt;
	}
	else if (moduleText[uk] == '.') {
		lexeme[i++] = moduleText[uk++];
		return TToch;
	}
	else if (moduleText[uk] == ';') {
		lexeme[i++] = moduleText[uk++];
		return TTZpt;
	}
	else {
		PrintError("Wrong character", lexeme); // ошибка
		uk++;
		return TErr;
	}
}

void Scanner::GetData(const char* filename)
{
	// Open file
	ifstream file(filename);
	if (!file.is_open()) {
		PrintError("Error to open file!", "");
		exit(1);
	}

	// Read file
	int i = 0;
	char ch;
	while (file.get(ch)) {
		moduleText[i++] = ch;
		if (i > MAX_TEXT - 1) {
			PrintError("The size of the source module is too large", "");
			break;
		}
	}
	moduleText[i] = '\0';

	// Close file
	file.close();
}

Scanner::Scanner(const char* filename)
{
	GetData(filename);
	SetUK(0);
}

Scanner::~Scanner()
{
}
