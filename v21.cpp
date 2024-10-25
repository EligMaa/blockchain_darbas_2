#include "header.h"
#include "kod.h"


vector<vartotojas>vartotoju_generavimas(int dydis);
void transakciju_generavimas(int dydis, vector <vartotojas> &vartotojai);


int main(){

    vector<vartotojas> vartotojai = vartotoju_generavimas(1000);
    transakciju_generavimas(10000, vartotojai);

    return 0;
}
vector<vartotojas> vartotoju_generavimas (int dydis){

    vector <vartotojas> vartotojai;
    vartotojai.reserve(dydis);
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> balan(100, 1000000);     // intervalas tarp 100 ir 1000000, 
                                                        // parenka random skaiciu

    for (int i = 1; i <= dydis; ++i) {

        string vardai = "vardas";
        vardai += to_string(i);
        int balansas = balan(gen);
        vartotojai.emplace_back(balansas, vardai);
    }

    return vartotojai;
}



void transakciju_generavimas(int dydis, vector <vartotojas> &vartotojai){


    vector <transakcija> transakcijos;
    transakcijos.reserve(dydis);
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> rand_vartotojas(0, vartotojai.size() - 1);   // random parenka vartotojo numeri
    uniform_int_distribution<> balan(1, 50000);

    for (int i = 0; i < dydis; ++i){
    
        int siuntejas = rand_vartotojas(gen);
        int gavejas = rand_vartotojas(gen);
        int suma = balan(gen);
        
        while (gavejas == siuntejas) {
            gavejas = rand_vartotojas(gen);
        }
        transakcijos.emplace_back(vartotojai[siuntejas].viesasisis_raktas, vartotojai[gavejas].viesasisis_raktas, suma);    
    
    }

    // for (const auto& trans : transakcijos) {
    //     cout << "tansakcija: " << trans.siuntejas << " -> " << trans.gavejas << ", suma: " << trans.suma << endl;
    // }


}

