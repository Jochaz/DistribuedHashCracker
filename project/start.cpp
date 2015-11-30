#include "stdafx.h"
#include <iostream>
#include "Global.h"
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
//#include <unistd.h>
#include "CFileText.h"
#include "string"
#include "CException.h"
using namespace std;
#include "CUtil.h"
#include "Log.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include <pthread.h>
#include <thread>
Global *global;
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

//Fonction retournant si un fichier existe
inline bool file_exists(const std::string& name) {
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

void SavePasswordInFile(string MDP){
	try {
		std::vector<std::string> cnt;
		cnt.push_back(MDP);
		CFileText fout( "statut.txt", EFileOpenMode::write );
		fout.WriteAll( cnt, EFileEOL::UNIX );
		fout.Close();

	}
	catch (CException &e) {
		Log log(e.GetErrorMessage(), "Erreur", e.GetType());
	}
}

string LoadPasswordInFile(){	
	try {
		ifstream fichier("statut.txt", ios::in);  // on ouvre en lecture

		if (fichier)  // si l'ouverture a fonctionné
		{
			string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
			getline(fichier, contenu);  // on met dans "contenu" la ligne

			fichier.close();
			return contenu;
		}
		else
			cerr << "Impossible d'ouvrir le fichier !" << endl;
		
		}

		
	catch (CException &e) {			
		Log log(e.GetErrorMessage(), "Erreur", e.GetType());
	}
	return "";
}

string convertToMD5(string MDP){
	CHashMd5 md5;
	string resultat;
	md5.HashBuffer(reinterpret_cast<const unsigned char *>(MDP.c_str()), static_cast<int>(MDP.length()));
	resultat = md5.GetHash();
	for (size_t i = 0; i < resultat.size(); i++)
	{
		resultat[i] = tolower(resultat[i]);
	}
	return resultat;

}
string convertToCrc32(string MDP){
	CHashCrc32 crc32;
	string resultat;
	crc32.HashBuffer(reinterpret_cast<const unsigned char *>(MDP.c_str()), static_cast<int>(MDP.length()));
	resultat = crc32.GetHash();
	for (size_t i = 0; i<resultat.size(); i++)
	{
		resultat[i] = tolower(resultat[i]);
	}
	return resultat;

}

string convertToNone(string MDP){
	CHashNone none;
	string resultat;
	none.HashBuffer(reinterpret_cast<const unsigned char *>(MDP.c_str()), static_cast<int>(MDP.length()));
	resultat = none.GetHash();
	return resultat;

}

string convertToSha1(string MDP){
	CHashSha1 Sha1;
	string resultat;
	Sha1.HashBuffer(reinterpret_cast<const unsigned char *>(MDP.c_str()), static_cast<int>(MDP.length()));
	resultat = Sha1.GetHash();
	for (size_t i = 0; i<resultat.size(); i++)
	{
		resultat[i] = tolower(resultat[i]);
	}
	return resultat;

}
string convertToSha224(string MDP){
	CHashSha224 Sha224;
	string resultat;
	Sha224.HashBuffer(reinterpret_cast<const unsigned char *>(MDP.c_str()), static_cast<int>(MDP.length()));
	resultat = Sha224.GetHash();
	for (size_t i = 0; i<resultat.size(); i++)
	{
		resultat[i] = tolower(resultat[i]);
	}
	return resultat;

}
string convertToSha256(string MDP){
	CHashSha256 Sha256;
	string resultat;
	Sha256.HashBuffer(reinterpret_cast<const unsigned char *>(MDP.c_str()), static_cast<int>(MDP.length()));
	resultat = Sha256.GetHash();
	for (size_t i = 0; i<resultat.size(); i++)
	{
		resultat[i] = tolower(resultat[i]);
	}
	return resultat;

}
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
	//INITIALISATION
	std::string CurrentPassword = "";
	std::string PasswordHashed = "";
	//ON REGARDE SI ON C'ETAIT PAS ARRETE AVANT, SI C'EST LE CAS ON RECUPERE LE MDP EN COURS
	if (file_exists("statut.txt")){
		CurrentPassword = LoadPasswordInFile();
	}


	while (global->hash != PasswordHashed && !CUtil::IsEscKeyPressed()){
		std::cout << ".";
		CurrentPassword = nextPassword(CurrentPassword, global->alphabet);
		if (global->algo == "none")
			PasswordHashed = convertToNone(CurrentPassword);
		if (global->algo == "crc32")
			PasswordHashed = convertToCrc32(CurrentPassword);
		if (global->algo == "md5")
			PasswordHashed = convertToMD5(CurrentPassword);
		if (global->algo == "sha1")
			PasswordHashed = convertToSha1(CurrentPassword);
		if (global->algo == "sha224")
			PasswordHashed = convertToSha224(CurrentPassword);
		if (global->algo == "sha256")
			PasswordHashed = convertToSha256(CurrentPassword);
		CUtil::Sleep(0);
	}


	//On a pas trouver le MDP donc Escape a été pressé donc on log et on le sauvegarde
	if (PasswordHashed != global->hash){
		SavePasswordInFile(CurrentPassword);
		Log log("La touche échap a été pressé ! STOP", "Info", "Echap pressé");
	}
	else //SINON ON EFFACE LE CONTENU DU FICHIER VU QU'ON A TROUVER LE MDP
	{
		SavePasswordInFile("");
		Log log("Le mot de passe a été trouvé : " + CurrentPassword, "Info", "MDP Trouvé !");
	}
	std::cout << CurrentPassword;
	system("pause");
	return;
}

