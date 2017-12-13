#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

using vector = std::vector<double>;
using pair = std::pair<double, double>;
using pairVector = std::vector<pair>;

enum class Method { bisection = 1, approximation, newton };

class Equation
{
    private:
        vector exponents;
        vector coefficients;
        int terms;
        
        vector findInitialSubintervals(double, double, int, pairVector&);
        void findByBisection(const vector&, double, double, pairVector&);
        void findBySuccessiveApprox(const vector&, double, double, pairVector&);
        void findByNewtonRaphson(const vector&, double, double, pairVector&);

        static void swap(double&, double&);
        static bool compare(pair, pair);
        static void fillVector(pairVector&);
        pairVector joinExponentAndCoefficientVectors() const;
        void clearZeroCoefficients();

    public:
        Equation();
        Equation(vector, vector);

        friend std::ostream& operator<<(std::ostream&, const Equation&);
        friend std::istream& operator>>(std::istream&, Equation&);

        vector getExponents();
        vector getCoefficients();

        double f(double);
        double fprime(double);
        pairVector solve(Method, double, double, int, int);
        std::string possibleSolutions() const;
};