// BiLevel_Scheduling_Industry_4.0.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "Instance.h"
#include "Parser.h"
#include "Generateur.h" // for random instance generation
#include <iostream>
#include "LSolver.h"
#include "FSolver.h"
#include "LSortRule.h"
#include "FLateToEarly.h"
using namespace std;

int main()
{
    // TODO: Tester avec de grosse instance, générer plusieurs grosses instances
    // Check : taux d'amélioration entre solution initial et final
    // TODO : diapo règles essayées, tps de calcul pour instance X, comparaisons
    // TOTRY : Diviser par speed à la fin des calculs de dates de fin
    Parser parser = Parser();
    std::string path = "C:/Users/benhi/source/repos/BiLevel_Scheduling_Industry_4.0/instances/instance4.txt";
    Instance instance;
    bool generating = false;

    if (generating == false)
    {
        instance = parser.readFromFile(path);

        // LSolver : Generalisation d'un solveur
        std::vector<ILeaderSelectRule*> listLRules;
        std::vector<IFollowerSwapRule*> listFRules;

        // Allocatiing rules
        listLRules.push_back(new LSortRule(sortRule::LPTRULE));
        listLRules.push_back(new LSortRule(sortRule::LWPTRULE));

        listFRules.push_back(new FLateToEarly());

        // Creating the Solver for the Follower and Leader
        FSolver subsolver(listFRules);
        LSolver solver;
        solver.setInstance(&instance);
        solver.setRules(listLRules);
        solver.setSubSolver(&subsolver);

        // Solving the instance
        solver.solve();

        // Show solution
        Solution solution = Solution(*(solver.getSolution()));
        std::cout << "Final Solution\n\n";
        solution.compactPrint();

        // Deallocating Rules
        for (ILeaderSelectRule* selectRule : listLRules)
        {
            delete selectRule;
        }

        for (IFollowerSwapRule* swapRule : listFRules)
        {
            delete swapRule;
        }
    }
        

    

    //// Première heuristique : DeepestDescent
    //ISolver* solver = new DeepestDescent();

    //solver->setInstance(&instance);
    //solver->solve();

    //Solution s = Solution(*(solver->getSolution()));
    //cout << "Solver's Solution\n";
    //s.compactPrint();

    //delete solver;

    /* // Création d'une solution initiale 
    Generateur generateur;

    Solution solution_init = generateur.generateInitialSolution(instance);

    //cout << solution_init;
    solution_init.print();
    solution_init.compactPrint();
    */

    //Machine machine = Machine(10);

    if (generating)
    {
        // Sauvegarde et Chargement d'instance 
        Generateur randomInstancer = Generateur();
        Parser parser = Parser();

        Instance instance = randomInstancer.generateInstance(path, 30, 3, 2, 10, 5);

        cout << instance;

        cout << "sauvegarde\n";
        parser.serializeInstance(instance);
        cout << "chargement\n";
        Instance loadedInstance = parser.readFromFile(path);

        cout << loadedInstance;

        std::cout << "Hello World!\n";
    }
    
    
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
