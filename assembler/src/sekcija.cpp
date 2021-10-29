#include "sekcija.h"

void Sekcija::ipisi() {

    cout<<offset<<'|'<<sadrzaj;

}

int Sekcija::ident = 0;

string Sekcija::ispisiKraj() {
    
    return "" + offset + '|' + sadrzaj;

}