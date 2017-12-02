#include <vector>
#include <utility>

using vector = std::vector<double>;
using pair = std::pair<double, double>;
using pair_vector = std::vector<pair>;

struct equation;
struct eq_evaluation;

template<typename T>
void read_var(std::istream&, T&);

template<typename T>
void read_var(std::istream&, T&, char);

void read_variables(eq_evaluation&);
pair_vector find_bisection_answers(const eq_evaluation&);
double calc_step(const eq_evaluation&);
double calc_max_rel_error(int);
vector find_initial_subintervals(const eq_evaluation&, double, pair_vector&);
double f_x(const equation&, double);
pair binary_search_answer(const equation&, double, double, double);
void print_equation(const equation&);
void print_results(const pair_vector&, const eq_evaluation&);
void print_answers(const pair_vector&);
void print_error_values(double);