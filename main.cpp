#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

struct pole {
    int wartosc;                    //liczba bomb w pobliżu (9=bomba)
    bool czyOdkryte;                   // czy pole jest odkryte
};

void graj();

void wybierzPoziomTrudnosci(int *poziom, int *bokPlanszy, int *liczbaBomb, int *liczbaPodpowiedzi, int *polaDoOdkrycia);

void generujPlansze(const int *bokplanszy, pole **plansza);

void losujWspolrzedne(const int *liczbaBomb, const int *bokPlanszy, pole **plansza);

void automatyczneZakonczenie(int *bokPlanszy, pole **plansza);

void podajwspolrzedne(int *bokPlanszy, pole **plansza, bool *wygrana, bool *przegrana, int *poziom, int *x, int *y,
                      int *polaDoOdkrycia);

void udzielPodpowiedzi(int *x, int *y, pole **plansza, int *bokPlanszy, int *polaDoOdkrycia);

void wyswietlPlansze(pole **plansza, int *bokPlanszy);

void odkryjPola(int *polaDoOdkrycia, const int *x, int *y, pole **plansza, int *bokPlanszy);

void przegrales(pole **plansza, int *bokPlanszy);

void wygrales(pole **plansza, int *bokPlanszy);

void zapisWynikow(bool *przegrana, bool *wygrana, int *poziom);

void pokazWyniki();

void menu();

void menu2();

void menu3(int *bokPlanszy, pole **plansza, bool *wygrana, bool *przegrana, int *poziom, int *liczbapodpowiedzi,
           int *polaDoOdkrycia, int *x, int *y);


void zagrajPonownie();


int main() {
    menu();
}

void menu() {
    char lit;

    cout << "                                 ****************************************************************" << endl;
    cout << "                                 ****************************************************************" << endl;
    cout << "                                 **                                                            **" << endl;
    cout << "                                 **                            SAPER                           **" << endl;
    cout << "                                 **                                                            **" << endl;
    cout << "                                 ****************************************************************" << endl;
    cout << "                                 ****************************************************************" << endl;
    cout << "MENU: " << endl;
    cout << " G - nowa gra" << endl;
    cout << " W - wyświetl wyniki" << endl;
    cout << " E - wyjście" << endl << endl;
    cin >> lit;
    cout << endl;

    if ((lit != 'g') & (lit != 'G') & (lit != 'w') & (lit != 'W') & (lit != 'e') & (lit != 'E')) {
        cout << "Niewłaściwy wybór, spróbuj jeszcze raz" << endl;
        menu();
    } else if (lit == 'g' || lit == 'G') {
        graj();
    } else if (lit == 'e' || lit == 'E') {
        exit(0);
    } else if (lit == 'w' || lit == 'W') {
        pokazWyniki();
    }
    menu2();

}

void menu2() {

    char lit;
    cout << " P - Powrót do MENU" << endl;
    cout << " E - Wyjście" << endl;
    cin >> lit;
    if ((lit != 'p') & (lit != 'p') & (lit != 'e') & (lit != 'E')) {
        cout << "Niewłaściwy wybór, spróbuj jeszcze raz" << endl;
        menu2();
    } else if (lit == 'p' || lit == 'P') {
        menu();
    } else if (lit == 'e' || lit == 'E') {
        exit(0);
    }

}

