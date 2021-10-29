#include "linker.h"
#include "tabelaSimbola.h"
#include <sstream>
#include <fstream>
#include "math.h"

bool sortirajSekcije(TabelaSekcija& tab1, TabelaSekcija& tab2) {

    return tab1.adresa < tab2.adresa;

}

void Linker::linkable(list<string> ulazni, string izlazni, bool solo) {

    
    list<string>::iterator it;

    for(it = ulazni.begin(); it != ulazni.end(); ++it) {

        Fajl fajl;

        ifstream ulaz(*it);

        string lin;

        string deo;

        string prerada = "";

        TabelaSimbola* tabSimbola;
        SveSekcije* tabSekcija;
        TabelaRelZapisa* tabRelZapisa;

        TabelaSekcija* tekTabSekcija;

        string tekucaSekcija = "";

        while(getline(ulaz, lin)) {
            
            if(lin.length() == 0)
                continue;

            if(lin == "Tabela simbola:") {
                prerada = "simboli";
                tabSimbola = new TabelaSimbola();
                continue;
            }
            else if(lin == "Tabela sekcija:") {
                prerada = "sekcije";
                tabSekcija = new SveSekcije();
                continue;
            }
            else if(lin == "Tabela relokacionih zapisa:") {
                prerada = "zapisi";

                tabRelZapisa = new TabelaRelZapisa();
                continue;
            }
            else if(lin.find("Sekcija:") != string::npos) {

                tekucaSekcija = "";

                for(int i = 9; lin[i] != '('; i++) {
                    tekucaSekcija += lin[i];
                }

                bool citaj = false;

                string velicina = "";


                for(int i = 0; i < lin.length(); i++)
                    if(citaj == true)
                        velicina += lin[i];
                    else if(lin[i] == '(')
                        citaj = true;
                    else if(lin[i] == ')')
                        break;


                int vel = 0;

                stringstream s(velicina);

                s >> vel;

                fajl.tabSveSekcije.novaSekcija(tekucaSekcija);
                fajl.tabSveSekcije.dodajVelicinu(tekucaSekcija, vel);

                continue;
            }
            
            
            if(prerada == "simboli") {

                Simbol novi;

                string labela = "";
                string sekcija = "";
                string offset;
                string global;
                string redniBr; 

                int br = 0;

                for(int i = 0; i < lin.length(); i++)
                    if(lin[i] == '|')
                        ++br;
                    else {

                        if(br == 1)
                            labela += lin[i];
                       else if(br == 2)
                            sekcija += lin[i];
                        else if(br == 3)
                            offset += lin[i];
                        else if(br == 4)
                            global += lin[i];
                        else if(br == 5)
                            redniBr += lin[i];
                    }

                novi.labela = labela;
                novi.sekcija = sekcija;

                int broj = 0;
                
                stringstream o(offset);

                o >> broj;

                novi.offset = broj;

                if(global == "local")
                    novi.global = false;
                else
                    novi.global = true;

                stringstream r(redniBr);

                r >> broj;

                novi.redniBr = broj;


               fajl.tabSimbola.dodajSimbol(novi);

                continue;
            }

            if(prerada == "sekcije") {

                Sekcija nova;

                string naziv = tekucaSekcija;
                string offset = "";
                string sadrzaj = "";

                int br = 0;

                for(int i = 0; i < lin.length(); i++)
                    if(lin[i] == '|')
                        ++br;
                    else {

                        if(br == 0)
                            offset += lin[i];
                       else if(br == 1)
                            sadrzaj += lin[i];
                    }

                nova.naziv = naziv;
                nova.offset = offset;
                nova.sadrzaj = sadrzaj;

                fajl.tabSveSekcije.dodajSekciju(nova);

                continue;
            }

            if(prerada == "zapisi") {

                RelZapis novi;

                string sekcija = "";
                string pozicija = "";
                char adresiranje;
                string opis;

                int br = 0;

                for(int i = 0; i < lin.length(); i++)
                    if(lin[i] == '|')
                        ++br;
                    else {

                        if(br == 1)
                            sekcija += lin[i];
                       else if(br == 2)
                            pozicija += lin[i];
                        else if(br == 3)
                            adresiranje = lin[i];
                        else if(br == 4)
                            opis += lin[i];
                    }

                novi.sekcija = sekcija;
                novi.adresiranje = adresiranje;
                novi.opis = opis;

                int broj = 0;
                
                stringstream o(pozicija);

                o >> broj;

                novi.pozicija = broj;


                fajl.tabRelZapisa.dodajZapis(novi);

                continue;
            }
        }

        dodajFajl(fajl);

    }

    
    //ispisi();

    //cout<<"--------------------------------------------------------------GOTOVO OVO\n";

    

    //-----------------------------------------------------------------------------------------------SPAJANJE SEKCIJA


    list<Fajl>::iterator fajl;

    int i = 0;

    for(fajl = fajlovi.begin(); fajl != fajlovi.end(); ++fajl, i++) { // sve sekcije jednog fajla      

        int j = 0;
        
        list<TabelaSekcija>::iterator tab;

        TabelaSekcija* izbaci = nullptr;

        for(tab = fajl->tabSveSekcije.sekcije.begin(); tab != fajl->tabSveSekcije.sekcije.end(); ++tab, j++) { //jedna sekcija u faju

            if(kolikoOvih(tab->naziv) > 1) { // ima vise sa istim imenom

                list<Fajl>::iterator fajlDrugi;

                for(fajlDrugi = fajlovi.begin(); fajlDrugi != fajlovi.end(); ++fajlDrugi) {

                    if(fajlDrugi->id == fajl->id)
                        continue;
                    
                    int k = 0;

                    list<TabelaSekcija>::iterator tabDruga;

                    for(tabDruga = fajlDrugi->tabSveSekcije.sekcije.begin(); tabDruga != fajlDrugi->tabSveSekcije.sekcije.end(); ++tabDruga, k++)
                        
                        if(tabDruga->naziv == tab->naziv) {

                            int pomeraj = -1;

                            list<TabelaSekcija>::iterator s;

                            for(s = sveSekcije.sekcije.begin(); s != sveSekcije.sekcije.end(); s++)
                                if(s->naziv == tab->naziv)
                                    pomeraj = s->velicina;
                            
                            if(pomeraj == -1) {

                                pomeraj = tab->velicina;

                                sveSekcije.novaSekcija(tab->naziv);

                                sveSekcije.dodajVelicinu(tab->naziv, tab->velicina);

                                list<Sekcija>::iterator sekNove;

                                for(sekNove = tab->sekcije.begin(); sekNove != tab->sekcije.end(); sekNove++)
                                    sveSekcije.dodajSekciju(*sekNove);
                            }

                            list<Simbol>::iterator simb;

                            for(simb = fajlDrugi->tabSimbola.simboli.begin(); simb != fajlDrugi->tabSimbola.simboli.end(); simb++)
                                if(simb->sekcija == tabDruga->naziv)
                                    simb->pomerajSekcije = pomeraj;
                            
                            
                            list<Sekcija>::iterator sek;

                            for(sek = tabDruga->sekcije.begin(); sek != tabDruga->sekcije.end(); sek++) { // pomeranje offseta sekcija

                                string s1 = "";
                                string s2 = "";

                                bool prvi = true;

                                for(int p = 0; p < sek->offset.length(); p++)
                                    if(sek->offset[p] == '-') 
                                        prvi = false;
                                    else if(prvi == true)
                                        s1 += sek->offset[p];
                                    else
                                        s2 += sek->offset[p];
                                
                                
                                int br1 = 0;
                                int br2 = 0;
                                
                                stringstream ss1(s1);
                                
                                ss1 >> br1;

                                stringstream ss2(s2);

                                ss2 >> br2;

                                br1 += pomeraj;
                                br2 += pomeraj;

                                sek->offset = "";
                                sek->offset += to_string(br1);
                                sek->offset += "-";
                                sek->offset += to_string(br2);

                                sveSekcije.dodajSekciju(*sek);

                            }


                            sveSekcije.dodajVelicinu(tab->naziv, pomeraj + tabDruga->velicina);
                            
                            //pomeranje pozicije relokacionih zapisa

                            list<RelZapis>::iterator zap;

                            for(zap = fajlDrugi->tabRelZapisa.zapisi.begin(); zap != fajlDrugi->tabRelZapisa.zapisi.end(); zap++)
                                if(zap->sekcija == tab->naziv) {
                                    zap->pozicija += pomeraj;
                                }

                            
                            // brisanje te druge sekcije

                            fajlDrugi->tabSveSekcije.obrisiSekciju(tab->naziv);

                            break;
                        }     
                    }
            }
            else {

                sveSekcije.novaSekcija(tab->naziv);

                sveSekcije.dodajVelicinu(tab->naziv, tab->velicina);

                list<Sekcija>::iterator sekNove;

                for(sekNove = tab->sekcije.begin(); sekNove != tab->sekcije.end(); sekNove++)
                    sveSekcije.dodajSekciju(*sekNove);


            }
        }
    }

    //-------------------------------------------------------------------------------------------------------SIMBOLI

    if(jelGreskaSimbol() == true)
        return;


    list<TabelaSekcija>::iterator sek;

    Simbol* novi;

    for(sek = sveSekcije.sekcije.begin(); sek != sveSekcije.sekcije.end(); sek++) { // prvo ubacujemo sekcije
                                                                                    // u tabelu simbola
        novi = new Simbol();                                                        // i menjamo rel zapise
        novi->labela = sek->naziv;                                                  // za te sekcije
        novi->sekcija = sek->naziv;
        novi->offset = 0;
        novi->global = false;

        Simbol novi1(sek->naziv, sek->naziv, 0, false);

        tabSimbola.dodajSimbol(novi1);   

        for(fajl = fajlovi.begin(); fajl != fajlovi.end(); fajl++) {
            izmeniZapise(fajl->id, tabSimbola.redniBroj, sek->naziv);
        }
    }


    for(fajl = fajlovi.begin(); fajl != fajlovi.end(); fajl++) {
        
        list<Simbol>::iterator simb;

        for(simb = fajl->tabSimbola.simboli.begin(); simb != fajl->tabSimbola.simboli.end(); simb++) {

            if(simb->labela == simb->sekcija)
                continue;

            if(kolikoOvihSimb(simb->labela) == 1 && simb->sekcija == "n/a") {
                cout << "Greška: Linker nije naišao na definiciju simbola: " << simb->labela << " .";
                return;
            }
            
            if(kolikoOvihSimb(simb->labela) > 1) {

                if(simb->sekcija == "n/a")
                    continue;
                

                novi = new Simbol();                                                        
                novi->labela = simb->labela;                                                  
                novi->sekcija = simb->sekcija;
                novi->offset = simb->offset + simb->pomerajSekcije;
                novi->global = simb->global;   

                Simbol novi2(simb->labela, simb->sekcija, simb->offset + simb->pomerajSekcije, simb->global);                                                    
                

                tabSimbola.dodajSimbol(novi2);

                list<Fajl>::iterator fajl1;      


                for(fajl1 = fajlovi.begin(); fajl1 != fajlovi.end(); fajl1++) {
                    izmeniZapise(fajl1->id, tabSimbola.redniBroj, simb->labela);
                }


            }
            else { // pojedinacan simbol

                novi = new Simbol();                                                        
                novi->labela = simb->labela;                                                  
                novi->sekcija = simb->sekcija;
                novi->offset = simb->offset + simb->pomerajSekcije;
                novi->global = simb->global;

                Simbol novi3(simb->labela, simb->sekcija, simb->offset + simb->pomerajSekcije, simb->global);

                tabSimbola.dodajSimbol(novi3);

            }

        }   
    }

     //--------------------------------------------------------------------------------ZAPISI
        
    list<Fajl>::iterator fajlO;
    
    for(fajlO = fajlovi.begin(); fajlO != fajlovi.end(); fajlO++) {


        list<RelZapis>::iterator zap;

        for(zap = fajlO->tabRelZapisa.zapisi.begin(); zap != fajlO->tabRelZapisa.zapisi.end(); zap++)
            tabRelZapisa.dodajZapis(*zap);

        }


    if(solo == true) {

        ofstream izlaz(izlazni);

        izlaz << tabSimbola;
        izlaz << sveSekcije;
        izlaz << tabRelZapisa;


    }

}

