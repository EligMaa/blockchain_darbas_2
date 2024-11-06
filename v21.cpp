#include "header.h"
#include "kod.h"
#include <omp.h>


std::vector<vartotojas>vartotoju_generavimas(int dydis);
std::vector <transakcija> transakciju_generavimas(int dydis, vector <vartotojas> &vartotojai);
bool trans_ivykdymas( std::vector<vartotojas>& vartotojai, const string& siuntejas, const string& gavejas, const int suma);
bool get_block_transactions( std::vector<blokas>& blokai, const std::string& bloko_hash);
std::vector<blokas> generuoti_kandidatinius_blokus( std::vector<transakcija>& transakcijos, 
const std::string& pries_blokas, const std::string& versija, int sudetingumas, int kiekis_bloku);

std::vector<blokas> blokai;
std::vector<vartotojas> vartotojai;
// std::vector<blokas> blokai_kandidatai;

int main(){

    blokas();



    int meniu;
    bool baigti = false;

    do{ 
       
            cout << "\n---------- Menu ----------\n";
            cout << "1 - isspausdinti visus blokus\n";
            cout << "2 - isspausdinti transakcijas bloke\n";
            cout << "3 - isspaustinti transakcijos informacija\n";
            cout << "4 - isspausdinti vartotoja\n";
      
            cout<< "0 - baigti darba"<<endl;
            cout << "Pasirink nuo 0 iki 4: ";

            int choice;
            try {
                    cout << "Iveskite pasirinkima: " << endl;
                    string input;
                    cin >> input;

                    if (input.find_first_not_of("0123456789") != string::npos) {     ///tikrina ar ivestas skaicius naturalusis
                    throw invalid_argument("Netinkamas pasirinkimas");
                    }

                    meniu = stoi(input);

                    if (meniu < 0 || meniu > 5) {                                   ///tikrina ar ivestas skaicius yra nuo 0 iki 9
                        throw invalid_argument("Klaida! Iveskite nuo 0 iki 4 ");
                    }

                } 

                catch (const invalid_argument & e) {
                    cerr << "Klaida: " << e.what() << endl;
                    cout << endl;

                    continue;
                }

            std::string bloko_hash;
            auto it = blokai.end();
            std::string ivedimas;

            switch (meniu) {
                case 1:
                    for ( auto& blok : blokai) {
                        blok.info();
                        cout << endl;
                    }
                    break;

                case 2:{

                    cout << "Iveskite bloko hash: ";
                    cin >> ivedimas; 
                    
                    auto it = std::find_if(blokai.begin(), blokai.end(), [&](const blokas& blokass){
                        return blokass.get_hashas() == ivedimas;
                    });
                    if(it != blokai.end()){
                        cout<<"\nInformacija apie transakcijas bloke "<< ivedimas<<":\n";
                        cout<<"Is viso transakciju: "<< it->get_transakcijas().size() << endl;
                        for (const auto& transakcija : it->get_transakcijas()) {
                            cout << transakcija.info() << endl;
                        }
                    }
                    else{
                        cout << "Blokas nerastas\n";
                    }
                    break;
                }

                case 3:{

                    string transId;
                    cout << "Iveskite transakcijos ID: \n";
                    cin >> transId;
                    bool found = false;
                    for (const auto& blokas : blokai) {
                        for (const auto& transakcija : blokas.get_transakcijas()) {
                            if (transakcija.get_ID() == transId) {
                                cout<<transakcija.info();
                                found = true;
                                break;
                            }
                        }

                        if (found) break;
                    }
                    if (!found) {
                        cout << "Transakcija su šiuo ID nerasta.\n";
                    }

                    break;
                }

                case 4:{
                    cout << "Iveskite vartotojo viesasisis_raktas: ";
                    cin >> ivedimas; 
                    
                    auto it = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const vartotojas& vartotojas){
                        return vartotojas.get_viesasisis_raktas() == ivedimas;
                    });
                    if(it != vartotojai.end()){
                        cout<<"\nInformacija apie vartotoja "<< ivedimas<<":\n";
                        it->info();
                    }
                    else{
                        cout << "Vartotojas nerastas\n";
                    }
                    break;
                }
                case 0:
                    baigti = true;
                    break; 
                    
            }
    

    }while (!baigti);


    return 0;
}