void menu3(int *bokPlanszy, pole **plansza, bool *wygrana, bool *przegrana, int *poziom,
           int *liczbapodpowiedzi, int *polaDoOdkrycia, int *x, int *y) {
    char lit;
    if (*liczbapodpowiedzi > 0) {
        cout<< " Liczba dostępnych podpowiedzi " << *liczbapodpowiedzi<<endl<<endl;

        cout << " P - Podpowiedź " << endl;
    }
    cout << " A - Rozwiąż automatycznie" << endl;
    cout << " W - Podaj współrzędne" << endl;
    cout << " E - Wyjście" << endl << endl;
    cin >> lit;
    cout << endl;

    if ((lit != 'p') & (lit != 'p') & (lit != 'e') & (lit != 'E') & (lit != 'w') & (lit != 'W') & (lit != 'a') &
        (lit != 'A')) {
        cout << "Niewłaściwy wybór, spróbuj jeszcze raz" << endl;
        menu3(bokPlanszy, plansza, wygrana, przegrana, poziom, liczbapodpowiedzi, polaDoOdkrycia, x, y);
    } else if (lit == 'p' || lit == 'P') {
        if (*liczbapodpowiedzi > 0) {
            udzielPodpowiedzi(x, y, plansza, bokPlanszy, polaDoOdkrycia);
            *liczbapodpowiedzi -= 1;
            wyswietlPlansze(plansza, bokPlanszy);
            menu3(bokPlanszy, plansza, wygrana, przegrana, poziom, liczbapodpowiedzi, polaDoOdkrycia, x, y);
        }else if (*liczbapodpowiedzi==0){
            menu3(bokPlanszy, plansza, wygrana, przegrana, poziom, liczbapodpowiedzi, polaDoOdkrycia, x, y);
        }

    } else if (lit == 'a' || lit == 'A') {
        automatyczneZakonczenie(bokPlanszy, plansza);
    } else if (lit == 'w' || lit == 'W') {
        podajwspolrzedne(bokPlanszy, plansza, wygrana, przegrana, poziom, x, y, polaDoOdkrycia);
        wyswietlPlansze(plansza, bokPlanszy);
        menu3(bokPlanszy, plansza, wygrana, przegrana, poziom, liczbapodpowiedzi, polaDoOdkrycia, x, y);
    } else if (lit == 'e' || lit == 'E') {
        exit(0);
    }
}

void pokazWyniki() {
    string line;
    ifstream file("wyniki.txt");
    if (file.is_open()) {
        while (getline(file, line))
            cout << line << '\n';
        file.close();
    } else {
        cout << "Brak wyników" << endl;
    }
    cout << endl;
}


void
wybierzPoziomTrudnosci(int *poziom, int *bokPlanszy, int *liczbaBomb, int *liczbaPodpowiedzi, int *polaDoOdkrycia) {

    cout << "Wybierz poziom trudności: \n "
            "1 - Początkujący\n "
            "2 - Zaawansowany\n "
            "3 - Ekspert" << endl << endl;

    cin >> *poziom;
    if ((*poziom != 1) & (*poziom != 2) & (*poziom != 3)) {
        cout << "Podałeś nieprawidłowy poziom, spróbuj jeszcze raz. " << endl;

        wybierzPoziomTrudnosci(poziom, bokPlanszy, liczbaBomb, liczbaPodpowiedzi, polaDoOdkrycia);
    }

    if (*poziom == 1) {
        *bokPlanszy = 5;
        *liczbaBomb = 3;
        *liczbaPodpowiedzi = 1;
        *polaDoOdkrycia = (*bokPlanszy) * (*bokPlanszy) - (*liczbaBomb);
    } else if (*poziom == 2) {
        *bokPlanszy = 10;
        *liczbaBomb = 14;
        *liczbaPodpowiedzi = 3;
        *polaDoOdkrycia = (*bokPlanszy) * (*bokPlanszy) - (*liczbaBomb);
    } else if (*poziom == 3) {
        *bokPlanszy = 15;
        *liczbaBomb = 35;
        *liczbaPodpowiedzi = 5;
        *polaDoOdkrycia = (*bokPlanszy) * (*bokPlanszy) - (*liczbaBomb);
    }

    cout << "Wybrałeś poziom " << *poziom << endl;
    cout << "Plansza ma rozmiar: " << *bokPlanszy - 1 << " X " << *bokPlanszy - 1 << endl;
    cout << "Znajduje się na niej " << *liczbaBomb << "  bomb" << endl;
    cout << "Liczba dostępnych podpowiedzi " << *liczbaPodpowiedzi << endl << "Powodzenia!" << endl << endl;

}

