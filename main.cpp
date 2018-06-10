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

struct SymbolScope;

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

inline constexpr long unsigned int val(const char *str, long unsigned int h = 0) {
    return !str[h] ? 5381 : (str2tag(str, h + 1) * 33) ^ static_cast<unsigned char>(str[h]);
}

inline constexpr long unsigned int tailedRec(const char *str, long unsigned int h = 0)
{
    return !str[h] ? 5381 : (str2tag(str, h + 1) * 33) ^ static_cast<unsigned char>(str[h]);
}

inline constexpr long unsigned int operator "" _(const char *str, long unsigned int h = 0)
{
    return tailedRec(str);
}

void pokus(const shared_ptr<Ast> ast)
{
    switch (ast->tag)
    {
    case "CONFIG"_:
        break;
    case "INCLUDE"_:
        break;
    case "SECTION"_:
        break;
    case "SECTION_DEFINITION"_:
        break;
    case "SECTION_VARIABLE"_:
        break;
    case "SYMBOL_NAME"_:
        break;
    case "VALUE"_:
        break;
    case "COMMENT"_:
        break;
    default:

        std::cout<<"(Default) tag number: "<<ast->tag<<" and token: "<<ast->token<<" name: "<<ast->name<<"\n";
        break;
    }
}

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

    peg::parser parser(syntax);
    parser.log = [&](size_t ln, size_t col, const string& msg) {
            cerr << format_error_message("TODO: file", ln, col, msg) << endl;
        };

    shared_ptr<Ast> ast;
    parser.enable_ast<Ast>();
    bool ret = parser.parse("[Ahojda : SIID_LSKDFJ]    # asdfasdfasd asdf asdfasdf \n\r", ast);
    if(ret == true)
    {
        pokus(ast);
    }
    else
    {
        cout<<"error";
    }

    cout << "Hello World!" << endl;
    return 0;
}
