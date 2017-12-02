#include <iostream>
#include <cmath>
#include "solv-eq.hpp"

struct equation
{
    vector coefficients, exponents;
};

struct eq_evaluation
{
    equation& eq;
    double start, end;
    int n_intervals, signif_figs;
    eq_evaluation(equation& eq) : eq(eq) {}
};

int main()
{
    equation eq;
    eq_evaluation eval(eq);

    read_variables(eval);
    pair_vector answers = find_bisection_answers(eval);
    print_results(answers, eval);

    return 0;
}

template<typename T>
void read_var(std::istream& cin, T& var)
{
    cin >> var;
}

template<typename T>
void read_var(std::istream& cin, T& var, char delim)
{
    cin >> var;
    std::cin.ignore(256, delim);
}

void read_variables(eq_evaluation& eval)
{
    int n_terms;
    std::cout << "Número de términos: ";
    read_var(std::cin, n_terms, '\n');

    std::cout << "Exponentes: ";
    for (int i = 0; i < n_terms; i++)
    {
        double e;
        read_var(std::cin, e);
        eval.eq.exponents.push_back(e);
    }
    std::cin.ignore(256, '\n');

    std::cout << "Coeficientes: ";
    for (int i = 0; i < n_terms; i++)
    {
        double c;
        std::cin >> c;
        eval.eq.coefficients.push_back(c);
    }
    std::cin.ignore(256, '\n');

    std::cout << "Intervalo: ";
    read_var(std::cin, eval.start);
    read_var(std::cin, eval.end);
    std::cin.ignore(256, '\n');
    std::cout << "Subdiv. y cifras signif.: ";
    read_var(std::cin, eval.n_intervals);
    read_var(std::cin, eval.signif_figs);
}

pair_vector find_bisection_answers(const eq_evaluation& eval)
{
    pair_vector answers;
    double step = calc_step(eval);
    double max_rel_error = calc_max_rel_error(eval.signif_figs);
    vector subintervals = find_initial_subintervals(eval, step, answers);

    for (int i = 0; i < subintervals.size(); i++)
    {
        double start = subintervals.at(i);
        double end = start + step;

        answers.push_back(binary_search_answer(eval.eq, start, end, max_rel_error));
    }

    return answers;
}

double calc_step(const eq_evaluation& eval)
{
    return std::abs(eval.end - eval.start) / eval.n_intervals;
}

double calc_max_rel_error(int signif_figs)
{
    return 0.5 * std::pow(10, -signif_figs);
}

vector find_initial_subintervals(const eq_evaluation& eval, double step, pair_vector& answers)
{
    vector subintervals;
    double start = eval.start;
    double end;
    
    for (int i = 0; i < eval.n_intervals; i++)
    {
        end = start + step;

        if ((f_x(eval.eq, start) * f_x(eval.eq, end)) < 0)
        {
            subintervals.push_back(start);
        }

        else if ((f_x(eval.eq, start) == 0))
        {
            answers.push_back(pair(start, 0));
        }

        start = end;
    }

    if ((f_x(eval.eq, end) == 0))
    {
        answers.push_back(pair(end, 0));
    }

    return subintervals;
}

double f_x(const equation& eq, double x)
{
    double result = 0;
    
    for (int i = 0; i < eq.coefficients.size(); i++)
    {
        result += std::pow(x, eq.exponents.at(i)) * eq.coefficients.at(i);
    }

    return result;
}

pair binary_search_answer(const equation& eq, double start, double end, double max_rel_error = 0)
{
    double prev_guess = start - 1;
    double mid, rel_error;

    while (true)
    {
        mid = (start + end) / 2;
        rel_error = std::abs((mid - prev_guess) / mid);

        if (rel_error == 0 || rel_error < max_rel_error)
        {
            break;
        }

        prev_guess = mid;

        if ((f_x(eq, start) * f_x(eq, mid)) < 0)
        {
            end = mid;
        }

        else if ((f_x(eq, mid) * f_x(eq, end)) < 0)
        {
            start = mid;
        }
    }

    return pair(mid, rel_error);
}

void print_results(const pair_vector& answers, const eq_evaluation& eval)
{
    double max_rel_error = calc_max_rel_error(eval.signif_figs);
    double step = calc_step(eval);

    std::cout << std::endl;
    print_equation(eval.eq);
    std::cout << "Intervalo de análisis: [" << eval.start << ", " << eval.end << "]" << std::endl;
    std::cout << "Subdivisiones: " << eval.n_intervals << std::endl;
    std::cout << "Delta x: " << step << std::endl;
    std::cout << "Cifras significativas confiables: " << eval.signif_figs << std::endl;
    std::cout << "Error relativo aproximado porcentual aceptable: " << max_rel_error * 100 << "%";
    std::cout << std::endl << std::endl;
    print_answers(answers);
}

void print_equation(const equation& eq)
{
    std::cout << "Ecuación ingresada: ";

    for (int i = 0; i < eq.coefficients.size(); i++)
    {
        if (eq.coefficients.at(i) == 0)
        {
            continue;
        }

        else if (eq.exponents.at(i) == 0)
        {
            std::cout << eq.coefficients.at(i);
            continue;
        }

        else if (eq.coefficients.at(i) > 1 || eq.coefficients.at(i) < -1)
        {
            std::cout << eq.coefficients.at(i);
        }

        else if (eq.coefficients.at(i) == -1)
        {
            std::cout << "-";
        }

        std::cout << "x^" << eq.exponents.at(i) << " ";
    }

    std::cout << std::endl;
}

void print_answers(const pair_vector& answers)
{
    if (answers.size() < 1)
    {
        std::cout << "No se hallaron respuestas" << std::endl << std::endl;
    }
    
    for (int i = 0; i < answers.size(); i++)
    {
        std::cout << "Respuesta " << (i + 1) << ": " << std::endl;
        std::cout << "\t* Valor: " << answers.at(i).first << std::endl;
        print_error_values(answers.at(i).second);
    }
}

void print_error_values(double rel_error)
{
    std::cout << "\t* Error relativo aproximado porcentual: " << rel_error * 100 << "%";
    std::cout << std::endl << std::endl;
}