#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "kostka.h"
#include "wymiana.h"
/*
STADO:
0 - króliki
1 - owce
2 - świnie
3 - krowy
4 - konie
5 - małe psy
6 - duże psy
GRACZ:
0 - czy uczestniczy
1 - króliki
2 - owce itd.
*/
//deklaracja zmiennych globalnych dzielonych między plikami
int STADO[7] = {60, 24, 20, 12, 6, 4, 2};
int GRACZE[4][8] = {0};
int OBECNY_GRACZ = 0;
int GRA = 1;
int czyKomputer = 0;
int zKim;
int ROW, COL; //wiersze i kolumny w terminalu
int menuHeight = 11;
int menuWidth = 40;
int graczHeight = 9;
int graczWidth = 20;
int stadoHeight = 6;
int wymiany[8]; //tablica wykorzystywana do wywoływania funckji związanych z wymianami 0-6 ilość zwierząt 7 - numer zwierzaka na którego wymieniamy
WINDOW *gracz1, *gracz2, *gracz3, *gracz4, *menuWindow, *stadoWindow, *tabelaWindow;

void zmianaGracza(int iluGraczy){
    if(OBECNY_GRACZ+1>=iluGraczy){
        OBECNY_GRACZ = 0;
    }
    else OBECNY_GRACZ++;
}
//funckje odpowiadające za wyswietlanie
int menuStartowe(WINDOW *menu_win){
    refresh();
    char opcje[4][40];
    strcpy(opcje[0],"1. Gra od 2 do 4 graczy");
    strcpy(opcje[1], "2. Gra 1 vs 1 z komputerem");
    strcpy(opcje[2], "3. Wyjdz z gry");
    strcpy(opcje[3], "Wybierz opcje i nacisnij ENTER");
	int wybor = 0;
	box(menu_win, 0, 0);
	for(int i = 0; i<4; i++){
        mvwprintw(menu_win, menuHeight/2+i-1,(menuWidth-strlen(opcje[i]))/2,"%s", opcje[i]);
    }
	while(wybor <= 0 || wybor > 3){
        wscanw(menu_win, " %d", &wybor);
	}
	if(wybor == 3){
        endwin();
        exit(0);
	}
	if(wybor == 2){
        czyKomputer = 1;
        return 2;
	}
    wclear(menu_win);
    wrefresh(menu_win);
    echo();
    int iluGraczy = 0;
    while(wybor == 1 && (iluGraczy < 2 || iluGraczy > 4)){
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth-strlen("Podaj liczbe graczy: "))/2, "%s", "Podaj liczbe graczy: ");
        wrefresh(menu_win);
        wscanw(menu_win, " %d", &iluGraczy);
        wclear(menu_win);
        wrefresh(menu_win);
    }
    return iluGraczy;
}

int menuGry(WINDOW *menu_win){
    wclear(menu_win);
    box(menu_win, 0, 0);
    char opcje[8][40];
    strcpy(opcje[0], "Kolej gracza nr: ");
    strcpy(opcje[1], "1. Rzut kostkami");
    strcpy(opcje[2], "2. Wymiana ze stadem (kilka - 1)");
    strcpy(opcje[3], "3. Wymiana ze stadem (1 - kilka)");
    strcpy(opcje[4], "4. Wymiana z graczem (kilka - 1)");
    strcpy(opcje[5], "5. Wymiana z graczem (1 - kilka)");
    if(czyKomputer){
        strcpy(opcje[6], "4. Zakoncz gre");
    }
    else{
        strcpy(opcje[6], "6. Zakoncz gre");
    }
    strcpy(opcje[7], "Wybierz opcje i nacisnij ENTER");
    mvwprintw(menu_win, 1, (menuWidth-strlen(opcje[0]))/2, "%s%d", opcje[0], OBECNY_GRACZ+1);
    if(czyKomputer == 1){
        for(int i = 1; i<4; i++){
            mvwprintw(menu_win, (menuHeight)/2+i-3,(menuWidth-strlen(opcje[i]))/2,"%s", opcje[i]);
        }
        for(int i = 6; i<8; i++){
            mvwprintw(menu_win, (menuHeight)/2+i-5,(menuWidth-strlen(opcje[i]))/2,"%s", opcje[i]);
        }
        box(menu_win, 0, 0);
        wrefresh(menu_win);
        if(OBECNY_GRACZ == 1){
            return 0;
        }
        noecho();
        int wybranaOpcja = 0;
        while(wybranaOpcja < 1 || wybranaOpcja > 4) wscanw(menu_win, " %d", &wybranaOpcja);
        if(czyKomputer && wybranaOpcja == 4) return 6;
        return wybranaOpcja;
    }
    for(int i = 1; i<8; i++){
        mvwprintw(menu_win, (menuHeight)/2+i-3,(menuWidth-strlen(opcje[i]))/2,"%s", opcje[i]);
    }
    box(menu_win, 0, 0);
    wrefresh(menu_win);
    noecho();
    int wybranaOpcja = 0;
    while(wybranaOpcja < 1 || wybranaOpcja >6) wscanw(menu_win, " %d", &wybranaOpcja);
    return wybranaOpcja;
}

