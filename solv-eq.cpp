#include <iostream>
#include <cmath>
#include <map>
#include "solv-eq.hpp"

int main()
{
    Equation equation;
    EqEvaluation eval;

    std::cin >> equation;
    readSolveParameters(eval);

    pairVector* answers = equation.solve(eval.method, eval.start, eval.end, eval.subdiv, eval.figs);
    print_results(*answers, equation, eval);
    delete answers;

    return 0;
}

void readSolveParameters(EqEvaluation& eval)
{
    askPossibleMethods();
    int method;
    std::cin >> method;
    eval.method = static_cast<Method>(method);
    std::cin.ignore(256, '\n');

    std::cout << "Intervalo: ";
    std::cin >> eval.start >> eval.end;
    std::cin.ignore(256, '\n');

    std::cout << "Subdivisiones: ";
    std::cin >> eval.subdiv;
    std::cin.ignore(256, '\n');

    std::cout << "Cifras significativas: ";
    std::cin >> eval.figs;
}

void askPossibleMethods()
{
    std::cout << std::endl;

    for (auto i = methodDescriptions.begin(); i != methodDescriptions.end(); ++i)
    {
        int index = static_cast<int>(i->first);
        std::cout << index << ". " << i->second << std::endl;
    }
}

void print_results(const pairVector& answers, const Equation& equation, const EqEvaluation& eval)
{
    double max_rel_error = 0.5 * pow(10, -eval.figs);
    double step = std::abs((eval.start - eval.end) / eval.subdiv);

    std::cout << std::endl;
    std::cout << "Ecuación ingresada: " << equation << std::endl;
    std::cout << "Intervalo de análisis: [" << eval.start << ", " << eval.end << "]" << std::endl;
    std::cout << "Subdivisiones: " << eval.subdiv << std::endl;
    std::cout << "Delta x: " << step << std::endl;
    std::cout << "Cifras significativas confiables: " << eval.figs << std::endl;
    std::cout << "Error aceptable: " << max_rel_error * 100 << "%";
    std::cout << std::endl << std::endl;
    print_answers(answers);
}

void print_answers(const pairVector& answers)
{
    if (answers.size() < 1)
    {
        std::cout << "No se hallaron respuestas" << std::endl << std::endl;
    }
    
    for (int i = 0; i < answers.size(); i++)
    {
        std::cout << "Respuesta " << (i + 1) << ": " << std::endl;
        std::cout << "\t* Valor: " << answers[i].first << std::endl;
        std::cout << "\t* Error relativo aproximado porcentual: " << answers[i].second * 100 << "%";
        std::cout << std::endl << std::endl;
    }
}