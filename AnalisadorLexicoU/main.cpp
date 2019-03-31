#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef string token;

token lex_keywords[] = {"Program", "Var", "int", "real", "Const", "Begin", "Read", "Write", "End", "End." };
token lex_symbols[] = {",", ";",":", "+", "-", "*", "/", "=", ":=","(",")"};

ifstream file;

token keywords[500];
int kw_it = 0;
token symbols[500];
int sb_it = 0;
token identifiers[500];
int id_it = 0;

typedef struct integer_Reg
{
    string name;
    int value;
    integer_Reg* prev;
    integer_Reg* next;
} integer_Reg;
typedef struct float_Reg
{
    string name;
    float value;
    float_Reg* prev;
    float_Reg* next;
} float_Reg;
typedef struct const_Reg
{
    string name;
    int value;
    const_Reg* prev;
    const_Reg* next;
} const_Reg;

integer_Reg* iR;
float_Reg* fR;
const_Reg* cR;

void read_file()
{
    ifstream file;
    string data;
    file.open ("prog.txt");
    if (!file.is_open())
    {
        cout << "File not found." << endl;
        return;
    }
    cout << "Reading from the file" << endl;
    //while(file != NULL)
    while(true)
    {
        file >> data;
        cout << data << endl;
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            break;
        }
    }
}

void string_lexing(token word)
{
    int i = 0;
    while(lex_keywords[i].compare(""))
    {
        if(word.compare(lex_keywords[i])==0)
        {
            cout << word <<" -- keyword found" << endl;
            keywords[kw_it] = word;
            kw_it++;
            return;
        }
        if(word.compare(lex_symbols[i])==0)
        {
            cout << word <<" -- Symbol found" << endl;
            symbols[sb_it] = word;
            sb_it++;
            return;
        }
        i++;
    }
    if(word.length() != 0)
    {
        for(int j=0; j<id_it; j++)
        {
            if(identifiers[j].compare(word)==0)
            {
                return;
            }
        }
        cout << word <<" -- Identifiers found" << endl;
        identifiers[id_it] = word;
        id_it++;
    }
}

