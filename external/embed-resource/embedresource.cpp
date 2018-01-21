#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch10s15.html
string getFileName(const string& s)
{
    size_t i = s.rfind('/', s.length());
    if (i != string::npos)
        return(s.substr(i + 1, s.length() - i));
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s {sym} {rsrc}\n\n"
                        "  Creates {sym}.c from the contents of {rsrc}\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    string dst{argv[1]};
    string src{argv[2]};

    string sym = getFileName(src);
    replace(sym.begin(), sym.end(), '.', '_');

    ofstream ofs{dst};
    ifstream ifs{src};

    ofs << "#include <stdlib.h>" << endl;
    ofs << "const char _resource_" << sym << "[] = {" << endl;

    size_t lineCount = 0;
    while (!ifs.eof())
    {
        char c;
        ifs.get(c);
        ofs << "0x" << hex << (int)c << ", ";
        if (++lineCount == 10) {
            ofs << endl;
            lineCount = 0;
        }
    }


    ofs << "};" << endl;
    ofs << "const size_t _resource_" << sym << "_len = sizeof(_resource_" << sym << ");";

    return EXIT_SUCCESS;
}