void wyswietlGraczy(){
    char zwierzeta[7][20];
    strcpy(zwierzeta[0], "Kroliki: ");
    strcpy(zwierzeta[1], "Owce: ");
    strcpy(zwierzeta[2], "Swinie: ");
    strcpy(zwierzeta[3], "Krowy: ");
    strcpy(zwierzeta[4], "Konie: ");
    strcpy(zwierzeta[5], "Male psy: ");
    strcpy(zwierzeta[6], "Duze psy: ");
    WINDOW *oknaGracze[4] = {gracz1, gracz2, gracz3, gracz4};
    for(int j = 0; j<4; j++){
        wclear(oknaGracze[j]);
        box(oknaGracze[j], 0, 0);
        mvwprintw(oknaGracze[j], 0, (graczWidth-strlen("Gracz  "))/2, "%s%d", "GRACZ ", j+1);
        for(int i = 1; i<8; i++){
            wattron(oknaGracze[j], COLOR_PAIR(i+1));
            mvwprintw(oknaGracze[j], i, 1, "%d. %s%d", i, zwierzeta[i-1], GRACZE[j][i]);
            wattroff(oknaGracze[j], COLOR_PAIR(i+1));
        }
        wrefresh(oknaGracze[j]);
    }
}

int zKimWymiana(WINDOW *menu_win){ //zwraca z którym graczem chcemy dokonać wymiany
    echo();
    int zKim = 0;
    while((zKim < 1 || zKim >5) || zKim == OBECNY_GRACZ+1){
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Z kim chcesz sie wymienic: "))/2, "%s", "Z kim chcesz sie wymienic: ");
        wrefresh(menu_win);
        wscanw(menu_win, " %d", &zKim);
    }
    return --zKim;
}

void wymiana(WINDOW *menu_win, int rodzaj){
    char zwierzeta[12][30];
    strcpy(zwierzeta[0], "krolikow ");
    strcpy(zwierzeta[1], "owiec ");
    strcpy(zwierzeta[2], "swin ");
    strcpy(zwierzeta[3], "krow ");
    strcpy(zwierzeta[4], "koni ");
    strcpy(zwierzeta[5], "malych psow ");
    strcpy(zwierzeta[6], "duzych psow ");
    strcpy(zwierzeta[7], "Ile ");
    strcpy(zwierzeta[8], "chcesz wymienic: ");
    strcpy(zwierzeta[9], "chcesz otrzymac: ");
    strcpy(zwierzeta[10], "Numer zwierzaka do wymiany: ");
    strcpy(zwierzeta[11], "Numer zwierzaka do oddania: ");
    for(int i = 0; i<7; i++){
            wclear(menu_win);
            box(menu_win, 0, 0);
            wattron(menu_win, COLOR_PAIR(i+2));
            mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen(zwierzeta[7]) - strlen(zwierzeta[i]) - strlen(zwierzeta[8]))/2, "%s%s%s", zwierzeta[7], zwierzeta[i], zwierzeta[rodzaj]);
            wattroff(menu_win, COLOR_PAIR(i+2));
            wrefresh(menu_win);
            echo();
            wscanw(menu_win, " %d", &wymiany[i]);
    }
    while(wymiany[7] < 1 || wymiany[7] > 6){
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen(zwierzeta[rodzaj+2]))/2, "%s", zwierzeta[rodzaj+2]);
        wrefresh(menu_win);
        wscanw(menu_win, " %d", &wymiany[7]);
        wymiany[7]--;
    }
}

