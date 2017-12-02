#include "equation.hpp"

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

std::ostream& operator<<(std::ostream& out, const Equation& eq)
{
    for (int i = 0; i < eq.coefficients.size(); i++)
    {
        if (eq.coefficients.at(i) == 0) continue;

        else if (eq.exponents.at(i) == 0)
        {
            out << eq.coefficients.at(i);
            continue;
        }

        else if (eq.coefficients.at(i) > 1 || eq.coefficients.at(i) < -1)
        {
            out << eq.coefficients.at(i);
        }

        else if (eq.coefficients.at(i) == -1)
        {
            out << "-";
        }

        out << "x^" << eq.exponents.at(i) << " ";
    }
}

std::istream& operator>>(std::istream& in, Equation& eq)
{
    double n;
    while (in >> n) { eq.exponents.push_back(n); }
    in.clear();
    in.ignore(256, '\n');
    while (in >> n) { eq.coefficients.push_back(n); }
    in.clear();
    in.ignore(256, '\n');
}