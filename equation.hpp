#pragma once
#include <iostream>
#include <cmath>
#include "dependencies.hpp"

class Equation
{
    private:
        vector exponents;
        vector coefficients;
    public:
        Equation(vector, vector);
        vector GetExponents();
        vector GetCoefficients();
        void SetExponents(vector);
        void SetCoefficients(vector);
        double fx (double);
        void PrintEquation();
        friend std::ostream& operator<<(std::ostream&, const Equation&);
        friend std::istream& operator>>(std::istream&, Equation&);
};