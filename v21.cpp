#include "header.h"
#include "kod.h"


std::vector<vartotojas>vartotoju_generavimas(int dydis);
// void transakciju_generavimas(int dydis, vector <vartotojas> &vartotojai);
std::vector <transakcija> transakciju_generavimas(int dydis, vector <vartotojas> &vartotojai);
void trans_ivygdymas( std::vector<vartotojas> vartotojai, const string siuntejas, const string gavejas, const int suma);



int main(){

    blokas();
    
    

    return 0;
}

blokas::blokas(){

    vector<blokas> blokai;
    transakciju_itraukimas_i_bloka(100, blokai);    
}

std::vector<vartotojas> vartotoju_generavimas (int dydis){

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

std::vector <transakcija> transakciju_generavimas(int dydis, vector <vartotojas> &vartotojai){

    ofstream of("transakcijos.txt");
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

        transakcijos.emplace_back(vartotojai[siuntejas].get_viesasisis_raktas(), vartotojai[gavejas].get_viesasisis_raktas(), suma);    
    
        of << "tansakcija "<< i << ": " << endl;
        of << transakcijos[i].info() << endl;

    }


    of.close();
    return transakcijos;
}

void blokas::transakciju_itraukimas_i_bloka(int dydis, vector<blokas>& blokai){

    vector<vartotojas> vartotojai = vartotoju_generavimas(1000);
    vector<transakcija> transakcijos = transakciju_generavimas(1000, vartotojai);
    random_device rd; 
    mt19937 gen(rd());

    ofstream of("itrauktos_transakcijos.txt");


    if(blokai.empty()){
        
        vector<transakcija> genesiui;
        blokas genesis(genesiui, "0", "0.1", 1);
        cout<<"Genesis blokas generuotas: "<< endl;
        genesis.bloku_kasimas();
        genesis.info();
        blokai.emplace_back(genesis);

    }

    while ( !transakcijos.empty() ) {
        
        vector<transakcija> tran100;

        for(int i = 0; i < dydis && !transakcijos.empty(); ++i){

            uniform_int_distribution<> rand_transakcija(0, transakcijos.size() - 1);   // random parenka vartotojo numeri
            int idx = rand_transakcija(gen);
            tran100.emplace_back(transakcijos[idx]);
            
            of << transakcijos[idx].info() << std::endl;
            trans_ivygdymas(vartotojai, transakcijos[idx].get_siuntejas(), transakcijos[idx].get_gavejas(), transakcijos[idx].get_suma());

            transakcijos.erase(transakcijos.begin() + idx);
        }
        


        blokas blok( tran100, blokai.back().get_hashas(), "0.1", 2);
        cout<<"\nbloku kasimas: "<<endl;
        blok.bloku_kasimas();
        blok.info();
        blokai.emplace_back(blok);

        
        // for(const auto& tran : tran100){
        
        //     auto it = std::find_if(transakcijos.begin(), transakcijos.end(), [&](const transakcija& t) {
        //         return t.get_ID() == tran.get_ID();
        //     });

        //     for (const auto& tran : transakcijos) {
            
        //         if (it != transakcijos.end()) {
                    
        //             of << it->info() << endl;
        //             trans_ivygdymas(vartotojai, it->get_siuntejas(), it->get_gavejas(), it->get_suma());

        //             transakcijos.erase(it);
                    
        //         }
        //     }
            
        
        // }
        

    }

    of.close();
}

void trans_ivygdymas( std::vector<vartotojas> vartotojai, const string siuntejas, const string gavejas, const int suma){

    auto siuntejass = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const vartotojas& var) {
        return (var.get_viesasisis_raktas() == siuntejas) ;
    });

    auto gavejass = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const vartotojas& var) {
        return (var.get_viesasisis_raktas() == gavejas) ;
    });


    if (siuntejass != vartotojai.end() && gavejass != vartotojai.end()) {
        // cout<<"\n\nsuma: " << suma << endl;
        // cout<<"siuntejas: " << siuntejass->get_viesasisis_raktas() << endl;
        // cout<<"buvo suma: " << siuntejass->get_valiutos_balansas() << endl;
        siuntejass->set_valiutos_balansas(siuntejass->get_valiutos_balansas() - suma);
        // cout<<"dabar suma: " << siuntejass->get_valiutos_balansas() << endl;
        
        // cout<<"\ngavejas: " << gavejass->get_viesasisis_raktas() << endl ;
        // cout<<"buvo suma: " << gavejass->get_valiutos_balansas() << endl;
        gavejass->set_valiutos_balansas(gavejass->get_valiutos_balansas() + suma);
    
        // cout<<"dabar suma: " << gavejass->get_valiutos_balansas() << endl  ;
    
    }

}

