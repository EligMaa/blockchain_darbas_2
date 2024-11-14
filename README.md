# blockchain_darbas_2
# versija 0.2

## Darbo aprašas
Šis projektas imituoja centralizuotą blokų grandinę (blockchain) naudojant individualiai sukurtą maišos (hash) funkciją. Programa generuoja vartotojus, simuliuoja transakcijas tarp jų ir formuoja bei "kasa" blokus su šiose transakcijose pateikta informacija. Naudojant supaprastintą Proof-of-Work (PoW) algoritmo principą, kiekvienas blokas įtraukiamas į blokų grandinę po sėkmingo kasimo proceso. Blokų grandinė apima transakcijų įtraukimo, blokų maišos generavimo ir Merkle šaknies (Merkle root) skaičiavimo funkcionalumą.


## Kaip veikia kodas 

* ```std::vector<vartotojas> vartotoju_generavimas (int n)``` sukuria n vartotojų su atsitiktiniais balansais ir viešaisiais raktais
* ```std::vector <transakcija> transakciju_generavimas(int m, vector <vartotojas> &vartotojai)``` generuoja m atsitiktinių transakcijų tarp vartotojų (siuntėjo, gavėjo ir atsitiktinę sumos)
* ```void trans_ivykdymas( std::vector<vartotojas>& vartotojai, const string& siuntejas, const string& gavejas, const int suma)``` perveda lėšas tarp vartotojų pagal nurodytą transakciją ir atnaujina siuntėjo bei gavėjo balansus
* ```void transakciju_itraukimas_i_bloka(int dydis, vector<blokas>& blokai)``` iš kandidatų sąrašo parenka 5 blokus, įtraukia juos į blokų grandinę ir atlieka bloko kasimą
* ```trans_ivykdymas(vector<vartotojas>& vartotojai, const string& siuntejas, const string& gavejas, const int suma)``` ši funkcija atlieka transakciją tarp dviejų vartotojų, naudodama Unspent Transaction Output (UTXO) mechanizmą.
* ```string skaiciuoti_merkel_root(vector<transakcija>& transakcijos)``` apskaičiuoja Merkle šaknį pagal bloko transakcijas
* ```bloku_kasimas(const int laikas_max, const int bandymai_max)``` ši funkcija bando iškasti bloką per nustatytą laiką ir bandymų skaičių
* ```generuoti_kandidatinius_blokus(vector<transakcija>& transakcijos, const std::string& pries_blokas, const std::string& versija, int sudetingumas, int kiekis_bloku)``` ši funkcija generuoja kandidatinius blokus su atsitiktinai parinktomis transakcijomis, kurias vėliau galima bandyti iškasti

## Merkle medis
```
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
```
Kaip apskaičiuojama Merkelio šaknis šiame kode:
* funkcija pirmiausia surenka visų bloko transakcijų ID. Šie tampa „lapais“ Merkelio medyje
* jeigu transakcijų skaičius yra nelyginis, paskutinis ID yra padvigubinamas, kad lygmenyje būtų lyginis skaičius elementų. Tai dažna praktika Merkelio medžiuose
* paruošus pradinį transakcijų ID rinkinį, funkcija sujungia poromis kiekvieną gretimą ID ir juos hash'ina. Šis procesas kartojamas kiekviename lygyje, kol lieka tik viena maišos reikšmė
* kai lieka tik viena maišos reikšmė, ji tampa Merkelio šaknimi ir yra grąžinama kaip rezultatas

## Realizuota transakcijų verifikavimas
Funkcijoje ```trans_ivykdymas``` tikrinama, ar siuntėjo balansas yra pakankamas, kad būtų galima atlikti transakciją.
```
if(siuntejass->apskaiciuoti_balansa() < suma) {
    return false;
}
```
Funkcijoje ```transakcija::patikrinti_hash``` atliekamas transakcijos maišos reikšmės tikrinimas.
```
bool transakcija::patikrinti_hash()  {  
    string input = siuntejas + gavejas + to_string(suma);
    string expected_ID = nuu.hash(input);
    return expected_ID == ID;
}
```
## Patobulintas blokų kasimo procesas
Funkcija ```generuoti_kandidatinius_blokus``` sugeneruoja 5 potencialius blokus (kandidatus) iš ~100 atsitiktinai pasirinktų transakcijų. Transakcijos blokuose gali kartotis. Funkcija ```blokas::transakciju_itraukimas_i_bloka``` pasinaudoja prieš tai pateikta funkcija ir sugeneruoja blokus. Tada sukuriamas kasimo ciklas, kuriuo metu atsitiktinai pasirenkamas kandidatinis blokas iš ```kandidat_blokai``` sąrašo ir bandoma jį iškast. Jeigu bloką pavyksta iškasti (t. y., sukurta hash reikšmė atitinka sudėtingumo lygį) ir pats kasimas neužtruko ilgiau nei 5 sekundžių arba 100000 bandymų, blokas yra pridedamas prie ```blokai``` grandinės. Jeigu nė vienas kandidatinis blokas nebuvo iškastas per nustatytą laiką ar bandymų skaičių, padidinamas maksimalus laiko limitas ir bandymų skaičius, kad padidėtų tikimybė iškasti bloką kitame cikle.

