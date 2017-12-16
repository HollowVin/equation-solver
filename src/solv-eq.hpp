#include <vector>
#include <map>
#include <utility>
#include <string>
#include "equation.hpp"

using enumMap = std::map<Method, std::string>;
enumMap methodDescriptions = 
{
    {Method::bisection, "Bisection"}, 
    {Method::approximation, "Successive Approximations"}, 
    {Method::newton, "Newton-Raphson"}
};

struct EqEvaluation
{
    Method method;
    double start, end;
    int subdiv, figs;
};

void readSolveParameters(EqEvaluation&);
void askPossibleMethods();
void print_results(const pairVector&, const Equation&, const EqEvaluation&);
void print_answers(const pairVector& answers);