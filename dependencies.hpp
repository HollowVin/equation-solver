#pragma once
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

struct eq_evaluation
{
    equation& eq;
    double start, end;
    int n_intervals, signif_figs;
    method use_method;
    eq_evaluation(equation& eq) : eq(eq) {}
};