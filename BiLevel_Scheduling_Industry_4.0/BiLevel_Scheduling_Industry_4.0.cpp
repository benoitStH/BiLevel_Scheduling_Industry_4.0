// BiLevel_Scheduling_Industry_4.0.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "Instance.h"
#include "Parser.h"
#include "Generateur.h" // for random instance generation
#include <iostream>
#include "DeepestDescent.h"
using namespace std;

int main()
{
    Parser parser = Parser();
    std::string path = "C:/Users/benhi/Desktop/instance1.txt";
    Instance instance = parser.readFromFile(path);

    // Première heuristique : DeepestDescent
    ISolver* solver = new DeepestDescent();

    solver->setInstance(&instance);
    solver->solve();

    Solution s = Solution(*(solver->getSolution()));
    cout << "Solver's Solution\n";
    s.compactPrint();

    delete solver;

    /* // Création d'une solution initiale 
    Generateur generateur;

    Solution solution_init = generateur.generateInitialSolution(instance);

    //cout << solution_init;
    solution_init.print();
    solution_init.compactPrint();
    */

    //Machine machine = Machine(10);

    /* // Sauvegarde et Chargement d'instance 
    Generateur randomInstancer = Generateur();
    Parser parser = Parser();

    std::string path = "C:/Users/benhi/Desktop/instance1.txt";
    Instance instance = randomInstancer.generateInstance(path, 10, 3, 2, 10, 5);

    cout << instance;
    
    cout << "sauvegarde\n";
    parser.serializeInstance(instance);
    cout << "chargement\n";
    Instance loadedInstance = parser.readFromFile(path);

    cout << loadedInstance;

    std::cout << "Hello World!\n";
    */
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
