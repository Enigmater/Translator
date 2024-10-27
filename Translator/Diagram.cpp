#include "Diagram.h"

void Diagram::Z()
{
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	while (t == TConst || t == TStruct || t == TInt || t == TShort || t == TLong || t == TChar) {
		sc->SetUK(uk);
		C();
		uk = sc->GetUK();
		t = sc->Scan(l);
	}
}

void Diagram::C()
{
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	sc->SetUK(uk);
	if (t == TConst || t == TStruct) V();
	else {
		B();
		t = sc->Scan(l);
		if (t != TMain) sc->PrintError("ќжидалось KEYWORD main", l);
		t = sc->Scan(l);
		if (t != TLS) sc->PrintError("ќжидалс€ знак (", l);
		t = sc->Scan(l);
		if (t != TRS) sc->PrintError("ќжидалс€ знак )", l);
		F();
	}
}

void Diagram::V()
{
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	sc->SetUK(uk);
	if (t == TStruct) A();
	else {
		if (t == TConst) {
			sc->Scan(l);
		};
		B();
		do {
			t = sc->Scan(l);
			if (t != TIdent) sc->PrintError("ќжидалс€ идентификатор", l);
			
			uk = sc->GetUK();
			t = sc->Scan(l);

			if (t == TSave) T();
			else sc->SetUK(uk);

			t = sc->Scan(l);
		} while (t == TZpt);
		if (t != TTZpt) sc->PrintError("ќжидалс€ знак ;", l);

	}
	
}

void Diagram::B()
{
	TypeLex l;
	int t, uk;
	t = sc->Scan(l);
	if (t == TShort || t == TLong) {
		t = sc->Scan(l);
		if (t != TInt) sc->PrintError("ќжидалс€ тип", l);
	}
	else if (t != TInt && 
		t != TChar && 
		t != TIdent) sc->PrintError("ќжидалс€ тип", l);
}

void Diagram::A()
{
	TypeLex l;
	int t, uk;
	t = sc->Scan(l);
	if (t != TStruct) sc->PrintError("ќжидалось KEYWORD struct", l);
	t = sc->Scan(l);
	if (t != TIdent) sc->PrintError("ќжидалс€ идентификатор структуры", l);
	t = sc->Scan(l);
	if (t != TTZpt) {
		if (t != TFLS) sc->PrintError("ќжидалс€ знак {", l);
		uk = sc->GetUK();
		t = sc->Scan(l);
		while (t != TFRS) {
			sc->SetUK(uk);
			V();
			uk = sc->GetUK();
			t = sc->Scan(l);
		}
		t = sc->Scan(l);
		if (t != TTZpt) sc->PrintError("ќжидалс€ знак ;", l);
	}
}


void Diagram::F()
{
	TypeLex l;
	int t, uk;
	// {
	t = sc->Scan(l);
	if (t != TFLS) sc->PrintError("ќжидалс€ знак {", l);
	// Body
	uk = sc->GetUK();
	t = sc->Scan(l);
	sc->SetUK(uk);
	while (t != TFRS) {
 		if (t == TConst || t == TStruct || t == TInt || t == TShort || t == TLong || t == TChar || 
			(t == TIdent && lookForward(2) == TIdent)) V();
		else G();
		uk = sc->GetUK();
		t = sc->Scan(l);
		sc->SetUK(uk);
	}
	// }
	t = sc->Scan(l);
	if (t != TFRS) sc->PrintError("ќжидалс€ знак }", l);
}

void Diagram::G()
{
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	if (t == TTZpt) return;
	else if (t == TIF) {
		t = sc->Scan(l);
		if (t != TLS) sc->PrintError("ќжидалс€ знак (", l);
		T();
		t = sc->Scan(l);
		if (t != TRS) sc->PrintError("ќжидалс€ знак )", l);
		F();
		uk = sc->GetUK();
		t = sc->Scan(l);
		if (t == TElse) F();
		else sc->SetUK(uk);
	}
	else if (t == TReturn) {
		T();
	}
	else if (t == TIdent) {
		t = sc->Scan(l);
		while (t == TToch) {
			t = sc->Scan(l);
			if (t != TIdent) sc->PrintError("ќжидас€ идентификатор (поле структуры)", l);
			t = sc->Scan(l);
		}
		if (t != TSave) sc->PrintError("ќжидалс€ оператор =", l);
		T();
	}
	else if (t == TFLS) {
		sc->SetUK(uk);
		F();
	}
}

void Diagram::T()
{
	Y();
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	while (t == TEQ || t == TNEQ) {
		Y();
		uk = sc->GetUK();
		t = sc->Scan(l);
	}
	sc->SetUK(uk);
}

void Diagram::Y()
{
	U();
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	while (t >= TLT && t <= TGE)  {
		U();
		uk = sc->GetUK();
		t = sc->Scan(l);
	}
	sc->SetUK(uk);
}

void Diagram::U()
{
	I();
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	while (t == TPlus || t == TMinus) {
		I();
		uk = sc->GetUK();
		t = sc->Scan(l);
	}
	sc->SetUK(uk);
}

void Diagram::I()
{
	O();
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	while (t == TMult || t == TDiv || t == TMod) {
		O();
		uk = sc->GetUK();
		t = sc->Scan(l);
	}
	sc->SetUK(uk);
}

void Diagram::O()
{
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);
	if (t != TNOT && t != TAmper && t != TPlus && t != TMinus && t != TMult) sc->SetUK(uk);
	P();
	
}

void Diagram::P()
{
	TypeLex l;
	int t;
	t = lookForward(1);
	if (t == TIncr || t == TDecr) t = sc->Scan(l);
	if (t != TTZpt) EL();
	t = lookForward(1);
	if (t == TIncr || t == TDecr) t = sc->Scan(l);
}

void Diagram::EL()
{
	TypeLex l;
	int t, uk;
	uk = sc->GetUK();
	t = sc->Scan(l);

	if (t == TLS) {
		T();
		uk = sc->GetUK();
		t = sc->Scan(l);
		if (t != TRS) sc->PrintError("ќжидалс€ знак )", l);
	}
	else if (t == TIdent) {
		uk = sc->GetUK();
		t = sc->Scan(l);
		while (t == TToch) {
			t = sc->Scan(l);
			if (t != TIdent) sc->PrintError("ќжидас€ идентификатор (", l);
			uk = sc->GetUK();
			t = sc->Scan(l);
		}
	}
	else if (t == TConstInt || t == TConstChar) {
		return;
	}
	else sc->PrintError("¬ыражение не верно", l);
	sc->SetUK(uk);
}

int Diagram::lookForward(int steps)
{
	TypeLex l;
	int type = -1;
	int uk = sc->GetUK();
	while (steps-- > 0) type = sc->Scan(l);
	sc->SetUK(uk);
	return type;
}