blokas::blokas(){

    // vector<blokas> blokai;
    transakciju_itraukimas_i_bloka(100, blokai);    
}

std::vector<vartotojas> vartotoju_generavimas (int dydis){

    ofstream of("vartotojai.txt");

    vartotojai.reserve(dydis);
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> balan(100, 1000000); 
    uniform_int_distribution<> randomas(1, 10);
    

    for (int i = 0; i < dydis; ++i) {

        string vardai = "vardas";
        vardai += to_string(i + 1);
        vartotojai.emplace_back(vardai);

        int kiek = randomas(gen);
        of << vardai << ": "  << endl;

        for (int j = 0; j < kiek; j++) {
            
            int balansas = balan(gen);
            of << balansas << ", " ;
            UTXO n (vardai, balansas);
            vartotojai[i].prideti_UTXO(n);
        }

        of<<endl;
    }

    of.close();

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

    vector<blokas> blokai_kandidatai;

    random_device rd; 
    mt19937 gen(rd());

    ofstream of("itrauktos_transakcijos.txt");

    int laikas_max = 5000;
    int bandymai_max = 100000;


    if(blokai.empty()){
        
        vector<transakcija> genesiui;
        blokas genesis(genesiui, "0", "0.2", 4);
        cout<<"Genesis blokas generuotas: "<< endl;
        genesis.bloku_kasimas(5000, 100000);
        genesis.info();
        blokai.emplace_back(genesis);

    }

    while ( blokai.size() < 6 ) {
        
        vector<blokas> kandidat_blokai = generuoti_kandidatinius_blokus(transakcijos, blokai.back().get_hashas(), "0.2", 4, 5);
        bool block_mined = false;
        vector<int> panaudoti;  // ineksai
        uniform_int_distribution<> rand_kandidatas(0, kandidat_blokai.size() - 1);



        while (!block_mined && panaudoti.size() < kandidat_blokai.size()) {

            int idx;
            
            do {
                idx = rand_kandidatas(gen);
            } while (find(panaudoti.begin(), panaudoti.end(), idx) != panaudoti.end());    // uztikrina kad nekastu to pacio bloko kelis kartus


            panaudoti.push_back(idx);   

            cout << "\nKasamas " << idx + 1 << endl;
            string iskastas = kandidat_blokai[idx].bloku_kasimas(laikas_max, bandymai_max);

            if (!iskastas.empty()) {
                blokai.push_back(kandidat_blokai[idx]);
                block_mined = true;
                break;
            } else {
                cout << "Nepavyko iskasti bloko " << idx + 1 << "\n";
            }
           
        }        
            

        if (!block_mined) {
            
            cout <<"------------------------------------------------------------"<<endl;
            cout << "Neiskastas nei vienas blokas. Didinamas laikas ir bandymai \n";
            laikas_max += 5000; 
            bandymai_max += 100000; 
        }


    }

    of.close();
    
}


