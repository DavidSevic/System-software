#pragma once
#include <string>
#include <iostream>
using namespace std;


class Simbol {

    public:

        static int redniBroj;

        string labela;
        string sekcija;
        int offset;
        bool global;
        int redniBr;

        int pomerajSekcije = 0;

        Simbol() {}

        Simbol(string labela, string sekcija, int offset, bool global) {

            this->labela = labela;
            this->sekcija = sekcija;
            this->offset = offset;
            this->global = global;
        }
};