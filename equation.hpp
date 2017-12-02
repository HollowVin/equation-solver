#pragma once
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
};