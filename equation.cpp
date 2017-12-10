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
        result += std::pow(x, exponents[i]) * coefficients[i];
    }

    return result;
}

double Equation::fprime(double x)
{
    vector terms;
    double currentTerm = coefficients[0];
    terms.push_back(currentTerm);
    
    for (int i = 1; i < coefficients.size() - 1; i++)
    {
        terms.push_back(terms[i - 1] * x + coefficients[i]);
    }

    double derivative = terms[0];

    for (int i = 1; i < terms.size(); i++)
    {
        derivative = derivative * x + terms[i];
    }

    return derivative;
}

std::ostream& operator<<(std::ostream& out, const Equation& eq)
{
    for (int i = 0; i < eq.terms; i++)
    {
        double coefficient = eq.coefficients.at(i);
        double exponent = eq.exponents.at(i);
        
        if (coefficient == 0)       { continue; }
        else if (coefficient < 0)   { out << "- "; }
        else if (i > 0)             { out << "+ "; }

        if (exponent == 0 || coefficient > 1 || coefficient < -1) { out << std::abs(coefficient); } 
        if (exponent != 0) { out << "x"; }
        if (exponent > 1 || exponent < 0) { out << "^" << exponent; }

        out << " ";
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
    iss.str(str);
    while (iss >> n) 
    { 
        eq.coefficients.push_back(n);
        size2++; 
    }
    iss.clear();

    eq.terms = size1 <= size2 ? size1 : size2;
    return in;
}

pairVector Equation::solve(Method m, double start, double end, int subdiv, int figs)
{
    if (start > end) { swap(start, end); }

    pairVector answers;
    double step = (end - start) / subdiv;
    double error = 0.5 * std::pow(10, -figs);
    vector subintervals = findInitialSubintervals(start, step, subdiv, answers);
    
    switch (m)
    {
        case Method::bisection:
            findByBisection(subintervals, step, error, answers);
            break;
        case Method::approximation:
            findBySuccessiveApprox(subintervals, step, error, answers);
            break;
        /*case Method::newton:
            newtonRaphson();
            break;*/
    }

    return answers;
}

void Equation::swap(double& a, double& b)
{
    double temp = a;
    a = b;
    b = temp;
}

vector Equation::findInitialSubintervals(double start, double step, int subdiv, pairVector& answers)
{
    vector subintervals;
    double end = start + step;
    
    for (int i = 0; i < subdiv; i++)
    {   
        if (f(start) * f(end) < 0) { subintervals.push_back(start); }
        else if (f(start) == 0) { answers.push_back(pair(start, 0)); }

        start = end;
        end = start + step;
    }

    if ((f(end) == 0)) { answers.push_back(pair(end, 0)); }

    return subintervals;
}

void Equation::findByBisection(const vector& subintervals, double step, double error, pairVector& answers)
{
    for (int i = 0; i < subintervals.size(); i++)
    {
        double start = subintervals[i];
        double mid = start + step / 2;
        double end = start + step;
        double currentError = 0;
        double prevGuess = 0;

        for (int j = 0;; j++)
        {
            if (f(mid) == 0) 
            {
                currentError = 0;
                break;
            }

            currentError = std::abs((mid - prevGuess) / mid);

            if (j > 0 && currentError <= error)  { break; }
            if (f(start) * f(mid) < 0)  { end = mid;}
            else                        { start = mid;}

            prevGuess = mid;
            mid = (start + end) / 2;
        }

        answers.push_back(pair(mid, currentError));
    }
}

void Equation::findBySuccessiveApprox(const vector& subintervals, double step, double error, pairVector& answers)
{
    for (int i = 0; i < subintervals.size(); i++)
    {
        double start = subintervals[i];
        double prevGuess = start + step / 2;
        double prevError = 0;

        if (f(prevGuess) == 0)
        {
            answers.push_back(pair(prevGuess, 0));
            continue;
        }

        for (int j = 0;; j++)
        {
            double x = f(prevGuess) + prevGuess;
            if (x < start || x > start + step || j > 20) { break; }

            double currentError = std::abs((x - prevGuess) / x);
            if (currentError < error)
            {
                answers.push_back(pair(x, currentError));
                break;
            }

            prevGuess = x;
            prevError = currentError;
        }
    }
}