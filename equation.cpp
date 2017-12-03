#include "equation.hpp"

Equation::Equation()
{
    terms = 0;
}

Equation::Equation(vector exp, vector coeff): 
    exponents(exp), coefficients(coeff) 
{
    int expSize = exp.size();
    int coeffSize = coeff.size();
    terms = expSize < coeffSize ? expSize : coeffSize;
}

vector Equation::getExponents() { return exponents; }
vector Equation::getCoefficients() { return coefficients; }

double Equation::f(double x)
{
    double result = 0;
    
    for (int i = 0; i < terms; i++)
    {
        result += std::pow(x, exponents.at(i)) * coefficients.at(i);
    }

    return result;
}

std::ostream& operator<<(std::ostream& out, const Equation& eq)
{
    for (int i = 0; i < eq.terms; i++)
    {
        double coefficient = eq.coefficients.at(i);
        double exponent = eq.exponents.at(i);
        
        if (coefficient == 0) { continue; }

        else if (exponent == 0)
        {
            out << coefficient;
            continue;
        }

        else if (coefficient > 1 || coefficient < -1)
        {
            out << coefficient;
        }

        else if (coefficient == -1)
        {
            out << "-";
        }

        out << "x^" << exponent << " ";
    }

    return out;
}

std::istream& operator>>(std::istream& in, Equation& eq)
{
    eq.exponents.clear();
    eq.coefficients.clear();
    
    double n;
    std::string str;
    int size1 = 0;

    std::getline(in, str);
    std::istringstream iss(str);    
    while (iss >> n)
    { 
        eq.exponents.push_back(n); 
        size1++; 
    }
    iss.clear();
    
    int size2 = 0;
    std::getline(in, str);
    while (iss >> n) 
    { 
        eq.coefficients.push_back(n); 
        size2++; 
    }
    iss.clear();

    eq.terms = size1 <= size2 ? size1 : size2;
    return in;
}

pairVector* Equation::solve(Method m, double start, double end, int subdiv, int figs)
{
    if (start > end) { swap(start, end); }

    pairVector* answers = new pairVector();
    double step = (end - start) / subdiv;
    double error = 0.5 * pow(10, -figs);
    vector subintervals = findInitialSubintervals(start, step, subdiv, answers);
    
    switch (m)
    {
        case Method::bisection:
            findByBisection(subintervals, step, error, answers);
        case Method::approximation:
            findBySuccessiveApprox(subintervals, step, error, answers);
        /*case Method::newton:
            newtonRaphson();*/
    }

    return answers;
}

void Equation::swap(double& a, double& b)
{
    double temp = a;
    a = b;
    b = temp;
}

vector Equation::findInitialSubintervals(double start, double step, int subdiv, pairVector* answers)
{
    vector subintervals;
    double end = start + step;
    
    for (int i = 0; i < subdiv; i++)
    {   
        if (f(start) * f(end) < 0) { subintervals.push_back(start); }
        else if (f(start) == 0) { answers->push_back(pair(start, 0)); }

        start = end;
        end = start + step;
    }

    if ((f(end) == 0)) { answers->push_back(pair(end, 0)); }

    return subintervals;
}

void Equation::findByBisection(const vector& subintervals, double step, double error, pairVector* answers)
{
    for (int i = 0; i < subintervals.size(); i++)
    {
        double start = subintervals[i];
        double mid = start + step / 2;
        double end = start + step;
        double currentError = error + 1;
        double prevGuess = start - 1;

        while (true)
        {
            if (f(mid) == 0) 
            {
                currentError = 0;
                break;
            }

            currentError = abs((mid - prevGuess) / mid);

            if (currentError <= error)  { break; }
            if (f(start) * f(mid) < 0)  { end = mid; }
            else                        { start = mid; }

            prevGuess = mid;
            mid = (start + end) / 2;
        }

        answers->push_back(pair(mid, currentError));
    }
}

void Equation::findBySuccessiveApprox(const vector& subintervals, double step, double error, pairVector* answers)
{
    for (int i = 0; i < subintervals.size(); i++)
    {
        double start = subintervals[i];
        double prevGuess = start + step / 2;
        double prevError = 0;

        if (f(prevGuess) == 0)
        {
            answers->push_back(pair(prevGuess, 0));
            continue;
        }

        for (int i = 0;; i++)
        {
            double x = f(prevGuess) + prevGuess;
            double currentError = abs((x - prevGuess) / x);
            
            if (i > 0 && currentError > prevError) { break; }

            if (currentError < error)
            {
                answers->push_back(pair(x, currentError));
                break;
            }

            prevGuess = x;
            prevError = currentError;
        }
    }
}