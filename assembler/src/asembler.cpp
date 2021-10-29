#include "asembler.h"
#include "simbol.h"
#include "sekcija.h"
#include "tabelaSimbola.h"
#include "tabelaSekcija.h"
#include <sstream>
#include <fstream>
#include <regex>
#include <string>

void Asembler::asembliraj(string ulazni, string izlazni)
{
   // cout << "Prvi prolaz:\n\n";

    int ret = prviProlaz(ulazni);

    if(ret == 0) {
        //cout << "_____________________\n\n";
        //cout << "Drugi prolaz:\n\n";
        drugiProlaz(ulazni, izlazni);
    }
}

int Asembler::prviProlaz(string ulaz)
{

    ifstream ulazni(ulaz);

    string lin;

    string deo;

    bool kraj = false;

    bool greska = false;

    string sekcija;

    int brLinije = 0;

    bool odradjena = false;

    bool labelaJe = false;

    bool komentar = false;

    while (getline(ulazni, lin))
    {

        string s = "";

        for(int i = 0; i < lin.length(); i++)
            if(lin[i] == '#')
                break;
            else
                s += lin[i];

        lin = s;

        ++brLinije;

        if (lin.length() == 0)
            continue;


        istringstream linija(lin);

        odradjena = false;

        labelaJe = false;

        komentar = false;

        while (linija >> deo)
        {

            if (odradjena == true)
            {
                cout << "Greška: Jedna linija izvornog koda mora da sadrži najviše jednu asemblersku naredbu ili direktivu. Na liniji broj: " << brLinije << ".\n";
                greska = true;
                break;
            }

            if (kraj == true)
                break;

            if (deo == ".end")
            {
                odradjena = true;
                kraj = true;
                continue;
            }

            if (deo == ".section")
            {

                if(linija >> deo) {

                    regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");

                    if(regex_match(deo, simbol)) {
                        
                        sekcija = deo;

                        Simbol novi(deo, sekcija, 0, false);

                        if(!tabSimbola.dodajSimbol(novi)) {
                            cout << "Greška: Ponovna definicija simbola: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }

                        sveSekcije.novaSekcija(sekcija);

                        odradjena = true;

                        continue;
                    }
                    else {
                        cout << "Greška: Neodgovarajući naziv sekcije. Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }
                }
                else {
                    cout << "Greška: Izostavljen naziv sekcije. Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }
            }
            if (deo == ".text")
            {

                sekcija = deo.substr(1);

                Simbol novi(deo, sekcija, 0, false);

                if(!tabSimbola.dodajSimbol(novi)) {
                    cout << "Greška: Ponovna definicija simbola: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                sveSekcije.novaSekcija(sekcija);

                odradjena = true;

                continue;
            }
            if (deo == ".rodata")
            {

                sekcija = deo.substr(1);

                Simbol novi(deo, sekcija, 0, false);

                if(!tabSimbola.dodajSimbol(novi)) {
                    cout << "Greška: Ponovna definicija simbola: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                sveSekcije.novaSekcija(sekcija);

                odradjena = true;

                continue;
            }
            if (deo == ".data")
            {

                sekcija = deo.substr(1);

                Simbol novi(deo, sekcija, 0, false);

                if(!tabSimbola.dodajSimbol(novi)) {
                    cout << "Greška: Ponovna definicija simbola: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                sveSekcije.novaSekcija(sekcija);

                odradjena = true;

                continue;
            }
            if (deo == ".bss")
            {

                sekcija = deo.substr(1);

                Simbol novi(deo, sekcija, 0, false);

                if(!tabSimbola.dodajSimbol(novi)) {
                    cout << "Greška: Ponovna definicija simbola: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                sveSekcije.novaSekcija(sekcija);

                odradjena = true;

                continue;
            }
            if (deo == ".global")
            {
                while (true)
                {

                    linija >> deo;

                    if (deo.substr(deo.length() - 1) != ",") {

                        break;
                    }
                }

                odradjena = true;

                continue;
            }
            if (deo == ".extern")
            {

                bool barJedan = false;

                while (true) {

                    if(linija >> deo) {
                        barJedan = true;
                    }
                    else {
                        if(barJedan == false) {

                            cout << "Greška: Izostavljen operand za direktivu: .extern. Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            kraj = true;
                            break;

                        }
                    }

                    if (deo.substr(deo.length() - 1) != ",")
                    {

                        Simbol novi(deo, "n/a", 0, true);

                        if(!tabSimbola.dodajSimbol(novi)) {
                            cout << "Greška: Ponovna definicija simbola: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }

                        break;
                    }

                    Simbol novi(deo.substr(0, deo.length() - 1), "n/a", 0, true);

                    if(!tabSimbola.dodajSimbol(novi)) {
                        cout << "Greška: Ponovna definicija simbola: "<< deo.substr(0, deo.length() - 1) <<". Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }
                }

                if(kraj == true)
                    break;

                odradjena = true;

                continue;
            }
            if (deo == ".word") {

                if(linija >> deo) {

                    bool izadji = false;

                    while (true) {

                        regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");

                        regex regBr("[0-9]+");

                        regex regHex("0[xX][0-9a-fA-F]+");

                        string provera;

                        if(deo[deo.length() - 1] == ',')
                            provera = deo.substr(0, deo.length() - 1);
                        else
                            provera = deo;

                        if(!(regex_match(provera, simbol) && (provera != "r0" && provera != "r1" && provera != "r2" && provera != "r3" && provera != "r4" && provera != "r5" && provera != "r6" && provera != "r7" && provera != "pc" && provera != "sp" && provera != "psw")) && !regex_match(provera, regBr) && !regex_match(provera, regHex)) {
                            cout << "Greška: Neodgovarajući operand " << '"' << provera << '"' << " za direktivu .word, očekuje se simbol ili literal. Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            izadji = true;
                            break;
                        }

                        if (deo.substr(deo.length() - 1) != ",")
                        {

                            sveSekcije.povecajCounter(sekcija, 2);

                            break;
                        }

                        sveSekcije.povecajCounter(sekcija, 2);
                        

                        linija >> deo;
                    }

                    if(izadji == true) {
                        break;
                    }      



                    odradjena = true;

                    continue;
                }
                else {
                    cout << "Greška: Izostavljen operand za direktivu: .word. Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }
            }
            if (deo == ".skip")
            {

                if(linija >> deo) {

                    regex regBr("[0-9]+");

                    regex regHex("0[xX][0-9a-fA-F]+");

                    if(!regex_match(deo, regBr) && !regex_match(deo, regHex)) {
                        cout << "Greška: Neodgovarajući operand " << '"' << deo << '"' << " za direktivu .word, očekuje se literal. Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }

                    int br = 0;

                    if (deo.find("0x") != string::npos)
                    {

                        stringstream h;

                        h << hex << deo;

                        h >> br;
                    }
                    else
                    {

                        stringstream s(deo);

                        s >> br;
                    }

                        sveSekcije.povecajCounter(sekcija, br);

                    odradjena = true;

                    continue;

                }
                else {
                    cout << "Greška: Izostavljen operand za direktivu .skip. Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                
            }
            if (deo == ".equ")
            {

                if(linija >> deo) {

                    string naziv;

                    if(deo[deo.length() - 1] == ',')
                        naziv = deo.substr(0, deo.length() - 1);
                    else 
                        naziv = deo;

                    regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");

                    if(!(regex_match(naziv, simbol) && (naziv != "r0" && naziv != "r1" && naziv != "r2" && naziv != "r3" && naziv != "r4" && naziv != "r5" && naziv != "r6" && naziv != "r7" && naziv != "pc" && naziv != "sp" && naziv != "psw"))) {
                        cout << "Greška: Neodgovarajući prvi operand " << '"' << deo << '"' << " za direktivu .equ, očekuje se simbol. Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }

                    if(linija >> deo) {

                        regex regBr("[0-9]+");

                        regex regHex("0[xX][0-9a-fA-F]+");

                        int br = 0;

                        if (regex_match(deo, regBr))
                        {
                            stringstream s(deo);

                            s >> br;
                        }
                        else if (regex_match(deo, regHex))
                        {
                            stringstream s;

                            s << hex << deo;

                            s >> br;
                        } 
                        else {
                            cout << "Greška: Neodgovarajući drugi operand " << '"' << naziv << '"' << " za direktivu .equ, očekuje se literal. Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }

                        Simbol novi(naziv, "abs", br, false);

                        if(!tabSimbola.dodajSimbol(novi)) {
                            cout << "Greška: Ponovna definicija simbola: "<< naziv <<" . Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }

                        odradjena = true;

                        continue;
                    }
                    else {
                        cout << "Greška: Izostavljen drugi operand za direktivu .equ. Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }

                    
                }
                else {
                    cout << "Greška: Izostavljen prvi operand za direktivu .equ. Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                
            }
            if (deo == ".string")
            {

                linija >> deo;

                int br = 0;

                for (int i = 0; i < deo.length(); i++)
                {

                    if (deo[i] == '"')
                        continue;

                    ++br;
                }

                int index = 0;

                while (deo.find("\\n", index) != string::npos)
                {

                    --br;

                    index = deo.find("\\n", index) + 1;
                }

                odradjena = true;

                continue;
            }
            if (deo == "halt" || deo == "int" || deo == "iret" || deo == "call" || deo == "ret" || deo == "jmp" || deo == "jeq" || deo == "jne" || deo == "jgt" || deo == "push" || deo == "pop" || deo == "xchg" || deo == "add" || deo == "sub" || deo == "mul" || deo == "div" || deo == "cmp" || deo == "not" || deo == "and" || deo == "or" || deo == "xor" || deo == "test" || deo == "shl" || deo == "shr" || deo == "ldr" || deo == "str")
            {

                string inst = deo;

                if (deo == "halt" || deo == "iret" || deo == "ret")
                {
                    sveSekcije.povecajCounter(sekcija, 1);

                    odradjena = true;

                    continue;
                }

                if (deo == "int" || deo == "push" || deo == "pop" || deo == "not")
                {

                    string inst = deo;

                    if (linija >> deo)
                    {

                        if (deo == "r0" || deo == "r1" || deo == "r2" || deo == "r3" || deo == "r4" || deo == "r5" || deo == "r6" || deo == "r7" || deo == "pc" || deo == "sp" || deo == "psw")
                        {
                        
                       

                            if(inst == "push" || inst == "pop")
                                sveSekcije.povecajCounter(sekcija, 3);
                            else
                                sveSekcije.povecajCounter(sekcija, 2);


                            odradjena = true;

                            continue;
                        }
                        else
                        {
                            cout << "Greška: Neodgovarajući operand " << '"' << deo << '"' << " za instrukciju: " << inst << ", očekuje se registar. Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }
                    }
                    else
                    {
                        cout << "Greška: Izostavljen operand za instrukciju: " << inst << ". Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }
                }

                if (deo == "xchg" || deo == "add" || deo == "sub" || deo == "mul" || deo == "div" || deo == "cmp" || deo == "and" || deo == "or" || deo == "xor" || deo == "test" || deo == "shl" || deo == "shr")
                {

                    if (linija >> deo)
                    {

                        if (deo[deo.length() - 1] == ',')
                            deo = deo.substr(0, deo.length() - 1);

                        if (deo == "r0" || deo == "r1" || deo == "r2" || deo == "r3" || deo == "r4" || deo == "r5" || deo == "r6" || deo == "r7" || deo == "pc" || deo == "sp" || deo == "psw")
                        {

                            if (linija >> deo)
                            {

                                if (deo == "r0" || deo == "r1" || deo == "r2" || deo == "r3" || deo == "r4" || deo == "r5" || deo == "r6" || deo == "r7" || deo == "pc" || deo == "sp" || deo == "psw")
                                {

                                    sveSekcije.povecajCounter(sekcija, 2);

                                    odradjena = true;

                                    continue;
                                }
                                else
                                {
                                    cout << "Greška: Neodgovarajući drugi operand " << '"' << deo << '"' << " za instrukciju: " << inst << ", očekuje se registar. Na liniji broj: " << brLinije << ".\n";
                                    greska = true;
                                    break;
                                }
                            }
                            else
                            {
                                cout << "Greška: Izostavljen drugi operand za instrukciju: " << inst << ". Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }
                        }
                        else
                        {
                            cout << "Greška: Neodgovarajući prvi operand " << '"' << deo << '"' << " za instrukciju: " << inst << ", očekuje se registar. Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }
                    }
                    else
                    {
                        cout << "Greška: Izostavljen prvi operand za instrukciju: " << inst << ". Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }
                }

                if (deo == "call" || deo == "jmp" || deo == "jeq" || deo == "jne" || deo == "jgt")
                {

                    string operand;

                    if (linija >> operand)
                    {

                        regex regBr("[0-9]+");

                        regex regBrMem("\\*[0-9]+");

                        regex regHex("0[xX][0-9a-fA-F]+");

                        regex regHexMem("\\*0[xX][0-9a-fA-F]+");

                        if (regex_match(operand, regBr) || regex_match(operand, regBrMem) || regex_match(operand, regHex) || regex_match(operand, regHexMem))
                        {

                            sveSekcije.povecajCounter(sekcija, 5);

                            odradjena = true;

                            continue;
                        }

                        if (operand == "*r0" || operand == "*r1" || operand == "*r2" || operand == "*r3" || operand == "*r4" || operand == "*r5" || operand == "*r6" || operand == "*r7" || operand == "*pc" || operand == "*sp" || operand == "*psw")
                        {

                            
                            sveSekcije.povecajCounter(sekcija, 3);

                            odradjena = true;

                            continue;
                        }

                        if (operand == "*[r0]" || operand == "*[r1]" || operand == "*[r2]" || operand == "*[r3]" || operand == "*[r4]" || operand == "*[r5]" || operand == "*[r6]" || operand == "*[r7]" || operand == "*[pc]" || operand == "*[sp]" || operand == "*[psw]")
                        {

                            sveSekcije.povecajCounter(sekcija, 3);

                            odradjena = true;

                            continue;
                        }

                        regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");
                        regex simbolMem("^[%,*][a-zA-Z][a-zA-Z0-9_]*$");

                        if ((regex_match(operand, simbol) || regex_match(operand, simbolMem))  && (operand != "r0" && operand != "r1" && operand != "r2" && operand != "r3" && operand != "r4" && operand != "r5" && operand != "r6" && operand != "r7" && operand != "pc" && operand != "sp" && operand != "psw"))
                        {

                            sveSekcije.povecajCounter(sekcija, 5);

                            odradjena = true;

                            continue;
                        }

                        if (operand[0] == '*' && operand[1] == '[')
                        {
                            if (operand.substr(2) == "r0" || operand.substr(2) == "r1" || operand.substr(2) == "r2" || operand.substr(2) == "r3" || operand.substr(2) == "r4" || operand.substr(2) == "r5" || operand.substr(2) == "r6" || operand.substr(2) == "r7" || operand.substr(2) == "pc" || operand.substr(2) == "sp" || operand.substr(2) == "psw")
                            {

                                linija >> deo;

                                operand += " " + deo + " ";

                                if (deo == "+")
                                {

                                    linija >> deo;

                                    string provera = deo.substr(0, deo.length() - 1);

                                    operand += deo;

                                    if ((regex_match(deo.substr(0, deo.length() - 1), regBr) || regex_match(deo.substr(0, deo.length() - 1), regHex) || regex_match(deo.substr(0, deo.length() - 1), simbol)) && (provera != "r0" && provera != "r1" && provera != "r2" && provera != "r3" && provera != "r4" && provera != "r5" && provera != "r6" && provera != "r7" && provera != "pc" && provera != "sp" && provera != "psw"))
                                    {

                                        if (deo[deo.length() - 1] == ']')
                                        {

                                            sveSekcije.povecajCounter(sekcija, 5);

                                            odradjena = true;

                                            continue;
                                        }
                                    }
                                }
                            }
                        }

                        cout << "Greška: Neodgovarajući operand " << '"' << operand << '"' << " za instrukciju: " << inst << ", očekuje se neki od dozvoljenih operanada. Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }
                    else
                    {
                        cout << "Greška: Izostavljen operand za instrukciju: " << inst << ". Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }

                    break;
                }
                if (deo == "ldr" || deo == "str")
                {

                    bool storeJe = false;
                    if (deo == "str")
                        storeJe = true;

                    if (linija >> deo)
                    {

                        if (deo[deo.length() - 1] == ',')
                            deo = deo.substr(0, deo.length() - 1);

                        if (deo == "r0" || deo == "r1" || deo == "r2" || deo == "r3" || deo == "r4" || deo == "r5" || deo == "r6" || deo == "r7" || deo == "pc" || deo == "sp" || deo == "psw")
                        {

                            string operand;

                            if (linija >> operand)
                            {

                                regex regBr("[0-9]+");

                                regex regBrMem("\\$[0-9]+");

                                regex regHex("0[xX][0-9a-fA-F]+");

                                regex regHexMem("\\$0[xX][0-9a-fA-F]+");

                                if (regex_match(operand, regBr) || regex_match(operand, regBrMem) || regex_match(operand, regHex) || regex_match(operand, regHexMem))
                                {

                                    if (storeJe == true)
                                    {
                                        cout << "Greška: Neodgovarajući drugi " << '"' << operand << '"' << " za instrukciju: " << inst << ", literal ne može biti odredište. Na liniji broj: " << brLinije << ".\n";
                                        greska = true;
                                        break;
                                    }

                                    sveSekcije.povecajCounter(sekcija, 5);

                                    odradjena = true;

                                    continue;
                                }

                                if (operand == "r0" || operand == "r2" || operand == "r3" || operand == "r4" || operand == "r5" || operand == "r6" || operand == "r7" || operand == "pc" || operand == "sp" || operand == "psw")
                                {

                                    
                                    sveSekcije.povecajCounter(sekcija, 3);

                                    odradjena = true;

                                    continue;
                                }

                                if (operand == "[r0]" || operand == "[r1]" || operand == "[r2]" || operand == "[r3]" || operand == "[r4]" || operand == "[r5]" || operand == "[r6]" || operand == "[r7]" || operand == "[pc]" || operand == "[sp]" || operand == "[psw]")
                                {

                                    sveSekcije.povecajCounter(sekcija, 3);
                        

                                    odradjena = true;

                                    continue;
                                }

                                regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");
                                regex simbolMem("^[%,$][a-zA-Z][a-zA-Z0-9_]*$");

                                if (regex_match(operand, simbol) || regex_match(operand, simbolMem))
                                {

                                    sveSekcije.povecajCounter(sekcija, 5);

                                    odradjena = true;

                                    continue;
                                }

                                if (operand[0] == '[')
                                {
                                    if (operand.substr(1) == "r0" || operand.substr(1) == "r1" || operand.substr(1) == "r2" || operand.substr(1) == "r3" || operand.substr(1) == "r4" || operand.substr(1) == "r5" || operand.substr(1) == "r6" || operand.substr(1) == "r7" || operand.substr(1) == "pc" || operand.substr(1) == "sp" || operand.substr(1) == "psw")
                                    {

                                        linija >> deo;

                                        operand += " " + deo + " ";

                                        if (deo == "+")
                                        {

                                            linija >> deo;

                                            string provera = deo.substr(0, deo.length() - 1);

                                            operand += deo;

                                            if ((regex_match(deo.substr(0, deo.length() - 1), regBr) || regex_match(deo.substr(0, deo.length() - 1), regHex) || regex_match(deo.substr(0, deo.length() - 1), simbol)) && (provera != "r0" && provera != "r1" && provera != "r2" && provera != "r3" && provera != "r4" && provera != "r5" && provera != "r6" && provera != "r7" && provera != "pc" && provera != "sp" && provera != "psw"))
                                            {



                                                if (deo[deo.length() - 1] == ']')
                                                {

                                                    sveSekcije.povecajCounter(sekcija, 5);

                                                    odradjena = true;

                                                    continue;
                                                }
                                            }
                                        }
                                    }
                                }

                                cout << "Greška: Neodgovarajući drugi operand " << '"' << operand << '"' << " za instrukciju: " << inst << ", očekuje se neki od dozvoljenih operanada. Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }
                            else
                            {
                                cout << "Greška: Izostavljen drugi operand za instrukciju: " << inst << ". Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }
                        }
                        else
                        {
                            cout << "Greška: Neodgovarajući prvi operand " << '"' << deo << '"' << " za instrukciju: " << inst << ", očekuje se registar. Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }
                    }
                    else
                    {
                        cout << "Greška: Izostavljen prvi operand za instrukciju: " << inst << ". Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }
                }
            }

            regex labela("^[a-zA-Z][a-zA-Z0-9_]*$");

            if (regex_match(deo.substr(0, deo.length() - 1), labela) && deo[deo.length() - 1] == ':')
            {

                if (labelaJe == true)
                {
                    cout << "Greška: Jedna linija izvornog koda mora da sadrži najviše jednu asemblersku naredbu ili direktivu. Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                Simbol novi(deo.substr(0, deo.length() - 1), sekcija, sveSekcije.offset(sekcija), false);

                if(!tabSimbola.dodajSimbol(novi)) {
                    cout << "Greška: Ponovna definicija simbola: "<< deo.substr(0, deo.length() - 1) <<" . Na liniji broj: " << brLinije << ".\n";
                    greska = true;
                    break;
                }

                labelaJe = true;

                continue;
            }

            cout << "Greška: Nepoznato " << '"' << deo << '"' << ". Na liniji broj: " << brLinije << ".\n";

            greska = true;

            break;
        }

        if (kraj == true)
            break;
    }


    if(greska == true)
        return 1;
    else
        return 0;
}

void Asembler::drugiProlaz(string ulaz, string izlaz)
{

    ifstream ulazni(ulaz);

    string lin;

    string deo;

    bool kraj = false;

    bool greska = false;

    int brLinije = 0;

    string sekcija;

    string nazivFajla;

    sveSekcije.anulirajCountere();

    getline(ulazni, lin);

    while (getline(ulazni, lin)) {

        string s = "";

        for(int i = 0; i < lin.length(); i++)
            if(lin[i] == '#')
                break;
            else
                s += lin[i];

        ++brLinije;

        lin = s;

        if (lin.length() == 0)
            continue;

        istringstream linija(lin);

        while (linija >> deo) {

            if (deo == ".end")
            {
                kraj = true;
                break;
            }

            if (deo == ".section")
            {
                linija >> deo;

                sekcija = deo;
                
                break;
            }

            if (deo == ".text" || deo == ".data" || deo == ".rodata" || deo == ".bss")
            {

                sekcija = deo.substr(1);

                break;
            }

            if (deo == ".global")
            {
                while (true)
                {
                    linija >> deo;

                    if(tabSimbola.provera(deo) == false) {
                        cout << "Greška: Uneti simbol nije definisan niti uvezen: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                        greska = true;
                        break;
                    }

                    if (deo.substr(deo.length() - 1) != ",")
                    {
                        tabSimbola.postaviGlobal(deo);

                        break;
                    }

                    tabSimbola.postaviGlobal(deo);
                }

                break;
            }

            if (deo == ".word")
            {

                regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");

                regex regBr("[0-9]+");

                regex regHex("0[xX][0-9a-fA-F]+");

                int br = 0;

                while (linija >> deo)
                {

                    string sadrzaj = "";
                    string konacni = "";


                    if(deo[deo.length() - 1] == ',')
                        deo = deo.substr(0, deo.length() - 1);

                    if(regex_match(deo, simbol)) {

                        if(tabSimbola.provera(deo) == false) {
                            cout << "Greška: Uneti simbol nije definisan niti uvezen: "<< deo <<" . Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }

                        if(tabSimbola.sekcijaSimbola(deo) == "n/a") {

                            RelZapis novi(sekcija, sveSekcije.offset(sekcija), 'A', to_string(tabSimbola.redniBrSimbola(deo)));

                            tabRelZapisa.dodajZapis(novi);
                            
                            br = 0;
                        } else {

                            if(tabSimbola.sekcijaSimbola(deo) != "abs") {
                                
                                RelZapis novi(sekcija, sveSekcije.offset(sekcija), 'A', to_string(tabSimbola.redniBrSimbola(sekcija)));

                                tabRelZapisa.dodajZapis(novi);
                            }

                            stringstream s(tabSimbola.offsetSimbola(deo));

                            s >> br;
                        }
                    }
                    else {
                        
                        if (regex_match(deo, regBr))
                        {
                            stringstream s(deo);
                            s >> br;
                        }
                        else if (regex_match(deo, regHex))
                        {
                            stringstream s;
                            s << hex << deo;
                            s >> br;
                        } 
                    }
                    
                    std::stringstream stream;

                    stream << std::hex << br;

                    std::string broj(stream.str());

                    if(broj.length() < 4) {
                        for(int i = 0; i < 4 - broj.length(); i++)
                        sadrzaj += "0";
                    }

                    sadrzaj += broj;

                    konacni += sadrzaj.substr(2);

                    konacni += " ";
                    
                    konacni += sadrzaj.substr(0, 2);

                    string offset = "";

                    offset += to_string(sveSekcije.offset(sekcija));
                    sveSekcije.povecajCounter(sekcija, 1);
                    offset += "-";
                    offset += to_string(sveSekcije.offset(sekcija));
                    sveSekcije.povecajCounter(sekcija, 1);

                    Sekcija nova(sekcija, offset, konacni);

                    sveSekcije.dodajSekciju(nova);
                }

                break;
            }
            if(deo == ".skip") {

                linija >> deo;

                int br = 0;

                regex regBr("[0-9]+");

                regex regHex("0[xX][0-9a-fA-F]+");

                if (regex_match(deo, regBr)) {

                    stringstream s(deo);
                    s >> br;
                }
                else if (regex_match(deo, regHex)) {

                    stringstream s;
                    s << hex << deo;
                    s >> br;
                }

                if(br == 0)
                    break;

                string offset = "";

                if(br > 1) {

                    offset += to_string(sveSekcije.offset(sekcija));
                    sveSekcije.povecajCounter(sekcija, br);
                    offset += "-";
                    offset += to_string(sveSekcije.offset(sekcija) - 1);

                }
                else {
                    offset += to_string(sveSekcije.offset(sekcija));
                    sveSekcije.povecajCounter(sekcija, br);
                }
                    

                string sadrzaj = "";

                for(int i = 0; i < br; i++)
                    if(i == 0)
                        sadrzaj += "00";
                    else
                        sadrzaj += " 00";

                Sekcija nova(sekcija, offset, sadrzaj);

                sveSekcije.dodajSekciju(nova);

                break;

            }
            if(deo == ".equ") {
                
                break;
            }
            if (deo == "halt" || deo == "int" || deo == "iret" || deo == "call" || deo == "ret" || deo == "jmp" || deo == "jeq" || deo == "jne" || deo == "jgt" || deo == "push" || deo == "pop" || deo == "xchg" || deo == "add" || deo == "sub" || deo == "mul" || deo == "div" || deo == "cmp" || deo == "not" || deo == "and" || deo == "or" || deo == "xor" || deo == "test" || deo == "shl" || deo == "shr" || deo == "ldr" || deo == "str")
            {

                string offset = "";
                string sadrzaj = "";

                if (deo == "halt" || deo == "iret" || deo == "ret")
                {

                    offset += to_string(sveSekcije.offset(sekcija));
                    sveSekcije.povecajCounter(sekcija, 1);      

                    if(deo == "halt")
                        sadrzaj += "00";
                    else if(deo == "iret")
                        sadrzaj += "20";
                    else if(deo == "ret")
                        sadrzaj += "40";

                    Sekcija nova(sekcija, offset, sadrzaj);

                    sveSekcije.dodajSekciju(nova);            

                    break;
                }
                if (deo == "call" || deo == "jmp" || deo == "jeq" || deo == "jne" || deo == "jgt") {

                    regex regBr("[0-9]+");

                    regex regBrMem("\\*[0-9]+");

                    regex regHex("0[xX][0-9a-fA-F]+");

                    regex regHexMem("\\*0[xX][0-9a-fA-F]+");

                    regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");
        
                    regex simbolMem("^[%,*][a-zA-Z][a-zA-Z0-9_]*$");        

                    string operand;

                    linija >> operand;
                    
                    string offset = "";
                    
                    string sadrzaj = "";

                    string konacni = "";

                    if(deo == "call")  
                        sadrzaj += "30";
                    else if(deo == "jmp")
                        sadrzaj += "50";
                    else if(deo == "jeq")
                        sadrzaj += "51";
                    else if(deo == "jne")
                        sadrzaj += "52";                         
                     else if(deo == "jgt")
                        sadrzaj += "53";

                    if((regex_match(operand, simbol) || regex_match(operand, simbolMem)) && (operand != "*r0" && operand != "*r1" && operand != "*r2" && operand != "*r3" && operand != "*r4" && operand != "*r5" && operand != "*r6" && operand != "*r7" && operand != "*pc" && operand != "*sp" && operand != "*psw")) { // za simbole

                        if(regex_match(operand, simbol)) {// valjda ovo treba 
                            
                            if(tabSimbola.provera(operand) == false) {
                                cout << "Greška: Uneti simbol nije definisan niti uvezen: "<< operand <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }

                            if(tabSimbola.sekcijaSimbola(operand) == "abs") {
                                cout << "Greška: Simbol definisan .equ direktivom ne može se koristiti adresiranjem: "<< operand <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }
                            
                            sadrzaj += " f9 00";
                        }
                        else {

                            if(tabSimbola.provera(operand.substr(1)) == false) {
                                cout << "Greška: Uneti simbol nije definisan niti uvezen: "<< operand.substr(1) <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }

                            

                            if(operand[0] == '*')
                                sadrzaj += " f9 04";
                            else if(operand[0] == '%') {

                                if(tabSimbola.sekcijaSimbola(operand.substr(1)) == "abs") {
                                    cout << "Greška: Simbol definisan .equ direktivom ne može se koristiti adresiranjem: "<< operand.substr(1) <<" . Na liniji broj: " << brLinije << ".\n";
                                    greska = true;
                                    break;
                                }

                                sadrzaj += " f9 00";
                            }
                        }    
                            

                        if(regex_match(operand, simbol) || (regex_match(operand, simbolMem) && operand[0] == '*')) { // apsolutno adresiranje

                                if(operand[0] == '*')
                                    operand = operand.substr(1, operand.length() - 1);
                                
                                offset += to_string(sveSekcije.offset(sekcija));
                                sveSekcije.povecajCounter(sekcija, 5);
                                offset += "-";
                                offset += to_string(sveSekcije.offset(sekcija) - 1);

                                int br = 0;

                                stringstream s(tabSimbola.offsetSimbola(operand));
                                
                                s >> br;

                                std::stringstream stream;

                                stream << std::hex << br;

                                std::string broj(stream.str());

                                string broj1;

                                if(broj.length() < 4) {
                                    for(int i = 0; i < 4 - broj.length(); i++) 
                                        broj1 += "0";
                                    for(int i = 0; i < broj.length(); i++)
                                        broj1 += broj[i];
                                }
                                else {
                                    broj1 = broj.substr(broj.length() - 4);
                                }

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(0, 2);

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(2);


                                Sekcija nova(sekcija, offset, sadrzaj);

                                sveSekcije.dodajSekciju(nova); 

                                int pozicija = sveSekcije.offset(sekcija) - 2;

                                char adresiranje = 'A';

                                string opis;

                                if(tabSimbola.sekcijaSimbola(operand) != "abs") {

                                    if(tabSimbola.sekcijaSimbola(operand) == "n/a")
                                        opis = to_string(tabSimbola.redniBrSimbola(operand));
                                    else
                                        opis = to_string(tabSimbola.redniBrSimbola(tabSimbola.sekcijaSimbola(operand)));

                                    RelZapis novi(sekcija, pozicija, adresiranje, opis);

                                    tabRelZapisa.dodajZapis(novi);
                                }

                                break;

                        }
                        else if(regex_match(operand, simbolMem)) { // ako je % ili * simbol

                            if(operand[0] == '%') { // pc relativno

                                operand = operand.substr(1, operand.length() - 1);


                                offset += to_string(sveSekcije.offset(sekcija));
                                sveSekcije.povecajCounter(sekcija, 5);
                                offset += "-";
                                offset += to_string(sveSekcije.offset(sekcija) - 1);

                                int offSimb = 0;

                                int br = sveSekcije.offset(sekcija);

                                stringstream s(tabSimbola.offsetSimbola(operand));

                                s >> offSimb;

                                offSimb -= br;

                                std::stringstream stream;

                                stream << std::hex << offSimb;

                                std::string broj(stream.str());

                                string broj1;

                                if(broj.length() < 4) {
                                    for(int i = 0; i < 4 - broj.length(); i++) 
                                        broj1 += "0";
                                    for(int i = 0; i < broj.length(); i++)
                                        broj1 += broj[i];
                                }
                                else {
                                    broj1 = broj.substr(broj.length() - 4);
                                }

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(0, 2);

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(2);


                                Sekcija nova(sekcija, offset, sadrzaj);

                                sveSekcije.dodajSekciju(nova);

                                if(tabSimbola.sekcijaSimbola(operand) != sekcija && tabSimbola.sekcijaSimbola(operand) != "abs") { // onda treba da se pravi relokacioni

                                    int pozicija = sveSekcije.offset(sekcija) - 2;

                                    char adresiranje = 'R';

                                    string opis;

                                    if(tabSimbola.sekcijaSimbola(operand) == "n/a")
                                        opis = to_string(tabSimbola.redniBrSimbola(operand));
                                    else
                                        opis = to_string(tabSimbola.redniBrSimbola(tabSimbola.sekcijaSimbola(operand)));

                                    RelZapis novi(sekcija, pozicija, adresiranje, opis);

                                    tabRelZapisa.dodajZapis(novi);
                                }

                                break;

                            } // kraj ifa za %

                        } // kraj ifa za simbolMem

                    } // kraj ifa za simbol sve ukupno

                    if(regex_match(operand, regBr) || regex_match(operand, regBrMem) || regex_match(operand, regHex) || regex_match(operand, regHexMem)) { // literal
                        
                        if(regex_match(operand, regBr) || regex_match(operand, regHex)) {
                            sadrzaj += " f9 00";
                        }
                        else if(regex_match(operand, regBrMem) || regex_match(operand, regHexMem)) {
                            sadrzaj += " f9 04";
                            operand = operand.substr(1, operand.length() - 1);
                        }

                        offset += to_string(sveSekcije.offset(sekcija));
                        sveSekcije.povecajCounter(sekcija, 5);
                        offset += "-";
                        offset += to_string(sveSekcije.offset(sekcija) - 1);

                        int br = 0;

                        if (regex_match(operand, regBr) || regex_match(operand, regBrMem)) {

                            stringstream s(operand);
                            s >> br;
                        }
                        else if (regex_match(operand, regHex) || regex_match(operand, regHexMem)) {

                            stringstream s;
                            s << hex << operand;
                            s >> br;
                        } 

                        std::stringstream stream;

                        stream << std::hex << br;

                        std::string broj(stream.str());

                        string broj1;

                        if(broj.length() < 4) {
                            for(int i = 0; i < 4 - broj.length(); i++) 
                                broj1 += "0";
                            for(int i = 0; i < broj.length(); i++)
                                broj1 += broj[i];
                        }
                        else 
                            broj1 = broj.substr(broj.length() - 4);
                        
                        sadrzaj += " ";

                        sadrzaj += broj1.substr(0, 2);

                        sadrzaj += " ";

                        sadrzaj += broj1.substr(2);

                        Sekcija nova(sekcija, offset, sadrzaj);

                        sveSekcije.dodajSekciju(nova);

                        break;
                    
                    } // kraj ifa za literale

                    if ((operand == "*r0" || operand == "*r1" || operand == "*r2" || operand == "*r3" || operand == "*r4" || operand == "*r5" || operand == "*r6" || operand == "*r7" || operand == "*pc" || operand == "*sp" || operand == "*psw")
                        || (operand == "*[r0]" || operand == "*[r1]" || operand == "*[r2]" || operand == "*[r3]" || operand == "*[r4]" || operand == "*[r5]" || operand == "*[r6]" || operand == "*[r7]" || operand == "*[pc]" || operand == "*[sp]" || operand == "*[psw]")) {
        

                        string reg;

                        if(operand[1] != '[')
                            reg = operand.substr(1, operand.length() -1);
                        else
                            reg = operand.substr(2, operand.length() - 3);

                        sadrzaj += " ";

                        if(reg == "r0")
                            sadrzaj += "f0";
                        else if(reg == "r1")
                            sadrzaj += "f1";
                        else if(reg == "r2")
                            sadrzaj += "f2";
                        else if(reg == "r3")
                            sadrzaj += "f3";
                        else if(reg == "r4")
                            sadrzaj += "f4";
                        else if(reg == "r5")
                            sadrzaj += "f5";
                        else if(reg == "r6" || reg == "sp")
                            sadrzaj += "f6";
                        else if(reg == "r7" || reg == "pc")
                            sadrzaj += "f7";
                        else if(reg == "psw")
                            sadrzaj += "f8";

                        sadrzaj += " ";
                        
                        if(operand[1] != '[')
                            sadrzaj += "01";
                        else
                            sadrzaj += "02";

                        offset += to_string(sveSekcije.offset(sekcija));
                        sveSekcije.povecajCounter(sekcija, 3);
                        offset += "-";
                        offset += to_string(sveSekcije.offset(sekcija) - 1);

                        Sekcija nova(sekcija, offset, sadrzaj);

                        sveSekcije.dodajSekciju(nova);

                        break;

                    } // kraj ifa za registre

                    if(operand[0] == '*' && operand[1] == '[') {

                        string reg = operand.substr(2, operand.length() -2);

                        linija >> deo; // +

                        linija >> deo;

                        operand = deo.substr(0, deo.length() - 1);

                        sadrzaj += " ";

                        if(reg == "r0")
                            sadrzaj += "f0";
                        else if(reg == "r1")
                            sadrzaj += "f1";
                        else if(reg == "r2")
                            sadrzaj += "f2";
                        else if(reg == "r3")
                            sadrzaj += "f3";
                        else if(reg == "r4")
                            sadrzaj += "f4";
                        else if(reg == "r5")
                            sadrzaj += "f5";
                        else if(reg == "r6" || reg == "sp")
                            sadrzaj += "f6";
                        else if(reg == "r7" || reg == "pc")
                            sadrzaj += "f7";
                        else if(reg == "psw")
                            sadrzaj += "f8";

                        sadrzaj += " ";

                        sadrzaj += "03";

                        offset += to_string(sveSekcije.offset(sekcija));
                        sveSekcije.povecajCounter(sekcija, 5);
                        offset += "-";
                        offset += to_string(sveSekcije.offset(sekcija) - 1);

                        
                        if(regex_match(operand, regBr) || regex_match(operand, regHex)) { // literal u onom cudu

                            int br = 0;

                            if (regex_match(operand, regBr) || regex_match(operand, regBrMem)) {

                                stringstream s(operand);
                                s >> br;
                            }
                            else if (regex_match(operand, regHex) || regex_match(operand, regHexMem)) {

                                stringstream s;
                                s << hex << operand;
                                s >> br;
                            } 

                            std::stringstream stream;

                            stream << std::hex << br;

                            std::string broj(stream.str());

                            string broj1;

                            if(broj.length() < 4) {
                                for(int i = 0; i < 4 - broj.length(); i++) 
                                    broj1 += "0";
                                for(int i = 0; i < broj.length(); i++)
                                    broj1 += broj[i];
                            }
                            else 
                                broj1 = broj.substr(broj.length() - 4);

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(0, 2);

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(2);

                            Sekcija nova(sekcija, offset, sadrzaj);

                            sveSekcije.dodajSekciju(nova);

                            break;
                        
                        } // kraj ifa za literale u onom cudu
                        else if(regex_match(operand, simbol)) { // simbol u onom cudu

                            if(tabSimbola.provera(operand) == false) {
                                cout << "Greška: Uneti simbol nije definisan niti uvezen: "<< operand <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }

                            if(tabSimbola.sekcijaSimbola(operand) == "abs") {
                                cout << "Greška: Simbol definisan .equ direktivom ne može se koristiti adresiranjem: "<< operand <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }

                            int br = 0;

                            stringstream s(tabSimbola.offsetSimbola(operand));
                            
                            s >> br;

                            std::stringstream stream;

                            stream << std::hex << br;

                            std::string broj(stream.str());

                            string broj1;

                            if(broj.length() < 4) {
                                for(int i = 0; i < 4 - broj.length(); i++) 
                                    broj1 += "0";
                                for(int i = 0; i < broj.length(); i++)
                                    broj1 += broj[i];
                            }
                            else {
                                broj1 = broj.substr(broj.length() - 4);
                            }

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(0, 2);

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(2);

                            Sekcija nova(sekcija, offset, sadrzaj);

                            sveSekcije.dodajSekciju(nova); 

                            int pozicija = sveSekcije.offset(sekcija) - 2;

                            char adresiranje = 'A';

                            string opis;

                            if(tabSimbola.sekcijaSimbola(operand) != "abs") {

                                if(tabSimbola.sekcijaSimbola(operand) == "n/a")
                                    opis = to_string(tabSimbola.redniBrSimbola(operand));
                                else
                                    opis = to_string(tabSimbola.redniBrSimbola(tabSimbola.sekcijaSimbola(operand)));

                                RelZapis novi(sekcija, pozicija, adresiranje, opis);

                                tabRelZapisa.dodajZapis(novi);
                            }

                            break;
                        } // kraj simbola u onom cudu

                    } // kraj ifa za ono cudo
                        

                } // kraj ifa za skokove

                if(deo == "ldr" || deo == "str") {

                    regex regBr("[0-9]+");

                    regex regBrMem("\\$[0-9]+");

                    regex regHex("0[xX][0-9a-fA-F]+");

                    regex regHexMem("\\$0[xX][0-9a-fA-F]+");

                    regex simbol("^[a-zA-Z][a-zA-Z0-9_]*$");
        
                    regex simbolMem("^[%,$][a-zA-Z][a-zA-Z0-9_]*$");        

                    string operand;

                    string reg;

                    string regOdr = "";

                    string regIzv = "";

                    string aaaa;


                    linija >> reg;

                    reg = reg.substr(0, reg.length() - 1);

                    string sadrzaj = "";

                    string offset = "";

                    string konacni = "";


                
                    if(reg == "r0")
                        reg = "0";
                    else if(reg == "r1")
                        reg = "1";
                    else if(reg == "r2")
                        reg = "2";
                    else if(reg == "r3")
                        reg = "3";
                    else if(reg == "r4")
                        reg = "4";
                    else if(reg == "r5")
                        reg = "5";
                    else if(reg == "r6" || reg == "sp")
                        reg = "6";
                    else if(reg == "r7" || reg == "pc")
                        reg = "7";
                    else if(reg == "psw")
                        reg = "8";

                regOdr = reg;

                if(deo == "ldr") {
                    sadrzaj += "a0";
                }
                else if(deo == "str") {
                    sadrzaj += "b0";
                }


                operand = "";

                string s;

                while(linija >> s) {
                    operand += s;
                    operand += " ";
                }

                if(operand[operand.length() - 1] == ' ')
                    operand = operand.substr(0, operand.length() - 1);


                if((regex_match(operand, simbol) || regex_match(operand, simbolMem)) && (operand != "r0" && operand != "r1" && operand != "r2" && operand != "r3" && operand != "r4" && operand != "r5" && operand != "r6" && operand != "r7" && operand != "pc" && operand != "sp" && operand != "psw")) { // za simbole

                        if(tabSimbola.provera(operand) == false && tabSimbola.provera(operand.substr(1)) == false) {
                            if(operand[0] == '%' || operand[0] == '$')
                                operand = operand.substr(1);
                            cout << "Greška: Uneti simbol nije definisan niti uvezen: "<< operand <<" . Na liniji broj: " << brLinije << ".\n";
                            greska = true;
                            break;
                        }

                
                        if(regex_match(operand, simbolMem)) {
                            if(operand[0] == '%' && tabSimbola.sekcijaSimbola(operand.substr(1)) == "abs") {
                                cout << "Greška: Simbol definisan .equ direktivom ne može se koristiti adresiranjem: "<< operand.substr(1) <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }
                        }
                        
                        
                        offset += to_string(sveSekcije.offset(sekcija));
                        sveSekcije.povecajCounter(sekcija, 5);
                        offset += "-";
                        offset += to_string(sveSekcije.offset(sekcija) - 1);
                        

                        regIzv = "9";

                        
                        
                        sadrzaj += " ";
                        sadrzaj += regOdr;
                        sadrzaj += regIzv;

                        if(regex_match(operand, simbol) || (regex_match(operand, simbolMem) && operand[0] == '$')) { // apsolutno adresiranje

                                string op = operand;
                                
                                if(operand[0] == '$')
                                    op = operand.substr(1, operand.length() - 1);


                                int br = 0;

                                stringstream s(tabSimbola.offsetSimbola(op));
                                
                                s >> br;

                                std::stringstream stream;

                                stream << std::hex << br;

                                std::string broj(stream.str());

                                string broj1;

                                if(broj.length() < 4) {
                                    for(int i = 0; i < 4 - broj.length(); i++) 
                                        broj1 += "0";
                                    for(int i = 0; i < broj.length(); i++)
                                        broj1 += broj[i];
                                }
                                else {
                                    broj1 = broj.substr(broj.length() - 4);
                                }

                            
                                sadrzaj += " ";
                                sadrzaj += "0"; // uuuu
                                if(regex_match(operand, simbol))
                                    sadrzaj += "4"; 
                                else if(regex_match(operand, simbolMem))
                                    sadrzaj += "0"; 

                                operand = op;

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(0, 2);

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(2);


                                Sekcija nova(sekcija, offset, sadrzaj);

                                sveSekcije.dodajSekciju(nova); 

                                int pozicija = sveSekcije.offset(sekcija) - 2;

                                char adresiranje = 'A';

                                string opis;

                                if(tabSimbola.sekcijaSimbola(operand) != "abs") {

                                    if(tabSimbola.sekcijaSimbola(operand) == "n/a")
                                        opis = to_string(tabSimbola.redniBrSimbola(operand));
                                    else
                                        opis = to_string(tabSimbola.redniBrSimbola(tabSimbola.sekcijaSimbola(operand)));

                                    RelZapis novi(sekcija, pozicija, adresiranje, opis);

                                    tabRelZapisa.dodajZapis(novi);
                                }

                                break;

                        }
                        else if(regex_match(operand, simbolMem)) { // ako je % ili * simbol

                            if(operand[0] == '%') { // pc relativno

                                operand = operand.substr(1, operand.length() - 1);


                                int offSimb = 0;

                                int br = sveSekcije.offset(sekcija);

                                stringstream s(tabSimbola.offsetSimbola(operand));

                                s >> offSimb;

                                offSimb -= br;

                                std::stringstream stream;

                                stream << std::hex << offSimb;

                                std::string broj(stream.str());

                                string broj1;

                                if(broj.length() < 4) {
                                    for(int i = 0; i < 4 - broj.length(); i++) 
                                        broj1 += "0";
                                    for(int i = 0; i < broj.length(); i++)
                                        broj1 += broj[i];
                                }
                                else {
                                    broj1 = broj.substr(broj.length() - 4);
                                }

                                sadrzaj += " ";
                                sadrzaj += "0"; // uuuu
                                sadrzaj += "4"; 

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(0, 2);

                                sadrzaj += " ";

                                sadrzaj += broj1.substr(2);

                                Sekcija nova(sekcija, offset, sadrzaj);

                                sveSekcije.dodajSekciju(nova);

                                if(tabSimbola.sekcijaSimbola(operand) != sekcija && tabSimbola.sekcijaSimbola(operand) != "abs") { // onda treba da se pravi relokacioni

                                    int pozicija = sveSekcije.offset(sekcija) - 2;

                                    char adresiranje = 'R';

                                    string opis;

                                    if(tabSimbola.sekcijaSimbola(operand) == "n/a")
                                        opis = to_string(tabSimbola.redniBrSimbola(operand));
                                    else
                                        opis = to_string(tabSimbola.redniBrSimbola(tabSimbola.sekcijaSimbola(operand)));

                                    RelZapis novi(sekcija, pozicija, adresiranje, opis);

                                    tabRelZapisa.dodajZapis(novi);
                                }

                                break;

                        } // kraj ifa za %

                    } // kraj ifa za simbolMem

                } // kraj ifa za simbole sve

                if(regex_match(operand, regBr) || regex_match(operand, regHex) || regex_match(operand, regBrMem) || regex_match(operand, regHexMem)) {
                        
                        offset += to_string(sveSekcije.offset(sekcija));
                        sveSekcije.povecajCounter(sekcija, 5);
                        offset += "-";
                        offset += to_string(sveSekcije.offset(sekcija) - 1);
                    

                        regIzv = "9";

                        
                        sadrzaj += " ";
                        sadrzaj += regOdr;
                        sadrzaj += regIzv;
                        sadrzaj += " ";
                        
                        if(regex_match(operand, regBr) || regex_match(operand, regHex)) {
                            sadrzaj += "04";
                        }
                        else if(regex_match(operand, regBrMem) || regex_match(operand, regHexMem)) {
                            sadrzaj += "00";
                            operand = operand.substr(1, operand.length() - 1);
                        }

                        int br = 0;

                        if (regex_match(operand, regBr) || regex_match(operand, regBrMem)) { 

                            stringstream s(operand);
                            s >> br;
                        }
                        else if (regex_match(operand, regHex) || regex_match(operand, regHexMem)) { 

                            stringstream s;
                            s << hex << operand;
                            s >> br;
                        } 

                        std::stringstream stream;

                        stream << std::hex << br;

                        std::string broj(stream.str());

                        string broj1;

                        if(broj.length() < 4) {
                            for(int i = 0; i < 4 - broj.length(); i++) 
                                broj1 += "0";
                            for(int i = 0; i < broj.length(); i++)
                                broj1 += broj[i];
                        }
                        else 
                            broj1 = broj.substr(broj.length() - 4);
                        
                        sadrzaj += " ";

                        sadrzaj += broj1.substr(0, 2);

                        sadrzaj += " ";

                        sadrzaj += broj1.substr(2);

                        Sekcija nova(sekcija, offset, sadrzaj);

                        sveSekcije.dodajSekciju(nova);

                        break;
                    } // kraj ifa za literal

                    if ((operand == "r0" || operand == "r1" || operand == "r2" || operand == "r3" || operand == "r4" || operand == "r5" || operand == "r6" || operand == "r7" || operand == "pc" || operand == "sp" || operand == "psw")
                        || (operand == "[r0]" || operand == "[r1]" || operand == "[r2]" || operand == "[r3]" || operand == "[r4]" || operand == "[r5]" || operand == "[r6]" || operand == "[r7]" || operand == "[pc]" || operand == "[sp]" || operand == "[psw]")) {
                        

                        offset += to_string(sveSekcije.offset(sekcija));
                        sveSekcije.povecajCounter(sekcija, 3);
                        offset += "-";
                        offset += to_string(sveSekcije.offset(sekcija) - 1);
                        
                        if(operand[0] == '[')
                            reg = operand.substr(1, operand.length() - 2);
                        else
                            reg = operand;

                        if(reg == "r0")
                            reg = "0";
                        else if(reg == "r1")
                            reg = "1";
                        else if(reg == "r2")
                            reg = "2";
                        else if(reg == "r3")
                            reg = "3";
                        else if(reg == "r4")
                            reg = "4";
                        else if(reg == "r5")
                            reg = "5";
                        else if(reg == "r6" || reg == "sp")
                            reg = "6";
                        else if(reg == "r7" || reg == "pc")
                            reg = "7";
                        else if(reg == "psw")
                            reg = "8";


                        regIzv = reg;

                        sadrzaj += " ";

                        sadrzaj += regOdr;
                        sadrzaj += regIzv;

                        sadrzaj += " ";

                        sadrzaj += "0";
                        if(operand[0] != '[')
                            sadrzaj += "1";
                        else
                            sadrzaj += "2";

                        Sekcija nova(sekcija, offset, sadrzaj);

                        sveSekcije.dodajSekciju(nova);

                        break;

                    } // kraj ifa za registre

                    if(operand[0] == '[') { 

                        offset += to_string(sveSekcije.offset(sekcija));
                        sveSekcije.povecajCounter(sekcija, 5);
                        offset += "-";
                        offset += to_string(sveSekcije.offset(sekcija) - 1);

                        reg = "";

                        for(int i = 1; i < operand.length(); i++)
                            if(operand[i] == ' ')
                                break;
                            else
                                reg += operand[i];


                        string o = "";

                        for(int i = reg.length() + 4; i < operand.length(); i++)
                            if(operand[i] == ']')
                                break;
                            else
                                o += operand[i];

                        operand = o;

                        if(reg == "r0")
                            reg = "0";
                        else if(reg == "r1")
                            reg = "1";
                        else if(reg == "r2")
                            reg = "2";
                        else if(reg == "r3")
                            reg = "3";
                        else if(reg == "r4")
                            reg = "4";
                        else if(reg == "r5")
                            reg = "5";
                        else if(reg == "r6" || reg == "sp")
                            reg = "6";
                        else if(reg == "r7" || reg == "pc")
                            reg = "7";
                        else if(reg == "psw")
                            reg = "8";


                        regIzv = reg;

                        sadrzaj += " ";

                        sadrzaj += regOdr;
                        sadrzaj += regIzv;

                        sadrzaj += " ";

                        sadrzaj += "03"; 

                        
                        if(regex_match(operand, regBr) || regex_match(operand, regHex)) { // literal u onom cudu

                            int br = 0;

                            if (regex_match(operand, regBr) || regex_match(operand, regBrMem)) {

                                stringstream s(operand);
                                s >> br;
                            }
                            else if (regex_match(operand, regHex) || regex_match(operand, regHexMem)) {

                                stringstream s;
                                s << hex << operand;
                                s >> br;
                            } 

                            std::stringstream stream;

                            stream << std::hex << br;

                            std::string broj(stream.str());

                            string broj1;

                            if(broj.length() < 4) {
                                for(int i = 0; i < 4 - broj.length(); i++) 
                                    broj1 += "0";
                                for(int i = 0; i < broj.length(); i++)
                                    broj1 += broj[i];
                            }
                            else 
                                broj1 = broj.substr(broj.length() - 4);

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(0, 2);

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(2);

                            Sekcija nova(sekcija, offset, sadrzaj);

                            sveSekcije.dodajSekciju(nova);

                            break;
                        
                        } // kraj ifa za literale u onom cudu
                        else if(regex_match(operand, simbol)) { // simbol u onom cudu

                            if(tabSimbola.provera(operand) == false) {
                                cout << "Greška: Uneti simbol nije definisan niti uvezen: "<< operand <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }

                            if(tabSimbola.sekcijaSimbola(operand) == "abs") {
                                cout << "Greška: Simbol definisan .equ direktivom ne može se koristiti adresiranjem: "<< operand <<" . Na liniji broj: " << brLinije << ".\n";
                                greska = true;
                                break;
                            }

                            int br = 0;

                            stringstream s(tabSimbola.offsetSimbola(operand));
                            
                            s >> br;

                            std::stringstream stream;

                            stream << std::hex << br;

                            std::string broj(stream.str());

                            string broj1;

                            if(broj.length() < 4) {
                                for(int i = 0; i < 4 - broj.length(); i++) 
                                    broj1 += "0";
                                for(int i = 0; i < broj.length(); i++)
                                    broj1 += broj[i];
                            }
                            else {
                                broj1 = broj.substr(broj.length() - 4);
                            }

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(0, 2);

                            sadrzaj += " ";

                            sadrzaj += broj1.substr(2);

                            Sekcija nova(sekcija, offset, sadrzaj);

                            sveSekcije.dodajSekciju(nova); 

                            int pozicija = sveSekcije.offset(sekcija) - 2;

                            char adresiranje = 'A';

                            string opis;

                            if(tabSimbola.sekcijaSimbola(operand) != "abs") {

                                if(tabSimbola.sekcijaSimbola(operand) == "n/a")
                                    opis = to_string(tabSimbola.redniBrSimbola(operand));
                                else
                                    opis = to_string(tabSimbola.redniBrSimbola(tabSimbola.sekcijaSimbola(operand)));

                                RelZapis novi(sekcija, pozicija, adresiranje, opis);

                                tabRelZapisa.dodajZapis(novi);
                            }
                            break;
                        } // kraj simbola u onom cudu

                    } // kraj ifa za ono cudo

            } // kraj ifa za ldr i str

            if(deo == "int" || deo == "not") {

                string reg;

                linija >> reg;

                if(reg == "r0")
                     reg = "0";
                else if(reg == "r1")
                    reg = "1";
                else if(reg == "r2")
                    reg = "2";
                else if(reg == "r3")
                    reg = "3";
                else if(reg == "r4")
                    reg = "4";
                else if(reg == "r5")
                    reg = "5";
                else if(reg == "r6" || reg == "sp")
                    reg = "6";
                else if(reg == "r7" || reg == "pc")
                    reg = "7";
                else if(reg == "psw")
                    reg = "8";

                
                if(deo == "int")
                    sadrzaj = "10 " + reg + "f";
                else if(deo == "not")
                    sadrzaj = "80 " + reg + "9";

                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);
                offset += "-";
                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);

                Sekcija nova(sekcija, offset, sadrzaj);

                sveSekcije.dodajSekciju(nova);

                break;

            } // kraj ifa za int i not

            if(deo == "push" || deo == "pop") {

                if(deo == "push")
                    sadrzaj = "b0 ";
                else if(deo == "pop")
                    sadrzaj = "a0 ";

                string reg;

                linija >> reg;

                if(reg == "r0")
                     reg = "0";
                else if(reg == "r1")
                    reg = "1";
                else if(reg == "r2")
                    reg = "2";
                else if(reg == "r3")
                    reg = "3";
                else if(reg == "r4")
                    reg = "4";
                else if(reg == "r5")
                    reg = "5";
                else if(reg == "r6" || reg == "sp")
                    reg = "6";
                else if(reg == "r7" || reg == "pc")
                    reg = "7";
                else if(reg == "psw")
                    reg = "8";

                sadrzaj += reg + "6";

                if(deo == "push")
                    sadrzaj += " 12";
                else if(deo == "pop")
                    sadrzaj += " 42";
                

                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 2);
                offset += "-";
                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);

                Sekcija nova(sekcija, offset, sadrzaj);

                sveSekcije.dodajSekciju(nova);

                break;

            } // kraj ifa za push i pop

            if(deo == "xchg") {

                string regD;
                string regS;

                linija >> regD;

                regD = regD.substr(0, regD.length() - 1);

                linija >> regS;

                if(regD == "r0")
                     regD = "0";
                else if(regD == "r1")
                    regD = "1";
                else if(regD == "r2")
                    regD = "2";
                else if(regD == "r3")
                    regD = "3";
                else if(regD == "r4")
                    regD = "4";
                else if(regD == "r5")
                    regD = "5";
                else if(regD == "r6" || regD == "sp")
                    regD = "6";
                else if(regD == "r7" || regD == "pc")
                    regD = "7";
                else if(regD == "psw")
                    regD = "8";

                if(regS == "r0")
                     regS = "0";
                else if(regS == "r1")
                    regS = "1";
                else if(regS == "r2")
                    regS = "2";
                else if(regS == "r3")
                    regS = "3";
                else if(regS == "r4")
                    regS = "4";
                else if(regS == "r5")
                    regS = "5";
                else if(regS == "r6" || regS == "sp")
                    regS = "6";
                else if(regS == "r7" || regS == "pc")
                    regS = "7";
                else if(regS == "psw")
                    regS = "8";

                sadrzaj = "60 " + regD + regS;

                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);
                offset += "-";
                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1); 

                Sekcija nova(sekcija, offset, sadrzaj);

                sveSekcije.dodajSekciju(nova);

                break;

            } // kraj ifa za xchg

            if(deo == "add" || deo == "sub" || deo == "mul" || deo == "div" || deo == "cmp") {

                if(deo == "add")
                    sadrzaj = "70 ";
                else if(deo == "sub")
                    sadrzaj = "71 ";
                else if(deo == "mul")
                    sadrzaj = "72 ";
                else if(deo == "div")
                    sadrzaj = "73 ";
                else if(deo == "cmp")
                    sadrzaj = "74 ";


                string regD;
                string regS;

                linija >> regD;

                regD = regD.substr(0, regD.length() - 1);

                linija >> regS;

                if(regD == "r0")
                     regD = "0";
                else if(regD == "r1")
                    regD = "1";
                else if(regD == "r2")
                    regD = "2";
                else if(regD == "r3")
                    regD = "3";
                else if(regD == "r4")
                    regD = "4";
                else if(regD == "r5")
                    regD = "5";
                else if(regD == "r6" || regD == "sp")
                    regD = "6";
                else if(regD == "r7" || regD == "pc")
                    regD = "7";
                else if(regD == "psw")
                    regD = "8";

                if(regS == "r0")
                     regS = "0";
                else if(regS == "r1")
                    regS = "1";
                else if(regS == "r2")
                    regS = "2";
                else if(regS == "r3")
                    regS = "3";
                else if(regS == "r4")
                    regS = "4";
                else if(regS == "r5")
                    regS = "5";
                else if(regS == "r6" || regS == "sp")
                    regS = "6";
                else if(regS == "r7" || regS == "pc")
                    regS = "7";
                else if(regS == "psw")
                    regS = "8";

                sadrzaj += regD + regS;

                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);
                offset += "-";
                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1); 

                Sekcija nova(sekcija, offset, sadrzaj);

                sveSekcije.dodajSekciju(nova);

                break;

            } // kraj ifa za matematicke inst

            if(deo == "and" || deo == "or" || deo == "xor" || deo == "test") {

                if(deo == "and")
                    sadrzaj = "81 ";
                else if(deo == "or")
                    sadrzaj = "82 ";
                else if(deo == "xor")
                    sadrzaj = "83 ";
                else if(deo == "test")
                    sadrzaj = "84 ";

                string regD;
                string regS;

                linija >> regD;

                regD = regD.substr(0, regD.length() - 1);

                linija >> regS;

                if(regD == "r0")
                     regD = "0";
                else if(regD == "r1")
                    regD = "1";
                else if(regD == "r2")
                    regD = "2";
                else if(regD == "r3")
                    regD = "3";
                else if(regD == "r4")
                    regD = "4";
                else if(regD == "r5")
                    regD = "5";
                else if(regD == "r6" || regD == "sp")
                    regD = "6";
                else if(regD == "r7" || regD == "pc")
                    regD = "7";
                else if(regD == "psw")
                    regD = "8";

                if(regS == "r0")
                     regS = "0";
                else if(regS == "r1")
                    regS = "1";
                else if(regS == "r2")
                    regS = "2";
                else if(regS == "r3")
                    regS = "3";
                else if(regS == "r4")
                    regS = "4";
                else if(regS == "r5")
                    regS = "5";
                else if(regS == "r6" || regS == "sp")
                    regS = "6";
                else if(regS == "r7" || regS == "pc")
                    regS = "7";
                else if(regS == "psw")
                    regS = "8";

                sadrzaj += regD + regS;

                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);
                offset += "-";
                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);

                Sekcija nova(sekcija, offset, sadrzaj);

                sveSekcije.dodajSekciju(nova);

                break;

            } // kraj ifa za logicke inst

            if(deo == "shr" || deo == "shl") {

                if(deo == "shr")
                    sadrzaj = "91 ";
                else if(deo == "shl")
                    sadrzaj = "90 ";

                string regD;
                string regS;

                linija >> regD;

                regD = regD.substr(0, regD.length() - 1);

                linija >> regS;

                if(regD == "r0")
                     regD = "0";
                else if(regD == "r1")
                    regD = "1";
                else if(regD == "r2")
                    regD = "2";
                else if(regD == "r3")
                    regD = "3";
                else if(regD == "r4")
                    regD = "4";
                else if(regD == "r5")
                    regD = "5";
                else if(regD == "r6" || regD == "sp")
                    regD = "6";
                else if(regD == "r7" || regD == "pc")
                    regD = "7";
                else if(regD == "psw")
                    regD = "8";

                if(regS == "r0")
                     regS = "0";
                else if(regS == "r1")
                    regS = "1";
                else if(regS == "r2")
                    regS = "2";
                else if(regS == "r3")
                    regS = "3";
                else if(regS == "r4")
                    regS = "4";
                else if(regS == "r5")
                    regS = "5";
                else if(regS == "r6" || regS == "sp")
                    regS = "6";
                else if(regS == "r7" || regS == "pc")
                    regS = "7";
                else if(regS == "psw")
                    regS = "8";

                sadrzaj += regD + regS;

                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);
                offset += "-";
                offset += to_string(sveSekcije.offset(sekcija));
                sveSekcije.povecajCounter(sekcija, 1);

                Sekcija nova(sekcija, offset, sadrzaj);

                sveSekcije.dodajSekciju(nova);

                break;
            }

        } // kraj ifa za instrukcije

        
            regex labela("^[a-zA-Z][a-zA-Z0-9_]*$");

            if (regex_match(deo.substr(0, deo.length() - 1), labela) && deo[deo.length() - 1] == ':')
            {

                break;
            }
        }

        if(greska == true)
            break;

        if(kraj == true)
            break;
    }

    if(greska == false) {
        //tabSimbola.ispisi();
        //sveSekcije.ispisiSekcije();
        //tabRelZapisa.ispisi();

        ofstream izlazni(izlaz);

        izlazni << tabSimbola;
        izlazni << sveSekcije;
        izlazni << tabRelZapisa;

    }
}