void Linker::hex(list<string> ulazni, list<string> lokacije, string izlazni) {
    
    linkable(ulazni, izlazni, false);

    //ispisiGotovo();

    list<TabelaSekcija> saAdresom;
    list<TabelaSekcija> bezAdrese;

    list<string>::iterator lok;



    //------------------------------------------------------------------------POMERANJE SEKCIJA

    for(lok = lokacije.begin(); lok != lokacije.end(); lok++) {

        string sek = "";
        string adr = "";
        bool br = false;
        
        for(int i = 0; i < lok->length(); i++)    
            if((*lok)[i] == '@')
                br = true;
            else if(br == false)
                sek += (*lok)[i];
            else if(br == true)
                adr += (*lok)[i];


        int broj = 0;

        stringstream s;

        s << std::hex << adr;

        s >> broj;


        list<TabelaSekcija>::iterator tab;

        for(tab = sveSekcije.sekcije.begin(); tab != sveSekcije.sekcije.end(); tab++)
            if(tab->naziv == sek) {
                tab->adresa = broj;
            } 

        list<TabelaSekcija>::iterator tabSek;

        bool nasao = false;

        for(tabSek = sveSekcije.sekcije.begin(); tabSek != sveSekcije.sekcije.end(); tabSek++)
            if(tabSek->naziv == sek) {
                saAdresom.push_back(*tabSek);
            }
    }

    saAdresom.sort(sortirajSekcije);

    list<TabelaSekcija>::iterator tabAdr;
    list<TabelaSekcija>::iterator tab;

    
    for(tab = sveSekcije.sekcije.begin(); tab != sveSekcije.sekcije.end(); tab++) {

        bool nasao = false;

        for(tabAdr = saAdresom.begin(); tabAdr != saAdresom.end(); tabAdr++)
            if(tabAdr->naziv == tab->naziv) {
                nasao = true;
                break;
            }

        if(nasao == false)
            bezAdrese.push_back(*tab);
    }
    

    int adresa = 0;

    string prosla = "";


    for(tabAdr = saAdresom.begin(); tabAdr != saAdresom.end(); tabAdr++) { // prvo one koje su navedene adresama

        for(tab = sveSekcije.sekcije.begin(); tab != sveSekcije.sekcije.end(); tab++)
            if(tabAdr->naziv == tab->naziv) {

                if(tab->adresa < adresa) {
                    
                    string adrG = "0x";
                    
                    std::stringstream stream;

                    stream << std::hex << adresa;

                    std::string broj(stream.str());

                    if(broj.length() < 4) {
                        for(int i = 0; i < 4 - broj.length(); i++)
                        adrG += "0";
                    }

                    adrG += broj;
                    
                    cout << "Greška: Linker je detektovao preklapanje na zadatoj adresi za sekciju: "<<tab->naziv<<" . Prva slobodna adresa je: "<<adrG<<".\n";
                    return;
                }

                int pomeraj = tab->adresa;

                list<Sekcija>::iterator sek;

                for(sek = tab->sekcije.begin(); sek != tab->sekcije.end(); sek++) {

                    string s1 = "";
                    string s2 = "";

                    bool prvi = true;

                    for(int p = 0; p < sek->offset.length(); p++)
                        if(sek->offset[p] == '-') 
                            prvi = false;
                        else if(prvi == true)
                            s1 += sek->offset[p];
                        else
                            s2 += sek->offset[p];
                                
                                
                    int br1 = 0;
                    int br2 = 0;
                                
                    stringstream ss1(s1);
                                
                    ss1 >> br1;

                    stringstream ss2(s2);

                    ss2 >> br2;

                    br1 += pomeraj;
                    br2 += pomeraj;

                    sek->offset = "";
                    sek->offset += to_string(br1);
                    if(s2 != "") {
                        sek->offset += "-";
                        sek->offset += to_string(br2);      
                    } 
                }

                
                list<Simbol>::iterator simb;

                for(simb = tabSimbola.simboli.begin(); simb != tabSimbola.simboli.end(); simb++)
                    if(simb->sekcija == tab->naziv)
                        simb->offset += pomeraj;
                
                list<RelZapis>::iterator zap;

                for(zap = tabRelZapisa.zapisi.begin(); zap != tabRelZapisa.zapisi.end(); zap++)
                    if(zap->sekcija == tab->naziv)
                        zap->pozicija += pomeraj;


                //novo
                if(prosla == "" || (tab->adresa - adresa) >= (8 - (adresa % 8))) {

                    sekcijeIspisSaAdresom.novaSekcija(tab->naziv);
                    sekcijeIspisSaAdresom.dodajVelicinu(tab->naziv, tab->velicina);
                    sekcijeIspisSaAdresom.dodajAdresu(tab->naziv, tab->adresa);

                    list<Sekcija>::iterator sekIspis;
                    for(sekIspis = tab->sekcije.begin(); sekIspis != tab->sekcije.end(); sekIspis++)
                        sekcijeIspisSaAdresom.dodajSekciju(*sekIspis);

                    prosla = tab->naziv;

                } 
                else {

                    list<Sekcija>::iterator sekIspis;
                    for(sekIspis = tab->sekcije.begin(); sekIspis != tab->sekcije.end(); sekIspis++) {


                        Sekcija* nova = new Sekcija(prosla, sekIspis->offset, sekIspis->sadrzaj);

                        nova->id = sekIspis->id;

                        sekcijeIspisSaAdresom.dodajSekciju(*nova);

                    }

                    list<TabelaSekcija>::iterator nadji;
                        for(nadji = sekcijeIspisSaAdresom.sekcije.begin(); nadji != sekcijeIspisSaAdresom.sekcije.end(); nadji++)
                            if(nadji->naziv == prosla) {

                                sekcijeIspisSaAdresom.dodajVelicinu(prosla, nadji->velicina + tab->velicina);
                                break;
                            }
                }

                
                adresa = pomeraj + tab->velicina;
            }
    }



    string prva = prosla;



    for(tabAdr = bezAdrese.begin(); tabAdr != bezAdrese.end(); tabAdr++) { // one koje nisu navedene adresama

        for(tab = sveSekcije.sekcije.begin(); tab != sveSekcije.sekcije.end(); tab++)
            if(tabAdr->naziv == tab->naziv) {

                int pomeraj = adresa;

                tab->adresa = adresa;

                list<Sekcija>::iterator sek;

                for(sek = tab->sekcije.begin(); sek != tab->sekcije.end(); sek++) {

                    string s1 = "";
                    string s2 = "";

                    bool prvi = true;

                    for(int p = 0; p < sek->offset.length(); p++)
                        if(sek->offset[p] == '-') 
                            prvi = false;
                        else if(prvi == true)
                            s1 += sek->offset[p];
                        else
                            s2 += sek->offset[p];
                                
                                
                    int br1 = 0;
                    int br2 = 0;
                                
                    stringstream ss1(s1);
                                
                    ss1 >> br1;

                    stringstream ss2(s2);

                    ss2 >> br2;

                    br1 += pomeraj;
                    br2 += pomeraj;

                    sek->offset = "";
                    sek->offset += to_string(br1);
                    if(s2 != "") {
                        sek->offset += "-";
                        sek->offset += to_string(br2);      
                    }      
                }

                
                list<Simbol>::iterator simb;

                for(simb = tabSimbola.simboli.begin(); simb != tabSimbola.simboli.end(); simb++)
                    if(simb->sekcija == tab->naziv)
                        simb->offset += pomeraj;
                
                list<RelZapis>::iterator zap;

                for(zap = tabRelZapisa.zapisi.begin(); zap != tabRelZapisa.zapisi.end(); zap++)
                    if(zap->sekcija == tab->naziv)
                        zap->pozicija += pomeraj;


                if(prva == "") {

                    sekcijeIspisSaAdresom.novaSekcija(tab->naziv);
                    sekcijeIspisSaAdresom.dodajVelicinu(tab->naziv, tab->velicina);
                    sekcijeIspisSaAdresom.dodajAdresu(tab->naziv, adresa);
                    

                    list<Sekcija>::iterator sekIspis;
                    for(sekIspis = tab->sekcije.begin(); sekIspis != tab->sekcije.end(); sekIspis++)
                        sekcijeIspisSaAdresom.dodajSekciju(*sekIspis);

                    prva = tab->naziv;

                } else {

                    list<Sekcija>::iterator sekIspis;
                    for(sekIspis = tab->sekcije.begin(); sekIspis != tab->sekcije.end(); sekIspis++) {


                        Sekcija* nova = new Sekcija(prva, sekIspis->offset, sekIspis->sadrzaj);

                        nova->id = sekIspis->id;

                        sekcijeIspisSaAdresom.dodajSekciju(*nova);
                    }

                    list<TabelaSekcija>::iterator nadji;
                        for(nadji = sekcijeIspisSaAdresom.sekcije.begin(); nadji != sekcijeIspisSaAdresom.sekcije.end(); nadji++)
                            if(nadji->naziv == prva) {
                                sekcijeIspisSaAdresom.dodajVelicinu(prva, nadji->velicina + tab->velicina);
                                break;
                            }

                }

                
                adresa += tab->velicina;

            }
    }

    //ispisiGotovo();

    //-----------------------------------------------------------RESAVANJE RELOKACIONIH ZAPISA


    list<RelZapis>::iterator zap;

    string zapRedniBr;
    string zapSekcija;
    int zapPozicija;
    int zapVrednost;

    for(zap = tabRelZapisa.zapisi.begin(); zap != tabRelZapisa.zapisi.end(); zap++) {


        zapRedniBr = zap->opis;
        zapSekcija = zap->sekcija;
        zapPozicija = zap->pozicija;
        

        list<Simbol>::iterator simb;

        for(simb = tabSimbola.simboli.begin(); simb != tabSimbola.simboli.end(); simb++)
            if(to_string(simb->redniBr) == zapRedniBr)
                zapVrednost = simb->offset;

        for(tab = sveSekcije.sekcije.begin(); tab != sveSekcije.sekcije.end(); tab++)
            if(tab->naziv == zapSekcija) {
                
                list<Sekcija>::iterator sek;

                for(sek = tab->sekcije.begin(); sek != tab->sekcije.end(); sek++) {

                    //pretvaranje pozicija u brojeve

                    string s1 = "";
                    string s2 = "";

                    bool prvi = true;

                    for(int p = 0; p < sek->offset.length(); p++)
                        if(sek->offset[p] == '-') 
                            prvi = false;
                        else if(prvi == true)
                            s1 += sek->offset[p];
                        else
                            s2 += sek->offset[p];
                                
                                
                    int br1 = 0;
                    int br2 = 0;
                                
                    stringstream ss1(s1);
                                
                    ss1 >> br1;

                    stringstream ss2(s2);

                    ss2 >> br2;

                    //pretvoreno

                    if(zapPozicija >= br1 && zapPozicija <= br2) {
                        
                        int preskoci = (zapPozicija - br1) * 2 + (zapPozicija - br1);

                        string sadrzaj = sek->sadrzaj.substr(preskoci, sek->sadrzaj.length() - preskoci);

        
                        string sadrzajHex = "0x" + sadrzaj.substr(0, 2) + sadrzaj.substr(3, 2);

                        int sadrzajDec = 0;

                        stringstream s;

                        s << std::hex << sadrzajHex;

                        s  >> sadrzajDec;

                        if(sadrzaj[0] == 'f') { // negativan
                            sadrzajDec = (65536 - sadrzajDec) * (-1);
                        }

                        int sadrzajDecc = sadrzajDec + zapVrednost;
                        
                        sadrzajDec += zapVrednost;
                        
                        sadrzajHex = "";
                    
                        std::stringstream stream;

                        stream << std::hex << sadrzajDecc;

                        std::string broj(stream.str());

                        if(broj.length() < 4) {
                            for(int i = 0; i < 4 - broj.length(); i++)
                                sadrzajHex += "0";
                        }
                        else 
                            broj = broj.substr(broj.length() - 4);

                        sadrzajHex += broj;

                        sadrzajHex = sadrzajHex.substr(0, 2) + " " + sadrzajHex.substr(2, 2);

                        for(int i = preskoci; i < sek->sadrzaj.length(); i++)
                            sek->sadrzaj[i] = sadrzajHex[i - preskoci];


                        list<TabelaSekcija>::iterator traziTab;
                        for(traziTab = sekcijeIspisSaAdresom.sekcije.begin(); traziTab != sekcijeIspisSaAdresom.sekcije.end(); traziTab++) {

                            list<Sekcija>:: iterator traziSek;
                            for(traziSek = traziTab->sekcije.begin(); traziSek != traziTab->sekcije.end(); traziSek++) {

                                if(traziSek->id == sek->id)
                                    traziSek->sadrzaj = sek->sadrzaj;
                            }
                        }

                        for(traziTab = sekcijeIspisBezAdrese.sekcije.begin(); traziTab != sekcijeIspisBezAdrese.sekcije.end(); traziTab++) {

                            list<Sekcija>:: iterator traziSek;
                            for(traziSek = traziTab->sekcije.begin(); traziSek != traziTab->sekcije.end(); traziSek++) {

                                if(traziSek->id == sek->id)
                                    traziSek->sadrzaj = sek->sadrzaj;
                            }
                        }

                    }
                }

            }
    }

    sveSekcije.sekcije.sort(sortirajSekcije);

    //sekcijeIspisSaAdresom.ispisiSekcije();

    //sekcijeIspisBezAdrese.ispisiSekcije();


    //ispisiGotovo();
    //ispisKrajHex();

    ofstream izlaz(izlazni);

    izlaz << krajHexFajl();
}

