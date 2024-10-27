//************************************************************
// модуль defs.h --- общие типы данных и макроопределения
//************************************************************
#pragma once
#ifndef __DEFS
#define __DEFS

#define MAX_TEXT 10000 // максимальная длина текста
#define MAX_LEX 100 // максимальная длина лексемы
#define MAX_KEYW 12 // число ключевых слов
typedef char TypeLex[MAX_LEX];
typedef char TypeMod[MAX_TEXT];

// идентификаторы и константы
#define TIdent 1
#define TConstInt 10
#define TConstChar 11
// знаки операций
#define TPlus 20
#define TMinus 21
#define TMult 22
#define TDiv 23
#define TMod 24
#define TLT 25
#define TLE 26
#define TGT 27
#define TGE 28
#define TEQ 54
#define TNEQ 55
#define TNOT 56
#define TAND 57
#define TOR 58
#define TSave 59
#define TIncr 60
#define TDecr 61
#define TAmper 62
// специальные знаки
#define TLS 70
#define TRS 71
#define TFLS 72
#define TFRS 73
#define TZpt 74
#define TToch 75
#define TTZpt 76
// ключевые слова
#define TIF 80
#define TElse 81
#define TWhile 82
#define TFor 83
#define TConst 84
#define TReturn 85
#define TStruct 86
#define TMain 87
// типы данных
#define TInt 90
#define TShort 91
#define TLong 92
#define TChar 93
// конец исходного модуля
#define TEnd 100
// ошибочный символ
#define TErr 200
#endif