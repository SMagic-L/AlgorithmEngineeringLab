#include "utils.hpp"
#include "adjoint_list.hpp"

string ref(const Graph &graph, uint32_t& result)
{
    string func_name = __FUNCTION__;

    uint32_t **A = graph.convert_to_adjoint_matrix();
    uint32_t **A2 = new uint32_t *[graph.vertex_count];
    for (int i = 0; i < graph.vertex_count; i++)
    {
        A2[i] = new uint32_t[graph.vertex_count];
        memset(A2[i], 0, graph.vertex_count * sizeof(uint32_t));
    }

    parallel_for(int i = 0; i < graph.vertex_count; i++)
        parallel_for(int k = 0; k < graph.vertex_count; k++) 
            for (int j = 0; j < graph.vertex_count; j++)
                A2[i][j] += A[i][k] * A[k][j];

    parallel_for(int i = 0; i < graph.vertex_count; i++)
        parallel_for(int j = 0; j < graph.vertex_count; j++)
            result += A[i][j] * A2[i][j];

    result /= 6;

    parallel_for(int i = 0; i < graph.vertex_count; i++)
    {
        delete[] A[i];
        delete[] A2[i];
    }
    delete[] A;
    delete[] A2;
    return func_name;
}