#pragma once
#include <string>
#include <iostream>
#include <list>
#include "fajl.h"
using namespace std;

class Linker {
    
    private:

        list<Fajl> fajlovi;

        TabelaSimbola tabSimbola;
        SveSekcije sveSekcije;
        TabelaRelZapisa tabRelZapisa;

        SveSekcije sekcijeIspisSaAdresom;
        SveSekcije sekcijeIspisBezAdrese;


    public:

    void linkable(list<string> ulazni, string izlazni, bool solo);
    void hex(list<string> ulazni, list<string> lokacije, string izlazni);

    void ispisKrajHex();

    void ispisi();
    void ispisiGotovo();
    bool jelImaIsta(SveSekcije* sve, string naziv, TabelaSekcija* ova);
    bool jelGreskaSimbol();
    int kolikoOvih(string naziv);
    int kolikoOvihSimb(string labela);
    void izmeniZapise(int idFajla, int nova, string labela);

    string krajHexFajl();

    TabelaSekcija* istogNaziva(string naziv, int poz);

    void dodajFajl(Fajl novi) {
        this->fajlovi.push_back(novi);
    }

};