void wyswietlStado(WINDOW *stado_win){
    char zwierzeta[7][20];
    strcpy(zwierzeta[0], "Kroliki: ");
    strcpy(zwierzeta[1], "Owce: ");
    strcpy(zwierzeta[2], "Swinie: ");
    strcpy(zwierzeta[3], "Krowy: ");
    strcpy(zwierzeta[4], "Konie: ");
    strcpy(zwierzeta[5], "Male psy: ");
    strcpy(zwierzeta[6], "Duze psy: ");
    wclear(stado_win);
    box(stado_win, 0, 0);
    mvwprintw(stado_win, 0, (menuWidth - strlen("STADO GLOWNE"))/2, "%s", "STADO GLOWNE");
    for(int i = 0; i<4; i++){
        wattron(stado_win, COLOR_PAIR(i+2));
        mvwprintw(stado_win, stadoHeight/2-2+i, (menuWidth-strlen(zwierzeta[0]))/4, "%s%d", zwierzeta[i], STADO[i]);
        wattroff(stado_win, COLOR_PAIR(i+2));
    }
    for(int i = 4; i<7; i++){
        wattron(stado_win, COLOR_PAIR(i+2));
        mvwprintw(stado_win, stadoHeight/2-6+i, ((menuWidth-strlen(zwierzeta[0]))*3)/4, "%s%d", zwierzeta[i], STADO[i]);
        wattroff(stado_win, COLOR_PAIR(i+2));
    }
    wrefresh(stado_win);
}

void wyswietTabele(WINDOW *tabela_win){
    char zwierzeta[6][20];
    char poprzednie[6][20];
    strcpy(poprzednie[0], " krolikow");
    strcpy(poprzednie[1], " owce");
    strcpy(poprzednie[2], " swinie");
    strcpy(poprzednie[3], " krowy");
    strcpy(poprzednie[4], " owca");
    strcpy(poprzednie[5], " krowa");
    strcpy(zwierzeta[0], "Owca ");
    strcpy(zwierzeta[1], "Swinia ");
    strcpy(zwierzeta[2], "Krowa ");
    strcpy(zwierzeta[3], "Kon ");
    strcpy(zwierzeta[4], "Maly pies ");
    strcpy(zwierzeta[5], "Duzy pies ");
    box(tabelaWindow, 0, 0);
    int ilePoprzednich[6] = {6, 2, 3, 2, 1, 1};
    mvwprintw(tabela_win, 0, (menuWidth - strlen("TABELA WYMIAN"))/2, "%s", "TABELA WYMIAN");
    for(int i = 0; i<3; i++){
        wattron(tabela_win, COLOR_PAIR(i+3));
        mvwprintw(tabela_win, stadoHeight/2-2+i, ((menuWidth-strlen(zwierzeta[0])-3-strlen(poprzednie[0])))/8, "%s", zwierzeta[i]);
        wattroff(tabela_win, COLOR_PAIR(i+3));
        wprintw(tabela_win, "= ");
        wattron(tabela_win, COLOR_PAIR(i+2));
        wprintw(tabela_win, "%d%s", ilePoprzednich[i], poprzednie[i]);
        wattroff(tabela_win, COLOR_PAIR(i+2));
    }
    for(int i = 3; i<6; i++){
        wattron(tabela_win, COLOR_PAIR(i+3));
        mvwprintw(tabela_win, stadoHeight/2-5+i, ((menuWidth-strlen(zwierzeta[0])-3-strlen(poprzednie[0]))*7)/8, "%s", zwierzeta[i]);
        wattroff(tabela_win, COLOR_PAIR(i+3));
        wprintw(tabela_win, "= ");
        int j = i+2;
        if(i == 5) j = 5;
        if(i == 4) j = 3;
        wattron(tabela_win, COLOR_PAIR(j));
        wprintw(tabela_win, "%d%s", ilePoprzednich[i], poprzednie[i]);
        wattroff(tabela_win, COLOR_PAIR(j));
    }
    wrefresh(tabelaWindow);
}

//koniec wyświetlania

void zerujWymiany(){
    for(int i = 0; i<8; i++) wymiany[i] = 0;
}

void czyKoniec(WINDOW *menu_win, WINDOW *stado_win, WINDOW *tabela_win){
    int czyWszystkie = 1;
    for(int j = 1; j<6; j++){
        if(GRACZE[OBECNY_GRACZ][j] < 1) czyWszystkie = 0;
    }
    if(czyWszystkie == 1){
        wclear(gracz1);
        wclear(gracz2);
        wclear(gracz3);
        wclear(gracz4);
        wclear(menu_win);
        wclear(stado_win);
        wclear(tabela_win);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Wygral gracz nr "))/2, "%s%d!", "Wygral gracz nr ", OBECNY_GRACZ+1);
        wrefresh(gracz1);
        wrefresh(gracz2);
        wrefresh(gracz3);
        wrefresh(gracz4);
        wrefresh(menu_win);
        wrefresh(stado_win);
        wrefresh(tabela_win);
        GRA = 0;
    }
}

