#pragma once
#include <string>
#include <iostream>
using namespace std;

class Sekcija {

    public:

    Sekcija() {
        this->id = ++ident;
    }

    Sekcija(string naziv, string offset, string sadrzaj) {
        this->naziv = naziv;
        this->offset = offset;
        this->sadrzaj = sadrzaj;
        this->id = ++ident;
    }

        string naziv;
        string offset;
        string sadrzaj;
        string objasnjenje;

        static int ident; 

        int id;

        int locationCounter = 0;

    void ipisi();
    string ispisiKraj();

};