#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
extern int GRACZE[4][8];
extern int OBECNY_GRACZ;
extern int STADO[7];
extern int menuHeight;
extern int menuWidth;
extern int czyKomputer;
/*
0 - króliki
1 - owce
2 - świnie
3 - krowy
4 - konie
5 - mały pies
6 - duży pies
*/
int wymianaStadoZ(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int naCo, WINDOW *menu_win){
    int przeliczniki[7] = {1, 6, 12, 36, 72, 6, 36};
    int ilosci[7] = {ileKrolikow, ileOwiec, ileSwin, ileKrow, ileKoni, ileMP, ileDP};
    int wartoscKroliki = 0;
    if((ileKrolikow > GRACZE[OBECNY_GRACZ][1]) || (ileOwiec > GRACZE[OBECNY_GRACZ][2]) || (ileSwin > GRACZE[OBECNY_GRACZ][3]) || (ileKrow > GRACZE[OBECNY_GRACZ][4]) || (ileKoni > GRACZE[OBECNY_GRACZ][5]) ||
       (ileMP > GRACZE[OBECNY_GRACZ][6]) || (ileDP > GRACZE[OBECNY_GRACZ][7])){
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Zbyt malo zwierzat w Twoim stadzie"))/2, "%s", "Zbyt malo zwierzat w Twoim stadzie");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    }
    if(STADO[naCo] > 0){
        if(naCo == 5){
            for(int i = 0; i<2; i++){
                wartoscKroliki += przeliczniki[i]*ilosci[i];
            }
        }
        else if(naCo == 6){
            for(int i = 0; i<4; i++){
                wartoscKroliki += przeliczniki[i]*ilosci[i];
            }
        }
        else{
            for(int i = 0; i<naCo; i++){
                wartoscKroliki += przeliczniki[i]*ilosci[i];
            }
        }
        wartoscKroliki+=ileMP*6;
        wartoscKroliki+=ileDP*36;
        if(wartoscKroliki == przeliczniki[naCo]){
            GRACZE[OBECNY_GRACZ][6] -= ileMP;
            STADO[5] += ileMP;
            GRACZE[OBECNY_GRACZ][7] -= ileDP;
            STADO[6] += ileDP;
            GRACZE[OBECNY_GRACZ][naCo+1]++;
            STADO[naCo]--;
            if(naCo == 5) naCo = 2;
            if(naCo == 6) naCo = 4;
            for(int i = 1; i<naCo+1; i++){
                GRACZE[OBECNY_GRACZ][i] -= ilosci[i-1];
                STADO[i-1] += ilosci[i-1];
            }
            if(czyKomputer == 0 && OBECNY_GRACZ == 1){
                wclear(menu_win);
                box(menu_win, 0, 0);
                mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Wymiana sie powiodla!"))/2, "%s", "Wymiana sie powiodla!");
                wrefresh(menu_win);
                wgetch(menu_win);
            }
            return 1;
        }
        else{
            wclear(menu_win);
            box(menu_win, 0, 0);
            mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Sprobuj ponownie :("))/2, "%s", "Sprobuj ponownie :(");
            wrefresh(menu_win);
            wgetch(menu_win);
            return 0;
        }
    }
    else{
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Nie ma tego zwierzaka w stadzie"))/2, "%s", "Nie ma tego zwierzaka w stadzie");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    } //brak wybranego zwierzęcia w stadzie
}

int wymianaStadoDo(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int zCzego, WINDOW *menu_win){
    int przeliczniki[5] = {1, 6, 12, 36, 72};
    int ilosci[7] = {ileKrolikow, ileOwiec, ileSwin, ileKrow, ileKoni, ileMP, ileDP};
    int wartoscKroliki = przeliczniki[zCzego];
    int czyMP = 0;
    int czyDP = 0;
    if(GRACZE[OBECNY_GRACZ][zCzego+1] < 1){
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Brak zwierzaka w Twoim stadzie"))/2, "%s", "Brak zwierzaka w Twoim stadzie");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    }
    if(ileKrolikow > STADO[0]) ileKrolikow = STADO[0];
    if(ileOwiec > STADO[1]) ileOwiec = STADO[1];
    if(ileSwin > STADO[2]) ileSwin = STADO[2];
    if(ileKrow > STADO[3]) ileKrow = STADO[3];
    if(ileKoni > STADO[4]) ileKoni = STADO[4];
    if(ileMP > STADO[5]) ileMP = STADO[5];
    if(ileDP > STADO[6]) ileDP = STADO[6];
    if(zCzego == 5){
        czyMP = 1;
        wartoscKroliki = 6;
        zCzego=2;
    }
    if(zCzego == 6){
        czyDP = 1;
        wartoscKroliki = 36;
        zCzego=3;
    }
    for(int i = 0; i<zCzego; i++){
        wartoscKroliki -= ilosci[i]*przeliczniki[i];
    }
    wartoscKroliki -= ilosci[5]*6;
    wartoscKroliki -= ilosci[6]*36;
    ilosci[0] = ileKrolikow;
    ilosci[1] = ileOwiec;
    ilosci[2] = ileSwin;
    ilosci[3] = ileKrow;
    ilosci[4] = ileKoni;
    ilosci[5] = ileMP;
    ilosci[6] = ileDP;
    if(wartoscKroliki == 0 && OBECNY_GRACZ == 1){
        for(int i = 1; i<zCzego+1; i++){
            GRACZE[OBECNY_GRACZ][i] += ilosci[i-1];
            STADO[i-1] -= ilosci[i-1];
        }
        GRACZE[OBECNY_GRACZ][6] += ileMP;
        STADO[5] -= ileMP;
        GRACZE[OBECNY_GRACZ][7] += ileDP;
        STADO[6] -= ileDP;
        if(czyDP) STADO[6]++;
        else if(czyMP) STADO[5]++;
        else{
            STADO[zCzego]++;
        }
        if(czyMP) zCzego = 5;
        if(czyDP) zCzego = 6;
        GRACZE[OBECNY_GRACZ][zCzego+1]--;
        if(czyKomputer == 0){
            wclear(menu_win);
            box(menu_win, 0, 0);
            mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Udalo sie dokonac wymiany"))/2, "%s", "Udalo sie dokonac wymiany");
            wrefresh(menu_win);
            wgetch(menu_win);
        }
        return 1;
    }
    else{
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Sprobuj ponownie"))/2, "%s", "Sprobuj ponownie");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    }
}

int wymianaGraczZ(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int naCo, int ktoryGracz, WINDOW *menu_win){
    int przeliczniki[7] = {1, 6, 12, 36, 72, 6, 36};
    int ilosci[7] = {ileKrolikow, ileOwiec, ileSwin, ileKrow, ileKoni, ileMP, ileDP};
    int wartoscKroliki = 0;
    if((ileKrolikow > GRACZE[OBECNY_GRACZ][1]) || (ileOwiec > GRACZE[OBECNY_GRACZ][2]) || (ileSwin > GRACZE[OBECNY_GRACZ][3]) || (ileKrow > GRACZE[OBECNY_GRACZ][4]) || (ileKoni > GRACZE[OBECNY_GRACZ][5]) ||
       (ileMP > GRACZE[OBECNY_GRACZ][6]) || (ileDP > GRACZE[OBECNY_GRACZ][7])){
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Zbyt malo zwierzat w Twoim stadzie"))/2, "%s", "Zbyt malo zwierzat w Twoim stadzie");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    }
    if(GRACZE[ktoryGracz][naCo] > 0){
        if(naCo == 5){
            for(int i = 0; i<2; i++){
                wartoscKroliki += przeliczniki[i]*ilosci[i];
            }
        }
        else if(naCo == 6){
            for(int i = 0; i<4; i++){
                wartoscKroliki += przeliczniki[i]*ilosci[i];
            }
        }
        else{
            for(int i = 0; i<naCo; i++){
                wartoscKroliki += przeliczniki[i]*ilosci[i];
            }
        }
        wartoscKroliki+=ileMP*6;
        wartoscKroliki+=ileDP*36;
        if(wartoscKroliki == przeliczniki[naCo]){
            GRACZE[OBECNY_GRACZ][6] -= ileMP;
            GRACZE[ktoryGracz][6] += ileMP;
            GRACZE[OBECNY_GRACZ][7] -= ileDP;
            GRACZE[ktoryGracz][7] += ileDP;
            GRACZE[OBECNY_GRACZ][naCo+1]++;
            GRACZE[ktoryGracz][naCo+1]--;
            if(naCo == 5) naCo = 2;
            if(naCo == 6) naCo = 4;
            for(int i = 1; i<naCo+1; i++){
                GRACZE[OBECNY_GRACZ][i] -= ilosci[i-1];
                GRACZE[ktoryGracz][i] += ilosci[i-1];
            }
            wclear(menu_win);
            box(menu_win, 0, 0);
            mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Wymiana sie powiodla!"))/2, "%s", "Wymiana sie powiodla!");
            wrefresh(menu_win);
            wgetch(menu_win);
            return 1;
        }
        else{
            wclear(menu_win);
            box(menu_win, 0, 0);
            mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Sprobuj ponownie :("))/2, "%s%d %d", "Sprobuj ponownie :(", wartoscKroliki, ileKrolikow);
            wrefresh(menu_win);
            wgetch(menu_win);
            return 0;
        }
    }
    else{
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Nie ma tego zwierzaka u gracza nr "))/2, "%s%d", "Nie ma tego zwierzaka u gracza nr ", ktoryGracz+1);
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    } //brak wybranego zwierzęcia u gracza nr ktoryGracz
}

int wymianaGraczDo(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int zCzego, int ktoryGracz, WINDOW *menu_win){
    int przeliczniki[5] = {1, 6, 12, 36, 72};
    int ilosci[7] = {ileKrolikow, ileOwiec, ileSwin, ileKrow, ileKoni, ileMP, ileDP};
    int wartoscKroliki = przeliczniki[zCzego];
    int czyMP = 0;
    int czyDP = 0;
    if(GRACZE[OBECNY_GRACZ][zCzego+1] < 1){
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Brak zwierzaka w Twoim stadzie"))/2, "%s", "Brak zwierzaka w Twoim stadzie");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    }
    if(ileKrolikow > GRACZE[ktoryGracz][1]) ileKrolikow = GRACZE[ktoryGracz][1];
    if(ileOwiec > GRACZE[ktoryGracz][2]) ileOwiec = GRACZE[ktoryGracz][2];
    if(ileSwin > GRACZE[ktoryGracz][3]) ileSwin = GRACZE[ktoryGracz][3];
    if(ileKrow > GRACZE[ktoryGracz][4]) ileKrow = GRACZE[ktoryGracz][4];
    if(ileKoni > GRACZE[ktoryGracz][5]) ileKoni = GRACZE[ktoryGracz][5];
    if(ileMP > GRACZE[ktoryGracz][6]) ileMP = GRACZE[ktoryGracz][6];
    if(ileDP > GRACZE[ktoryGracz][7]) ileDP = GRACZE[ktoryGracz][7];
    if(zCzego == 5){
        czyMP = 1;
        wartoscKroliki = 6;
        zCzego=2;
    }
    if(zCzego == 6){
        czyDP = 1;
        wartoscKroliki = 36;
        zCzego=3;
    }
    for(int i = 0; i<zCzego; i++){
        wartoscKroliki -= ilosci[i]*przeliczniki[i];
    }
    wartoscKroliki -= ilosci[5]*6;
    wartoscKroliki -= ilosci[6]*36;
    ilosci[0] = ileKrolikow;
    ilosci[1] = ileOwiec;
    ilosci[2] = ileSwin;
    ilosci[3] = ileKrow;
    ilosci[4] = ileKoni;
    ilosci[5] = ileMP;
    ilosci[6] = ileDP;//gdyby wartości się zmieniły
    if(wartoscKroliki == 0){
        for(int i = 1; i<zCzego+1; i++){
            GRACZE[OBECNY_GRACZ][i] += ilosci[i-1];
            GRACZE[ktoryGracz][i] -= ilosci[i-1];
        }
        GRACZE[OBECNY_GRACZ][6] += ileMP;
        GRACZE[ktoryGracz][6] -= ileMP;
        GRACZE[OBECNY_GRACZ][7] += ileDP;
        GRACZE[ktoryGracz][7] -= ileDP;
        if(czyDP){
            GRACZE[ktoryGracz][7]++;
            GRACZE[OBECNY_GRACZ][7]--;
        }
        else if(czyMP){
            GRACZE[ktoryGracz][6]++;
            GRACZE[ktoryGracz][7]--;
        }
        else{
            GRACZE[ktoryGracz][zCzego+1]++;
            GRACZE[OBECNY_GRACZ][zCzego+1]--;
        }
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Wymiana sie powiodla!"))/2, "%s", "Wymiana sie powiodla!");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 1;
    }
    else{
        wclear(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, menuHeight/2, (menuWidth - strlen("Sprobuj ponownie :("))/2, "%s", "Sprobuj ponownie :(");
        wrefresh(menu_win);
        wgetch(menu_win);
        return 0;
    }
}
