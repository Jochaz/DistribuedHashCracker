#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Log.h"
#include <iostream>
#include "Time.h"
#include <fstream>
#include <mutex>
#include <string>
#include "CException.h"
#include "CFileText.h"
#include <ctime>
using namespace std ;

Log::Log(string UnMessage, string UneSeverite, string UnTitre) {
	severite LaSeverite;

	FMessage = UnMessage;

	if (UneSeverite == "Erreur") {
		LaSeverite = Serreur;
		Erreur();
	}
	else if (UneSeverite == "Avertissement") {
		LaSeverite = Savertissement;
		Avertissement();
	}
	else if (UneSeverite == "Info") {
		LaSeverite = Sinfo;
		Info();
	}
	else if (UneSeverite == "Debug") {
		LaSeverite = Sdebug;
		Debug();
	}

	//std::vector<std::string> cnt;
	//string nameFile;

	//time_t _time;
	//struct tm timeInfo;
	//char format[32];

	//time(&_time);
	//localtime_s(&timeInfo, &_time);

	//strftime(format, 32, "%Y-%m-%d %H-%M", &timeInfo);
	//nameFile = format + '.txt';


	//cnt.push_back(UnMessage);

	//CFileText fout(format, EFileOpenMode::write);
	//fout.WriteAll(cnt, EFileEOL::UNIX);
	//fout.Close();
	

}
void Log::EcritureLog(string LeMessage) {
	//
	//mutex pause;
	//
	//	CDateTime time;
	//
	//	time.Now();
	//
	//	ofstream ecriture(FTitre, ios::out | ios::app);
	//
	//
	//	if (ecriture) {
	//
	//		pause.lock();
	//
	//		ecriture << "========================" << endl;
	//		ecriture << "Date : " << time.m_wDay << "/" << time.m_wMonth << "/" << time.m_wYear << endl;
	//		ecriture << "heure : " << time.m_wHour << ":" << time.m_wMinute << ":" << time.m_wSecond << endl;
	//		ecriture << FMessage << endl;
	//		ecriture << "========================" << endl;
	//
	//		pause.unlock();
	//	}
	//
	//	ecriture.close() ;
	////}
	try {
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%d%m%Y %I%M%S", timeinfo);
		std::string str(buffer);

		std::vector<std::string> cnt;
		cnt.push_back(LeMessage);
		CFileText fout(str + ".txt", EFileOpenMode::write);
		fout.WriteAll(cnt, EFileEOL::UNIX);
		fout.Close();

	}
	catch (CException &e) {
		Log log(e.GetErrorMessage(), "Erreur", e.GetType());
	}

}
void Log::Erreur() {

	string error("!! ERROR !! ");
	error += FTitre;
	error += FMessage;
	EcritureLog(error);

}
void Log::Avertissement() {

	string warning("! WARNING ! ");
	warning += FTitre;
	warning += FMessage;
	EcritureLog(warning);

}

void Log::Info() {

	string info("* INFO * ");
	info += FTitre;
	info += FMessage;
	EcritureLog(info);

}
void Log::Debug() {

	string debug(" o DEBUG o ");
	debug += FTitre;
	debug += FMessage;
	EcritureLog(debug);

}
