#include "stdafx.h"
#include <iostream>
#include "Global.h"
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
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
#include "Chunk.h"

Global *global;
bool Fin = false;
int size = 24;
vector<CPasswordChunk> arrayChunk(size);
int CurrentChunk = -1;
string debut;
string fin;

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

string LoadPasswordInFile(){
	try {
		ifstream fichier("statut.txt", ios::in);  // on ouvre en lecture

		if (fichier)  // si l'ouverture a fonctionn�
		{
			string contenu;  // d�claration d'une cha�ne qui contiendra la ligne lue
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

string chunkSuivantDebut(string Debut){
	string leDebut = "";
	for (int i = 0; i < global->chunkSize; i++)
		leDebut = leDebut + global->alphabet[0];
	
	return leDebut;
}


string chunkSuivantFin(string Fin){
	string laFin = "";
	for (int i = 0; i < global->chunkSize; i++)
		laFin = laFin + global->alphabet[global->alphabet.size() -1];
	return laFin;
}
void initialisationChunk(){
	string Debut = "";
	string fin = "";
	if (file_exists("statut.txt")){
		Debut = LoadPasswordInFile();
	}

	if (Debut == ""){
		Debut = global->alphabet[0];
		fin = fin + chunkSuivantFin(fin);
		CPasswordChunk chunk;
		chunk.SetPasswordRange(Debut, fin);
		arrayChunk[0] = chunk;
	}
	else {
		for (int i = 0; i < Debut.size(); i++)
			fin = fin + global->alphabet[global->alphabet.size() - 1];
		fin = fin + chunkSuivantFin(fin);
		CPasswordChunk chunk;
		chunk.SetPasswordRange(Debut, fin);
		arrayChunk[0] = chunk;
		Debut = "";
		for (int i = 0; i < chunk.GetPasswordBegin().size(); i++)
			Debut = Debut + global->alphabet[0];
	}

	for (int i = 1; i < size - 1; i++)
	{
		Debut = Debut + chunkSuivantDebut(Debut);
		fin = fin + chunkSuivantFin(fin);
		CPasswordChunk chunk;
		chunk.SetPasswordRange(Debut, fin);
		arrayChunk[i] = chunk;
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
bool isThreadAlive(pthread_t tid) {
	return pthread_kill(tid, 0) != ESRCH;
}

void bruteForce(string BeginChunk, string EndChunk, int ith){
	//INITIALISATION
	std::string CurrentPassword = BeginChunk;
	std::string PasswordHashed = "";

	while (global->hash != PasswordHashed && !CUtil::IsEscKeyPressed() && CurrentPassword != EndChunk && !Fin){
		
		
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


	//On a pas trouver le MDP donc Escape a �t� press� donc on log et on le sauvegarde
	if (PasswordHashed != global->hash && CurrentPassword != EndChunk && !Fin){
		SavePasswordInFile(CurrentPassword);
		Log log("La touche �chap a �t� press� ! STOP", "Info", "Echap press�");
		Fin = true;
	}
	else if (PasswordHashed == global->hash) //SINON ON EFFACE LE CONTENU DU FICHIER VU QU'ON A TROUVER LE MDP
	{
		SavePasswordInFile("");
		Log log("Le mot de passe a �t� trouv� : " + CurrentPassword, "Info", "MDP Trouv� !");
		Fin = true;
		cout << endl << CurrentPassword <<endl;
		system("pause");
	}
	return;
}



void * LancementBruteForce(void *p_arg){
	int number = reinterpret_cast<int>(p_arg);
	CurrentChunk++;
	while (!Fin && CurrentChunk < arrayChunk.size()){
		bruteForce(arrayChunk[CurrentChunk].GetPasswordBegin(), arrayChunk[CurrentChunk].GetPasswordEnd(), number);
		CurrentChunk++;
	}
	return nullptr;
}

void initialisationThread(){
	int nbThread = CUtil::GetCpuCoreCount(); // A remplir avec ton nombre de threads
	pthread_t* threads = (pthread_t*)malloc(nbThread*sizeof(pthread_t));
	for (int i = 0; i < nbThread; ++i)
		pthread_create(&threads[i], NULL, LancementBruteForce, (void *)i);
	for (int i = 0; i < nbThread; ++i)
			pthread_join(threads[i], nullptr);
}
int main(int n, const char*params[]){
	int i;
	string param;
	if((n-1)%2 != 0 || (n-1)/2 != 4)
	{
		std::cout << "EXCEPTION, le nb d'argument est incoh�rent ou incomplet" << std::endl;

	}
	else{
		global = Global::getInstance();
		//OK, le nb d'argument est coh�rent
		for (i = 1; i<n; i = i + 2) // i+2 car i = la cl� & i+1 la valeur, donc on boucle sur les cl�s, ce qui justifie le fait d'avoir un pas de 2 et non de 1
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
				global->chunkSize = atoi(params[i + 1]);
			}
		}

		if (global->hash != "" && global->algo != ""  && global->alphabet != ""  && global->chunkSize != 0)
		{
			std::cout << "Ok, les parametres ont tous ete saisis" << std::endl;
			initialisationChunk();
			initialisationThread();
		}
		else
		{
			Log log("certains parametres sont non saisis", "Erreur", "Param�tres manquants");
			std::cout << "Certains parametres sont non saisis" << std::endl;

		}
		return 0;
	}
		system("pause");

}
