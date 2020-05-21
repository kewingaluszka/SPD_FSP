#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
char in[] = "opt.txt";

struct data {
    int numerZ;
    int maszyna;
    int p;

};

bool sortnr(const data &data_1, const data &data_2) {
    return (data_1.numerZ < data_2.numerZ);
}

struct dataJonh {
    int numerZ;
    int p1;
    int p2;

    bool operator<(dataJonh zad) const {
        return min(p1, p2) < min(zad.p1, zad.p2);
    }
};


vector<dataJonh> algorytmJonhsona(vector<dataJonh> zad);
int cmaxJonh(vector<dataJonh> zad);
int cemax(vector<vector<data>> zadanie);
int bruteforce(vector<vector<data>> zadania);


int main() {
    fstream plik;
    int ileZadan, ileMaszyn;
    plik.open(in, ios::in);
    if (plik.good() == true) {
        plik >> ileZadan >> ileMaszyn;
        vector<vector<data>> zadania(ileMaszyn, vector<data>(ileZadan));
        for (int i = 0; i < ileZadan; i++) {
            for (int j = 0; j < ileMaszyn; j++) {
                zadania[j][i].numerZ = i;
                plik >> zadania[j][i].maszyna >> zadania[j][i].p;
            }
        }
        auto poczatek = chrono::high_resolution_clock::now();
        cout << cemax(zadania) << endl;
        auto koniec = chrono::high_resolution_clock::now();
        auto delta = chrono::duration_cast<chrono::microseconds>(koniec - poczatek).count();
        cout << "Czas sortowania wyniosl: " << delta << "us" << endl;



        /*
        vector<dataJonh> zadanka(ileZadan);
        vector<dataJonh> gotowe(ileZadan);
        for (int j = 0; j <= ileZadan; j++)
        {
            zadanka[j].numerZ = j;
            zadanka[j].p1 = zadania[0][j].p;
            zadanka[j].p2 = zadania[1][j].p;
        }
           gotowe = algorytmJonhsona(zadanka);
        for (int i = 0; i< ileZadan; i++){
            if (i==0)
                cout << "NR " << "P1 " << "P2" <<endl;
            cout << gotowe[i].numerZ << "  "
                 << gotowe[i].p1     << "  "
                 << gotowe[i].p2     << " "
                 << endl;
        }
        cout << "cmax = " << cmaxJonh(gotowe) << endl; */

    }
    plik.close();
    return 0;
}


int cemax(vector<vector<data>> zadania) {

    //zmiennne pomocnicze
    int tmp_start = 0;
    int tmp_koniec = 0;
    int Cmax = 0;

    int l_maszyn = zadania.size(); //liczba maszyn
    int l_zadan = zadania[0].size(); //liczba zadan

    vector<vector<int>> v_start(l_maszyn, vector<int>(l_zadan,0)); //wektor wektorów zawierajacy momenty startowe zadań dla zadanej maszyny
    vector<vector<int>> v_koniec(l_maszyn, vector<int>(l_zadan,0)); //wektor wektorów zawierajacy momenty końcowe zadań dla zadanej maszyny


    for (int i = 0; i < l_maszyn; i++) {
        for (int j = 0; j < l_zadan; j++) {
            if (i == 0) {                                           //maszyna o najniższym numerze - nie ma wcześniejszej maszyny
                tmp_start = tmp_koniec;
                tmp_koniec = tmp_start + zadania[i][j].p;
            } else {                                                  //dla maszyn pozostałych
                if (j == 0) {
                    tmp_start = v_koniec[i - 1][j];                   //czas ukonczenia pierwszego zadania na poprzedniej maszynie
                } else {
                    tmp_start = max(v_koniec[i - 1][j], v_koniec[i][j -1]);  //większy czas z czasu wykonania tego zadania na wcześniejszej maszynie i czasu wykonania poprzedniego zadania
                }
                tmp_koniec = tmp_start + zadania[i][j].p;
            }

            v_start[i][j] = tmp_start;
            v_koniec[i][j] = tmp_koniec;


        }
    }
    Cmax = v_koniec[l_maszyn - 1][l_zadan - 1];

    return Cmax;
}

int bruteforce(vector<vector<data>> zadania){

    //zmiennne pomocnicze
    int tmp_Cmax = 0;
    int Cmax = 1000000000;
    vector<int> permutejszyn;

    int l_maszyn = zadania.size(); //liczba maszyn
    int l_zadan = zadania[0].size(); //liczba zadan

    tmp_Cmax = cemax(zadania);

    while (next_permutation(zadania[0].begin(), zadania[0].end(), sortnr)) {

        tmp_Cmax = cemax(zadania);

        if (Cmax > tmp_Cmax) {
            Cmax = tmp_Cmax;

            permutejszyn.clear();
            for (int i = 0; i < l_zadan; i++) {                         //aktualizacja permutacji
                permutejszyn.push_back(zadania[0][i].numerZ);
             // cout << zadania[0][i].numerZ <<endl;
            }
        }
        for (int j = 1; j < l_maszyn; j++) {
            next_permutation(zadania[j].begin(), zadania[j].end(), sortnr);
        }
    }
    cout<<"permutacja koncowa:" <<endl;
    for (int k = 0; k < l_zadan; k++) {                         //wyswietlenie końcowej permutacji
          cout << permutejszyn[k] << " ";
    }
    cout<<endl;

return Cmax;

}

vector<dataJonh> algorytmJonhsona(vector<dataJonh> zad) {
    sort(zad.begin(), zad.end());
    vector<dataJonh> left, right;
    for (int j = 0; j<zad.size(); j++) {
        if (zad[j].p1 < zad[j].p2)
            left.push_back(zad[j]);
        else
            right.push_back(zad[j]);
    }
    left.insert(left.end(), right.rbegin(), right.rend());
    return left;
}

int cmaxJonh(vector<dataJonh> zad) {
    int t1 = 0, t2 = 0;
    for (int j = 0; j<zad.size(); j++) {
        t1 += zad[j].p1;
        t2 = max(t2, t1) + zad[j].p2;
    }
    return max(t1, t2);
}
