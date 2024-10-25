#include "header.h"

nuu::nuu( int nr ) {

    if(nr == 7)   {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear leftover neweilute
    getline(cin, ivestis);
    }
    else
    ivestis = skaitymas(failai[nr]);

    laiko_matavimas(ivestis);
    cout << uzhashuotas <<endl;

    // hash(ivestis);

}


string nuu::hash(string &ivestis){

    unsigned long long randomas = 9999854665226455ULL;
    unsigned long long randomas2 = 98880025054544054ULL;

    array<unsigned long long, 4> hash = {0, 0, 0, 0};
    int j = 0;
    char temp = 'n';


    for (char simbolis : ivestis) { 

        for (int i = 0; i < 4; ++i) {
            
            hash[i] ^= (randomas + simbolis );
            hash[i] *= (randomas2 ^ hash[i] * hash[i] );
        }
        
        
        if (j % 3 == 0) {
            swap(hash[0], hash[3]);
            hash[1] += hash[2] * temp * simbolis;
        }
        else if (j % 3 == 1) {
            hash[1] += hash[2] * simbolis;
            swap(hash[1], hash[2]);
        }
        temp = simbolis;

        ++j;
    }
    return uzhashuotas = konvertavimas(hash);
       
    
}
string nuu::konvertavimas(const array<unsigned long long, 4>& hash){

    vector <unsigned long long> druska = {192837645ULL, 1098765432ULL, 123456789ULL, 987654321ULL};
    vector<char> druska1 = {'-','$','#','@'};
    vector<char> druska2 = {'&','/','*',','};
    stringstream ss;
    int i = 0;

    for (const auto& n : hash) {
        
        unsigned long long druskintas = n * druska[3 - i] + druska1[3 - i] * druska2[3 - i];
        // 64 bits -> 16 hex 
        ss << setw(16) << setfill('5') << hex << druskintas; 
        ++i;
    }
    return ss.str();

}

string nuu::skaitymas(const string &ivestis){

    ifstream failas(ivestis);
    
    if (!failas) {
        throw invalid_argument("Nepavyko atidaryti failo: " + ivestis);
    }
    
    string eilute;

    if(failas.peek() == ifstream::traits_type::eof()) eilute = "da39a3ee5e6b4b0d3255bfef95601890afd80709";  // tikrina ar failas tuscias

    while (getline(failas, eilute)) {
        ilgis = eilute.length();
        return eilute;
    }
    failas.close();
    return eilute;
}

void generuoja_failus_Ats(const string& pav, int ilgis) {
    ofstream failas(pav);

    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<> intervalas('A', 'Z'); // intervalas tarp A ir Z

    for (int i = 0; i < ilgis; ++i) {
        char random = static_cast<char>(intervalas(gen)); // generuoja atsitiktine raide
        int sk = rand() % 100 + 1;
        failas << random << sk;
    }

    failas.close();
}

void generuoja_f_su_viena_skirtinga(const string& pav,const string& pav2, int ilgis) {
    ofstream failas(pav);
    ofstream failas2(pav2);

    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<> intervalas('A', 'Z'); // intervalas tarp A ir Z

    for (int i = 0; i < ilgis; ++i) {
        char random = static_cast<char>(intervalas(gen)); // generuoja atsitiktine raide
        int sk = rand() % 10 ;
        failas << random << sk;
        failas2 << random << sk;
        if(i == ilgis / 2 ){
            failas << 'A';
            failas2 << 'B';
        }
    }

    failas.close();
    failas2.close();

}

double nuu::laiko_matavimas( string& ivestis) {
    auto pradzia = chrono::high_resolution_clock::now();
    
    hash(ivestis); 

    auto pabaiga = chrono::high_resolution_clock::now();
    chrono::duration<double> laikas = pabaiga - pradzia;
    cout<<laikas.count()<<endl;
    return laikas.count();  // grazina sekundes
}

vector<string> eiluciu_skaitymas( string& pav) {
    ifstream failas(pav);
    string eilute;
    vector<string> eilutes;
    while (getline(failas, eilute)) {
        eilutes.push_back(eilute);
    }
    return eilutes;
}

void testavimas( string pav) {
    vector<string> lines = eiluciu_skaitymas(pav);
    
    for (int eiluciu_sk = 1; eiluciu_sk <= 791 || eiluciu_sk == 512*2 ; eiluciu_sk *= 2) {

        if (eiluciu_sk == 512 * 2)eiluciu_sk = 789;
        double laikas = 0.0;

        string ivestis;
        for (int i = 0; i < eiluciu_sk && i < lines.size(); ++i) {
            ivestis += lines[i] + "\n";  // pridedame eilute i ivesti
        }

        for (int repeat = 0; repeat < 5; ++repeat) {
            laikas += nuu().laiko_matavimas(ivestis);
        }

        double avg_time = laikas / 5;
        cout << "perskaitytos eilutes: " << eiluciu_sk << ". Vidutinis laikas: " << avg_time << " sekundes" << endl;
    }
}

