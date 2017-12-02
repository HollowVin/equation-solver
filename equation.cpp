#include "equation.hpp"

Equation::Equation(vector exp, vector coeff): 
    exponents(exp), coefficients(coeff) 
{
    int expSize = exp.size();
    int coeffSize = coeff.size();
    terms = expSize < coeffSize ? expSize : coeffSize;
}

vector Equation::GetExponents() { return exponents; }
vector Equation::GetCoefficients() { return coefficients; }

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
    int size1 = 0;
    while (in >> n) 
    { 
        eq.exponents.push_back(n); 
        size1++; 
    }
    in.clear();
    in.ignore(256, '\n');
    
    int size2 = 0;
    while (in >> n) 
    { 
        eq.coefficients.push_back(n); 
        size2++; 
    }
    in.clear();
    in.ignore(256, '\n');

    eq.terms = size1 <= size2 ? size1 : size2;
    return in;
}