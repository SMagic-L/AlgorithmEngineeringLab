#include "utils.hpp"

string mode;
int file_type_id, mode_id;

ostringstream ostr;

void generate_random(AdjointList &graph);

void (*generate[2])(AdjointList &) = {
    generate_random};

int main(int argc, char **argv)
{
    AdjointList graph;
    if (argc < 4)
    {
        cerr << "Usage: <file_type:text|binary> <mode:random|sparse> <vertex_count>" << endl;
        return 1;
    }
    try
    {
        if (0 == strcmp(argv[1], "text"))
            file_type_id = 0;
        else if (0 == strcmp(argv[1], "binary"))
            file_type_id = 1;
        else
            throw argv[1];
        if (0 == strcmp(argv[2], "random"))
            mode_id = 0;
        else
            throw argv[2];
        mode = argv[2];
        int vertex_vount = atoi(argv[3]);
        graph.vertex_count = vertex_vount;
        graph.initialize();
    }
    catch (char *argument)
    {
        cerr << "Argument \'" << argument << "\' invalid" << endl;
        return 1;
    }
    catch (exception e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    ostr << "adjoint_list_" << mode << '_' << graph.vertex_count;

    generate[mode_id](graph);
    if(0 == file_type_id)
    {
        ostr << ".txt";
        graph.save_file_text(ostr.str().c_str());
    }
    else
    {
        ostr << ".bin";
        graph.save_file_binary(ostr.str().c_str());
    }

    return 0;
}

void generate_random(AdjointList &graph)
{

}