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

        string get_viesasisis_raktas() const { return viesasisis_raktas; }


    private:
        string viesasisis_raktas;
        int valiutos_balansas;
        string vardas;   
        
};

class transakcija{

    public:

        nuu nuu;
      
        transakcija() : siuntejas(""), gavejas(""), suma(0), ID(""){}
        transakcija( string siuntejas, string gavejas, int suma) 
            : siuntejas(siuntejas), gavejas(gavejas), suma(suma)
            {  string ivestis = siuntejas + gavejas + to_string(suma);
                ID = (nuu.hash(ivestis)) ;}

        inline ~transakcija() {}

    private:
        string ID;
        string siuntejas;
        string gavejas;
        int suma;
       
        
};

class blokai{

    public:

        nuu nuu;
      
        blokai() ;
        blokai(const vector<transakcija>& transakcijos, const string& pries_blokas, 
            const string& versija, const string& merkel_root, int sudetingumas):
            transakcijos(transakcijos),
            pries_blokas(pries_blokas),
            versija(versija),
            merkel_root(merkel_root),
            sudetingumas(sudetingumas),
            nonce(0)
        {
            auto now = chrono::system_clock::now();
            auto time_t_now = chrono::system_clock::to_time_t(now);
            laikas = ctime(&time_t_now);
            laikas.pop_back(); // Remove newline character from ctime result
        }

        string bloku_kasimas(){
            string ivestis = pries_blokas + laikas + versija + merkel_root + to_string(nonce);
            string hash = nuu.hash(ivestis);
            string ieskomas_hash = hash.substr(0, sudetingumas);

            while (ieskomas_hash != hash.substr(0, sudetingumas)){
                ++nonce;
                ivestis = pries_blokas + laikas + versija + merkel_root + to_string(nonce);
                hash = nuu.hash(ivestis);
                
            }
            
            return hash;
        
        }

        inline ~blokai() {}


    private:
        vector<transakcija> transakcijos;
        string pries_blokas;
        string laikas;
        string versija;
        string merkel_root;
        int nonce;
        int sudetingumas;
       
        
};

#endif
