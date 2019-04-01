#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

typedef string token;

token lex_keywords[] = {"Program", "Var", "int", "real", "Const", "Begin", "Read", "Write", "End", "End." ,""};
token lex_symbols[] = {",", ";",":", "+", "-", "*", "/", "=", ":=","(",")",""};
token lex_math_sym[] = {"+", "-", "*", "/",""};

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
} integer_Reg;
typedef struct float_Reg
{
    string name;
    float value;
} float_Reg;
typedef struct const_Reg
{
    string name;
    int value;
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
    while(!lex_keywords[i].empty())
    {
        if(word.compare(lex_keywords[i])==0)
        {
            cout << word <<" -- keyword found" << endl;
            keywords[kw_it] = word;
            kw_it++;
            return;
        }
        i++;
    }
    i=0;
    while(!lex_symbols[i].empty())
    {
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
    while(!lex_keywords[i].empty())
    {
        if(lex_keywords[i].compare(word)==0)
        {
            cout << word <<" -- keyword found" << endl;
            return "keyword";
        }
        i++;
    }
    i=0;
    while(!lex_symbols[i].empty())
    {
        if(lex_symbols[i].compare(word)==0)
        {
            ///cout << word <<" -- Symbol found" << endl;
            return "symbol";
        }
        i++;
    }
    if(word.length() != 0)
    {
        ///cout << word <<" -- Identifiers found" << endl;
        return "identifier";
    }
    return NULL;
}

string string_math(token word)
{
    int i = 0;
    while(!lex_symbols[i].empty())
    {
        if(lex_math_sym[i].compare(word)==0)
        {
            return "math";
        }
        i++;
    }
}

int find_reg_type(token word)
{
    int i=0;
    while(!iR[i].name.empty())
    {
        if(word.compare(iR[i].name))
        {
            return 0;
        }
        i++;
    }
    i=0;
    while(fR[i].name.empty())
    {
        if(word.compare(fR[i].name))
        {
            return 1;
        }
        i++;
    }
    i=0;
    while(cR[i].name.empty())
    {
        if(word.compare(cR[i].name))
        {
            return 2;
        }
        i++;
    }
}

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
    //while(file7 != NULL)
    ///Fase 1: declaração de variáveis.
    int linha = 1;
    /*while(true)
    {
        file >> word;
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            break;
        }
        string_lexing(word);
    }*/
    while(true)
    {
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            break;
        }
        file >> word;
        if(word.compare("Program")==0)
        {
            file >> word;
            if(string_id(word).compare("identifier")==0)
            {
                file >> word;
                if(word.compare(";")==0)
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
        else if(word.compare("Var")==0)
        {
            int i=0;
            string* reg;
            while(true)
            {
                file >> word;
                if(string_id(word).compare("identifier")==0)
                {
                    reg = new string;
                    reg[i] = word;
                    i++;
                    file >> word;
                    if(word.compare(",")==0)
                    {
                        continue;
                    }
                    else if(word.compare(":")==0)
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
            if(word.compare("int")==0)
            {
                for(int j=0; j<i; j++)
                {
                    iR = new integer_Reg;
                    iR[j].name = reg[j];
                    iR[j].value = NULL;
                }
                delete reg;
            }
            else if(word.compare("real")==0)
            {
                for(int j=0; j<i; j++)
                {
                    fR = new float_Reg;
                    fR[j].name = reg[j];
                    fR[j].value = NULL;
                }
                delete reg;
            }
            else
            {
                cout << "Erro: tipo de variável não especificado: linha "<< linha << endl;
                return 0;
            }
            file >> word;
            if(word.compare(";")==0)
            {
                delete reg;
                linha++;
                break;
            }
            else
            {
                cout << "Erro: fim de linha não esperado: linha "<< linha << endl;
                return 0;
            }
        }
        else if(word.compare("Const")==0)
        {
            int i = 0;
            while(true)
            {
                file >> word;
                if(string_id(word).compare("identifier")==0)
                {
                    string name = word;
                    file >> word;
                    if(word.compare("=")==0)
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
                        cR = new const_Reg;
                        cR[i].name = name;
                        cR[i].value = stoi(word);
                        i++;
                        file >> word;
                        if(word.compare(",")==0)
                        {
                            continue;
                        }
                        if(word.compare(";")==0)
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
        else if(word.compare("Begin")==0)
        {
            linha++;
            break;
        }
        else if(string_id(word).compare("identifier")==0)
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
        if(word.compare("Read")==0)
        {
            file >> word;
            if(word.compare("(")==0)
            {
                file >> word;
                int reg_type = find_reg_type(word);
                if(reg_type == 0)
                {
                    continue;
                }
                else if(reg_type == 1)
                {
                    continue;
                }
                else if(reg_type == 2)
                {
                    cout << "Erro: Valores constantes não podem ser mudados: linha "<< linha << endl;
                    return 0;
                }
                else
                {
                    cout << "Erro: Identificador não declarado: linha "<< linha << endl;
                    return 0;
                }
                file >> word;
                if(word.compare(")")==0)
                {
                    file >> word;
                    if(word.compare(";")==0)
                    {
                        linha++;
                    }
                    else
                    {
                        cout << "Erro: Fim de linha não esperado: linha "<< linha << endl;
                        return 0;
                    }
                }
                else
                {
                    cout << "Erro: Espera-se encerramento com parenteses: linha "<< linha << endl;
                    return 0;
                }
            }
            else
            {
                cout << "Erro: Começo deve ser com parenteses: linha "<< linha << endl;
                return 0;
            }
        }
        else if(word.compare("Write")==0)
        {
            file >> word;
            if(word.compare("(")==0)
            {
                file >> word;
                int reg_type = find_reg_type(word);
                if(reg_type == 0)
                {
                    continue;
                }
                else if(reg_type == 1)
                {
                    continue;
                }
                else if(reg_type == 2)
                {
                    continue;
                }
                else
                {
                    cout << "Erro: Identificador não declarado: linha "<< linha << endl;
                    return 0;
                }
                file >> word;
                if(word.compare(")")==0)
                {
                    file >> word;
                    if(word.compare(";")==0)
                    {
                        linha++;
                    }
                    else
                    {
                        cout << "Erro: Fim de linha não esperado: linha "<< linha << endl;
                        return 0;
                    }
                }
                else
                {
                    cout << "Erro: Espera-se encerramento com parenteses: linha "<< linha << endl;
                    return 0;
                }
            }
            else
            {
                cout << "Erro: Começo deve ser com parenteses: linha "<< linha << endl;
                return 0;
            }
        }
        else if(string_id(word).compare("identifier")==0)
        {
            int reg_type = find_reg_type(word);
            if(reg_type == 0)
            {
                continue;
            }
            else if(reg_type == 1)
            {
                continue;
            }
            else if(reg_type == 2)
            {
                continue;
            }
            else
            {
                cout << "Erro: Identificador não declarado: linha "<< linha << endl;
                return 0;
            }
            file >> word;
            if(word.compare(":=")==0)
            {
                file >> word;
                while(true)
                {
                    int reg_type = find_reg_type(word);
                    if(reg_type == 0)
                    {
                        continue;
                    }
                    else if(reg_type == 1)
                    {
                        continue;
                    }
                    else if(reg_type == 2)
                    {
                        continue;
                    }
                    else
                    {
                        cout << "Erro: Identificador não declarado: linha "<< linha << endl;
                        return 0;
                    }
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
                    file >> word;
                    if(!word.compare(";")==0)
                    {
                        if(string_math(word).compare("math")==0)
                        {
                            file >> word;
                            if(word.compare(";")==0)
                            {
                                cout << "Erro: valor deve ser numérico: linha "<< linha << endl;
                                return 0;
                            }
                        }
                    }
                    else if(word.compare(";")==0)
                    {
                        linha++;
                        break;
                    }
                    else
                    {
                        cout << "Erro: valor deve ser numérico: linha "<< linha << endl;
                        return 0;
                    }
                }
            }
        }
        else if(word.compare("End.")==0)
        {
            break;
        }
    }
    cout << "...fim da analise." << endl;
    file.close();
    return 0;
}