int main(){
    srand(time(NULL));
    zerujWymiany();
    resizeterm(24, 80); //zmiana rozmiaru okna do wyświetlania
    initscr(); //start okna curses
    refresh();
    curs_set(0);
    getmaxyx(stdscr,ROW,COL); //liczba wierszy i kolumn
    noecho();
    cbreak();
    start_color();	//start colors
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_GREEN, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);
    init_pair(8, COLOR_BLUE, COLOR_BLACK);
	int starty = (ROW - menuHeight) / 2;	//ustawienie menu na środku
	int startx = (COL - menuWidth) / 2;
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
	bkgd(COLOR_PAIR(1)); //ustawienie koloru tła na czarny i czcionki na biały
	menuWindow = newwin(menuHeight, menuWidth, starty, startx);
	stadoWindow = newwin(stadoHeight, menuWidth, ROW-stadoHeight, graczWidth);
    tabelaWindow = newwin(stadoHeight-1, menuWidth, 0, graczWidth);
    int iluGraczy = menuStartowe(menuWindow);
    if(iluGraczy == 4){
        gracz4 = newwin(graczHeight, graczWidth, 0, COL-graczWidth);
        gracz3 = newwin(graczHeight, graczWidth, ROW-graczHeight, 0);
        gracz1 = newwin(graczHeight, graczWidth, 0, 0);
        gracz2 = newwin(graczHeight, graczWidth, ROW-graczHeight, COL-graczWidth);
    }
    else if(iluGraczy == 3){
        gracz3 = newwin(graczHeight, graczWidth, ROW-graczHeight, 0);
        gracz1 = newwin(graczHeight, graczWidth, 0, 0);
        gracz2 = newwin(graczHeight, graczWidth, ROW-graczHeight, COL-graczWidth);
    }
    gracz1 = newwin(graczHeight, graczWidth, 0, 0);
    gracz2 = newwin(graczHeight, graczWidth, ROW-graczHeight, COL-graczWidth);
    for(int i = 0; i<iluGraczy; i++){
        GRACZE[i][0] = 1;
        GRACZE[i][1] = 1;
        STADO[0]--;
    }
    wyswietTabele(tabelaWindow);
    while(GRA){
        int akcja;
        wyswietlGraczy();
        wyswietlStado(stadoWindow);
        akcja = menuGry(menuWindow);
        if(czyKomputer == 1 && OBECNY_GRACZ == 1){
            int ruch = wybierzRuch();
            if(ruch == 1){
                int K1 = rzut();
                int K2 = rzut();
                przydzielZwierzeta(K1, K2, menuWindow);
            }
            czyKoniec(menuWindow, stadoWindow, tabelaWindow);
            zmianaGracza(iluGraczy);
        }
        if(akcja == 1){
            int K1 = rzut();
            int K2 = rzut();
            przydzielZwierzeta(K1, K2, menuWindow);
            wgetch(menuWindow);
            czyKoniec(menuWindow, stadoWindow, tabelaWindow);
            zmianaGracza(iluGraczy);
        }
        if(akcja == 2){
            wymiana(menuWindow, 8);
            if(wymianaStadoZ(wymiany[0], wymiany[1], wymiany[2], wymiany[3], wymiany[4], wymiany[5], wymiany[6], wymiany[7], menuWindow)){
                zerujWymiany();
                czyKoniec(menuWindow, stadoWindow, tabelaWindow);
                zmianaGracza(iluGraczy);
            }
            else zerujWymiany();
        }
        if(akcja == 3){
            wymiana(menuWindow, 9);
            if(wymianaStadoDo(wymiany[0], wymiany[1], wymiany[2], wymiany[3], wymiany[4], wymiany[5], wymiany[6], wymiany[7], menuWindow)){
                zerujWymiany();
                czyKoniec(menuWindow, stadoWindow, tabelaWindow);
                zmianaGracza(iluGraczy);
            }
            else zerujWymiany();
        }
        if(akcja == 4){
            wymiana(menuWindow, 8);
            zKim = zKimWymiana(menuWindow);
            if(wymianaGraczZ(wymiany[0], wymiany[1], wymiany[2], wymiany[3], wymiany[4], wymiany[5], wymiany[6], wymiany[7], zKim, menuWindow)){
                zerujWymiany();
                czyKoniec(menuWindow, stadoWindow, tabelaWindow);
                zmianaGracza(iluGraczy);
            }
            else zerujWymiany();
        }
        if(akcja == 5){
            wymiana(menuWindow, 9);
            zKim = zKimWymiana(menuWindow);
            if(wymianaGraczDo(wymiany[0], wymiany[1], wymiany[2], wymiany[3], wymiany[4], wymiany[5], wymiany[6], wymiany[7], zKim, menuWindow)){
                zerujWymiany();
                czyKoniec(menuWindow, stadoWindow, tabelaWindow);
                zmianaGracza(iluGraczy);
            }
            else zerujWymiany();
        }
        else if(akcja == 6){
            endwin();
            return 0;
        }
    }
    wgetch(menuWindow);
    endwin();
    return 0;
}
