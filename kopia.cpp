#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Osoba {
    char* nazwisko;
    char* imie;
    int wiek;
    long long pesel;
    char plec;

    struct Osoba* next;
    struct Osoba* prev;
};

struct ListaDwukierunkowa {
    struct Osoba* head;
    struct Osoba* tail;
};

struct Osoba* utworz_osobe(const char* nazwisko, const char* imie, int wiek, long long pesel, char plec) {
    struct Osoba* osoba = (struct Osoba*)malloc(sizeof(struct Osoba));

    osoba->nazwisko = strdup(nazwisko);
    osoba->imie = strdup(imie);
    osoba->wiek = wiek;
    osoba->pesel = pesel;
    osoba->plec = plec;

    osoba->next = NULL;
    osoba->prev = NULL;

    return osoba;
}

void dodaj_osobe(struct ListaDwukierunkowa* lista, const char* nazwisko, const char* imie, int wiek, long long pesel, char plec) {
    struct Osoba* osoba = utworz_osobe(nazwisko, imie, wiek, pesel, plec);

    if (!lista->head) {
        lista->head = osoba;
        lista->tail = osoba;
    } else {
        osoba->prev = lista->tail;
        lista->tail->next = osoba;
        lista->tail = osoba;
    }
}

void wyszukaj_osobe(struct ListaDwukierunkowa* lista, const char* klucz) {
    struct Osoba* temp = lista->head;
    while (temp) {
        if (strcmp(temp->nazwisko, klucz) == 0 || strcmp(temp->imie, klucz) == 0) {
            printf("Znaleziono osobę:\n");
            printf("Nazwisko: %s\n", temp->nazwisko);
            printf("Imię: %s\n", temp->imie);
            printf("Wiek: %d\n", temp->wiek);
            printf("PESEL: %lld\n", temp->pesel);
            printf("Płeć (k/m): %c\n", temp->plec);
            return;
        }
        temp = temp->next;
    }

    printf("Osoba o podanym nazwisku/imieniu nie została znaleziona.\n");
}

