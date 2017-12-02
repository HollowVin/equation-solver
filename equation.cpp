#include "equation.hpp"
#include <cmath>
#include <iostream>

Equation::Equation(vector exp, vector coeff): 
    exponents(exp), coefficients(coeff) {}

vector Equation::GetExponents() { return exponents; }
vector Equation::GetCoefficients() { return coefficients; }

void Equation::SetExponents(vector exps) { exponents = exps; }
void Equation::SetCoefficients(vector coeffs) { coefficients = coeffs; }

double Equation::fx(double x)
{
    double result = 0;
    
    for (int i = 0; i < coefficients.size(); i++)
    {
        result += std::pow(x, exponents.at(i)) * coefficients.at(i);
    }

    return result;
}

void Equation::PrintEquation()
{
    for (int i = 0; i < coefficients.size(); i++)
    {
        if (coefficients.at(i) == 0) continue;

        else if (exponents.at(i) == 0)
        {
            std::cout << coefficients.at(i);
            continue;
        }

        else if (coefficients.at(i) > 1 || coefficients.at(i) < -1)
        {
            std::cout << coefficients.at(i);
        }

        else if (coefficients.at(i) == -1)
        {
            std::cout << "-";
        }

        std::cout << "x^" << exponents.at(i) << " ";
    }
}