#include "Log.h"
#include <iostream>
#include "Time.h"
#include <fstream>
#include <mutex>
#include <string>
#include "CFileText.h"
using namespace std ;

Log::Log(string UnMessage, string UneSeverite, string UnTitre) {
	severite LaSeverite;

	FTitre = UnTitre;
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
		mutex pause;
	
		CDateTime time;
	
		time.Now();
	
		ofstream ecriture(FTitre, ios::out | ios::app);
	
	
		if (ecriture) {
	
			pause.lock();
	
			ecriture << "========================" << endl;
			ecriture << "Date : " << time.m_wDay << "/" << time.m_wMonth << "/" << time.m_wYear << endl;
			ecriture << "heure : " << time.m_wHour << ":" << time.m_wMinute << ":" << time.m_wSecond << endl;
			ecriture << FMessage << endl;
			ecriture << "========================" << endl;
	
			pause.unlock();
		}
	
		ecriture.close() ;

}
void Log::EcritureLog(string LeMessage) {
	//

	//}

}
void Log::Erreur() {

	string error("!! ERROR !! ");
	error += FMessage;
	EcritureLog(error);

}
void Log::Avertissement() {

	string warning("! WARNING ! ");
	warning += FMessage;
	EcritureLog(warning);

}

void Log::Info() {

	string info("* INFO * ");
	info += FMessage;
	EcritureLog(info);

}
void Log::Debug() {

	string debug(" o DEBUG o ");
	debug += FMessage;
	EcritureLog(debug);

}
