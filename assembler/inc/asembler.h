#pragma once
#include "tabelaSimbola.h"
#include "tabelaRelZapisa.h"
#include "sveSekcije.h"
#include <string>
#include <iostream>
using namespace std;



class Asembler {

    private:

        double r0;
        double r1;
        double r2;
        double r3;
        double r4;
        double r5;
        double sp; //r6
        double pc; //r7
        double psw;

        TabelaSimbola tabSimbola;
        SveSekcije sveSekcije;
        TabelaRelZapisa tabRelZapisa; 

    public:
        void asembliraj(string ulazni, string izlazni);
        int prviProlaz(string ulazni);
        void drugiProlaz(string ulazni, string izlazni);
    private:
        bool jelImaKom(string deo);
        string izbaciKom(string deo);

};