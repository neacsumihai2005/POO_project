/*
    Am 3 clase:
    Antrenament: tine:
        -data efectuarii
        -o lista de Bubbles

    Bubble: tine un Exercitiu impreuna cu greutatea efectuata si o lista de seturi, exemplu:
    flotari: 80kg
    10 20 30
    inseamna ca s-au facut 3 seturi de 10, 20, 30 repetitii cu 80kg
    variaza kilogramele la fiecare exercitiu pt ca se pot adauga veste cu greutate care sa adauge kg; noi notam aici
    kilogramele totale

    Exercitiu: tine numele unui exercitiu si recordurile pentru fiecare greutate efectuata, printr-un map
    --in editiile urmatoare va tine si descrierea fiecarui exercitiu

    deci Exercitiul e ceva general, pe cand un Bubble e ceva mai local, efectuata in fiecare antrenament

    planuri pt editiile urmatoare:
    sa aranjez mai bine clasele
    sa adaug functii noi la clasa de Exercitiu si la clasa de Antrenament (de exemplu, observatii pt cum m-am simtit
                                                                           in timpul acelui antrenament sau cat de grele
                                                                           s-au simtit exercitiile)
    sa adaug o clasa noua de Jurnal, care sa fie practic baza tuturora si sa le lege impreuna mai bine

*/
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <assert.h>

using namespace std;


class Exercitiu;
class Bubble;
class Antrenament;


struct data_calendar{
    int zi;
    int luna;
    int an;

    friend ostream& operator << (ostream& out, data_calendar const& X){
        out << X.zi << ' ' << X.luna << ' ' << X.an << "\n";
    }
};

/*
struct Set_Exercitiu{
    int greutate;
    int repetitii;
};
*/
class Exercitiu{
    private:
        string nume;
        string descriere;
        map<int, int> record_reps; ///record_reps[greutate] = maximul de repetari cu greutatea aia


    public:
        Exercitiu(){
        }
        Exercitiu(string ex){
            nume = ex;
        }
        Exercitiu(string ex, string desc){
            nume = ex;
            descriere = desc;
        }

        tryToUpdatePR(int greutate, int nr_repetari){
            if(record_reps.find(greutate) == record_reps.end()){
                ///e prima oara cand fac cu greutatea asta deci devine record pt greutatea respectiva
                record_reps[greutate] = nr_repetari;
            }
            else {
                record_reps[greutate] = max(record_reps[greutate],
                                            nr_repetari
                                            );
            }
        }

        string getNume(){
            return nume;
        }

        void printRecords(){
            cout << "Recorduri la " << nume << ":\n";
            for(auto it : record_reps){
                cout << it.second << " repetitii cu " << it.first << " kg\n";
            }
            cout << "\n";
        }

        friend class Bubble;
};


class Bubble{
    private:
        ///Exercitiu exercitiul_curent;
        string nume_exercitiu;
        int greutate;
        vector<int> lista_repetitii;

    public:
        ~Bubble(){
            nume_exercitiu.erase();
            lista_repetitii.clear();
        }
        Bubble (const Bubble& t){
            nume_exercitiu = t.nume_exercitiu;
            greutate = t.greutate;

            lista_repetitii.clear();
            for(int i = 0; i < t.lista_repetitii.size(); i++){
                lista_repetitii.push_back(t.lista_repetitii[i]);
            }
        }

        Bubble (string ex, auto &listaExercitii){
            nume_exercitiu = ex;
            /*
                daca ex nu se afla in lista de exercitii ii aloc spatiul
            */
            if( listaExercitii.find(ex) == listaExercitii.end() ){
                ///am un exercitiu ne-mai-intalnit
                listaExercitii.insert(make_pair(ex, Exercitiu(ex)));
            }
            /*else {

            }*/
        }

        void setGreutate(int X){
            greutate = X;
        }
        string getExercitiu(){
            return nume_exercitiu;
        }
        void afis_reps(){
            for(int i = 0; i < lista_repetitii.size(); i++){
                cout << lista_repetitii[i] << ' ';
            }
        }
        void adaugaSet(string ex, int greutate, int reps, auto &listaExercitii){
            lista_repetitii.push_back(reps);

            listaExercitii[ex].tryToUpdatePR(greutate, reps);
        }

        friend ostream& operator << (ostream& out, Bubble &X){
            cout << X.nume_exercitiu << " : ";
            for(int i = 0; i < X.lista_repetitii.size(); i++){
                cout << X.lista_repetitii[i] << ' ';
            }
            cout << "\n";
        }