void graj() {

    int bokPlanszy = 0;
    int liczbaBomb = 0;
    int poziom = 0;
    int liczbaPodpowiedzi = 0;
    pole **plansza;
    bool wygrana = false;
    bool przegrana = false;
    int polaDoOdkrycia = 0;
    int x = 0;
    int y = 0;

    wybierzPoziomTrudnosci(&poziom, &bokPlanszy, &liczbaBomb, &liczbaPodpowiedzi, &polaDoOdkrycia);

    plansza = new pole *[bokPlanszy];               //tworzy tablice dwuwymiarową (plansza do gry)
    for (int i = 0; i < bokPlanszy; i++)
        plansza[i] = new pole[bokPlanszy];

    generujPlansze(&bokPlanszy, plansza);
    losujWspolrzedne(&liczbaBomb, &bokPlanszy, plansza);
    wyswietlPlansze(plansza, &bokPlanszy);
    menu3(&bokPlanszy, plansza, &wygrana, &przegrana, &poziom, &liczbaPodpowiedzi, &polaDoOdkrycia, &x, &y);
}

void generujPlansze(const int *bokPlanszy, pole **plansza) {

    for (int i = 0; i < *bokPlanszy; i++) {
        for (int j = 0; j < *bokPlanszy; j++) {
            plansza[i][j].wartosc = 0;
            plansza[i][j].czyOdkryte = false;
        }
    }
}

void losujWspolrzedne(const int *liczbaBomb, const int *bokPlanszy, pole **plansza) {
    int wspX;
    int wspY;
    int bombyDoUstawienia = *liczbaBomb;

    srand(time(nullptr));
    while (bombyDoUstawienia > 0) {            //losowo wybiera współrzędne dla bomb
        wspX = (random() % (*bokPlanszy)) + 0;
        wspY = (random() % (*bokPlanszy)) + 0;


        if (plansza[wspY][wspX].wartosc != 9) {

            plansza[wspY][wspX].wartosc = 9;                                   //ustawia minę

            for (int i = wspY - 1; i <= wspY + 1; i++) {                                //zmiana wartości pól wokół miny
                for (int j = wspX - 1; j <= wspX + 1; j++) {
                    if ((i < 0) || (j < 0))continue;                               //poza planszą
                    if ((i > *bokPlanszy - 1) || (j > *bokPlanszy - 1))continue;   //poza planszą
                    if ((plansza[i][j].wartosc) == 9)continue;                   //na pozycji znajduje się bomba
                    plansza[i][j].wartosc += 1;
                }
            }
            bombyDoUstawienia--;
        }

    }
}

void automatyczneZakonczenie(int *bokPlanszy, pole **plansza) {
    for (int i = 0; i < *bokPlanszy; i++) {
        for (int j = 0; j < *bokPlanszy; j++) {
            plansza[i][j].czyOdkryte = true;
        }
    }
    wyswietlPlansze(plansza, bokPlanszy);
    zagrajPonownie();
}

void podajwspolrzedne(int *bokPlanszy, pole **plansza, bool *wygrana, bool *przegrana, int *poziom, int *x, int *y,
                      int *polaDoOdkrycia) {

    cout << " Podaj współrzędne pola ( liczby z zakresu 0 - " << (*bokPlanszy) - 1 << ")" << endl;
    cout << "Numer kolumny: ";
    cin >> *x;
    cout << "Numer wiersza: ";
    cin >> *y;

    if (((*x > *bokPlanszy - 1) || *x < 0) ||
        ((*y > *bokPlanszy - 1) || *y < 0)) {                                             //walidacja współrzędnych
        cout << " Podałeś nieprawidłowe współrzędne. Spróbuj jeszcze raz " << endl;
        podajwspolrzedne(bokPlanszy, plansza, wygrana, przegrana, poziom, x, y, polaDoOdkrycia);
    }
    if (*polaDoOdkrycia > 0) {
        if (plansza[*y][*x].wartosc != 9) {
            odkryjPola(polaDoOdkrycia, x, y, plansza, bokPlanszy);
        } else {
            *przegrana = true;
            zapisWynikow(przegrana, wygrana, poziom);
            przegrales(plansza, bokPlanszy);
        }
    }

    if (*polaDoOdkrycia == 0) {
        *wygrana = true;
        zapisWynikow(przegrana, wygrana, poziom);
        wygrales(plansza, bokPlanszy);
    }
}

