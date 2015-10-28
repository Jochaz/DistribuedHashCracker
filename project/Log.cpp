#include "Log.h"
#include <fstream>
Log::Log(std::string UnMessage, std::string UneSeverite){
	severite LaSeverite;

	if (UneSeverite == "Erreur"){
		LaSeverite = Serreur;
	}
	else if (UneSeverite == "Avertissement"){
		LaSeverite = Savertissement;
	} 
	else if (UneSeverite == "Info"){
		LaSeverite = Sinfo;
	}
	else if (UneSeverite == "Debug"){
		LaSeverite = Sdebug;
	}
	else {
		//Throw exception
	}
	FSeverite = LaSeverite;
	FMessage = UnMessage;
}
void Log::EcritureLog(){
	//FINIR LECRITURE DANS UN FICHIER
}
void Log::Erreur(){
	
}
void Log::Avertissement(){

}

void Log::Info(){

}
void Log::Debug(){

}