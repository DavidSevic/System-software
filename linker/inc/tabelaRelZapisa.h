#pragma once
#include "relZapis.h"
#include <list>
#include <string>
#include <iostream>
using namespace std;

class TabelaRelZapisa {

    public:

    list<RelZapis> zapisi;

    void dodajZapis(RelZapis zapis);
    void ispisi();

    friend ostream& operator<< (ostream& out, TabelaRelZapisa& tab) {

        out<<"Tabela relokacionih zapisa:\n\n";
            
        list<RelZapis>::iterator it;

        for(it = tab.zapisi.begin(); it != tab.zapisi.end(); ++it)
            out<<"|"<<it->sekcija<<"|"<<it->pozicija<<"|"<<it->adresiranje<<"|"<<it->opis<<"|\n";
        
        return out;
    }

};