string string_id(token word)
{
    int i = 0;
    while(lex_keywords[i].compare(""))
    {
        if(word.compare(lex_keywords[i])==0)
        {
            cout << word <<" -- keyword found" << endl;
            return "keyword";
        }
        if(word.compare(lex_symbols[i])==0)
        {
            cout << word <<" -- Symbol found" << endl;
            return "symbol";
        }
        i++;
    }
    if(word.length() != 0)
    {
        cout << word <<" -- Identifiers found" << endl;
        return "identifier";
    }
    return NULL;
}
int find_id()
int main(int argc, char **argv)
{
    file.open (argv[1]);
    if (!file.is_open())
    {
        cout << "Arquivo não encontrado." << endl;
        return 0;
    }
    string line;
    cout << "Começo da Analise..." << endl;

    string msg;
    token word;
    //while(file != NULL)
    ///Fase 1: declaração de variáveis.
    int linha = 1;
    while(true)
    {
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            break;
        }
        file >> word;
        if(word.compare("Program"))
        {
            file >> word;
            if(string_id(word).compare("indentifier"))
            {
                file >> word;
                if(word.compare(";"))
                {
                    linha++;
                }
                else
                {
                    cout << "Erro: fim de linha não esperado: linha "<< linha << endl;
                    return 0;
                }
            }
            else
            {
                cout << "Erro: nome do programa não pode ser uma palavra reservada: linha "<< linha << endl;
                return 0;
            }
        }
        else if(word.compare("Var"))
        {
            int i=0;
            string* reg;
            while(true)
            {
                file >> word;
                if(string_id(word).compare("indentifier"))
                {
                    reg = new string[1];
                    reg[i] = word;
                    i++;
                    file >> word;
                    if(word.compare(","))
                    {
                        continue;
                    }
                    else if(word.compare(":"))
                    {
                        break;
                    }
                    else
                    {
                        cout << "Erro: simbolo não esperado: linha "<< linha << endl;
                        return 0;
                    }
                }
                else
                {
                    cout << "Erro: varivável não pode ser uma palavra reservada: linha "<< linha << endl;
                    return 0;
                }
            }
            file >> word;
            if(word.compare("int"))
            {
                for(int j=0; j<i; j++)
                {
                    iR = new integer_Reg[1];
                    iR[j].name = reg[j];
                    iR[j].value = NULL;
                    iR[j].next = NULL;
                    if(iR[j-1].name.compare(NULL))
                    {
                        iR[j].prev = &iR[j-1];
                        iR[j-1].prev = &iR[j];
                    }
                }
                delete reg;
            }
            else if(word.compare("real"))
            {
                for(int j=0; j<i; j++)
                {
                    fR = new float_Reg[1];
                    fR[j].name = reg[j];
                    fR[j].next = NULL;
                    if(iR[j-1].name.compare(NULL))
                    {
                        fR[j].prev = &fR[j-1];
                        fR[j-1].prev = &fR[j];
                    }
                }
                delete reg;
            }
            else
            {
                cout << "Erro: tipo de variável não especificado: linha "<< linha << endl;
                return 0;
            }
            file >> word;
            if(word.compare(";"))
            {
                linha++;
                break;
            }
            else
            {
                cout << "Erro: fim de linha não esperado: linha "<< linha << endl;
                return 0;
            }
        }
        else if(word.compare("Const"))
        {
            int i = 0;
            while(true)
            {
                file >> word;
                if(string_id(word).compare("identifier"))
                {
                    string name = word;
                    file >> word;
                    if(word.compare("="))
                    {
                        file >> word;
                        for(int len=0; len<word.length(); len++)
                        {
                            if(int(word[len]) >= 48 || int(word[len]) <= 57)
                            {
                                continue;
                            }
                            else
                            {
                                cout << "Erro: valor deve ser numérico: linha "<< linha << endl;
                                return 0;
                            }
                        }
                        cR = new const_Reg[1];
                        cR[i].name = name;
                        cR[i].value = stoi(word);
                        cR[i].next = NULL;
                        if(cR[i-1].name.compare(NULL))
                        {
                            cR[i].prev = &cR[i-1];
                            cR[i-1].prev = &cR[i];
                        }
                        i++;
                        file >> word;
                        if(word.compare(","))
                        {
                            continue;
                        }
                        if(word.compare(";"))
                        {
                            linha++;
                            break;
                        }
                    }
                    else
                    {
                        cout << "Erro: simbolo não esperado: linha "<< linha << endl;
                        return 0;
                    }
                }
                else
                {
                    cout << "Erro: constante não pode ser uma palavra reservada: linha "<< linha << endl;
                    return 0;
                }
            }
        }
        else if(word.compare("Begin"))
        {
            break;
        }
        else if(string_id(word).compare("identifier"))
        {
            cout << "Erro: Comando não reconhecido: linha "<< linha << endl;
            return 0;
        }
    }
    /// Fase 2: computação dos dados
    while(true)
    {
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            break;
        }
        if(word.compare("Read"))
        {
            file >> word;
            if(word.compare("("))
            {
                file >> word;
                int i = 0;
                while(iR[i].next->name.compare(NULL) && fR[i].next->name.compare(NULL) && cR[i].next->name.compare(NULL) )
                {
                    if(word.compare(iR[i].name))
                    {
                        break;
                    }
                    if(word.compare(fR[i].name))
                    {
                        break;
                    }
                    if(word.compare(cR[i].name))
                    {
                        cout << "Erro: Valores constantes não podem ser mudados: linha "<< linha << endl;
                        return 0;
                    }
                    i++;
                }
            }
        }
        else if(word.compare("Write"))
        {
            file >> word;
            if(word.compare("("))
            {
                file >> word;
                int i = 0;
                while(iR[i].next->name.compare(NULL) && fR[i].next->name.compare(NULL) && cR[i].next->name.compare(NULL) )
                {
                    if(word.compare(iR[i].name))
                    {
                        break;
                    }
                    if(word.compare(fR[i].name))
                    {
                        break;
                    }
                    if(word.compare(cR[i].name))
                    {
                        cout << "Erro: Valores constantes não podem ser mudados: linha "<< linha << endl;
                        return 0;
                    }
                    i++;
                }
            }
        }
        else if(string_id(word).compare("identifier"))
        {
            while(iR[i].next->name.compare(NULL) && fR[i].next->name.compare(NULL) && cR[i].next->name.compare(NULL) )
            {
                if(word.compare(iR[i].name))
                {
                    break;
                }
                if(word.compare(fR[i].name))
                {
                    break;
                }
                if(word.compare(cR[i].name))
                {
                    break;
                }
                i++;
            }
            file >> word;
            if(word.compare(":="))
            {
                file >> word;

            }
        }
        else if(word.compare("End."))
        {
            break;
        }
    }
    cout << "...fim da analise." << endl;
    file.close();
    return 0;
}
