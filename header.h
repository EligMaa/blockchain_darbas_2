#ifndef v01_H
#define v01_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
#include <array>
#include <sstream>
#include <random>
#include <chrono>
#include <cctype>  // isupper()
#include <unordered_set> 
#include <unordered_map>




using namespace std;
const std::vector<std::string> failai {"testasA.txt" , "testasB.txt" , "testasAts1.txt" , "testasAts2.txt" ,
 "testas_v_A.txt" , "testas_v_B.txt", "testas_tuscias.txt", "konstitucija.txt"};


void ivedimas ( );
void atsitiktiniu_eiluciu_poros ();
int bitu_lyginimas(const string& hash1, const string& hash2);
string hex_i_binaru(const string& hex);
int hex_lyginimas(const string& hash1, const string& hash2);
void lavinos_failas();

void vartotoju_generavimas(int dydids);



class nuu{

    public:
        nuu();
        nuu(int nr);
        inline ~nuu() {}

        string skaitymas (const string &ivestis);
        double laiko_matavimas( string& ivestis);
        void tikrinti_hash_kolizijas();
        void lavinos_testavimas();
        string hash (string &ivestis );


    private:
        int nr;
        string ivestis;
        string uzhashuotas;
        int ilgis;
        // string hash (string &ivestis );
        string konvertavimas(const array<unsigned long long, 4>& hash);
        
};
nuu::nuu(){
}

class vartotojas{

    public:
        nuu nuu;

        vartotojas() : viesasisis_raktas(""), valiutos_balansas(0), vardas("") {}
        vartotojas(int valiutos_balansas, string vardas) 
            : viesasisis_raktas(nuu.hash(vardas)), 
            valiutos_balansas(valiutos_balansas), vardas(vardas) {}

        inline ~vartotojas() {}

        string viesasisis_raktas;
        int valiutos_balansas;
        string vardas;   
        
};


#endif
