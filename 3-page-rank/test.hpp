#pragma once
#include "utils.hpp"
#include <iomanip>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

struct TestCase
{
    string name;
    fun_ptr func;
};

struct TestResult
{
    string name;
    int nworkers;
    double time;
    bool correctness;
};

TestResult run_and_measure_time(TestCase);
vector<TestResult> test_all();
void dump_result(vector<TestResult> &cases);

void ref(const AdjointList &graph, Solution &solution);
void iterate_rank(const AdjointList &graph, Solution &solution);
void accurate_rank(const AdjointList &graph, Solution &solution);