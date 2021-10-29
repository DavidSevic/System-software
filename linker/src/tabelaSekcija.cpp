#include "tabelaSekcija.h"
#include "sekcija.h"

int TabelaSekcija::ident = 0;

void TabelaSekcija::ispisi() {

    list<Sekcija>::iterator it;

    if(velicina == 0)
        velicina  = locationCounter;

    cout<<naziv<<"("<<velicina<<")"<<" sekcija:\n";

    for(it = sekcije.begin(); it != sekcije.end(); ++it) {
        it->ipisi();
        cout<<"\n";
    }

}

void TabelaSekcija::dodajSekciju(Sekcija sekcija) {

    this->sekcije.push_back(sekcija);

}