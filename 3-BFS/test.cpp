#include "utils.hpp"
#include "test.hpp"
#include "adjoint_list.hpp"
#include "solution.hpp"

AdjointList Source;
Solution TheSolution;

vector<TestCase> test_list =
{
    {"buttom_up",buttom_up}
};

bool initialize(int argc, char **argv)
{
    bool is_binary, is_text;
    if (argc < 3)
    {
        cerr << "Usage: <path_to_graph> <file_type:text|binary>" << endl;
        return false;
    }
    else
    {
        is_binary = !strcmp(argv[2],"binary");
        is_text = !strcmp(argv[2],"text");
    }

    if ((!is_binary && !is_text) || (is_binary && is_text))
    {
        cerr << " Invalid argument <file_type>: " << argv[2] << endl;
        return false;
    }

    bool load_result = false;
    if (is_binary)
        load_result = Source.load_data_binary(argv[1]);
    if (is_text)
        load_result = Source.load_data_text(argv[1]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }

    TheSolution.attach_to_graph(Source);
    return true;
}

TestResult run_and_measure_time(TestCase test_case)
{
    TestResult test_result;
    test_result.name = test_case.name;
    test_result.nworkers = __cilkrts_get_nworkers();
    try
    {
        TheSolution.clear();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        test_case.func(Source, TheSolution);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        test_result.time = time_span.count();
        return test_result;
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred while running " << test_result.name << endl;
        cerr << e.what() << endl;
        test_result.time = NAN;
        return test_result;
    }
}

vector<TestResult> test_all()
{
    vector<TestResult> results;
    TestResult current_result;

    TestCase ref_case = {"ref_BFS",ref_BFS};
    TestResult ref_result = run_and_measure_time(ref_case);
    Solution compare(TheSolution);
    ref_result.correctness = true;
    results.push_back(ref_result);

    for (int i = 0; i < test_list.size(); i++)
    {
        current_result = run_and_measure_time(test_list[i]);
        current_result.correctness = (TheSolution == compare);
        results.push_back(current_result);
    }

    return results;
}

void dump_result(vector<TestResult> &cases)
{
    const int TAB_WIDTH = 12;
    cout << left;
    if (1 == cases[0].nworkers)
    {
        cout << setw(TAB_WIDTH) << "Workers";
        for (int i = 0; i < cases.size(); i++)
            cout << setw(TAB_WIDTH) << cases[i].name;
        cout << endl;
    }

    cout << setw(TAB_WIDTH) << __cilkrts_get_nworkers();
    for (int i = 0; i < cases.size(); i++)
    {
        cout << setw(TAB_WIDTH);
        if (!cases[i].correctness)
        {
            cout << "Wrong Ans";
        }
        else if (NAN == cases[i].time)
        {
            cout << "#Error";
        }
        else
            cout << cases[i].time;
    }
    cout << endl;
}