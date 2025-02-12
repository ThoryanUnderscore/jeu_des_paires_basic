#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>  // PDCurses ou ncurses

#define ROWS 4
#define COLS 4
#define PAIRS (ROWS * COLS) / 2
#define HIDDEN '?'

void melangerCartes(char cartes[PAIRS * 2]) {
    srand(time(NULL));
    for (int i = 0; i < PAIRS * 2; i++) {
        int r = rand() % (PAIRS * 2);
        char temp = cartes[i];
        cartes[i] = cartes[r];
        cartes[r] = temp;
    }
}

int toutesTrouvees(int visible[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (!visible[i][j]) return 0;
    return 1;
}

void afficherPlateau(char grille[ROWS][COLS], int visible[ROWS][COLS], int cursorX, int cursorY) {
    clear();
    mvprintw(1, 4, "    ");
    for (int i = 0; i < COLS; i++) printw("%d   ", i);
    mvprintw(2, 4, "  +");
    for (int i = 0; i < COLS; i++) printw("---+");
    
    for (int i = 0; i < ROWS; i++) {
        mvprintw(3 + i * 2, 4, "%d | ", i);
        for (int j = 0; j < COLS; j++) {
            if (i == cursorX && j == cursorY) {
                attron(A_REVERSE);
                printw(" %c ", visible[i][j] ? grille[i][j] : HIDDEN);
                attroff(A_REVERSE);
            } else {
                printw(" %c ", visible[i][j] ? grille[i][j] : HIDDEN);
            }
            printw("| ");
        }
        mvprintw(4 + i * 2, 4, "  +");
        for (int j = 0; j < COLS; j++) printw("---+");
    }
    refresh();
}

void jouer() {
    char cartes[PAIRS * 2] = {'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D',
                              'E', 'E', 'F', 'F', 'G', 'G', 'H', 'H'};
    melangerCartes(cartes);
    char grille[ROWS][COLS];
    int visible[ROWS][COLS] = {0};
    int index = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            grille[i][j] = cartes[index++];
    int coups = 0, cursorX = 0, cursorY = 0;
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    while (!toutesTrouvees(visible)) {
        afficherPlateau(grille, visible, cursorX, cursorY);
        int key = getch();
        switch (key) {
            case 'z': cursorX = (cursorX > 0) ? cursorX - 1 : ROWS - 1; break;
            case 's': cursorX = (cursorX < ROWS - 1) ? cursorX + 1 : 0; break;
            case 'q': cursorY = (cursorY > 0) ? cursorY - 1 : COLS - 1; break;
            case 'd': cursorY = (cursorY < COLS - 1) ? cursorY + 1 : 0; break;
            case ' ': // Touche Entrée
                if (!visible[cursorX][cursorY]) {
                    if (x1 == -1) {
                        x1 = cursorX;
                        y1 = cursorY;
                        visible[x1][y1] = 1;
                    } else if (x2 == -1 && (x1 != cursorX || y1 != cursorY)) {
                        x2 = cursorX;
                        y2 = cursorY;
                        visible[x2][y2] = 1;
                        afficherPlateau(grille, visible, cursorX, cursorY);
                        napms(500);
                        if (grille[x1][y1] != grille[x2][y2]) {
                            visible[x1][y1] = 0;
                            visible[x2][y2] = 0;
                        }
                        x1 = y1 = x2 = y2 = -1;
                        coups++;
                    }
                }
                break;
        }
    }
    clear();
    mvprintw(ROWS + 5, 4, "Bravo ! Vous avez trouve toutes les paires en %d coups !", coups);
    refresh();
    getch();
    endwin();
}

int main() {
    jouer();
    return 0;
}


/* 
Ligne de compilation du fichier (d'après mon architecture)
gcc main.c -o jeu_paires.exe -I C:\PDCurses-3.9 -L C:\PDCurses-3.9\wincon -l:pdcurses.a

Execution du fichier en cmd (dans le dossier, pas en absolu)
jeu_paires.exe

*/