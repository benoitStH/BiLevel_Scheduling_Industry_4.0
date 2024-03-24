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
#include <fstream>
#include <sstream>
using namespace std;

// Method called after main has returned and before program terminates.
// We use a breakpoint here to take a snapshot of the memory in VS 2019 and check any memory leaks between the two snapshots
// No memory leaks should be here, but strangely some memory are still allocated by fstream library
struct AtExit
{
    ~AtExit() { float x = 0; }
} doAtExit;

int main()
{
    // TODO: Tester avec de grosse instance, générer plusieurs grosses instances
    // Check : taux d'amélioration entre solution initial et final
    // TODO : diapo règles essayées, tps de calcul pour instance X, comparaisons
    // TOTRY : Diviser par speed à la fin des calculs de dates de fin

    // Start of the program, we take a snapshot at the brekpoint
    Parser parser = Parser();
    std::string path = "C:/Users/benhi/source/repos/BiLevel_Scheduling_Industry_4.0/instances/performances/n_3_N_15_tf_0.2_rdd_0.2_mMax_2_m0_2/instance0.txt";
    //Instance instance;
    bool generating = false;


    std::vector<ILeaderSelectRule*> sortRules;
    sortRules.push_back(new LSortRule(sortRule::LPTRULE));
    //sortRules.push_back(new LSortRule(sortRule::LWPTRULE));
    //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_CONST_LATENESS));
    //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_VAR_LATENESS));

    std::vector<IFollowerSwapRule*> swapRules;
    //swapRules.push_back(new FLateToEarly(swapRule::DUMB_METHOD));
    swapRules.push_back(new FLateToEarly(swapRule::LATE2EARLY));


    //// Testing single instance //
    //instance = parser.readFromFile(path);
    //cout << instance.getInstancePath() << std::endl;

    //for (ILeaderSelectRule* selectRule : sortRules)
    //{
    //    for (IFollowerSwapRule* swapRule : swapRules)
    //    {
    //        FSolver subSolver;
    //        subSolver.addRule(swapRule);

    //        LSolver solver;
    //        solver.setInstance(&instance);
    //        solver.setSubSolver(&subSolver);
    //        solver.addRule(selectRule);

    //        // Le solveur résout l'instance
    //        solver.solve();

    //        // Affichage du résultat
    //        //cout << "Time : " << solver.getTimeResol() << " microseconde(s)\n";
    //        Solution solution = Solution(*(solver.getSolution()));
    //        std::cout << "Final Solution\n\n";
    //        solution.compactPrint();

    //        cout << solver.getHeuristicName() << std::endl;
    //        cout << solver.getHeuristicDescription();

    //        // Enregistrement du résultat
    //        //parser.saveInFile(saveFile, instance, &solver, optimal_objective);

    //        //std::cout << "\n\n";
    //    }
    //}
    //return 0;

    // Tests avec plusieurs solveurs ayant chacun une règle
    if (generating == false)
    {
        std::vector<std::string> instancePath_list;
        std::vector<std::string> exactMethodTimeScore_list;
        std::vector<unsigned int> exactMethodObjectiveScore_list;

        std::filesystem::path testFile("C:/Users/benhi/source/repos/BiLevel_Scheduling_Industry_4.0/instances/instances60job/resultsMIP.csv");

        std::string saveFile = "C:\\Users\\benhi\\source\\repos\\BiLevel_Scheduling_Industry_4.0/instances/instances60job/resultLPT_LATE2EARLY.csv";
        std::fstream fileStreamSaveFile(saveFile, std::fstream::out);
        if (fileStreamSaveFile.is_open())
        {
            fileStreamSaveFile << "InstanceName;InstancePath;sumCj;fonction objective (sum wjUj);fonction objective MIP;";
            fileStreamSaveFile << "N;n;m;HighSpeed Scheduling;LowSpeed Scheduling;Heuristic name;Heuristic description;time;deviation;precision\n";
            fileStreamSaveFile.close();
        }
        else {
            cout << "erreur ouverture savefile\n";
            return 1;
        }


        unsigned int optimal_objective;
        std::string line;
        std::string element;

        ISolver* solver = nullptr;
        ISubSolver* subSolver = nullptr;

        unsigned int counter = 1;

        std::fstream filePerf(testFile, std::fstream::in);
        if (filePerf.is_open())
        {

            std::getline(filePerf, line); // skipping first line with column's name
            while (std::getline(filePerf, line)) {

                std::istringstream stream(line);


                // instance name
                std::getline(stream, element, '\t');
                // instance path
                std::getline(stream, element, '\t');
                instancePath_list.push_back(element);

                // Time
                std::getline(stream, element, '\t');
                exactMethodTimeScore_list.push_back(element);

                // LimitTime
                std::getline(stream, element, '\t');

                // Objective
                stream >> optimal_objective;
                exactMethodObjectiveScore_list.push_back(optimal_objective);
            };
            filePerf.close();
        }


        for (unsigned int i = 0; i < instancePath_list.size(); i++)
        {
            Instance instance = parser.readFromFile(instancePath_list[i]);
            optimal_objective = exactMethodObjectiveScore_list[i];

            //cout << instance->getInstancePath() << std::endl;
            if ((counter + 1) % 100 == 0)
                cout << counter + 1 << "\r";

            /* if (instance->getInstancePath() == "C:/Users/benhi/source/repos/BiLevel_Scheduling_Industry_4.0/instances/performances/n_10_N_20_tf_0.8_rdd_0.2_mMax_1_m0_1/instance2.txt")
             {
                 instance->getHighSpeed();
             }*/

            for (ILeaderSelectRule* selectRule : sortRules)
            {
                for (IFollowerSwapRule* swapRule : swapRules)
                {
                    if (subSolver != nullptr) { delete subSolver; }
                    subSolver = new FSolver();
                    subSolver->addRule(swapRule);

                    if (solver != nullptr) { delete solver; }
                    solver = new LSolver();
                    solver->setInstance(&instance);
                    solver->setSubSolver(subSolver);
                    solver->addRule(selectRule);

                    // Le solveur résout l'instance
                    solver->solve();

                    // Affichage du résultat
                    //cout << "Time : " << solver.getTimeResol() << " microseconde(s)\n";
                    Solution solution = Solution(*(solver->getSolution()));
                    /*std::cout << "Final Solution\n\n";
                    solution.compactPrint();

                    cout << solver.getHeuristicName() << std::endl;
                    cout << solver.getHeuristicDescription();*/

                    // Enregistrement du résultat
                    parser.saveInFile(saveFile, instance, solver, optimal_objective);

                    //std::cout << "\n\n";
                }
            }
            counter++;
        }

        if (solver != nullptr) { delete solver; }
        if (subSolver != nullptr) { delete subSolver; }
        
    }


    for (ILeaderSelectRule* selectRule : sortRules)
    {
        delete selectRule;
    }

    for (IFollowerSwapRule* swapRule : swapRules)
    {
        delete swapRule;
    }

    // Test avec un solveur utilisant plusieurs règles //
    //if (generating == false)
    //{
    //    instance = parser.readFromFile(path);
    //    cout << instance;

    //    // LSolver : Generalisation d'un solveur
    //    std::vector<ILeaderSelectRule*> listLRules;
    //    std::vector<IFollowerSwapRule*> listFRules;

    //    // Allocatiing rules
    //    listLRules.push_back(new LSortRule(sortRule::LPTRULE));
    //    listLRules.push_back(new LSortRule(sortRule::SPT_EDD_CONST_LATENESS));
    //    listLRules.push_back(new LSortRule(sortRule::SPT_EDD_VAR_LATENESS));
    //    //listLRules.push_back(new LSortRule(sortRule::InvEDDRULE));

    //    //listFRules.push_back(new FLateToEarly(swapRule::LATE2EARLY));
    //    listFRules.push_back(new FLateToEarly(swapRule::DUMB_METHOD));

    //    // Creating the Solver for the Follower and Leader
    //    FSolver subsolver(listFRules);

    //    LSolver solver;
    //    solver.setInstance(&instance);
    //    solver.setRules(listLRules);
    //    solver.setSubSolver(&subsolver);

    //    // Solving the instance
    //    solver.solve();

    //    cout << "Time : " << solver.getTimeResol() << " microseconde(s)\n";

    //    // Show solution
    //    Solution solution = Solution(*(solver.getSolution()));
    //    std::cout << "Final Solution\n\n";
    //    solution.compactPrint();
    //    
    //    cout << solver.getHeuristicName() << std::endl;
    //    cout << solver.getHeuristicDescription();

    //    // Deallocating Rules
    //    for (ILeaderSelectRule* selectRule : listLRules)
    //    {
    //        delete selectRule;
    //    }

    //    for (IFollowerSwapRule* swapRule : listFRules)
    //    {
    //        delete swapRule;
    //    }
    //}
    //    

    

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

        Instance instance = randomInstancer.generateInstance(path, 32, 3, 2, 10, 5);

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
