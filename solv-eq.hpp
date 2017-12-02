#include <vector>
#include <map>
#include <utility>
#include <string>

struct equation;
enum method { bisection, approximation, newton };

using vector = std::vector<double>;
using pair = std::pair<double, double>;
using enum_map = std::map<method, std::string>;
using pair_vector = std::vector<pair>;
using func_ptr = pair (*)(const equation&, double, double, double);

enum_map method_descriptions = 
{
    {bisection, "Bisección"}, 
    {approximation, "Aproximaciones Sucesivas"}, 
    {newton, "Newton-Raphson"}
};

struct equation
{
    vector coefficients, exponents;
};

struct eq_evaluation
{
    equation& eq;
    double start, end;
    int n_intervals, signif_figs;
    method use_method;
    eq_evaluation(equation& eq) : eq(eq) {}
};

template<typename T>
void read_var(std::istream&, T&, char);

void read_variables(eq_evaluation&);
void ask_possible_methods();
pair_vector find_answers(const eq_evaluation&);
double calc_step(const eq_evaluation&);
double calc_max_rel_error(int);
vector find_initial_subintervals(const eq_evaluation&, double, pair_vector&);
double f_x(const equation&, double);
pair binary_search_answer(const equation&, double, double, double);
void print_equation(const equation&);
void print_results(const pair_vector&, const eq_evaluation&);
void print_answers(const pair_vector&);
void print_error_values(double);