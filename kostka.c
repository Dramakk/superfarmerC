#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "wymiana.h"
extern int GRACZE[4][8];
extern int OBECNY_GRACZ;
extern int STADO[7];
extern int menuHeight;
extern int menuWidth;
extern WINDOW *menuWindow;
int rzut(){
    srand(rand());
    return rand() % 12 + 1;
}
//K1 - kostka zielona (wilk, krowa, świnia, 3 owce, 6 królików)
//K2 - kostka czerwona (lis, koń, 2 świnie, 2 owce, 6 królików)
/*
Dla funkcji związanych z kostką
1 - królik
2 - owca
3 - świnia
4 - krowa
5 - koń
6 - lis
7 - wilk
*/
void przydzielZwierzeta(int K1, int K2, WINDOW *menu_win){
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_GREEN, COLOR_BLACK);
    int ktoryKolor1, ktoryKolor2;
    //Wybór zwierzęcia w zależności od wyrzuconych oczek na kostce
    char jakieZwierzeta[3][30];
    strcpy(jakieZwierzeta[2], "Otrzymujesz: ");
    switch(K1){
        case 12:
            K1 = 7;
            strcpy(jakieZwierzeta[0], "Wilk!");
            break;
        case 11:
            K1 = 4;
            strcpy(jakieZwierzeta[0], "krowe/ow");
            ktoryKolor1 = 5;
            break;
        case 10:
            K1 = 3;
            strcpy(jakieZwierzeta[0], "swin/ie");
            ktoryKolor1 = 4;
            break;
        case 7 ... 9:
            K1 = 2;
            strcpy(jakieZwierzeta[0], "owce/iec");
            ktoryKolor1 = 3;
            break;
        default:
            K1 = 1;
            strcpy(jakieZwierzeta[0], "kroliki/ow");
            ktoryKolor1 = 2;
            break;
    }
    switch(K2){
        case 12:
            K2 = 6;
            strcpy(jakieZwierzeta[1], "Lis!");
            break;
        case 11:
            K2 = 5;
            strcpy(jakieZwierzeta[1], "koni/a");
            ktoryKolor2 = 6;
            break;
        case 9 ... 10:
            K2 = 3;
            strcpy(jakieZwierzeta[1], "swin/ie");
            ktoryKolor2 = 4;
            break;
        case 7 ... 8:
            K2 = 2;
            strcpy(jakieZwierzeta[1], "owce/iec");
            ktoryKolor2 = 3;
            break;
        default:
            K2 = 1;
            strcpy(jakieZwierzeta[1], "kroliki/ow");
            ktoryKolor2 = 2;
            break;
    }
    //Przydział odpowiednich zwierząt oraz sytuacje z wilkiem i lisem
    if(GRACZE[OBECNY_GRACZ][0] == 1){
        if(K1 != 7 && K2 != 6){
            if(K1 == K2){
                int nK = (GRACZE[OBECNY_GRACZ][K1] + 2)/2; //liczymy ile mamy par wylosowanych zwierząt
                if(nK > STADO[K1-1]) nK = STADO[K1-1];  //jeżeli wylosowaliśmy więcej niż dostępne, dostajemy tylko dostępne
                GRACZE[OBECNY_GRACZ][K1] += nK;
                STADO[K1-1] -= nK;
                wclear(menu_win);
                box(menu_win, 0, 0);
                mvwprintw(menu_win, (menuHeight)/2,(menuWidth-(strlen(jakieZwierzeta[0]) + strlen(jakieZwierzeta[2])))/2,"%s", jakieZwierzeta[2]);
                wattron(menu_win, COLOR_PAIR(ktoryKolor1));
                wprintw(menu_win, "%d %s", nK, jakieZwierzeta[0]);
                wattroff(menu_win, COLOR_PAIR(ktoryKolor1));
                wrefresh(menu_win);
            }
            else{
                int nK1 = (GRACZE[OBECNY_GRACZ][K1] + 1)/2; //liczymy ile mamy par wylosowanych zwierząt
                int nK2 = (GRACZE[OBECNY_GRACZ][K2] + 1)/2;
                if(nK1 > STADO[K1-1]) nK1 = STADO[K1-1]; //jeżeli wylosowaliśmy więcej niż dostępne, dostajemy tylko dostępne
                if(nK2 > STADO[K2-1]) nK2 = STADO[K2-1];
                GRACZE[OBECNY_GRACZ][K1] += nK1;
                GRACZE[OBECNY_GRACZ][K2] += nK2;
                STADO[K1-1] -= nK1;
                STADO[K2-1] -= nK2;
                wclear(menu_win);
                box(menu_win, 0, 0);
                mvwprintw(menu_win, (menuHeight)/2-2,(menuWidth-(strlen(jakieZwierzeta[2])))/2,"%s", jakieZwierzeta[2]);
                wattron(menu_win, COLOR_PAIR(ktoryKolor1));
                mvwprintw(menu_win, (menuHeight)/2-1,(menuWidth-(strlen(jakieZwierzeta[2])))/2,"%d %s", nK1, jakieZwierzeta[0]);
                wattroff(menu_win, COLOR_PAIR(ktoryKolor1));
                wattron(menu_win, COLOR_PAIR(ktoryKolor2));
                mvwprintw(menu_win, (menuHeight)/2,(menuWidth-(strlen(jakieZwierzeta[2])))/2,"%d %s", nK2, jakieZwierzeta[1]);
                wattroff(menu_win, COLOR_PAIR(ktoryKolor2));
                wrefresh(menu_win);
            }
        }
        if(K1 == 7){
            if(GRACZE[OBECNY_GRACZ][7] == 0){
                STADO[1] += GRACZE[OBECNY_GRACZ][2];
                STADO[2] += GRACZE[OBECNY_GRACZ][3];
                STADO[3] += GRACZE[OBECNY_GRACZ][4];
                GRACZE[OBECNY_GRACZ][2] = 0;
                GRACZE[OBECNY_GRACZ][3] = 0;
                GRACZE[OBECNY_GRACZ][4] = 0;
            }
            else{
                GRACZE[OBECNY_GRACZ][7]--;
                STADO[6]++;
            }
            if(K2 != 6){ //wyświetlanie komunikatu o wilku
                wclear(menu_win);
                box(menu_win, 0, 0);
                mvwprintw(menu_win, (menuHeight)/2,(menuWidth-(strlen(jakieZwierzeta[0])))/2,"%s", jakieZwierzeta[0]);
                wrefresh(menu_win);
            }
        }
        if(K2 == 6){
            if(GRACZE[OBECNY_GRACZ][6] == 0){
                int strata = GRACZE[OBECNY_GRACZ][1];
                GRACZE[OBECNY_GRACZ][1] = 1;
                STADO[0] += strata-1;
            }
            else{
                GRACZE[OBECNY_GRACZ][6]--;
                STADO[5]++;
            }
            if(K1 != 7){ //wyświetlanie komunikatu o lisie
                wclear(menu_win);
                box(menu_win, 0, 0);
                mvwprintw(menu_win, (menuHeight)/2,(menuWidth-(strlen(jakieZwierzeta[1])))/2,"%s", jakieZwierzeta[1]);
                wrefresh(menu_win);
            }
        }
        if(K1 == 7 && K2 == 6){ //wyświetlanie komunikatu o wilku i lisie
            wclear(menu_win);
            box(menu_win, 0, 0);
            mvwprintw(menu_win, (menuHeight)/2,(menuWidth-(strlen(jakieZwierzeta[1]) + strlen(jakieZwierzeta[0])))/2,"%s i %s", jakieZwierzeta[0], jakieZwierzeta[1]);
            wrefresh(menu_win);
        }
    }
}
int wybierzRuch(){
    int ruch = 0;
    while(ruch == 0){
        int jakiRuch = rzut();
        if(jakiRuch <= 8){
            ruch = 1;
        }
        else{
            if(GRACZE[1][3] >= 3){
                if(wymianaStadoZ(0, 0, 3, 0, 0, 0, 0, 3, menuWindow)) return 0;
            }
            if(GRACZE[1][4] >= 1 && GRACZE[1][7] == 0){
                if(wymianaStadoZ(0, 0, 0, 1, 0, 0, 0, 6, menuWindow)) return 0;
            }
            if(GRACZE[1][4] >= 2 && GRACZE[1][4] % 2 == 0){
                if(wymianaStadoZ(0, 0, 0, 2, 0, 0, 0, 4, menuWindow)) return 0;
            }
            if(GRACZE[1][3] >= 5 && GRACZE[1][3] % 2 == 0){
                if(wymianaStadoDo(0, 2, 0, 0, 0, 0, 0, 2, menuWindow)) return 0;
            }
            if(GRACZE[1][4] >= 5 && GRACZE[1][4] % 2 == 0){
                if(wymianaStadoDo(12, 2, 1, 0, 0, 0, 0, 3, menuWindow)) return 0;
            }
            if(GRACZE[1][1] >= 36){
                if(wymianaStadoZ(36, 0, 0, 0, 0, 0, 0, 3, menuWindow)) return 0;
            }
            if(GRACZE[1][1] >= 15){
                if(wymianaStadoZ(12, 0, 0, 0, 0, 0, 0, 2, menuWindow)) return 0;
            }
            if(GRACZE[1][1] >= 12 && GRACZE[1][6] == 0){
                if(wymianaStadoZ(6, 0, 0, 0, 0, 0, 0, 5, menuWindow)) return 0;
            }
            if(GRACZE[1][1] >= 6 && GRACZE[1][1] % 2 == 0){
                if(wymianaStadoZ(6, 0, 0, 0, 0, 0, 0, 1, menuWindow)) return 0;
            }
            if(GRACZE[1][2] >= 2){
                if(wymianaStadoZ(0, 2, 0, 0, 0, 0, 0, 2, menuWindow)) return 0;
            }
            if(GRACZE[1][2] >= 6){
                if(wymianaStadoZ(0, 6, 0, 0, 0, 0, 0, 3, menuWindow)) return 0;
            }
            if(GRACZE[1][2] >= 6 && GRACZE[1][7] == 0){
                if(wymianaStadoZ(0, 6, 0, 0, 0, 0, 0, 6, menuWindow)) return 0;
            }
            if(GRACZE[1][2] >= 6){
                if(wymianaStadoZ(0, 12, 0, 0, 0, 0, 0, 4, menuWindow)) return 0;
            }
        }
    }
    return 1;
}