void udzielPodpowiedzi(int *x, int *y, pole **plansza, int *bokPlanszy, int *polaDoOdkrycia) {
    int wspX = *x;
    int wspY = *y;
    int najmOdleglosc = 100;
    int najmX = 15;
    int najmY = 15;
    for (int i = 0; i < *bokPlanszy; i++) {
        for (int j = 0; j < *bokPlanszy; j++) {
            if ((plansza[i][j].wartosc == 9) && (plansza[i][j].czyOdkryte == false)) {
                int odleglosc = (abs(wspY - i)) + (abs(wspX - j));
                if (odleglosc < najmOdleglosc) {
                    najmOdleglosc = odleglosc;
                    najmY = i;
                    najmX = j;
                }
            }
        }
    }
    plansza[najmY][najmX].czyOdkryte = true;
    cout << "Najbliższa bomba znajduje się na pozycji (" << najmX << "," << najmY << ")" << endl << endl;

    int n_wspX = najmX;
    int n_wspY = najmY - 1;
    odkryjPola(polaDoOdkrycia, &n_wspX, &n_wspY, plansza, bokPlanszy);

    int s_wspX = najmX;
    int s_wspY = najmY + 1;
    odkryjPola(polaDoOdkrycia, &s_wspX, &s_wspY, plansza, bokPlanszy);

    int e_wspX = najmX + 1;
    int e_wspY = najmY;
    odkryjPola(polaDoOdkrycia, &e_wspX, &e_wspY, plansza, bokPlanszy);

    int w_wspX = najmX - 1;
    int w_wspY = najmY;
    odkryjPola(polaDoOdkrycia, &w_wspX, &w_wspY, plansza, bokPlanszy);

    int ne_wspX = najmX + 1;
    int ne_wspY = najmY - 1;
    odkryjPola(polaDoOdkrycia, &ne_wspX, &ne_wspY, plansza, bokPlanszy);

    int se_wspX = najmX + 1;
    int se_wspY = najmY + 1;
    odkryjPola(polaDoOdkrycia, &se_wspX, &se_wspY, plansza, bokPlanszy);

    int sw_wspX = najmX - 1;
    int sw_wspY = najmY + 1;
    odkryjPola(polaDoOdkrycia, &sw_wspX, &sw_wspY, plansza, bokPlanszy);

    int nw_wspX = najmX - 1;
    int nw_wspY = najmY - 1;
    odkryjPola(polaDoOdkrycia, &nw_wspX, &nw_wspY, plansza, bokPlanszy);
}

void odkryjPola(int *polaDoOdkrycia, const int *x, int *y, pole **plansza, int *bokPlanszy) {
    int wspX = *x;
    int wspY = *y;
    if ((wspX < 0) || (wspY < 0))return;
    if ((wspX > *bokPlanszy - 1) || (wspY > *bokPlanszy - 1))return;
    if (!plansza[wspY][wspX].czyOdkryte && (plansza[wspY][wspX].wartosc != 9)) {
        plansza[wspY][wspX].czyOdkryte = true;
        *polaDoOdkrycia -= 1;

        if (plansza[wspY][wspX].wartosc != 0)return;

        int n_wspX = wspX;
        int n_wspY = wspY - 1;
        odkryjPola(polaDoOdkrycia, &n_wspX, &n_wspY, plansza, bokPlanszy);

        int s_wspX = wspX;
        int s_wspY = wspY + 1;
        odkryjPola(polaDoOdkrycia, &s_wspX, &s_wspY, plansza, bokPlanszy);

        int e_wspX = wspX + 1;
        int e_wspY = wspY;
        odkryjPola(polaDoOdkrycia, &e_wspX, &e_wspY, plansza, bokPlanszy);

        int w_wspX = wspX - 1;
        int w_wspY = wspY;
        odkryjPola(polaDoOdkrycia, &w_wspX, &w_wspY, plansza, bokPlanszy);

        int ne_wspX = wspX + 1;
        int ne_wspY = wspY - 1;
        odkryjPola(polaDoOdkrycia, &ne_wspX, &ne_wspY, plansza, bokPlanszy);

        int se_wspX = wspX + 1;
        int se_wspY = wspY + 1;
        odkryjPola(polaDoOdkrycia, &se_wspX, &se_wspY, plansza, bokPlanszy);

        int sw_wspX = wspX - 1;
        int sw_wspY = wspY + 1;
        odkryjPola(polaDoOdkrycia, &sw_wspX, &sw_wspY, plansza, bokPlanszy);

        int nw_wspX = wspX - 1;
        int nw_wspY = wspY - 1;
        odkryjPola(polaDoOdkrycia, &nw_wspX, &nw_wspY, plansza, bokPlanszy);
    }
}

