#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Osoba {
    char *nazwisko, *imie;
    int wiek;
    long long pesel;
    char plec;
    struct Osoba *nastepny, *poprzedni;
};

struct ListaDwukierunkowa {
    Osoba *poczatek, *koniec;
};

Osoba *utworz_osobe(const char *nazwisko, const char *imie, int wiek, long long pesel, char plec) {
    Osoba *osoba = (Osoba*)malloc(sizeof(Osoba));
    osoba->nazwisko = strdup(nazwisko);
    osoba->imie = strdup(imie);
    osoba->wiek = wiek;
    osoba->pesel = pesel;
    osoba->plec = plec;
    osoba->nastepny = NULL;
    osoba->poprzedni = NULL;
    return osoba;
}

void dodaj_osobe(ListaDwukierunkowa *lista, const char *nazwisko, const char *imie, int wiek, long long pesel, char plec) {
    Osoba *osoba = utworz_osobe(nazwisko, imie, wiek, pesel, plec);
    if (lista->poczatek) {
        osoba->poprzedni = lista->koniec;
        lista->koniec->nastepny = osoba;
    } else {
        lista->poczatek = osoba;
    }
    lista->koniec = osoba;
}

void wyswietl_liste_od_przodu(ListaDwukierunkowa *lista) {
    cout << "Lista osob od przodu:\n";
    for (Osoba *person = lista->poczatek; person; person = person->nastepny) {
        cout << "Nazwisko: " << person->nazwisko << ", Imie: " << person->imie << ", Wiek: " << person->wiek << endl;
    }
}

void wyswietl_liste_od_tylu(ListaDwukierunkowa *lista) {
    cout << "Lista osob od tylu:\n";
    for (Osoba *person = lista->koniec; person; person = person->poprzedni) {
        cout << "Nazwisko: " << person->nazwisko << ", Imie: " << person->imie << ", Wiek: " << person->wiek << endl;
    }
}

void wyszukaj_osobe(ListaDwukierunkowa *lista, const char *klucz) {
    for (Osoba *person = lista->poczatek; person; person = person->nastepny) {
        if (strcmp(person->nazwisko, klucz) == 0 || strcmp(person->imie, klucz) == 0) {
            cout << "Znaleziono osobe: " << person->nazwisko << " " << person->imie << " " << person->wiek << " " << person->pesel << " " << person->plec << endl;
            return;
        }
    }
    cout << "Nie ma takiej osoby\n";
}

void usun_osobe(ListaDwukierunkowa *lista, const char *klucz) {
    Osoba *person = lista->poczatek;
    while (person) {
        Osoba *nastepny = person->nastepny;

        if (strcmp(person->nazwisko, klucz) == 0 || strcmp(person->imie, klucz) == 0) {
            if (person->poprzedni) person->poprzedni->nastepny = person->nastepny;
            else lista->poczatek = person->nastepny;

            if (person->nastepny) person->nastepny->poprzedni = person->poprzedni;
            else lista->koniec = person->poprzedni;

            free(person->nazwisko);
            free(person->imie);
            free(person);
            cout << "Osoba zostala usunieta.\n";
        }

        person = nastepny;
    }

    if (lista->poczatek == NULL) {
        lista->koniec = NULL; // Jeśli lista jest pusta, aktualizacja konca
    }

}

void odczytaj_z_pliku(ListaDwukierunkowa *lista, const char *nazwa_pliku) {
    FILE *plik = fopen(nazwa_pliku, "r");
    
    char nazwisko[100], imie[100];
    int wiek;
    long long pesel;
    char plec;
    while (fscanf(plik, "%s %s %d %lld %c", nazwisko, imie, &wiek, &pesel, &plec) == 5) {
        dodaj_osobe(lista, nazwisko, imie, wiek, pesel, plec);
    }
    fclose(plik);
    cout << "Dane wczytane z pliku.\n";
}

