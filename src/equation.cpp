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
    clearZeroCoefficients();
}

vector Equation::getExponents() const { return exponents; }
vector Equation::getCoefficients() const { return coefficients; }

double Equation::f(double x) const
{
    double result = 0;
    
    for (int i = 0; i < terms; i++)
    {
        result += std::pow(x, exponents[i]) * coefficients[i];
    }

    return result;
}

double Equation::fprime(double x) const
{
    pairVector joinedVectors = joinExponentAndCoefficientVectors();
    std::sort(joinedVectors.begin(), joinedVectors.end(), compare);
    fillVector(joinedVectors);
    
    vector termsVector;
    double currentTerm = joinedVectors[0].second;
    termsVector.push_back(currentTerm);
    
    for (int i = 1; i < joinedVectors.size() - 1; i++)
    {
        termsVector.push_back(termsVector[i - 1] * x + joinedVectors[i].second);
    }

    double derivative = termsVector[0];

    for (int i = 1; i < termsVector.size(); i++)
    {
        derivative = derivative * x + termsVector[i];
    }

    return derivative;
}

bool Equation::compare(pair first, pair second)
{
    return first > second;
}

void Equation::fillVector(pairVector& vector)
{
    double largestExponent = vector[0].first;
    for (int i = 1; i <= largestExponent; i++)
    {
        if (vector[i].first != largestExponent - i || i > vector.size() - 1)
        {
            vector.insert(vector.begin() + i, pair(largestExponent - i, 0));
        }
    }
}

pairVector Equation::joinExponentAndCoefficientVectors() const
{
    pairVector joinedVector;
    
    for (int i = 0; i < terms; i++)
    {
        joinedVector.push_back(pair(exponents[i], coefficients[i]));
    }

    return joinedVector;
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
    eq.clearZeroCoefficients();
    return in;
}

pairVector Equation::solve(Method m, double start, double end, int subdiv, int figs) const
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
        case Method::newton:
            findByNewtonRaphson(subintervals, step, error, answers);
            break;
    }

    return answers;
}

void Equation::swap(double& a, double& b)
{
    double temp = a;
    a = b;
    b = temp;
}

vector Equation::findInitialSubintervals(double start, double step, int subdiv, pairVector& answers) const
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

void Equation::findByBisection(const vector& subintervals, double step, double error, pairVector& answers) const
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

void Equation::findBySuccessiveApprox(const vector& subintervals, double step, double error, pairVector& answers) const
{
    for (int i = 0; i < subintervals.size(); i++)
    {
        double start = subintervals[i];
        double prevGuess = start + step / 2;

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
        }
    }
}

void Equation::findByNewtonRaphson(const vector& subintervals, double step, double error, pairVector& answers) const
{
    for (int i = 0; i < subintervals.size(); i++)
    {
        double start = subintervals[i];
        double prevGuess = start + step / 2;

        if (f(prevGuess) == 0)
        {
            answers.push_back(pair(prevGuess, 0));
            continue;
        }

        for (int j = 0;; j++)
        {
            std::cout << "Testing: " << prevGuess << std::endl;
            double x = prevGuess - f(prevGuess) / fprime(prevGuess);
            std::cout << fprime(prevGuess) << std::endl;
            if (x < start || x > start + step || j > 20) { break; }

            double currentError = std::abs((x - prevGuess) / x);
            if (currentError < error)
            {
                answers.push_back(pair(x, currentError));
                break;
            }

            prevGuess = x;
        }
    }
}

std::string Equation::possibleSolutions() const
{
    pairVector joinedVectors = joinExponentAndCoefficientVectors();
    std::sort(joinedVectors.begin(), joinedVectors.end(), compare);

    bool isPositive1 = joinedVectors[0].second > 0;
    bool isPositive2 = (fmod(joinedVectors[0].first, 2) == 0 && joinedVectors[0].second > 0) 
        || (fmod(joinedVectors[0].first, 2) != 0 && joinedVectors[0].second < 0);

    int maxPossiblePositives = 0;
    int maxPossibleNegatives = 0;
    int maxPossibleImaginary = fmod(joinedVectors[0].first, 2) == 0 ? joinedVectors[0].first : joinedVectors[0].first - 1;
    int maxPossibleSolutions = joinedVectors[0].first;
    int maxPossibleZeroes = 1;

    for (int i = 1; i < terms; i++)
    {
        if ((isPositive1 && joinedVectors[i].second < 0) || (!isPositive1 && joinedVectors[i].second > 0))
        {
            maxPossiblePositives += 1;
            isPositive1 = !isPositive1;
        }

        if ((isPositive2 && ((fmod(joinedVectors[i].first, 2) == 0 && joinedVectors[i].second < 0) 
                || (fmod(joinedVectors[i].first, 2) != 0 && joinedVectors[i].second > 0)))
            || (!isPositive2 && ((fmod(joinedVectors[i].first, 2) == 0 && joinedVectors[i].second > 0) 
                || (fmod(joinedVectors[i].first, 2) != 0 && joinedVectors[i].second < 0))))
        {
            maxPossibleNegatives += 1;
            isPositive2 = !isPositive2;
        }
    }
    
    std::stringstream solutions;
    solutions << "Posibilidades de soluciones:\n";

    for (int i = maxPossiblePositives; i >= 0; i -= 2)
    {
        for (int j = maxPossibleNegatives; j >= 0; j -= 2)
        {
            for (int k = maxPossibleImaginary; k >= 0; k -= 2)
            {
                for (int m = maxPossibleZeroes; m >= 0; m--)
                {
                    if (maxPossibleSolutions == i + j + k + m)
                    {
                        solutions << "* Positivas: " << i << " | Negativas: " << j << " | Cero: " << m << " | Imaginarias: " << k << "\n";
                    }
                }
            }
        }
    }

    return solutions.str();
}

void Equation::clearZeroCoefficients()
{
    for (int i = 0; i < terms; i++)
    {
        if (coefficients[i] == 0)
        {
            coefficients.erase(coefficients.begin() + i);
            exponents.erase(exponents.begin() + i);
            terms -= 1;
            i -= 1;
        }
    }
}