void MiseEnPlaceDesThreads(){
	for (int i = 0; i < CUtil::GetCpuCoreCount(); i++)
	{
		//pthread_h t;
	}
}

bool isThreadAlive(pthread_t tid) {
	return pthread_kill(tid, 0) != ESRCH;
}
void * maFonction(void *p_arg){
	int number = reinterpret_cast<int>(p_arg);

	//for (int i = 0; i < 5000; i++) {
	//	std::cout << number;
	//}
	return nullptr;
}

void initialisationThread(){
	int nbThread = CUtil::GetCpuCoreCount() - 1; // A remplir avec ton nombre de threads

	pthread_t* threads = (pthread_t*)malloc(nbThread*sizeof(pthread_t));

	for (int i = 0; i < nbThread; ++i)
		pthread_create(&threads[i], NULL, maFonction, (void *)i);
	for (int i = 0; i < nbThread; ++i)
		pthread_join(threads[i], nullptr);
}
int main(int n, const char*params[]){
	int i;
	string param;
	//if((n-1)%2 != 0 || (n-1)/2 != 4)
	if (1 == 2)
	{
		std::cout << "EXCEPTION, le nb d'argument est incohérent ou incomplet" << std::endl;

	}
	else{

		// t = CTest::getInstance();
		global = Global::getInstance();
		global->hash = "34cafdb9a33d05a68ac5cecdb76b0085";
		global->algo = "md5";
		global->alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/*-+$%^!:;.,?@&~#()[]{}|_";
		global->chunkSize = 2;
		//OK, le nb d'argument est cohérent
		for (i = 1; i<n; i = i + 2) // i+2 car i = la clé & i+1 la valeur, donc on boucle sur les clés, ce qui justifie le fait d'avoir un pas de 2 et non de 1
		{
			param = params[i];

			if (param == "-hash")
			{
				global->hash = params[i + 1];
			}

			if (param == "-algo")
			{
				global->algo = params[i + 1];
			}

			if (param == "-alphabet")
			{
				global->alphabet = params[i + 1];
			}
			if (param == "-chunksize")
			{
				global->chunkSize = (int)params[i + 1];
			}
		}

		if (global->hash != "" && global->algo != ""  && global->alphabet != ""  && global->chunkSize != 0)
		{
			std::cout << "Ok, les parametres ont tous ete saisis" << std::endl;
			initialisationThread();
		//	bruteForce();
		}
		else
		{
			Log log("certains parametres sont non saisis", "Erreur", "Paramètres manquants");
			std::cout << "Certains parametres sont non saisis" << std::endl;

		}
		return 0;

	}

	system("pause");
}
