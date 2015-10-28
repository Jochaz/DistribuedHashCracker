#include <string>
class Chunk{
public:
	Chunk(std::string unePlageDebut, std::string unePlageFin);
	std::string GetPlageDebut();
	std::string GetPlageFin();
private:
	std::string FPlageDebut;
	std::string FPlageFin;
};