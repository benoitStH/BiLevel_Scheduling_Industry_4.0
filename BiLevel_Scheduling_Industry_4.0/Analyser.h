#ifndef BILEVEL_SCHEDULING_ANALYSER
#define BILEVEL_SCHEDULING_ANALYSER

#include "LSolver.h"
#include "FSolver.h"
#include "LSortRule.h"
#include "FSwapRule.h"
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
    Analyser() {};
    ~Analyser() {};

	int run(int argc, char* argv[]) // .exe testFile saveFile [verbose]
	{
        int minArgC = 2;
        if (argc - 1 < minArgC)
        {
            cout << "This program requires at least " << minArgC << " argument(s)\n";
            cout << "Mandatory Arguments are : file path of MIP Results to compare & which file path of heuristics results will be saved\n";
            return -1;
        }

        int maxArgC = 3;
        if (argc - 1 > maxArgC)
        {
            cout << "This program handles at most " << maxArgC << " arguments, " << argc - 1 << " were given\n";
            cout << "Arguments are : file path of MIP Results to compare & which file path of heuristics results will be saved & verbose\n";
            return -1;
        }

        // Setting the level of verbose (default = 0)
        verbose.setLevel((argc - 1 == maxArgC ? std::atoi(argv[argc - 1]) : 0));

        // Start of the program, we take a snapshot at the breakpoint
        Parser parser;
        std::string path;

        std::vector<ILeaderSelectRule*> sortRules;
        sortRules.push_back(new LSortRule(sortRule::LPTRULE));
        //sortRules.push_back(new LSortRule(sortRule::LWPTRULE));
        //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_CONST_LATENESS));
        //sortRules.push_back(new LSortRule(sortRule::SPT_EDD_VAR_LATENESS));

        std::vector<IFollowerSwapRule*> swapRules;
        //swapRules.push_back(new FLateToEarly(swapRule::DUMB_METHOD));
        swapRules.push_back(new FSwapRule(swapRule::LATE2EARLY));

        std::vector<std::string> instancePath_list;
        std::vector<std::string> exactMethodTimeScore_list;
        std::vector<unsigned int> exactMethodObjectiveScore_list;

        std::filesystem::path testFile(argv[1]);

        std::string saveFile = argv[2];
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

        unsigned int counter = 0;

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

            if ((counter + 1) % 100 == 0)
            {
                verbose.setRequiredLevel(1);
                verbose << counter + 1 << "\r";
                verbose.endRequiredLevel();
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

                    // Solving the instance
                    solver->solve();

                    // Extracting solution
                    Solution solution = Solution(*(solver->getSolution()));

                    // Saving result
                    parser.saveInFile(saveFile, solver, optimal_objective);

                    verbose.setRequiredLevel(2);
                    verbose << "\n\n";
                    verbose.endRequiredLevel();
                }
            }
            counter++;
        }

        verbose.setRequiredLevel(1);
        verbose << "\n" << counter << " instances solved\n";
        verbose.endRequiredLevel();

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

        return 0;
	}

    int runSingleInstance(int argc, char* argv[]) // .exe instanceFile saveFile [verbose]
    {
        int minArgC = 1;
        if (argc - 1 < minArgC)
        {
            cout << "This program requires at least " << minArgC << " argument(s)\n";
            cout << "Mandatory Arguments are : instance's file path & which file path the heuristic's results will be saved\n";
            return -1;
        }

        int maxArgC = 2;
        if (argc - 1 > maxArgC)
        {
            cout << "This program handles at most " << maxArgC << " arguments, " << argc - 1 << " were given\n";
            cout << "Arguments are : instance's file path & which file path the heuristic's results will be saved & verbose\n";
            return -1;
        }

        // Setting the level of verbose (default = 0)
        verbose.setLevel((argc - 1 == maxArgC ? std::atoi(argv[argc - 1]) : 0));


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
        swapRules.push_back(new FSwapRule(swapRule::LATE2EARLY));

        std::string saveFile = argv[2];
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

        std::string instanceFile = argv[1];
        Instance instance = parser.readFromFile(instanceFile);
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

                // Solving the instance
                solver->solve();

                // Extracting solution
                Solution solution = Solution(*(solver->getSolution()));

                // Saving result
                parser.saveSolutionInFile(saveFile, solver);
            }
        }
        return 0;
    }

    int generateRandomInstance(int argc, char* argv[]) // .exe instancePath [verbose]
    {
        int minArgC = 1;
        if (argc - 1 < minArgC)
        {
            cout << "This program requires at least " << minArgC << " argument(s)\n";
            cout << "Mandatory Arguments are : instance's file path where it will be saved\n";
            return -1;
        }

        int maxArgC = 2;
        if (argc - 1 > maxArgC)
        {
            cout << "This program handles at most " << maxArgC << " arguments, " << argc - 1 << " were given\n";
            cout << "Arguments are : instance's file path where it will be saved & verbose\n";
            return -1;
        }

        // Setting the level of verbose (default = 0)
        verbose.setLevel((argc - 1 == maxArgC ? std::atoi(argv[argc - 1]) : 0));

        // Sauvegarde et Chargement d'instance 
        Generateur randomInstancer = Generateur();
        Parser parser;
        std::string path = argv[1];


        Instance instance = randomInstancer.generateInstance(path, 32, 3, 2, 10, 5);

        verbose.setRequiredLevel(1);
        verbose << instance << "\n";

        verbose << "sauvegarde\n";
        parser.serializeInstance(instance);
        verbose << "chargement\n";
        Instance loadedInstance = parser.readFromFile(path);

        verbose << loadedInstance << "\n";
        verbose.endRequiredLevel();

        return 0;
    }
};

#endif // BILEVEL_SCHEDULING_ANALYSER
