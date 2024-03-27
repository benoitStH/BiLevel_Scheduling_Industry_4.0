#ifndef BILEVEL_SCHEDULING_VERBOSE
#define BILEVEL_SCHEDULING_VERBOSE

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

/*
* Verbose class allowing the print of messages depending on the level verbose defined in the program
* Intended use in main():
* Verbose verbose;
* verbose.setLevel(x);
* 
* Intended use to specify verbose message :
* Verbose verbose;
* verbose.setRequiredLevel(y); // Specify the minimal level of verbose required to show verbose
* verbose << message;
* functionWithVerbose();
* verbose.endRequiredLevel();
*/
class Verbose
{
private:
	static unsigned int level;
	static unsigned int maxRequiredLevel;
	static std::vector<unsigned int> requiredLevels;

public:

	// Définit le niveau de verbose actuel
	static void setLevel(unsigned int newLevel) { Verbose::level = newLevel; }

	// Définit le niveau de verbose minimum requis pour afficher les messages suivants
	static void setRequiredLevel(unsigned newRequiredLevel) 
	{ 
		Verbose::requiredLevels.push_back(newRequiredLevel);
		Verbose::maxRequiredLevel = *std::max_element(Verbose::requiredLevels.begin(), Verbose::requiredLevels.end()); 
	}

	// Retire le dernier niveau défini par setRequiredLevel()
	static void endRequiredLevel()
	{
		Verbose::requiredLevels.pop_back();
		Verbose::maxRequiredLevel = *std::max_element(Verbose::requiredLevels.begin(), Verbose::requiredLevels.end());
	}

	// Affiche le message seulement si le niveau de verbose est superieur ou égal à tous les niveaux requis
	inline const Verbose& operator<<(const char* text) const { if (Verbose::level >= Verbose::maxRequiredLevel) std::cout << text; return *this; }
	inline const Verbose& operator<<(std::string text) const { if (Verbose::level >= Verbose::maxRequiredLevel) std::cout << text; return *this; }
	inline const Verbose& operator<<(int number) const { if (Verbose::level >= Verbose::maxRequiredLevel) std::cout << number; return *this; }
	inline const Verbose& operator<<(unsigned int uiNumber) const { if (Verbose::level >= Verbose::maxRequiredLevel) std::cout << uiNumber; return *this; }
	inline const Verbose& operator<<(float floatNumber) const { if (Verbose::level >= Verbose::maxRequiredLevel)  std::cout << floatNumber; return *this; }
	inline const Verbose& operator<<(long longNumber) const { if (Verbose::level >= Verbose::maxRequiredLevel) std::cout << longNumber; return *this; }
};

#endif // BILEVEL_SCHEDULING_VERBOSE 