// string atsitiktinis_stringas(int ilgis) {
//     const string raides = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//     string atsitiktiniai_st;
//     for (int i = 0; i < ilgis; ++i) {
//         atsitiktiniai_st += raides[rand() % raides.size()];
//     }
//     return atsitiktiniai_st;
// }
string atsitiktinis_stringas(int ilgis) {
    static const char simb[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string atsitiktinis_st;
    atsitiktinis_st.reserve(ilgis);

    for (int i = 0; i < ilgis; ++i) {
        atsitiktinis_st += simb[rand() % (sizeof(simb) - 1)];
    }
    return atsitiktinis_st;
}

void poru_generavimas(int poros, int ilgis, ofstream &failo_pav) {
    for (int i = 0; i < poros; ++i) {
        string str1 = atsitiktinis_stringas(ilgis);
        string str2 = atsitiktinis_stringas(ilgis);
        failo_pav << str1 << " " << str2 << endl;
    }
}

void atsitiktiniu_eiluciu_poros (){
    srand(time(nullptr));  

    ofstream failo_pav("poros.txt");

    poru_generavimas(25000, 10, failo_pav);
    poru_generavimas(25000, 100, failo_pav);
    poru_generavimas(25000, 500, failo_pav);
    poru_generavimas(25000, 1000, failo_pav);

    failo_pav.close();

}


void nuu::tikrinti_hash_kolizijas() {

    ifstream poru_failas("poros.txt");
    string eilute;
    unordered_set<string> hash_rinkinys;  //  laikyti hashams
    int kolizijos = 0;

    ofstream f ("testas_kolizijos.txt");

    if (!poru_failas) {
        cerr << "Nepavyko atidaryti failo." << endl;
        return;
    }

    while (getline(poru_failas, eilute)) {

        istringstream is(eilute);
        string pirmas, antras;
        is >> pirmas >> antras;

        string hash1 = hash(pirmas); 
        f<<hash1<<" ";
        string hash2 = hash(antras);
        f<<hash2<<endl;

        if (hash_rinkinys.find(hash1) != hash_rinkinys.end() || hash_rinkinys.find(hash2) != hash_rinkinys.end()) {
            kolizijos++;
        } else {
            hash_rinkinys.insert(hash1);
            hash_rinkinys.insert(hash2);
        }
    }

    poru_failas.close();
    f.close();

    cout << "Koliziju skaicius: " << kolizijos << endl;
}

void lavinos_failas(){

    ofstream failo_pav("lavinos.txt");

    for (int i = 0; i < 100000; ++i) {

        string str1 = atsitiktinis_stringas(999);
        string str2 = str1 + 'A';
        str1 += 'B';
        failo_pav << str1 << " " << str2 << endl;

    }
    failo_pav.close();
}


void nuu::lavinos_testavimas(){

    ifstream failas ("lavinos.txt");
    string eilute;

    int min_bit = 260 ;
    int max_bit = 0;
    int vidurkis_bit = 0;
    int min_hex = 65 ;
    int max_hex = 0;
    int vidurkis_hex = 0;

    while(getline(failas, eilute)) {

        istringstream is(eilute);
        string pirmas, antras;
        is >> pirmas >> antras;
        
        string hash1 = hash(pirmas);
        string hash2 = hash(antras);

        int skirtumas = bitu_lyginimas(hex_i_binaru( hash1), hex_i_binaru(hash2));
        vidurkis_bit += skirtumas;
        min_bit = min(min_bit, skirtumas);
        max_bit = max(max_bit, skirtumas);

        skirtumas = hex_lyginimas(hash1, hash2);
        vidurkis_hex += skirtumas;
        min_hex = min(min_hex, skirtumas);
        max_hex = max(max_hex, skirtumas);
       
    }
    vidurkis_bit /= 100000;
    vidurkis_hex /= 100000;
    cout<<"Skirtingumas bitu lygmenyje: \nVidurkis "<< 100 * vidurkis_bit / (64 * 4)  <<"%\nMin "<< 100 * min_bit / (64 * 4) << "%\nMax " << 100 * max_bit / (64 * 4)<<"%"<<endl;
    cout<<"Skirtingumas hex'u lygmenyje: \nVidurkis "<< 100 * vidurkis_hex / (64)  <<"%\nMin "<< 100 * min_hex / (64) << "%\nMax " << 100 * max_hex / (64)<<"%"<<endl;

    failas.close();

}

string hex_i_binaru(const string& hex) {
    unordered_map<char, string> hex_bit{
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };

    string bitu_stringas;
    for (char hex_char : hex) {
        bitu_stringas += hex_bit[toupper(hex_char)];
    }
    return bitu_stringas;
}

int bitu_lyginimas(const string& hash1, const string& hash2) {
    
    int skirtumas = 0;

    for (size_t i = 0; i < hash1.length(); ++i) {
        if (hash1[i] != hash2[i]) 
            ++skirtumas;
    }
    return skirtumas;
}

int hex_lyginimas(const string& hash1, const string& hash2){

    int skirtumas = 0;
    for (size_t i = 0; i < hash1.length(); ++i) {
        if (hash1[i] != hash2[i]) ++skirtumas;
    }

    return skirtumas;   
}
 