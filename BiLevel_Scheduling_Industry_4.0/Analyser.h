#ifndef BILEVEL_SCHEDULING_ANALYSER
#define BILEVEL_SCHEDULING_ANALYSER

#include "LSolver.h"
#include "FSolver.h"
#include "LSortRule.h"
#include "FLateToEarly.h"
#include "Solution.h"
#include "Parser.h"
#include "Generateur.h"
#include <fstream>
#include <sstream>
#include <iostream>


class Analyser
{
private:
    Verbose verbose;

public:
	Analyser();
	~Analyser();

	int run()
	{
        // Start of the program, we take a snapshot at the brekpoint
        Parser parser;
        std::string path;

        std::vector<ILeaderSelectRule*> sortRules;
        sortRules.push_back(new LSortRule(sortRule::LPTRULE));
        //sortRules.push_back(new LSortRule(sortRule::LWPTRULE));
        //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_CONST_LATENESS));
        //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_VAR_LATENESS));

        std::vector<IFollowerSwapRule*> swapRules;
        //swapRules.push_back(new FLateToEarly(swapRule::DUMB_METHOD));
        swapRules.push_back(new FLateToEarly(swapRule::LATE2EARLY));

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
            std::cout << "erreur ouverture savefile\n";
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

            verbose.setRequiredLevel(2);
            verbose << "Instance File '" << instance.getInstancePath().string() << "'\n";
            if ((counter + 1) % 100 == 0)
            {
                verbose.setRequiredLevel(1);
                verbose << counter + 1 << "\r";
            }

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
                    Solution solution = Solution(*(solver->getSolution()));

                    // Enregistrement du résultat
                    parser.saveInFile(saveFile, solver, optimal_objective);

                    verbose.setRequiredLevel(1);
                    verbose << "\n\n";
                }
            }
            counter++;
        }

        verbose.setRequiredLevel(1);
        verbose << counter + 1 << "\n";

        if (solver != nullptr) { delete solver; }
        if (subSolver != nullptr) { delete subSolver; }

        for (ILeaderSelectRule* selectRule : sortRules)
        {
            delete selectRule;
        }

        for (IFollowerSwapRule* swapRule : swapRules)
        {
            delete swapRule;
        }
	}

    int run(std::string path)
    {
        //// Testing single instance //
        Parser parser;
        ISolver* solver = nullptr;
        ISubSolver* subSolver = nullptr;

        std::vector<ILeaderSelectRule*> sortRules;
        sortRules.push_back(new LSortRule(sortRule::LPTRULE));
        //sortRules.push_back(new LSortRule(sortRule::LWPTRULE));
        //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_CONST_LATENESS));
        //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_VAR_LATENESS));

        std::vector<IFollowerSwapRule*> swapRules;
        //swapRules.push_back(new FLateToEarly(swapRule::DUMB_METHOD));
        swapRules.push_back(new FLateToEarly(swapRule::LATE2EARLY));

        std::string saveFile = "C:\\Users\\benhi\\source\\repos\\BiLevel_Scheduling_Industry_4.0/instances/instances60job/resultLPT_LATE2EARLY.csv";
        std::fstream fileStreamSaveFile(saveFile, std::fstream::out);
        if (fileStreamSaveFile.is_open())
        {
            fileStreamSaveFile << "InstanceName;InstancePath;sumCj;fonction objective (sum wjUj);";
            fileStreamSaveFile << "N;n;m;HighSpeed Scheduling;LowSpeed Scheduling;Heuristic name;Heuristic description;time\n";
            fileStreamSaveFile.close();
        }
        else {
            std::cout << "erreur ouverture savefile\n";
            return 1;
        }


        Instance instance = parser.readFromFile(path);
        std::cout << instance.getInstancePath() << std::endl;

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
                Solution solution = Solution(*(solver->getSolution()));

                // Enregistrement du résultat
                parser.saveSolutionInFile(saveFile, solver);

                //std::cout << "\n\n";
            }
        }
        return 0;
    }

    int generateRandomInstance()
    {
        // Sauvegarde et Chargement d'instance 
        Generateur randomInstancer = Generateur();
        Parser parser;
        std::string path = "C:/Users/benhi/source/repos/BiLevel_Scheduling_Industry_4.0/instances/performances/n_3_N_15_tf_0.2_rdd_0.2_mMax_2_m0_2/instance0.txt";


        Instance instance = randomInstancer.generateInstance(path, 32, 3, 2, 10, 5);

        std::cout << instance << "\n";

        std::cout << "sauvegarde\n";
        parser.serializeInstance(instance);
        std::cout << "chargement\n";
        Instance loadedInstance = parser.readFromFile(path);

        std::cout << loadedInstance << "\n";

        std::cout << "Hello World!\n";
    }
};

#endif // BILEVEL_SCHEDULING_ANALYSER
