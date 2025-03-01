#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct osoba {
    char imie[50];
    int wiek;
    double pesel;
    struct osoba *next;
    struct osoba *prev;
};

struct osoba *first = NULL;
struct osoba *last = NULL;
struct osoba *current = NULL;

bool isEmpty() {
    return first == NULL;
}

void add() {
    int x = 0;
    int wiek;
    double pesel;

        struct osoba *dodaj = (struct osoba *)malloc(sizeof(struct osoba));

        printf("Podaj imie: \n");
        scanf("%s", dodaj->imie);

        printf("Podaj wiek: \n");
        scanf("%d", &wiek);
        dodaj->wiek = wiek;

        printf("Podaj pesel: \n");
        scanf("%lf", &pesel);
        dodaj->pesel = pesel;

        if (isEmpty()) {
            first = dodaj;
            last = dodaj;
        } else {
            last->next = dodaj;
            dodaj->prev = last;
            last = dodaj;
        }
    
}

void searchByName() {
    char targetName[50];
    printf("Podaj imie do wyszukania: \n");
    scanf("%s", targetName);

    struct osoba *current = first;
    bool found = false;

    while (current != NULL) {
        if (strcmp(current->imie, targetName) == 0) {
            printf("Imie: %s, Wiek: %d, Pesel: %lf\n", current->imie, current->wiek, current->pesel);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("Nie znaleziono osoby o imieniu %s.\n", targetName);
    }
}

void deleteByName() {
    char targetName[50];
    printf("Podaj imie do usuniecia: \n");
    scanf("%s", targetName);

    struct osoba *current = first;
    struct osoba *temp;

    while (current != NULL) {
        if (strcmp(current->imie, targetName) == 0) {
            if (current == first) {
                first = current->next;
                if (first != NULL) {
                    first->prev = NULL;
                } else {
                    last = NULL;
                }
            } else if (current == last) {
                last = current->prev;
                last->next = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            temp = current;
            current = current->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    printf("Osoba/usunieto osoby o imieniu %s.\n", targetName);
}


void viewForward() {
    struct osoba *ptr = first;

    printf("\n");
    while (ptr != NULL) {
        printf("Imie: %s, Wiek: %d, Pesel: %lf\n", ptr->imie, ptr->wiek, ptr->pesel);
        ptr = ptr->next;
    }
}

void viewBackwards() {
    struct osoba *ptr = last;

    printf("\n");
    while (ptr != NULL) {
        printf("Imie: %s, Wiek: %d, Pesel: %lf\n", ptr->imie, ptr->wiek, ptr->pesel);
        ptr = ptr->prev;
    }
}

void saveToFile() {
    FILE *file = fopen("file1.txt", "w");
    struct osoba *current = first;

    while (current != NULL) {
        fprintf(file, "%s %d %lf\n", current->imie, current->wiek, current->pesel);
        current = current->next;
    }

    fclose(file);
    printf("Zapisano liste do pliku");
}

void readFromFile() {
    FILE *file = fopen("file1.txt", "r");

    if (file == NULL) {
        printf("Nie udalo sie otworzyc pliku do odczytu.\n");
        return;
    }

    struct osoba *newPerson;
    struct osoba *current = last;

    while ((newPerson = (struct osoba *)malloc(sizeof(struct osoba))) != NULL &&
           fscanf(file, "%s %d %lf", newPerson->imie, &newPerson->wiek, &newPerson->pesel) == 3) {

        if (isEmpty()) {
            first = newPerson;
            last = newPerson;
        } else {
            current->next = newPerson;
            newPerson->prev = current;
            last = newPerson;
        }
        current = newPerson;
    }

    fclose(file);

    printf("Dodano dane z pliku do istniejacej listy.\n");
}


void sortByName() {
    struct osoba *current, *temp;

    if (isEmpty() || first->next == NULL) {
        printf("Lista jest pusta lub zawiera tylko jedna osobe. Nie ma potrzeby sortowania.\n");
        return;
    }

    bool swapped;
    do {
        swapped = false;
        current = first;

        while (current->next != NULL) {
            if (strcmp(current->imie, current->next->imie) > 0) {
                if (current == first) {
                    first = current->next;
                    current->next->prev = NULL;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                temp = current->next->next;
                current->next->next = current;
                current->next->prev = current->prev;
                current->prev = current->next;
                current->next = temp;

                if (temp != NULL) {
                    temp->prev = current;
                }

                swapped = true;
            } else {
                current = current->next;
            }
        }
    } while (swapped);

    printf("Lista zostala posortowana alfabetycznie wedlug imion.\n");
}


int main() {
    int x = 0;
    do {
        printf("Co chcesz zrobic?\n");
        printf("1. Dodaj osoby do listy\n");
        printf("2. Wyszukaj osobe\n");
        printf("3. Kasuj osobe\n");
        printf("4. Wyswietl liste od poczatku\n");
        printf("5. Wyswietl liste od tylu\n");
        printf("6. Odczytaj liste z pliku\n");
        printf("7. Zapisz liste do pliku\n");
        printf("8. Sortuj liste\n");
        printf("9. Wyjscie\n");
        scanf("%d", &x);
        switch (x) {
            case 1:
                add();
                break;
            case 2:
                searchByName();
                break;
            case 3:
                deleteByName();
                break;
            case 4:
                viewForward();
                break;
            case 5:
                viewBackwards();
                break;
            case 6:
                readFromFile();
                break;
            case 7:
                saveToFile();
                break;
            case 8:
                sortByName();
                break;
            default:
                break;
        }
    } while (x != 9);

    return 0;
}