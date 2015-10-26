#include "stdafx.h"
#include "NextPassword.h"
#include <iostream>
#include "string"
//
using namespace std;

int nextAlphabet(char C, string Alphabet){
	//VAR
	int Result;

	//INITIALISATION
	Result = 0;

	//TRAITEMENT
	for (size_t I = 0; I < Alphabet.size() - 1; ++I)
	{
		if (C == Alphabet[I])
		{
			if (I != Alphabet.size() - 1){
				Result = I + 1;
				break;
			}
		}
	}
	return Result;
}

string nextPassword(string Password, string Alphabet){
	//VAR
	bool Found;
	int I;
	int NextPos;
	string Result;

	//INITIALISATION
	I = Password.size() - 1;
	Found = false;
	Result = Password;

	//TRAITEMENT
	while (!Found){
		if (I != -1){
			NextPos = nextAlphabet(Password[I], Alphabet);
		}
		else {
			Result = Alphabet[0] + Result;
		}
		if (NextPos == 0){
			if (I != -1) {
				Result[I] = Alphabet[0];
				I--;
			}
			else{
				Found = true;
			}
		}
		else {
			Result[I] = Alphabet[NextPos];
			Found = true;
		}
	}
	return Result;
}