#ifndef WYMIANA_H_INCLUDED
#define WYMIANA_H_INCLUDED
int wymianaStadoZ(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int naCo, WINDOW *menu_win); //zamiana wielu na jedno
int wymianaStadoDo(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int zCzego, WINDOW *menu_win); //zamiana jednego na wiele
int wymianaGraczZ(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int naCo, int ktoryGracz, WINDOW *menu_win); //zamiana wielu na jedno
int wymianaGraczDo(int ileKrolikow, int ileOwiec, int ileSwin, int ileKrow, int ileKoni, int ileMP, int ileDP, int zCzego, int ktoryGracz, WINDOW *menu_win); //zamiana jednego na wiele
#endif // WYMIANA_H_INCLUDED
