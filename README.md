# blockchain_darbas_2
# versija 0.1

## Darbo aprašas
Šis projektas imituoja centralizuotą blokų grandinę (blockchain) naudojant individualiai sukurtą maišos (hash) funkciją. Programa generuoja vartotojus, simuliuoja transakcijas tarp jų ir formuoja bei "kasa" blokus su šiose transakcijose pateikta informacija. Naudojant supaprastintą Proof-of-Work (PoW) algoritmo principą, kiekvienas blokas įtraukiamas į blokų grandinę po sėkmingo kasimo proceso. Blokų grandinė apima transakcijų įtraukimo, blokų maišos generavimo ir Merkle šaknies (Merkle root) skaičiavimo funkcionalumą.


## Kaip veikia kodas 

* ```std::vector<vartotojas> vartotoju_generavimas (int n)``` sukuria n vartotojų su atsitiktiniais balansais ir viešaisiais raktais
* ```std::vector <transakcija> transakciju_generavimas(int m, vector <vartotojas> &vartotojai)``` generuoja m atsitiktinių transakcijų tarp vartotojų (siuntėjo, gavėjo ir atsitiktinę sumos)
* ```void trans_ivykdymas( std::vector<vartotojas>& vartotojai, const string& siuntejas, const string& gavejas, const int suma)``` perveda lėšas tarp vartotojų pagal nurodytą transakciją ir atnaujina siuntėjo bei gavėjo balansus
* ```void transakciju_itraukimas_i_bloka(int dydis, vector<blokas>& blokai)``` iš transakcijų sąrašo suformuoja bloką, įtraukia jį į blokų grandinę ir atlieka bloko kasimą
* ```string skaiciuoti_merkel_root(vector<transakcija>& transakcijos)``` apskaičiuoja Merkle šaknį pagal bloko transakcijas
* ```string blokas::bloku_kasimas()``` pradeda kasimo procesą su nonce reikšme. Didina nonce reikšmę, kol bloko maišos reikšmė atitinka nustatytą sudėtingumą (nulių seka hash pradžioje)

## Naudojimosi instrukcijos
1. Diegimas
* **Visual Studio Code** diegimas: jei jo neturite, galite sekti šią vaizdo įrašų instrukciją: https://www.youtube.com/watch?v=DMWD7wfhgNY
* g++ kompiliatorius: jei jūsų kompiuteryje dar nėra g++ kompiliatoriaus, galite jį įsidiegti vadovaudamiesi šia nuoroda https://code.visualstudio.com/docs/languages/cpp

2. Kompiliavimas ir programos paleidimas
* Paleiskite šias komandas terminale, kad sukompiliuotumėte ir paleistumėte programą
  - ```g++ v21.cpp header.h kod.h -o prog```
  - ```./prog```



