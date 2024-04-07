// BiLevel_Scheduling_Industry_4.0.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "Instance.h"
#include "Parser.h"
#include "Generateur.h" // for random instance generation
#include <iostream>
#include "LSolver.h"
#include "FSolver.h"
#include "LSortRule.h"
#include "FSwapRule.h"
#include <fstream>
#include <sstream>
#include "Analyser.h"
using namespace std;

// Method called after main has returned and before program terminates.
// We use a breakpoint here to take a snapshot of the memory in VS 2019 and check any memory leaks between the two snapshots
// No memory leaks should be here, but strangely some memory are still allocated by fstream library
struct AtExit
{
    ~AtExit() { float x = 0; }
} doAtExit;


std::vector<unsigned int> initVector()
{
    std::vector<unsigned int> v;
    v.push_back(0);
    return v;
}

unsigned int Verbose::level;
unsigned int Verbose::maxRequiredLevel;
vector<unsigned int> Verbose::requiredLevels = initVector();

int main(int argc, char* argv[]) // .exe instanceFile resultRepository [verbose]
{
    Verbose verbose; 
    
    // Example on how to use the analyser for several instance resolution and comparing with MIP results
    // Analyser analyser; analyser.run(argc, argv);

    // Example on how to use analyser for single instance resolution
    // Analyser analyser; analyser.runSingleInstance(argc, argv);

    // Example on how to use analyser for generating random solution
    // Analyser analyser; analyser.generateRandomInstance(argc, argv);

    int minArgC = 2;
    if (argc-1 < minArgC)
    {
        cout << "This program requires at least " << minArgC << " argument(s)\n";
        cout << "Mandatory Arguments are : instance's file path & result repository path\n";
        return -1;
    }

    int maxArgC = 3;
    if (argc-1 > maxArgC)
    {
        cout << "This program handles at most "<< maxArgC <<" arguments, " << argc - 1 << " were given\n";
        cout << "Arguments are : instance's file path & result repository path & verbose value\n";
        return -1;
    }

    Parser parser;
    std::string path = argv[1];
    std::string resultRepo = argv[2];
    resultRepo.append("/resultSPT_VL_DUMB1.csv");

    // Setting the level of verbose (default = 0)
    verbose.setLevel((argc-1 == maxArgC ? std::atoi(argv[argc-1]) : 0));

    verbose.setRequiredLevel(1);
    verbose << "\n";
    verbose << "Solving instance '" << path << "'\n";
    verbose.endRequiredLevel();

    Instance instance = parser.readFromFile(path);

    std::vector<ILeaderSelectRule*> sortRules;
    sortRules.push_back(new LSortRule(sortRule::SPT_EDD_VAR_LATENESS));

    std::vector<IFollowerSwapRule*> swapRules;
    swapRules.push_back(new FSwapRule(swapRule::DUMB_METHOD));


    ISolver* solver = nullptr;
    ISubSolver* subSolver = nullptr;

    subSolver = new FSolver(swapRules);

    solver = new LSolver();
    solver->setInstance(&instance);
    solver->setSubSolver(subSolver);
    solver->setRules(sortRules);

    // Solving the instance
    solver->solve();

    parser.saveSolutionInFile(resultRepo, solver);

    delete solver;
    delete subSolver;


    for (ILeaderSelectRule* selectRule : sortRules)
    {
        delete selectRule;
    }

    for (IFollowerSwapRule* swapRule : swapRules)
    {
        delete swapRule;
    }

    verbose.setRequiredLevel(1);
    verbose << "End of program\n";
    verbose.endRequiredLevel();

    return 0; 
    
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
