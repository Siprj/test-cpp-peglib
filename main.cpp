#include <iostream>
#include <peglib.h>
#include <sstream>

using namespace std;
using namespace peg;

struct Section {
    Section(){}
    Section(string name, string type):name(name), type(type){}

    friend ostream& operator<<  (ostream& stream, const Section &section)
    {
        return stream<<"Section{ name: \""<<section.name<<"\", type: \""<<section.type<<"\"}\n";
    }
    std::string name;
    std::string type;
};

string format_error_message
    ( const string& path
    , size_t ln
    , size_t col
    , const string& msg
    )
{
    stringstream ss;
    ss << path << ":" << ln << ":" << col << ": " << msg << endl;
    return ss.str();
}

int main()
{
    auto syntax = R"(
        SECTION      <- '[' SECTION_NAME ':' SECTION_TYPE ']'
        SECTION_NAME <- [a-zA-Z0-9]+
        SECTION_TYPE <- [a-zA-Z0-9]+
        %whitespace  <- [ \t]*
    )";

    peg::parser parser(syntax);
    parser.log = [&](size_t ln, size_t col, const string& msg) {
            cerr << format_error_message("TODO: file", ln, col, msg) << endl;
        };
    parser["SECTION_NAME"] = [](const peg::SemanticValues &sv)
    {
        cout<<"KRAKRA: "<<sv.token();
        return sv.token();
    };
    parser["SECTION_TYPE"] = [](const peg::SemanticValues &sv)
    {
        cout<<"AKAT\n"<<flush;
        return sv.token();
    };
    parser["SECTION"] = [](const peg::SemanticValues &sv)
    {
        cout<<"KWAKWA\n"<<flush;
        return Section(sv[0].get<string>(), sv[1].get<string>());
    };

    Section section;
    parser.enable_packrat_parsing();
    bool ret = parser.parse("[Ahojda : SIID_LSKDFJ]", section);
    if(ret == true)
    {
        cout<<"asdfasdf\n";
    }
    else
    {
        cout<<"error";
    }

    cout << "Hello World!" << endl << section;
    return 0;
}
