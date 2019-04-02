#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

typedef string token;

token lex_keywords[] = {"Program", "Var", "int", "real", "Const", "Begin", "Read", "Write", "End", "End.",""};
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

integer_Reg* iR = new integer_Reg;
float_Reg* fR = new float_Reg;
const_Reg* cR = new const_Reg;

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
            ///cout << word <<" -- keyword found" << endl;
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
        if(word.compare(iR[i].name)==0)
        {
            return 0;
        }
        i++;
    }
    i=0;
    while(!fR[i].name.empty())
    {
        if(word.compare(fR[i].name)==0)
        {
            return 1;
        }
        i++;
    }
    i=0;
    while(!cR[i].name.empty())
    {
        if(word.compare(cR[i].name)==0)
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
            string* reg = new string;
            string* reg_aux;
            int i = 0;
            while(true)
            {
                file >> word;
                if(string_id(word).compare("identifier")==0)
                {
                    int j = 0;
                    reg_aux = new string[sizeof(string)*(i+1)];
                    reg_aux[i] = word;
                    while(!reg[j].empty())
                    {
                        reg_aux[j] = reg[j];
                        j++;
                    }
                    //delete [] reg;
                    reg = reg_aux;
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
                iR = new integer_Reg[sizeof(integer_Reg)*(i+1)];
                for(int j=0; j<i; j++)
                {
                    iR[j].name = reg[j];
                    iR[j].value = NULL;
                }
                delete [] reg;
                //delete [] reg_aux;
            }
            else if(word.compare("real")==0)
            {
                fR = new float_Reg[sizeof(float_Reg)*(i+1)];
                for(int j=0; j<i; j++)
                {
                    fR[j].name = reg[j];
                    fR[j].value = NULL;
                }
                delete [] reg;
                //delete [] reg_aux;
            }
            else
            {
                cout << "Erro: tipo de variável não especificado: linha "<< linha << endl;
                return 0;
            }
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
        else if(word.compare("Const")==0)
        {
            int i = 0;
            const_Reg* cR_aux;
            const_Reg* reg = new const_Reg;
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
                        cR_aux = new const_Reg[sizeof(const_Reg)*(i+1)];
                        cR_aux[i].name = name;
                        cR_aux[i].value = stoi(word);
                        int j=0;
                        while(!reg[j].name.empty())
                        {
                            cR_aux[j] = reg[j];
                            j++;
                        }
                        reg = cR_aux;
                        i++;
                        file >> word;
                        if(word.compare(",")==0)
                        {
                            continue;
                        }
                        if(word.compare(";")==0)
                        {
                            if(!cR[0].name.empty()){
                                cR_aux = cR;
                                cR = new const_Reg[sizeof(const_Reg)*(i+1)];
                                int j=0;
                                while(!cR_aux[j].name.empty())
                                {
                                    cR[j] = cR_aux[j];
                                }
                                for(j=0;j<i;j++)
                                {
                                    cR[j+i] = reg[j];
                                }
                            }
                            else
                            {
                                cR = new const_Reg[sizeof(const_Reg)*(i+1)];
                                for(j=0;j<i;j++)
                                {
                                    cR[j] = reg[j];
                                }
                            }
                            delete [] reg;
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
        file >> word;
        if(word.compare("Read")==0)
        {
            file >> word;
            if(word.compare("(")==0)
            {
                file >> word;
                int reg_type = find_reg_type(word);
                if(reg_type == 0)
                {

                }
                else if(reg_type == 1)
                {

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
                    //continue;
                }
                else if(reg_type == 1)
                {
                    //continue;
                }
                else if(reg_type == 2)
                {
                    //continue;
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
                //continue;
            }
            else if(reg_type == 1)
            {
                //continue;
            }
            else if(reg_type == 2)
            {
                //continue;
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
                        //continue;
                    }
                    else if(reg_type == 1)
                    {
                        //continue;
                    }
                    else if(reg_type == 2)
                    {
                        //continue;
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
                            //continue;
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
            //linha++;
            cout << "...nenhuma falha encontrada em "<<linha<<" linha(s)..."<<endl;
            break;
        }
    }
    cout << "...fim da analise." << endl;
    file.close();
    return 0;
}
