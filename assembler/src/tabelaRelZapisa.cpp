#include "tabelaRelZapisa.h"

void TabelaRelZapisa::dodajZapis(RelZapis zapis) {
    zapisi.push_back(zapis);
}


void TabelaRelZapisa::ispisi() {
    
    //cout<<"____________________________________\n";

    cout<<"Relokacioni zapisi:\n\n";
    
    list<RelZapis>::iterator it;
    for(it = zapisi.begin(); it != zapisi.end(); ++it)
        cout<<"|"<<it->sekcija<<"|"<<it->pozicija<<"|"<<it->adresiranje<<"|"<<it->opis<<"|\n";
    
    cout<<"\n";

}