bool trans_ivykdymas( std::vector<vartotojas>& vartotojai, const string& siuntejas, const string& gavejas, const int suma){

    auto siuntejass = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const vartotojas& var) {
        return (var.get_viesasisis_raktas() == siuntejas) ;
    });

    auto gavejass = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const vartotojas& var) {
        return (var.get_viesasisis_raktas() == gavejas) ;
    });

    if(siuntejass->apskaiciuoti_balansa() < suma){         // tikrina balansa

        return false;
    }

    int totalSelectedAmount = 0;
    std::vector<UTXO> selectedUTXOs;
    
    // cout<<"sintejas siuncia: "<<endl;
    for (const auto& utxo : siuntejass->get_UTXOs()) {  // Assumes get_UTXOs() returns a list of sender's UTXOs
        selectedUTXOs.push_back(utxo);
        totalSelectedAmount += utxo.suma;
        // cout<< utxo.suma<<", ";
        
        if (totalSelectedAmount >= suma) break;
    }

    // // Check if we have enough UTXOs to cover the amount
    // if (totalSelectedAmount < suma) {
    //     std::cout << "Not enough UTXOs to cover the transaction amount.\n";
    //     return false;
    // }

    // Step 2: Calculate change, if any
    int change = totalSelectedAmount - suma;



    // Step 3: Update UTXO records for sender and receiver
    // Remove used UTXOs from the sender
    for (const auto& utxo : selectedUTXOs) {
        siuntejass->pasalinti_UTXO(utxo.ID);
    }

    // Create a new UTXO for the recipient with the amount of the transaction
    UTXO recipientUTXO(gavejas, suma);
    gavejass->prideti_UTXO(recipientUTXO);

    // If there's a change, create a new UTXO for the sender
    if (change > 0) {
        UTXO changeUTXO(siuntejas, change);
        siuntejass->prideti_UTXO(changeUTXO);
    }

    // std::cout << "Transaction successful.\n";
    // std::cout << "Sender's new balance: " << siuntejass->apskaiciuoti_balansa() << std::endl;
    // std::cout << "Recipient's new balance: " << gavejass->apskaiciuoti_balansa() << std::endl;


    return true;
}


string blokas::bloku_kasimas(const int laikas_max, const int bandymai_max){

    auto pradzia = chrono::high_resolution_clock::now();
    #pragma omp parallel num_thread(1)


    #pragma omp parallel shared(...,nthreads,chunk) private(...)

    do {
        ++nonce; 
        string ivestis = pries_blokas + laikas + versija + merkel_root + to_string(nonce);
        hashas = nuu.hash(ivestis);

        auto pabaiga = chrono::high_resolution_clock::now();        

        if (hashas.substr(0, sudetingumas) == string(sudetingumas, '0')) {


            auto laikas = chrono::duration_cast<chrono::milliseconds>(pabaiga - pradzia).count();
            if ( (laikas > laikas_max) || (nonce >= bandymai_max) ) { // milisekundemis
                cout << "Pasiektas ribotas kasimo laikas arba bandymu kiekis. Bloka kase: "<<laikas<<" milisekundes ir "<< nonce << " kartus"<<endl;
                return ""; 
            }
            else{
                cout << "Blokas iskastas per " << laikas << " milisekundes. bandymai: " << nonce << ", hash: " << hashas << endl;
                return hashas; 
            }
        }


    } while (true);

    return hashas;
              
}





bool transakcija::patikrinti_hash()  {              // transakcijos maišos reikšmės tikrinimas
    string input = siuntejas + gavejas + to_string(suma);
    string expected_ID = nuu.hash(input);
    return expected_ID == ID;
}

std::vector<blokas> generuoti_kandidatinius_blokus( std::vector<transakcija>& transakcijos, 
const std::string& pries_blokas, const std::string& versija, int sudetingumas, int kiekis_bloku) {

    random_device rd;
    mt19937 gen(rd());
    std::vector<blokas> blokai_kandidatai;

    ofstream of("transakcijos_kandidatai.txt");

    for (int i = 0; i < kiekis_bloku; ++i) {
        std::vector<transakcija> kandidato_transakcijos;
        uniform_int_distribution<> rand_transakcija(0, transakcijos.size() - 1);

        for (int j = 0; j < 100; ++j) {
            int idx = rand_transakcija(gen);

            if (!transakcijos[idx].patikrinti_hash()) {                 // transakcijos maišos reikšmės tikrinimas
                cout << "Transakcija su netinkamu ID, ignoruojama.\n";
                continue;
            }

            if(trans_ivykdymas(vartotojai, transakcijos[idx].get_siuntejas(), 
            transakcijos[idx].get_gavejas(), transakcijos[idx].get_suma()) ){
                
                kandidato_transakcijos.emplace_back(transakcijos[idx]);
                }
        }

        blokas kandidatas(kandidato_transakcijos, pries_blokas, versija, sudetingumas);
        blokai_kandidatai.push_back(kandidatas);

        for (const auto& trans : kandidato_transakcijos) {
            of << trans.info() << std::endl; 
        }        
    }

    of.close();

    return blokai_kandidatai;
}