string Linker::krajHexFajl() {

    stringstream fajl;

    string memorija = "";

    list<TabelaSekcija>::iterator tab;

    for(tab = sekcijeIspisSaAdresom.sekcije.begin(); tab != sekcijeIspisSaAdresom.sekcije.end(); tab++) {

        list<Sekcija>::iterator sek;

        memorija = "";

        for(sek = tab->sekcije.begin(); sek != tab->sekcije.end(); sek++) {

            memorija += sek->sadrzaj + " ";
        }


    int adresa = tab->adresa;

    if(adresa % 8 != 0) {

        adresa -= (adresa % 8);

        string nule = "";

        for(int i = 0; i < (tab->adresa % 8); i++)
            nule += "00 ";

        memorija = nule + memorija;

    }
    
    int br = adresa;

    istringstream mem(memorija);

    string bajt;

    string red = "";

    while(mem >> bajt) {

        red += " " + bajt;

        ++br;

        if(br % 8 == 0) {

            string h = "";
                    
            std::stringstream stream;

            stream << std::hex << br - 8;

            std::string broj(stream.str());

            if(broj.length() < 4) {
                for(int i = 0; i < 4 - broj.length(); i++)
                h += "0";
            }

            h += broj;

            fajl<<h<<": "<<red<<"\n";

            red = "";

            h = "";
        }
    }

    if((br % 8) != 0) {

        string h = "";

        std::stringstream stream;

        stream << std::hex << br - (br  % 8);

        std::string broj(stream.str());

        if(broj.length() < 4) {
            for(int i = 0; i < 4 - broj.length(); i++)
            h += "0";
        }

        h += broj;

        for(int i = 0; i < 8 - (br % 8); i++)
            red += " 00";

        fajl<<h<<": "<<red<<"\n";

        }

        fajl<<"\n";
    }

   for(tab = sekcijeIspisBezAdrese.sekcije.begin(); tab != sekcijeIspisBezAdrese.sekcije.end(); tab++) {

        list<Sekcija>::iterator sek;

        memorija = "";

        for(sek = tab->sekcije.begin(); sek != tab->sekcije.end(); sek++) {

            memorija += sek->sadrzaj + " ";
        }


    int adresa = tab->adresa;

    if(adresa % 8 != 0) {

        adresa -= (adresa % 8);

        string nule = "";

        for(int i = 0; i < (tab->adresa % 8); i++)
            nule += "00 ";

        memorija = nule + memorija;

    }
    
    int br = adresa;

    istringstream mem(memorija);

    string bajt;

    string red = "";

    while(mem >> bajt) {

        red += " " + bajt;

        ++br;

        if(br % 8 == 0) {

            string h = "";
                    
            std::stringstream stream;

            stream << std::hex << br - 8;

            std::string broj(stream.str());

            if(broj.length() < 4) {
                for(int i = 0; i < 4 - broj.length(); i++)
                h += "0";
            }

            h += broj;

            fajl<<h<<": "<<red<<"\n";

            red = "";

            h = "";
        }
    }

    if((br % 8) != 0) {

        string h = "";

        std::stringstream stream;

        stream << std::hex << br - (br  % 8);

        std::string broj(stream.str());

        if(broj.length() < 4) {
            for(int i = 0; i < 4 - broj.length(); i++)
            h += "0";
        }

        h += broj;

        for(int i = 0; i < 8 - (br % 8); i++)
            red += " 00";

        fajl<<h<<": "<<red<<"\n";

        }

        fajl<<"\n";
    }

    return fajl.str();
}


