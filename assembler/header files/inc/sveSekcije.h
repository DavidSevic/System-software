#pragma once
#include "tabelaSekcija.h"

class SveSekcije {

     public:

        list<TabelaSekcija> sekcije; 

   

        void novaSekcija(string sekcija);
        void dodajSekciju(Sekcija sekcija);
        void povecajCounter(string sekcija, int br);
        int offset(string sekcija);
        void anulirajCountere();
        void ispisiCountere();
        void ispisiSekcije();

        void dodajVelicinu(string sekcija, int vel);
        void dodajAdresu(string sekcija, int vel);
        void obrisiSekciju(string naziv);

        friend ostream& operator<< (ostream& out, SveSekcije& tab) {

            out<<"Tabela sekcija:\n\n";
            
            list<TabelaSekcija>::iterator it;

            for(it = tab.sekcije.begin(); it != tab.sekcije.end(); ++it) {
                out << it->ispisiKraj() << "\n";
            }

            return out;
        }

};