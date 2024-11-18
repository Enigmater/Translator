#include "Diagram.h"

void Diagram::Z()
{
	TypeLex l;
	int t = lookForward(1);
	while (t == TConst || t == TStruct || t == TInt || t == TShort || t == TLong || t == TChar) {
		C();
		t = lookForward(1);
	}
}

void Diagram::C()
{
	TypeLex l;
	int t = lookForward(2);
	if (t != TMain) V();
	else {
		B();
		t = sc->Scan(l);
		if (t != TMain) sc->PrintError("Ожидалось KEYWORD main", l);
		Tree* v = root->SemInclude(l, TYPE_UNKNOWN, TYPE_FUNC);
		t = sc->Scan(l);
		if (t != TLS) sc->PrintError("Ожидался знак (", l);
		t = sc->Scan(l);
		if (t != TRS) sc->PrintError("Ожидался знак )", l);
		
		F();
		root->SetCurr(v);
	}
}

void Diagram::V(OBJECT_TYPE ot)
{
	TypeLex l;
	int t = lookForward(1);
	if (t == TStruct) A(ot);
	else {
		/* set const flag */
		bool isConst = false;
		if (t == TConst) {
			sc->Scan(l);
			isConst = true;
		}

		DATA_TYPE dt;
		/* if var is struct object */
		Tree* structName = nullptr;
		if (lookForward(1) == TIdent) {
			dt = TYPE_STRUCTTYPE;
			t = sc->Scan(l);
			structName = root->SemGetStruct(l);	// if struct isn't exists - error : "Такой структуры не существует!"
		}
		else dt = B();	
		do {
			t = sc->Scan(l);
			if (t != TIdent) sc->PrintError("Ожидался идентификатор", l);
			
			if (structName != nullptr) {
				root->SemIncludeStructObj(l, structName); // if object with same name is exists - error : "Объект с таким именем уже существует!"
				t = sc->Scan(l); // scan ;
			}
			else {
				Tree* v = root->SemInclude(l, dt, ot);	  // if object with same name is exists - error : "Объект с таким именем уже существует!"
				v->setConst(isConst); // set const flag

				bool isInit = false;
				if (lookForward(1) == TSave) {
					t = sc->Scan(l);
					T();
					isInit = true;
				}
				v->setInit(isInit); // set init flag
				t = sc->Scan(l);
			}
		} while (t == TZpt);
		if (t != TTZpt) sc->PrintError("Ожидался знак ;", l);
	}
	
}

DATA_TYPE Diagram::B()
{
	TypeLex l;
	int t = sc->Scan(l);
	if (t == TShort || t == TLong) {
		int prevT = t;
		t = sc->Scan(l);
		if (t != TInt) sc->PrintError("Ожидался тип SHORT INT или LONG INT", l);

		if (prevT == TShort) return TYPE_SHORTINT;
		else return TYPE_LONGINT;
	}
	else if (t == TInt) return TYPE_INTEGER;
	else if (t == TChar) return TYPE_CHAR;
	else if (t == TIdent) return TYPE_STRUCTTYPE;
	else sc->PrintError("Ожидался тип", l);
}

void Diagram::A(OBJECT_TYPE ot)
{
	TypeLex l;
	int t = sc->Scan(l);
	if (t != TStruct) sc->PrintError("Ожидалось KEYWORD struct", l);
	t = sc->Scan(l);
	if (t != TIdent) sc->PrintError("Ожидался идентификатор структуры", l);

	OBJECT_TYPE otype = ot == TYPE_VAR ? TYPE_STRUCT : TYPE_STRUCTFIELD; // if struct is field of other struct
	Tree* v = root->SemInclude(l, TYPE_STRUCTTYPE, otype);
	 
	t = sc->Scan(l);
	if (t != TTZpt) {
		if (t != TFLS) sc->PrintError("Ожидался знак {", l);
		while (lookForward(1) != TFRS) V(TYPE_STRUCTFIELD);	 // add fields to struct
		
		t = sc->Scan(l); // scan }
		t = sc->Scan(l); // scan ;
		if (t != TTZpt) sc->PrintError("Ожидался знак ;", l);
	}
	root->SetCurr(v);
}

