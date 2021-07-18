#include "utils.hpp"

uint32_t size = 0, length = 0, mod = 1;

ostringstream ostr;

int file_type_id, mode_id;
string mode;

void generate_random(int32_t *result);
void generate_sparse(int32_t *result);
void generate_repeated(int32_t *result);

void save_text(int32_t *result);
void save_binary(int32_t *result);

void (*generate[3])(int32_t *) = {
    generate_random,
    generate_sparse,
    generate_repeated};

void (*save[2])(int32_t *) = {
    save_text,
    save_binary};

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        cerr << "Usage: <file_type:text|binary> <mode:random|sparse|repeated> <size> <length>" << endl;
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
        else if (0 == strcmp(argv[2], "sparse"))
            mode_id = 1;
        else if (0 == strcmp(argv[2], "repeated"))
            mode_id = 2;
        else
            throw argv[2];
        mode = argv[2];
        size = atoi(argv[3]);
        length = atoi(argv[4]);
    }
    catch (char *argument)
    {
        cerr << "Argument \'" << argument << "\' invalid" << endl;
        return 1;
    }

    for (int i = 0; i < length; i++)
        mod *= 10;

    ostr << "array_" << mode << '_' << size << '_' << length;

    int32_t *array = new int32_t[size];
    memset(array, 0, size * sizeof(int32_t));
    srand(time(NULL));

    generate[mode_id](array);
    save[file_type_id](array);

    delete[] array;
    array = nullptr;
}

void save_text(int32_t *result)
{
    ostr << ".txt";
    ofstream fout;
    fout.open(ostr.str());
    fout << size << endl;
    for (int i = 0; i < size; i++)
        fout << result[i] << " ";
    fout.close();
}

void save_binary(int32_t *result)
{
    ostr << ".bin";
    ofstream fout;
    fout.open(ostr.str(), ios::binary);
    fout.write((char *)&size, sizeof(uint32_t));
    fout.write((char *)result, size * sizeof(int32_t));
    fout.close();
}

void generate_random(int32_t *result)
{
    parallel_for(int i = 0; i < size; i++)
        result[i] = rand() % mod;
}

void generate_sparse(int32_t *result)
{
}

void generate_repeated(int32_t *result)
{
}