void wyswietlPlansze(pole **plansza, int *bokPlanszy) {
    cout << setw(35);
    for (int i = 0; i < *bokPlanszy; i++) {
        if (i < 9) {
            cout << i << "   ";
        } else
            cout << i << "  ";
    }
    cout << endl;

    cout << setw(36);
    for (int i = 0; i < *bokPlanszy; i++) {
        cout << "____";
    }
    cout << endl;
    for (int i = 0; i < *bokPlanszy; i++) {
        cout << setw(30);
        cout << i << " | ";
        for (int j = 0; j < *bokPlanszy; j++) {
            if (plansza[i][j].czyOdkryte) {
                if (plansza[i][j].wartosc == 0) {
                    cout << "    ";
                } else if (plansza[i][j].wartosc == 9) {
                    cout << " *  ";
                } else
                    cout << " " << plansza[i][j].wartosc << "  ";
            } else
                cout << " -  ";
        }
        cout << endl;
    }
    cout << endl;
}

void przegrales(pole **plansza, int *bokPlanszy) {

    cout << "                  ****************************************************************" << endl;
    cout << "                  *                                                              *" << endl;
    cout << "                  *                             BOMBA!!                          *" << endl;
    cout << "                  *                            PRZEGRANA                         *" << endl;
    cout << "                  *                                                              *" << endl;
    cout << "                  ****************************************************************" << endl << endl;


    for (int i = 0; i < *bokPlanszy; i++) {
        for (int j = 0; j < *bokPlanszy; j++) {
            plansza[i][j].czyOdkryte = true;
        }
    }
    wyswietlPlansze(plansza, bokPlanszy);
    zagrajPonownie();
}


void wygrales(pole **plansza, int *bokPlanszy) {

    cout << "                  ****************************************************************" << endl;
    cout << "                  *                                                              *" << endl;
    cout << "                  *                             BRAWO!!                          *" << endl;
    cout << "                  *                             WYGRANA                          *" << endl;
    cout << "                  *                                                              *" << endl;
    cout << "                  ****************************************************************" << endl << endl;

    for (int i = 0; i < *bokPlanszy; i++) {
        for (int j = 0; j < *bokPlanszy; j++) {
            plansza[i][j].czyOdkryte = true;
        }
    }
    wyswietlPlansze(plansza, bokPlanszy);
    zagrajPonownie();
}


void zapisWynikow(bool *przegrana, bool *wygrana, int *poziom) {
    fstream plik;
    plik.open("wyniki.txt", ios::out | ios::app);

    if (*poziom == 1) {
        plik << "Poziom: Początkujący" << endl;
    } else if (*poziom == 2) {
        plik << "Poziom: Zaawansowany" << endl;
    }
    if (*poziom == 3) {
        plik << "Poziom: Ekspert" << endl;
    }
    if (*wygrana) {
        plik << "Wynik gry: wygrana" << endl << endl;
    }
    if (*przegrana) {
        plik << "Wynik gry: przegrana" << endl << endl;
    }
    plik.close();
}

void zagrajPonownie() {
    char lit;
    cout << "Czy chcesz zagrać ponownie ? (T/N) " << endl;
    cin >> lit;
    if ((lit == 't') || (lit == 'T')) {
        graj();
    } else if (lit == 'n' || lit == 'N')
        exit(0);
}
