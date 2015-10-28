#include "Chunk.h"

Chunk::Chunk(std::string unePlageDebut, std::string unePlageFin){
	FPlageDebut = unePlageDebut;
	FPlageFin = unePlageFin;
}
std::string Chunk::GetPlageDebut(){
	return FPlageDebut;
}
std::string Chunk::GetPlageFin(){
	return FPlageFin;
}