## Gerosios OOP praktikos
### Encapsuliacija
Encapsuliacija yra viena iš svarbiausių OOP praktikų, kurios tikslas yra apsaugoti objekto duomenis nuo tiesioginio išorinio manipuliavimo. Ši praktika yra naudojama apibrėžiant klasių privačius atributus. Klasės ```vartotojas``` ir ```UTXO``` turi privačius atributus, pvz., viesasisis_raktas, vardas ir utxos, o prieiga prie šių atributų atliekama per viešas metodų funkcijas, pvz., get_viesasisis_raktas(). Atributai, kurie neturėtų būti prieinami tiesiogiai, yra apsaugoti nuo išorinio manipuliavimo, o prieiga prie jų kontroliuojama per metodus. 



## Naudojimosi instrukcijos
1. Diegimas
* **Visual Studio Code** diegimas: jei jo neturite, galite sekti šią vaizdo įrašų instrukciją: https://www.youtube.com/watch?v=DMWD7wfhgNY
* g++ kompiliatorius: jei jūsų kompiuteryje dar nėra g++ kompiliatoriaus, galite jį įsidiegti vadovaudamiesi šia nuoroda https://code.visualstudio.com/docs/languages/cpp

2. Kompiliavimas ir programos paleidimas
* Paleiskite šias komandas terminale, kad sukompiliuotumėte ir paleistumėte programą
  - ```g++ v21.cpp header.h kod.h -o prog```
  - ```./prog```
 
3. Instrukcija, kaip naudotis programa:
Po programos paleidimo jums bus pateiktas meniu su pasirinkimais.
```
---------- Menu ----------
1 - isspausdinti visus blokus
2 - isspausdinti transakcijas bloke
3 - isspaustinti transakcijos informacija
4 - isspausdinti vartotoja
0 - baigti darba
Pasirink nuo 0 iki 4:
```
1 - Išspausdinti visus blokus <br>
Pasirinkus šią komandą, programa peržiūri visus sukurtus blokus ir išspausdina kiekvieno bloko informaciją. <br>
2 - Išspausdinti transakcijas bloke <br>
Pasirinkus šią komandą, galėsite peržiūrėti visų transakcijų informaciją konkrečiame bloke. Jums bus paprašyta įvesti bloko hash reikšmę, kad būtų surastas norimas blokas. <br>
3 - Išspausdinti transakcijos informaciją <br>
Ši komanda leidžia jums gauti konkrečios transakcijos informaciją pagal jos ID. <br>
4 - Išspausdinti vartotoją <br>
Ši komanda leidžia ieškoti ir spausdinti konkretaus vartotojo informaciją, remiantis jo viešuoju raktu. <br>
0 - Baigti darbą <br>
Pasirinkus šią komandą, programa bus uždaryta. <br>

 
## Papildomos užduotys

### UTXO modelio naudojimas vietoj sąskaitos modelio

Sukurta klasė:
```
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
```
Vartotojas yra blockchain dalyvis, kuris gali turėti ir naudoti UTXO:
```
class vartotojas{
    public:
        nuu nuu;
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
```
* utxos: tai vektorius UTXO objektų, kurie priklauso vartotojui
* ```apskaiciuoti_balansa()```: ši funkcija skaičiuoja vartotojo balansą, sumuojant visas vartotojui priklausančias UTXO sumas. Ji pereina per visą utxos sąrašą ir sudeda kiekvieno UTXO sumas
* ```prideti_UTXO(const UTXO& utxo)```: prideda naują UTXO prie vartotojo UTXO sąrašo (utxos), kai vartotojas gauna naują transakciją
* ```pasalinti_UTXO(const string& ID)```: ištrina UTXO, kai jis yra panaudojamas (pvz., vykdant transakciją). Tai atliekama surandant atitinkamą UTXO pagal jo ID ir pašalinant jį iš utxos sąrašo


