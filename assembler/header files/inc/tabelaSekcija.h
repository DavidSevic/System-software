#pragma once
#include "sekcija.h"
#include <list>
#include <string>
#include <iostream>
using namespace std;

class TabelaSekcija {



    public:

    static int ident;
        
        string naziv;

        int locationCounter;

        int velicina;
        
        int id;

        list<Sekcija> sekcije;

        int adresa = 0;

        TabelaSekcija(string naziv) {
            this->naziv = naziv;
            this->locationCounter = 0;
            this->velicina = 0;
            this->id = ++ident;
        }

        void dodajSekciju(Sekcija sekcija);
        void ispisi();

        string ispisiKraj () {

            int ovo = -1;
            
            if(velicina == 0)
                ovo = locationCounter;
            else 
                ovo = velicina;

            string s = "Sekcija: " + naziv + "(" + to_string(ovo) + ")" + ":\n";

            list<Sekcija>::iterator it;

            for(it = sekcije.begin(); it != sekcije.end(); ++it) {
                s += it->ispisiKraj() + "\n";
            }

            return s;
        }

        /*bool operator==(const TabelaSekcija& tab) {
            return this->naziv == tab.naziv;
        }*/

};