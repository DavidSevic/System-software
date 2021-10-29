#include "sveSekcije.h"
#include "tabelaSekcija.h"

void SveSekcije::novaSekcija(string sekcija) {

    //cout<<"------------------------------DODAJE SE "<<sekcija<<"\n";

    bool ima = false;

    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        if(it->naziv == sekcija) {
            ima = true;
            break;
        }

    if(ima == true)
        return;

    TabelaSekcija nova(sekcija);

    sekcije.push_back(nova);

    //cout<<"------------------------------DODATA "<<sekcija<<"\n";

}

void SveSekcije::ispisiCountere() {

    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        cout<<it->naziv<<": "<<it->locationCounter<<"\n";
}

void SveSekcije::povecajCounter(string sekcija, int br) {
    
    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        if(it->naziv == sekcija)
            it->locationCounter += br;
}

int SveSekcije::offset(string sekcija) {
    
    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        if(it->naziv == sekcija)
            return it->locationCounter;

    return -1;
}

void SveSekcije::dodajSekciju(Sekcija sekcija) {

    list<TabelaSekcija>::iterator it;

    bool dodao = false;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        if(it->naziv == sekcija.naziv) {
            it->dodajSekciju(sekcija);
            dodao = true;
        }
}

void SveSekcije::obrisiSekciju(string naziv) {

    list<TabelaSekcija>::iterator it;

    list<TabelaSekcija> pomocna;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        if(it->naziv != naziv)
            pomocna.push_back(*it);

    this->sekcije.clear();

    for(it = pomocna.begin(); it != pomocna.end(); it++) {
        this->sekcije.push_back(*it);
    }
            
}

void SveSekcije::anulirajCountere() {

    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        it->locationCounter = 0;
}

void SveSekcije::ispisiSekcije() {

    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it) {
        it->ispisi();
        cout<<"\n";
    }

}

void SveSekcije::dodajVelicinu(string sekcija, int vel) {

    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        if(it->naziv == sekcija)
            it->velicina = vel;



}

void SveSekcije::dodajAdresu(string sekcija, int vel) {

    list<TabelaSekcija>::iterator it;

    for(it = sekcije.begin(); it != sekcije.end(); ++it)
        if(it->naziv == sekcija)
            it->adresa = vel;



}
