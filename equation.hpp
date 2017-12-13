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
        
        vector findInitialSubintervals(double, double, int, pairVector&) const;
        void findByBisection(const vector&, double, double, pairVector&) const;
        void findBySuccessiveApprox(const vector&, double, double, pairVector&) const;
        void findByNewtonRaphson(const vector&, double, double, pairVector&) const;

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

        vector getExponents() const;
        vector getCoefficients() const;

        double f(double) const;
        double fprime(double) const;
        pairVector solve(Method, double, double, int, int) const;
        std::string possibleSolutions() const;
};