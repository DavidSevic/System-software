#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
#include "asembler.h"
#include "linker.h"
#include <list>
#include <string>

#include <regex>

int main(int argc, char* argv[]) {


    if(argc < 5) {
        cout<<"Morate uneti minimum 4 argumenta";
        return -1;
    }

    
    string ko = "";

    for(int i = 0; 1 == 1; i++)
        if(argv[1][i] == '\0')
            break;
        else
            ko += argv[1][i];
    

    if(ko == "asembler") {
        
        Asembler a;

        a.asembliraj(argv[4], argv[3]);
    }
    else if(ko == "linker") {

        Linker l;

        list<string> ulazni;

        list<string> lokacije;

        string izlaz;

        for(int i = 2; i < argc; i++) {
            
            string sta = "";

            for(int j = 0; 1 == 1; j++)
                if(argv[i][j] == '\0')
                    break;
                else
                    sta += argv[i][j];

            if(sta == "-o") {

                ++i;

                izlaz = argv[i];

                continue;
            }

            if(sta.find("@") != string::npos)
                lokacije.push_back(sta.substr(7));
            else if(sta.find(".o") != string::npos)
                ulazni.push_back(sta);
        }

        for(int i = 2; i < argc; i++) {

            string koja = "";

            for(int j = 0; 1==1; j++)
                if(argv[i][j] == '\0')
                    break;
                else
                    koja += argv[i][j];

            if(koja == "-linkable" || koja == "-hex") {

                    if(koja == "-linkable") {
                        l.linkable(ulazni, izlaz, true);
                        break;
                    }
                    else if(koja == "-hex") {
                        l.hex(ulazni, lokacije, izlaz);
                        break;
                    }   
            }
            else
                cout<<"Morate uneti jednu od dve funkcije linkera: -linkable ili -hex\n";
        }
    }
}