#pragma once
#include "simbol.h"
#include <list>

class TabelaSimbola {


       

    public:

     list<Simbol> simboli;

        int redniBroj = 0;

        bool dodajSimbol(Simbol simbol);
        void postaviGlobal(string naziv);
        void ispisi();
        bool provera(string naziv);
        string offsetSimbola(string naziv);
        bool globalnostSimbola(string naziv);
        int redniBrSimbola(string naziv);
        string sekcijaSimbola(string naziv);

        friend ostream& operator<< (ostream& out, TabelaSimbola& tab) {

            out<<"Tabela simbola:\n\n";
            
            list<Simbol>::iterator it;

            for(it = tab.simboli.begin(); it != tab.simboli.end(); ++it)
                out<<"|"<<it->labela<<"|"<<it->sekcija<<"|"<<it->offset<<"|"<<((it->global == false) ? "local" : "global")<<"|"<<it->redniBr<<"|\n";

            out<<"\n";

            return out;
        }

};