void Linker::ispisKrajHex() {

    string memorija = "";

    list<TabelaSekcija>::iterator tab;

    for(tab = sekcijeIspisSaAdresom.sekcije.begin(); tab != sekcijeIspisSaAdresom.sekcije.end(); tab++) {

        list<Sekcija>::iterator sek;

        memorija = "";

        for(sek = tab->sekcije.begin(); sek != tab->sekcije.end(); sek++) {

            memorija += sek->sadrzaj + " ";
        }


    int adresa = tab->adresa;

    if(adresa % 8 != 0) {

        adresa -= (adresa % 8);

        string nule = "";

        for(int i = 0; i < (tab->adresa % 8); i++)
            nule += "00 ";

        memorija = nule + memorija;

    }
    
    int br = adresa;

    istringstream mem(memorija);

    string bajt;

    string red = "";

    while(mem >> bajt) {

        red += " " + bajt;

        ++br;

        if(br % 8 == 0){

            string h = "";
                    
            std::stringstream stream;

            stream << std::hex << br - 8;

            std::string broj(stream.str());

            if(broj.length() < 4) {
                for(int i = 0; i < 4 - broj.length(); i++)
                h += "0";
            }

            h += broj;

            cout<<h<<": "<<red<<"\n";

            red = "";

            h = "";
        }
    }

    if((br % 8) != 0) {

        string h = "";

        std::stringstream stream;

        stream << std::hex << br - (br  % 8);

        std::string broj(stream.str());

        if(broj.length() < 4) {
            for(int i = 0; i < 4 - broj.length(); i++)
            h += "0";
        }

        h += broj;

        for(int i = 0; i < 8 - (br % 8); i++)
            red += " 00";

        cout<<h<<": "<<red<<"\n";

        }

        cout<<"\n";
    }
    
   for(tab = sekcijeIspisBezAdrese.sekcije.begin(); tab != sekcijeIspisBezAdrese.sekcije.end(); tab++) {

        list<Sekcija>::iterator sek;

        memorija = "";

        for(sek = tab->sekcije.begin(); sek != tab->sekcije.end(); sek++) {

            memorija += sek->sadrzaj + " ";
        }


    int adresa = tab->adresa;

    if(adresa % 8 != 0) {


        adresa -= (adresa % 8);

        string nule = "";

        for(int i = 0; i < (tab->adresa % 8); i++)
            nule += "00 ";

        memorija = nule + memorija;
    }
    
    int br = adresa;

    istringstream mem(memorija);

    string bajt;

    string red = "";

    while(mem >> bajt) {

        red += " " + bajt;

        ++br;

        if(br % 8 == 0){

            string h = "";
                    
            std::stringstream stream;

            stream << std::hex << br - 8;

            std::string broj(stream.str());

            if(broj.length() < 4) {
                for(int i = 0; i < 4 - broj.length(); i++)
                h += "0";
            }

            h += broj;

            cout<<h<<": "<<red<<"\n";

            red = "";

            h = "";
        }
    }

    if((br % 8) != 0) {

        string h = "";

        std::stringstream stream;

        stream << std::hex << br - (br  % 8);

        std::string broj(stream.str());

        if(broj.length() < 4) {
            for(int i = 0; i < 4 - broj.length(); i++)
            h += "0";
        }

        h += broj;

        for(int i = 0; i < 8 - (br % 8); i++)
            red += " 00";

        cout<<h<<": "<<red<<"\n";

        }

        cout<<"\n";
    }
}

