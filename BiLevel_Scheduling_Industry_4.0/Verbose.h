#ifndef BILEVEL_SCHEDULING_VERBOSE
#define BILEVEL_SCHEDULING_VERBOSE

#include <iostream>
#include <string>

class Verbose
{
private:
	static unsigned int level;
	static unsigned int requiredLevel;

public:

	// Définit le niveau de verbose actuel
	static void setLevel(unsigned int newLevel) { Verbose::level = newLevel; }

	// Définit le niveau de verbose minimum requis pour afficher les messages suivants
	static void setRequiredLevel(unsigned newRequiredLevel) { Verbose::requiredLevel = newRequiredLevel; }

	// Affiche le message seulement si le niveau de verbose est superieur ou égal à celui requis
	const Verbose& operator<<(const char* text) const { if (Verbose::level >= Verbose::requiredLevel) std::cout << text; return *this; }
	const Verbose& operator<<(std::string text) const { if (Verbose::level >= Verbose::requiredLevel) std::cout << text; return *this; }
	const Verbose& operator<<(int number) const { if (Verbose::level >= Verbose::requiredLevel) std::cout << number; return *this; }
	const Verbose& operator<<(unsigned int uiNumber) const { if (Verbose::level >= Verbose::requiredLevel) std::cout << uiNumber; return *this; }
	const Verbose& operator<<(float floatNumber) const { if (Verbose::level >= Verbose::requiredLevel)  std::cout << floatNumber; return *this; }
	const Verbose& operator<<(long longNumber) const { if (Verbose::level >= Verbose::requiredLevel) std::cout << longNumber; return *this; }
};

#endif // BILEVEL_SCHEDULING_VERBOSE 