void usun_osobe(struct ListaDwukierunkowa* lista, const char* nazwisko) {
    struct Osoba* temp = lista->head;

    while (temp) {
        if (strcmp(temp->nazwisko, nazwisko) == 0) {
            if (temp->prev)
                temp->prev->next = temp->next;
            else
                lista->head = temp->next;

            if (temp->next)
                temp->next->prev = temp->prev;
            else
                lista->tail = temp->prev;

            free(temp->nazwisko);
            free(temp->imie);
            free(temp);
            printf("Osoba została usunięta.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Osoba o podanym nazwisku nie została znaleziona.\n");
}

void wyswietl_liste_od_przodu(struct ListaDwukierunkowa* lista) {
    struct Osoba* temp = lista->head;

    while (temp) {
        printf("Nazwisko: %s, Imię: %s, Wiek: %d\n", temp->nazwisko, temp->imie, temp->wiek);
        temp = temp->next;
    }
}

void wyswietl_liste_od_tylu(struct ListaDwukierunkowa* lista) {
    struct Osoba* temp = lista->tail;

    while (temp) {
        printf("Nazwisko: %s, Imię: %s, Wiek: %d\n", temp->nazwisko, temp->imie, temp->wiek);
        temp = temp->prev;
    }
}

void odczytaj_z_pliku(struct ListaDwukierunkowa* lista, const char* nazwa_pliku) {
    FILE* plik = fopen(nazwa_pliku, "r");

    if (!plik) {
        printf("Błąd otwarcia pliku.\n");
        return;
    }

    char nazwisko[100], imie[100];
    int wiek;
    long long pesel;
    char plec;

    while (fscanf(plik, "%s %s %d %lld %c", nazwisko, imie, &wiek, &pesel, &plec) == 5) {
        dodaj_osobe(lista, nazwisko, imie, wiek, pesel, plec);
    }

    fclose(plik);
    printf("Dane wczytane z pliku.\n");
}

void zapisz_do_pliku(struct ListaDwukierunkowa* lista, const char* nazwa_pliku) {
    FILE* plik = fopen(nazwa_pliku, "w");

    if (!plik) {
        printf("Błąd otwarcia pliku.\n");
        return;
    }

    struct Osoba* temp = lista->head;

    while (temp) {
        fprintf(plik, "%s %s %d %lld %c\n", temp->nazwisko, temp->imie, temp->wiek, temp->pesel, temp->plec);
        temp = temp->next;
    }

    fclose(plik);
    printf("Dane zapisane do pliku.\n");
}
//bubble sort
void sortuj_po_nazwisku(struct ListaDwukierunkowa* lista) {
    if (!lista->head || !lista->head->next) {
        printf("Lista jest pusta lub zawiera tylko jeden element, nie ma potrzeby sortowania.\n");
        return;
    }

    struct Osoba* i = lista->head;
    struct Osoba* j;

    while (i->next) {
        j = i->next;

        while (j) {
            if (strcmp(i->nazwisko, j->nazwisko) > 0) {
                char* temp_nazwisko = i->nazwisko;
                i->nazwisko = j->nazwisko;
                j->nazwisko = temp_nazwisko;

                char* temp_imie = i->imie;
                i->imie = j->imie;
                j->imie = temp_imie;

                int temp_wiek = i->wiek;
                i->wiek = j->wiek;
                j->wiek = temp_wiek;

                long long temp_pesel = i->pesel;
                i->pesel = j->pesel;
                j->pesel = temp_pesel;

                char temp_plec = i->plec;
                i->plec = j->plec;
                j->plec = temp_plec;
            }

            j = j->next;
        }

        i = i->next;
    }

    printf("Lista posortowana po nazwisku.\n");
}

void wyswietl_menu() {
    printf("1. Dodaj osobe\n");
    printf("2. Wyszukaj osobe\n");
    printf("3. Usun osobe\n");
    printf("4. Wyswietl liste od przodu\n");
    printf("5. Wyswietl liste od tylu\n");
    printf("6. Odczytaj z pliku\n");
    printf("7. Zapisz do pliku\n");
    printf("8. Sortuj po nazwisku\n");
    printf("9. Wyjdz\n");
}

int main() {
    struct ListaDwukierunkowa lista;
    int wybor;
    char nazwisko[100], imie[100], klucz[100];
    int wiek;
    long long pesel;
    char plec;

    do {
        wyswietl_menu();
        printf("Wybierz opcje: ");
        scanf("%d", &wybor);

        switch (wybor) {
            case 1:
                printf("Podaj nazwisko: ");
                scanf("%s", nazwisko);
                printf("Podaj imie: ");
                scanf("%s", imie);
                printf("Podaj wiek: ");
                scanf("%d", &wiek);
                printf("Podaj PESEL: ");
                scanf("%lld", &pesel);
                printf("Podaj plec: ");
                scanf(" %c", &plec);  // Uwaga: spacja przed %c pomija białe znaki, np. Enter
                dodaj_osobe(&lista, nazwisko, imie, wiek, pesel, plec);
                break;
            case 2:
                printf("Podaj nazwisko lub imie do wyszukania: ");
                scanf("%s", klucz);
                wyszukaj_osobe(&lista, klucz);
                break;
            case 3:
                printf("Podaj nazwisko osoby do usuniecia: ");
                scanf("%s", nazwisko);
                usun_osobe(&lista, nazwisko);
                break;
            case 4:
                wyswietl_liste_od_przodu(&lista);
                break;
            case 5:
                wyswietl_liste_od_tylu(&lista);
                break;
            case 6:
                printf("Podaj nazwe pliku do odczytu: ");
                scanf("%s", nazwisko);
                odczytaj_z_pliku(&lista, nazwisko);
                break;
            case 7:
                printf("Podaj nazwe pliku do zapisu: ");
                scanf("%s", nazwisko);
                zapisz_do_pliku(&lista, nazwisko);
                break;
            case 8:
                sortuj_po_nazwisku(&lista);
                break;
            case 9:
                printf("Wyjscie z programu.\n");
                break;
            default:
                printf("Nieprawidlowy wybor.\n");
        }

    } while (wybor != 9);

    return 0;
}
