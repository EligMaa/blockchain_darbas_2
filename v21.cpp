#include "header.h"
#include "kod.h"



int main(){

    vartotoju_generavimas(100);

    return 0;
}
void vartotoju_generavimas(int dydids){

    vector <vartotojas> vartotojai;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> balan(100, 1000000);

    for (int i = 1; i <= dydids; ++i) {

        string vardai = "vardas";
        vardai += to_string(i);
        int balansas = balan(gen);
        vartotojai.emplace_back(balansas, vardai);

    }

    cout<<"\n"<< vartotojai[3].vardas<<" "<< vartotojai[3].valiutos_balansas<<" "<<vartotojai[3].viesasisis_raktas<<endl;


}


