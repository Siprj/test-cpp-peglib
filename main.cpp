#include <iostream>
#include <peglib.h>
#include <sstream>
#include <tuple>

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

struct PropertyVariable {
    PropertyVariable(std::string name, std::string value): name(name), value(value) {}

    std::string name;
    std::string value;
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
        CONFIG <- INCLUDE* SECTION*
        INCLUDE <- _ '#include' _ VALUE __
        SECTION <- SECTION_DEFINITION SECTION_VARIABLE*
        SECTION_DEFINITION <- _ '[' _ SYMBOL_NAME ':' _ SYMBOL_NAME ']' _ COMMENT? __
        SECTION_VARIABLE <- _ SYMBOL_NAME '=' VALUE __
        SYMBOL_NAME <- [a-zA-Z0-9._]+ _
        VALUE <- (![\n\r# ].)*
        COMMENT <- '#' (![\r\n].)*
        ~_ <- [ \t]*
        ~__ <- ![a-zA-Z_] [ \t\r\n]*
    )";

    peg::parser parser(syntax);
    parser.log = [&](size_t ln, size_t col, const string& msg) {
            cerr << format_error_message("TODO: file", ln, col, msg) << endl;
        };
    parser["SYMBOL_NAME"] = [](const peg::SemanticValues &sv)
    {
        std::cout<<"SYMBOL_NAME";
        return sv.token();
    };
    parser["COMMENT"] = [](const peg::SemanticValues &sv)
    {
        return sv.token();
    };

    typedef std::tuple<string, string, string> StringTriple;

    parser["SECTION"] = [](const peg::SemanticValues &sv)
    {
        std::cout<<"BBBBBBB: "<<sv.token()<<"bbbb\n"<<"size: "<<sv.size()<<"\n"<<flush;
        StringTriple triple = sv[0].get<StringTriple>();
        return Section(std::get<0>(triple), std::get<1>(triple));
    };
    parser["SECTION_DEFINITION"] = [](const peg::SemanticValues &sv)
    {
        cout<<"SECTION_DEFINITION: "<<sv.size()<<"\n";
        return std::make_tuple(sv[0].get<string>(), sv[1].get<string>(), sv[2].get<string>());
    };
    parser["SECTION_VARIABLE"] = [](const peg::SemanticValues &sv)
    {
        cout<<"SECTION_VARIABLE: "<<sv.size()<<"\n";
        return PropertyVariable(sv[0].get<string>(), sv[1].get<string>());
    };
    parser["VALUE"] = [](const peg::SemanticValues &sv)
    {
        cout<<"VALUE\n";
        return sv.token();
    };

    Section section;
    parser.enable_packrat_parsing();
    bool ret = parser.parse("[Ahojda : SIID_LSKDFJ]    # asdfasdfasd asdf asdfasdf \n\r    pokus = ahojda\n\r pokus2 = ahojda2", section);
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