        operator=(Bubble &t){
            nume_exercitiu = t.nume_exercitiu;
            greutate = t.greutate;

            lista_repetitii.clear();
            for(int i = 0; i < t.lista_repetitii.size(); i++){
                lista_repetitii.push_back(t.lista_repetitii[i]);
            }
        }



        friend class Jurnal;
        friend class Exercitiu;


};

class Antrenament{
    private:
        data_calendar when;
        vector<Bubble> lista_Bubbles;
    public:
        Antrenament(int z, int l, int a){
            when.zi = z;
            when.luna = l;
            when.an = a;
        }

        void adaugaBubble(Bubble X){
            lista_Bubbles.push_back(X);
        }

        friend ostream& operator << (ostream& out, Antrenament & X){
            out << "Antrenamentul din " << X.when << ":\n";
            for(int i = 0; i < X.lista_Bubbles.size(); i++){
                cout << X.lista_Bubbles[i].getExercitiu() << " : ";
                X.lista_Bubbles[i].afis_reps();
                cout << "\n";
            }
            cout << "\n";
        }
};


/*
class Set_Exercitiu{
    private:
        string nume;
        int greutate;
        int repetitii;

    public:

};*/


inline void afisare_optiuni_0(){
    cout << "1 = inceput antrenament nou" << "\n";
    cout << "2 = afisare recorduri" << "\n";
}
inline void afisare_optiuni_2(){
    cout << "1 = afisarea tuturor recordurilor" << "\n";
    cout << "2 = afisarea unui record specific" << "\n";
}

void start_antrenament(auto &listaExercitii){

    cout << "Ziua = "; int ziua; cin >> ziua;
    cout << "Luna = "; int luna; cin >> luna;
    cout << "Anul = "; int anul; cin >> anul;
    Antrenament antrenament_nou(ziua, luna, anul);

    cout << "Denumire exercitiu curent = ";
    string denumire_exercitiu;
    cin >> denumire_exercitiu;

    while(denumire_exercitiu != "stop"){
        Bubble bubble_nou(denumire_exercitiu, listaExercitii);

        cout << "Greutatea (kg) = ";
        int greutate;
        cin >> greutate;
        bubble_nou.setGreutate(greutate);

        cout << "Set de ";
        int nr_repetari;
        cin >> nr_repetari;

        int nr_seturi = 0;
        while(nr_repetari != 0){
            bubble_nou.adaugaSet(denumire_exercitiu, greutate, nr_repetari, listaExercitii);

            cout << "Set de ";
            cin >> nr_repetari;
        }

        cout << bubble_nou << "\n";

        antrenament_nou.adaugaBubble(bubble_nou);


        cout << "Denumire exercitiu curent (sau scrie stop pt a opri) = ";
        cin >> denumire_exercitiu;
    }

    ///listaAntrenamente.push_back(antrenament_nou);
}

int main()
{
    ifstream fin ("test.in");
    ofstream fout ("test.out");

    map<string, Exercitiu> listaExercitii; ///lista exercitiilor existente

    afisare_optiuni_0();
    int optiune;
    cin >> optiune;
    assert(1 <= optiune && optiune <= 2);

    while(1 <= optiune && optiune <= 2){

        if(optiune == 1){ ///antrenament nou
            start_antrenament(listaExercitii);
        }
        else if(optiune == 2){ ///afisare recorduri

            afisare_optiuni_2();
            cin >> optiune;
            assert(optiune == 1 || optiune == 2);

            if(optiune == 1){ ///afisez toate recordurile
                for(auto it : listaExercitii){
                    it.second.printRecords();
                }
            }
            else { ///introduc exercitiul pt care vreau sa vad recordul
                cout << "Afiseaza recordurile pt exercitiul: ";
                string exercitiu;
                cin >> exercitiu;

                if(listaExercitii.find(exercitiu) != listaExercitii.end()){
                    listaExercitii[exercitiu].printRecords();
                }
                else {
                    cout << "Nu am facut inca exercitiul asta\n";
                }
                /*{for x in lista_exercitii:
                    cout << i << ' ' << x << "\n";

                cin >> numar
                cout << record[exercitiu[numar]] << "\n";

                }*/
            }
        }

        afisare_optiuni_0();
        cin >> optiune;
    }
    return 0;
}
