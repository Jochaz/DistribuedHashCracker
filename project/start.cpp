#include "stdafx.h"
#include <iostream>

//#include "DemoException.h"
//#include "DemoHash.h"
//#include "DemoSocket.h"
//#include "DemoThreads.h"
//#include "DemoFiles.h"
//#include "DemoDateTime.h"
#include <stdio.h>

#include <stdlib.h>

#include "string"
using namespace std;
#include "DemoUtil.h"
#include "NextPassword.h"

std::string PASSWORD;
std::string ALPHABET;

//void demos( int argc, char *argv[] ) {
//	//
//	// Display parameters received from command line
//	std::cout << std::endl;
//	std::cout << "** Parameters dump:" << std::endl;
//	for (int i = 0; i < argc; i++) {
//		std::cout << "argv[" << i << "] = \"" << argv[ i ] << "\"" << std::endl;
//	}
//
//	//
//	// Test libraries
//	std::cout << std::endl;
//	//TestThreads();
//	TestException();
//	
//	//TestThreads();
//	//TestException();
//	//TestHashes();
//	//TestSocket();
//	//TestFiles();
//	//TestDateTime();
//	//TestUtil();
//	TestUtil();
//
//	return;
//}

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

std::string nextPassword(std::string Password, std::string Alphabet){
	//VAR
	bool Found;
	int I;
	int NextPos;
	string Result;

	//INITIALISATION
	I = Password.size() - 1;
	Found = false;
	Result = Password;
	NextPos = NULL;
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

void bruteForce(){
	std::string CurrentPassword = "";
	while (PASSWORD != CurrentPassword){
		std::cout << CurrentPassword << std::endl;
		CurrentPassword = nextPassword(CurrentPassword, ALPHABET);
	}
	std::cout << CurrentPassword;
	return;
}

int main(int n, const char*params[])  
{
    int i;
	string hash = "";
	string algo = "";
	string alphabet = "";
	int chunksize = 0;
	string param;

	if((n-1)%2 != 0 || (n-1)/2 != 4)
	{
		std::cout << "EXCEPTION, le nb d'argument est incohérent ou incomplet" << std::endl;

	}else{
		//OK, le nb d'argument est cohérent
		for (i=1; i<n; i=i+2) // i+2 car i = la clé & i+1 la valeur, donc on boucle sur les clés, ce qui justifie le fait d'avoir un pas de 2 et non de 1
		{
			param = params[i];
		
			if(param == "-hash")
			{
				hash = params[i+1];
			}
		
			if(param == "-algo")
			{
				algo = params[i+1];
			}
		
			if(param == "-alphabet")
			{
				alphabet = params[i+1];
			}		
			if(param == "-chunksize")
			{
				chunksize = (int)params[i+1];
			}
		}

		if(hash != "" && algo != ""  && alphabet != ""  && chunksize != 0 )
		{
				std::cout << "Ok, les parametres ont tous ete saisis" << std::endl;
		}else
		{
				std::cout << "EXCEPTION, certains parametres sont non saisis" << std::endl;

		}
		return 0;
	}
}