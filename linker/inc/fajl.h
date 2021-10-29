#pragma once
#include <string>
#include <iostream>
#include "tabelaSimbola.h"
#include "sveSekcije.h"
#include "tabelaRelZapisa.h"
using namespace std;


class Fajl {

    public:

    static int ident;

    TabelaSimbola tabSimbola;
    SveSekcije tabSveSekcije;
    TabelaRelZapisa tabRelZapisa;

    int id;

    Fajl() {
        this->id = ++ident;
    }

    Fajl(TabelaSimbola tabSimbola, SveSekcije tabSveSekcije, TabelaRelZapisa tabRelZapisa) {

        this->tabSimbola = tabSimbola;
        this->tabSveSekcije = tabSveSekcije;
        this->tabRelZapisa = tabRelZapisa;

        this->id = ++ident;
    }



};
