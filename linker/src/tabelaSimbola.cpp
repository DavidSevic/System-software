#include "tabelaSimbola.h"

bool TabelaSimbola::dodajSimbol(Simbol simbol) {

    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        if(it->labela == simbol.labela)
            return false;

    //if(simbol.sekcija == "n/a") 
        //simbol.offset = simbol.redniBr;

    simbol.redniBr = ++redniBroj;
    
    simboli.push_back(simbol);

    /*cout<<"pre "<<simbol.redniBr<<"\n";

    cout<<"posle "<<simbol.redniBr<<"\n";*/

    //ispisi();

    return true;
}

string TabelaSimbola::offsetSimbola(string naziv) {

    bool nasao = false;
    bool offset = 0;

    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        if(it->labela == naziv) {
            return to_string(it->offset);
        }
    
    return "greska";

}

bool TabelaSimbola::globalnostSimbola(string naziv) {

    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        if(it->labela == naziv) {
            return it->global;
        }
    
    return false;

}

int TabelaSimbola::redniBrSimbola(string naziv) {

    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        if(it->labela == naziv) {
            return it->redniBr;
        }

    if(naziv == "abs")
        return 0;
    
    return -1;

}

string TabelaSimbola::sekcijaSimbola(string naziv) {

    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        if(it->labela == naziv) {
            return it->sekcija;
        }
    
    return "greska";

}

void TabelaSimbola::postaviGlobal(string naziv) {

    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        if(it->labela == naziv)
            it->global = true;

}

bool TabelaSimbola::provera(string naziv) {
    
    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        if(it->labela == naziv)
            return true;

    return false;
}

void TabelaSimbola::ispisi() {
    
    list<Simbol>::iterator it;
    for(it = simboli.begin(); it != simboli.end(); ++it)
        cout<<"|"<<it->labela<<"|"<<it->sekcija<<"|"<<it->offset<<"|"<<((it->global == false) ? "local" : "global")<<"|"<<it->redniBr<<"|\n";

    cout<<"\n";
}