void Linker::izmeniZapise(int idF, int nova, string labela) {

    

    list<Fajl>::iterator fajl;

    for(fajl = fajlovi.begin(); fajl != fajlovi.end(); fajl++) {

        if(fajl->id == idF) {

            list<Simbol>::iterator simb;

            for(simb = fajl->tabSimbola.simboli.begin(); simb != fajl->tabSimbola.simboli.end(); simb++)
                if(simb->labela == labela) {


                    int stara = simb->redniBr;

                    list<RelZapis>::iterator zap;

                    for(zap = fajl->tabRelZapisa.zapisi.begin(); zap != fajl->tabRelZapisa.zapisi.end(); zap++)
                        if(zap->opis == to_string(stara) && zap->promenjen == false) {

                            //cout<<"__________________________________\n";

                            zap->opis = to_string(nova);


                            //cout<<"__________________________________\n";

                            zap->promenjen = true;
                        }
                }

        }
    }
}

bool Linker::jelGreskaSimbol() {
    
    list<Fajl>::iterator fajl;

    for(fajl = fajlovi.begin(); fajl != fajlovi.end(); fajl++) {
        
        list<Simbol>::iterator simb;

        for(simb = fajl->tabSimbola.simboli.begin(); simb != fajl->tabSimbola.simboli.end(); simb++) {

            if(simb->labela == simb->sekcija)
                continue;

            if(simb->sekcija != "n/a" && simb->global == true) {

                list<Fajl>::iterator fajlDrugi;

                for(fajlDrugi = fajlovi.begin(); fajlDrugi != fajlovi.end(); fajlDrugi++) {

                    if(fajl->id == fajlDrugi->id)
                        continue;
                    
                    list<Simbol>::iterator simbDrugi;

                    for(simbDrugi = fajlDrugi->tabSimbola.simboli.begin(); simbDrugi != fajlDrugi->tabSimbola.simboli.end(); simbDrugi++) {
                        
                        if(simbDrugi->sekcija != "n/a" && simbDrugi->labela == simb->labela && simbDrugi->global == true) {
                            cout << "Greška: Linker je detektovao višestruku definiciju simbola: " << simb->labela << " .";
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void Linker::ispisi() {

    list<Fajl>::iterator f;

    for(f = fajlovi.begin(); f != fajlovi.end(); f++) {
        f->tabSimbola.ispisi();
        cout<<"\n";
        f->tabSveSekcije.ispisiSekcije();
        cout<<"\n";
        f->tabRelZapisa.ispisi();
        cout<<"\n";
        cout<<"\n";
        cout<<"\n";
    }
}

void Linker::ispisiGotovo() {


    cout<<"---------------GOTOVO----------------\n\n";
   
    tabSimbola.ispisi();
    
    cout<<"\n\n\n";

    sveSekcije.ispisiSekcije();

    cout<<"\n\n\n";

    tabRelZapisa.ispisi();

}

int Linker::kolikoOvih(string naziv) {

    int br = 0;

    list<Fajl>::iterator fajl;

    for(fajl = fajlovi.begin(); fajl != fajlovi.end(); fajl++) {

        list<TabelaSekcija>::iterator tab;

        for(tab = fajl->tabSveSekcije.sekcije.begin(); tab != fajl->tabSveSekcije.sekcije.end(); tab++)
            if(tab->naziv == naziv)
                ++br;
    }

    return br;
}

int Linker::kolikoOvihSimb(string labela) {

    int br = 0;

    list<Fajl>::iterator fajl;

    for(fajl = fajlovi.begin(); fajl != fajlovi.end(); fajl++) {

        list<Simbol>::iterator simb;

        for(simb = fajl->tabSimbola.simboli.begin(); simb != fajl->tabSimbola.simboli.end(); simb++)
            if(simb->labela == labela)
                ++br;
    }

    return br;
}

TabelaSekcija* Linker::istogNaziva(string naziv, int poz) {


    list<SveSekcije>::iterator sve;

    return nullptr;

}

bool Linker::jelImaIsta(SveSekcije* sve, string naziv, TabelaSekcija* ova) {
    return false;
} 