void Diagram::F()
{
	TypeLex l;
	// {
	int t = sc->Scan(l);
	if (t != TFLS) sc->PrintError("Ожидался знак {", l);
	root->SemIncludeLeftBlock();
	Tree* temp = root->SemIncludeRightBlock();
	// Body
	t = lookForward(1);
	while (t != TFRS) {
 		if (t == TConst || t == TStruct || t == TInt || t == TShort || t == TLong || t == TChar || 
			(t == TIdent && lookForward(2) == TIdent)) V();
		else G();
		t = lookForward(1);
	}
	// }
	root->SetCurr(temp);
	t = sc->Scan(l);
	if (t != TFRS) sc->PrintError("Ожидался знак }", l);
}

void Diagram::G()
{
	TypeLex l;
	int uk = sc->GetUK(), t = sc->Scan(l);
	if (t == TTZpt) return;
	else if (t == TIF) {
		t = sc->Scan(l);
		if (t != TLS) sc->PrintError("Ожидался знак (", l);
		T();
		t = sc->Scan(l);
		if (t != TRS) sc->PrintError("Ожидался знак )", l);
		F();
		if (lookForward(1) == TElse) {
			t = sc->Scan(l);
			F();
		}
	}
	else if (t == TReturn) {
		T();
	}
	else if (t == TIdent) {
		
		Tree* v = root->CheckVisibility(l);	// if obj isn't exists - error : "Идентификатор не определен!"	
		if (v->getConst()) sc->PrintError("Константа не может быть изменена!", l);
		
		while (lookForward(1) == TToch) {
			Tree* structName = root->SemGetStructObject(l); // if struct obj isn't exists - error : "Объект структуры не существует!"
			t = sc->Scan(l);
			t = sc->Scan(l);
			if (t != TIdent) sc->PrintError("Ожидася идентификатор (поле структуры)", l);
			root->CheckStructAccess(structName, l);			// if field l isn't field of structName - error: "Структура <$structName> не имеет такого поля!"
		}
		t = sc->Scan(l);
		if (t != TSave) sc->PrintError("Ожидался оператор =", l);
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
	int t = lookForward(1);
	while (t == TEQ || t == TNEQ) {
		t = sc->Scan(l);
		Y();
		t = lookForward(1);
	}
}

void Diagram::Y()
{
	U();
	TypeLex l;
	int t = lookForward(1);
	while (t >= TLT && t <= TGE)  {
		t = sc->Scan(l);
		U();
		t = lookForward(1);
	}
}

void Diagram::U()
{
	I();
	TypeLex l;
	int t = lookForward(1);
	while (t == TPlus || t == TMinus) {
		t = sc->Scan(l);
		I();
		t = lookForward(1);
	}
}

void Diagram::I()
{
	O();
	TypeLex l;
	int t = lookForward(1);
	while (t == TMult || t == TDiv || t == TMod) {
		t = sc->Scan(l);
		O();
		t = lookForward(1);
	}
}

void Diagram::O()
{
	TypeLex l;
	int t = lookForward(1);
	if (t == TNOT || t == TAmper || t == TPlus || t == TMinus || t == TMult) t = sc->Scan(l);
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
	int t = sc->Scan(l);

	if (t == TLS) {
		T();
		if (lookForward(1) != TRS) sc->PrintError("Ожидался знак )", l);
	}
	else if (t == TIdent) {
		root->CheckVisibility(l);	// if obj isn't exists - error : "Идентификатор не определен!"
		while (lookForward(1) == TToch) {
			Tree* structName = root->SemGetStructObject(l); // if struct obj isn't exists - error : "Объект структуры не существует!"
			t = sc->Scan(l);
			t = sc->Scan(l);
			if (t != TIdent) sc->PrintError("Ожидася идентификатор", l);
			root->CheckStructAccess(structName, l);			// if field l isn't field of structName - error: "Структура <$structName> не имеет такого поля!"
		}
	}
	else if (t != TConstInt && t != TConstChar) sc->PrintError("Выражение не верно", l);
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
