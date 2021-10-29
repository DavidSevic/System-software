#pragma once
#include <iostream>
using namespace std;

class RelZapis {

    public:

    RelZapis() {}

    RelZapis(string sekcija, int pozicija, char adresiranje, string opis) {

        this->sekcija = sekcija;
        this->pozicija = pozicija;
        this->adresiranje = adresiranje;
        this->opis = opis;
    }

    string sekcija;
    int pozicija;
    char adresiranje;
    string opis;

    bool promenjen = false;

};