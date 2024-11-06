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


class UTXO {
    public:

        nuu nuu;
        string ID;
        // int indeksas;
        string savininko_adresas;
        int suma;

        UTXO( ){};
        UTXO( string adr, int sum) 
            : savininko_adresas(adr), suma(sum){

                string ivestis = adr + to_string(sum);
                ID = nuu.hash(ivestis);
            }

        inline ~UTXO() {}

};

class vartotojas{

    public:
        nuu nuu;

        // vartotojas();
        vartotojas() : viesasisis_raktas(""), vardas("") {}
        vartotojas(string vardas) 
            : viesasisis_raktas(nuu.hash(vardas)), 
            vardas(vardas) {}

        inline ~vartotojas() {}

        int apskaiciuoti_balansa() const {
            int balansas = 0;
            for (const auto& ut : utxos) {
                balansas += ut.suma;
            }
            return balansas;
        }

        void prideti_UTXO(const UTXO& utxo) {
            utxos.push_back(utxo);
        }

        void pasalinti_UTXO(const string& ID) {
            auto it = std::find_if(utxos.begin(), utxos.end(), [&ID](const UTXO& ut) {
                return ut.ID == ID;
            });
            if (it != utxos.end()) {
                utxos.erase(it);
            }
        }

        std::vector<UTXO> get_UTXOs() const { return utxos; }

        

        string get_viesasisis_raktas() const { return viesasisis_raktas; }
        

        void info() const {
            cout<<"Viesasisis raktas: " << viesasisis_raktas  << "\nVardas: " << vardas << endl;
            cout<<"Balansas: " << apskaiciuoti_balansa() << endl;
            cout<<"Utxos: " << endl;
            for (const auto& utxo : utxos) {
                cout << utxo.suma << ", ID: " << utxo.ID << endl;    
            }
        }

    private:
        string viesasisis_raktas;
        string vardas;   
        vector<UTXO> utxos;
        
};

class transakcija{

    public:

        nuu nuu;
      
        // transakcija();
        transakcija() : siuntejas(""), gavejas(""), suma(0), ID(""){}
        transakcija( string siuntejas, string gavejas, int suma) 
            : siuntejas(siuntejas), gavejas(gavejas), suma(suma)
            {  string ivestis = siuntejas + gavejas + to_string(suma);
                ID = (nuu.hash(ivestis)) ;}

        inline ~transakcija() {}

        string info() const{ 
            return "\nSiuntejas: " + siuntejas 
            + "\ngavejas: " + gavejas + "\nsuma: " + to_string(suma) 
            + "\nID: " + ID;
        }

        bool patikrinti_hash( );

        string get_ID() const{ return ID;}
        string get_siuntejas() const{ return siuntejas;}
        string get_gavejas() const{ return gavejas;}
        int get_suma() const{ return suma;}

    private:
        string ID;
        string siuntejas;
        string gavejas;
        int suma;

       
        
};



class blokas{

    public:

        nuu nuu;
      
        blokas() ;
        blokas( vector<transakcija>& transakcijoss, const string& pries_blokas, 
            const string& versija, int sudetingumas):
            transakcijos(transakcijoss),
            pries_blokas(pries_blokas),
            versija(versija),
            merkel_root( skaiciuoti_merkel_root(transakcijos)) ,
            sudetingumas(sudetingumas),
            nonce(0)
        {
            auto now = chrono::system_clock::now();
            auto time_t_now = chrono::system_clock::to_time_t(now);
            laikas = ctime(&time_t_now);
            laikas.pop_back(); 
        }

        // string bloku_kasimas();
        string bloku_kasimas(const int laikas_max, const int bandymai_max );

        void info() const{
            cout << "Pries bloka: " << pries_blokas << endl;
            cout << "Hash: " << hashas << endl;
            cout << "Laikas: " << laikas << endl;
            cout << "Versija: " << versija << endl;
            cout << "Merkel root: " << merkel_root << endl;
            cout << "Nonce: " << nonce << endl;
            cout << "Sudetingumas: " << sudetingumas << endl;
            cout << "Transakciju kiekis: " << transakcijos.size() << endl;  
        }
        

        void transakciju_itraukimas_i_bloka(int dydis, vector<blokas>& blokai);
        void transakciju_itraukimas_i_bloka_1(int dydis, vector<blokas>& blokai_1);

        string get_markel_root() { return merkel_root;}
        string get_hashas() const { return hashas;}
        vector<transakcija> get_transakcijas() const{
            return transakcijos;
        }
        void spausdinti_bloka(const string& bloko_hash);


        inline ~blokas() {}



    private:
        string hashas;
        vector<transakcija> transakcijos;
        string pries_blokas;
        string laikas;
        string versija;
        string merkel_root;
        int nonce;
        int sudetingumas;

        string skaiciuoti_merkel_root( vector<transakcija>& tansakcijos);   
       
        
};

string blokas::skaiciuoti_merkel_root(vector<transakcija>& transakcijos) {
    // Jeigu transakcijų nėra, grąžinkite tuščią maišos reikšmę.
    if (transakcijos.empty()) {
        return "";
    }
    
    // Surinkite visų transakcijų ID kaip pradinį mazgų lygį.
    vector<string> lygis;
    for (auto& tx : transakcijos) {
        lygis.push_back(tx.get_ID());
    }

    // Kiekviename lygmenyje atlikite maišos reikšmių porų sujungimą.
    while (lygis.size() > 1) {
        // Jeigu mazgų skaičius yra nelyginis, paskutinis elementas kopijuojamas.
        if (lygis.size() % 2 != 0) {
            lygis.push_back(lygis.back());
        }

        // Sukurkite naują lygį sujungdami kiekvieną porą.
        vector<string> naujasLygis;
        for (size_t i = 0; i < lygis.size(); i += 2) {
            string pora = lygis[i] + lygis[i + 1];
            naujasLygis.push_back(nuu.hash(pora));  // Sujungtos poros maišas.
        }

        // Pereikite prie naujo lygmens.
        lygis = move(naujasLygis);
    }

    // Liko viena galutinė maišos reikšmė - Merkle šaknis.
    return lygis[0];
}


#endif