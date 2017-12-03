#pragma once
#include <iostream>
#include <cmath>
#include "dependencies.hpp"

class Equation
{
    private:
        vector exponents;
        vector coefficients;
        int terms;
        
        pair binary_search_answer(const equation&, double, double, double);
        vector findInitialSubintervals(double, double, int, pair_vector*);
        void findByBisection(vector&, double, double, pair_vector*);
        void findBySuccessiveApprox(vector&, double, double, pair_vector*);

    public:
        Equation(vector, vector);

        friend std::ostream& operator<<(std::ostream&, const Equation&);
        friend std::istream& operator>>(std::istream&, Equation&);

        vector getExponents();
        vector getCoefficients();

        double f(double);

        enum method { bisection, approximation, newton };
        pair_vector* solve(Equation::method, double, double, int, int);
};