void zapisz_do_pliku(ListaDwukierunkowa *lista, const char *nazwa_pliku) {
    FILE *plik = fopen(nazwa_pliku, "w");

    for (Osoba *person = lista->poczatek; person; person = person->nastepny) {
        fprintf(plik, "%s %s %d %lld %c\n", person->nazwisko, person->imie, person->wiek, person->pesel, person->plec);
    }
    fclose(plik);
    cout << "Dane zapisane do pliku.\n";
}

void sortuj_po_nazwisku(struct ListaDwukierunkowa* lista) {
    if (!lista->poczatek || !lista->poczatek->nastepny) {
        printf("nie ma potrzeby sortowania.\n");
        return;
    }
    struct Osoba* os1 = lista->poczatek;
    struct Osoba* os2;
    while (os1->nastepny) {
        os2 = os1->nastepny;
        while (os2) {
            if (strcmp(os1->nazwisko, os2->nazwisko) > 0) {
                char* temp_nazwisko = os1->nazwisko;
                os1->nazwisko = os2->nazwisko;
                os2->nazwisko = temp_nazwisko;

                char* temp_imie = os1->imie;
                os1->imie = os2->imie;
                os2->imie = temp_imie;

                int temp_wiek = os1->wiek;
                os1->wiek = os2->wiek;
                os2->wiek = temp_wiek;

                long long temp_pesel = os1->pesel;
                os1->pesel = os2->pesel;
                os2->pesel = temp_pesel;

                char temp_plec = os1->plec;
                os1->plec = os2->plec;
                os2->plec = temp_plec;
            }
            os2 = os2->nastepny;
        }
        os1 = os1->nastepny;
    }
    printf("Lista posortowana po nazwisku.\n");
}

void wyswietl_menu() {
    cout << "1. Dodaj osobe\n"
         << "2. Wyszukaj osobe\n"
         << "3. Usun osobe\n"
         << "4. Wyswietl liste od przodu\n"
         << "5. Wyswietl liste od tylu\n"
         << "6. Odczytaj z pliku\n"
         << "7. Zapisz do pliku\n"
         << "8. Sortuj po nazwisku\n"
         << "9. Wyjdz\n";
}

int main() {
    ListaDwukierunkowa lista = {NULL, NULL};
    int wybor;
    char nazwisko[100], imie[100], klucz[100];
    int wiek;
    long long pesel;
    char plec;
    do {
        wyswietl_menu();
        cout << "Wybierz opcje: ";
        cin >> wybor;
        switch (wybor) {
            case 1:
                cout << "Podaj nazwisko: ";
                cin >> nazwisko;
                cout << "Podaj imie: ";
                cin >> imie;
                cout << "Podaj wiek: ";
                cin >> wiek;
                cout << "Podaj PESEL: ";
                cin >> pesel;
                cout << "Podaj plec: ";
                cin >> plec;
                dodaj_osobe(&lista, nazwisko, imie, wiek, pesel, plec);
                break;
            case 2:
                cout << "Podaj nazwisko lub imie do wyszukania: ";
                cin >> klucz;
                wyszukaj_osobe(&lista, klucz);
                break;
            case 3:
                cout << "Podaj nazwisko osoby do usuniecia: ";
                cin >> nazwisko;
                usun_osobe(&lista, nazwisko);
                break;
            case 4:
                wyswietl_liste_od_przodu(&lista);
                break;
            case 5:
                wyswietl_liste_od_tylu(&lista);
                break;
            case 6:
                cout << "Podaj nazwe pliku do odczytu: ";
                cin >> nazwisko;
                odczytaj_z_pliku(&lista, nazwisko);
                break;
            case 7:
                cout << "Podaj nazwe pliku do zapisu: ";
                cin >> nazwisko;
                zapisz_do_pliku(&lista, nazwisko);
                break;
            case 8:
                sortuj_po_nazwisku(&lista);
                break;
            case 9:
                cout << "Wyjscie z programu.\n";
                break;
            default:
                cout << "Nieprawidlowy wybor.\n";
        }
    } while (wybor != 9);
    return 0;
}
