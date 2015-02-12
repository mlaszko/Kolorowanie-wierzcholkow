/* 
 * File:   main.c
 * Author: mlaszkow
 *
 * Created on 4 grudzień 2014, 21:53
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int V; //liczba wierzchołków
int **R; //binarna macierz przyleglosci
int **C; // macierz podobienstw
int *kolory;
int ileKolorow; //

void destruktor(){//;)
    int i;
    for(i = 0; i< V; i++){
        free(R[i]);
        free(C[i]);
    }
    free(R);
    free(C);
    free(kolory);
    V = 0;
    ileKolorow = 0;
    R = NULL;
    C = NULL;
}

void wczytaj(){
    int i, j;
    destruktor();
    fscanf (stdin, "%d", &V);
    
    R = (int**)malloc(V * sizeof(int*));
    
    for(i = 0; i< V; i++){
        R[i] = (int*)malloc(V * sizeof(int));
        for(j = 0; j< V; j++){          
            fscanf (stdin, "%d", &R[i][j]);
        }
    }

    
}

void wypisz(){
    int i, j;
    for(i=0; i<V; i++){
        for(j=0;j<V;j++)
            printf("%d ", R[i][j]);
        printf("\n");
    }
    printf("------- \n");
        for(i=0; i<V; i++){
        for(j=0;j<V;j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }
}

/// Oblicza macierz podobienstw
void macierz_podobienstw(){
    int i, j;
    int ile;//ile przejsc w 2 krokach
    C = (int**)malloc(V * sizeof(int*));
    for(i =0; i<V; i++)
        C[i] = (int*)malloc(V * sizeof(int));
    for(i =0; i<V; i++){        
        for(j = i; j<V; j++){
            //ten sam wierzchołek
            if(i==j){
                C[i][j] = 0;
                continue;
            }
            //wierzchołki są połączone
            if(R[i][j]){
                C[i][j] = C[j][i] = -1;
                continue;
            }
            //wierzchołki nie są połączone
            //szukamy ile jest przejść w dwóch krokach
            ile = 0;
            int k;
            for(k=0; k < V; k++){
                if(i==k || j==k)
                    continue;
                //przejscie w dwóch krokach przez wierzchołek k
                if(R[i][k] && R[j][k]){
                    ile++;
                }
            }
            C[i][j] = C[j][i] = ile;    
        }
    }
}

/// Znajduje maksymalną wartość w macierzy C
int maxC(){
    int i,j;
    int m = 0;
    for(i =0; i<V; i++)
        for(j=i; j<V; j++){
            if(C[i][j] > m)
                m = C[i][j];
        }
    return m;
}

///Koloruje pary wierzchołków o stopniu p
void koloruj(int p){
    int i, j;

    for(i =0; i<V; i++){        
        for(j = i; j<V; j++){
            //tylko pary o stopniu p
            if(C[i][j] != p)
                continue;
            //jezeli oba maja kolor
            if(kolory[i] && kolory[j])
                continue;
            //jeden z wierzchołkow ma kolor
            if(kolory[i] || kolory[j]){
                int k; //ktory kolorujemy
                if(kolory[i])
                    k = j;
                else
                    k = i;
                int l;
                //sprawdzamy czy mozemy dac juz wybrany kolor
                for(l = 1; l <= ileKolorow; l++){
                    int ii;
                    int mozna = 1; //czy mozna pokolorowac kolorem l
                    for(ii = 0; ii<V; ii++){
                        if(R[k][ii]){
                            if(kolory[ii] == l){
                                mozna = 0;
                                break;
                            }
                        }
                    }
                    if(mozna){
                            kolory[k] = l;
                            break;
                        }
                }
                continue;
            }
            //żaden nie ma koloru
            //sprawdzamy czy mozna nadac im juz wybrany kolor
                int l;
                for(l = 1; l <= ileKolorow; l++){
                    int ii;
                    int mozna = 1; //czy mozna pokolorowac kolorem l
                    for(ii = 0; ii<V; ii++){
                        if(R[i][ii] || R[j][ii]){
                            if(kolory[ii] == l){
                                mozna = 0;
                                break;
                            }
                        }
                    }
                    if(mozna){
                            kolory[i] = kolory[j] = l;
                            break;
                        }
                }
            if(kolory[i]) //nadalismy kolory
                continue;
            
            //jezeli nie mozna nadajemy nowy kolor
            ileKolorow++;
            kolory[i] = kolory[j] = ileKolorow;
        }
    }
}

void sprawdz(){
    int i, j;
    for(i = 0; i<V; i++){
        for(j=i+1; j<V; j++){
            if(R[i][j] && kolory[i]==kolory[j]){
                printf("Błedne kolorowanie! Wierzchołki %d %d mają ten sam kolor\n%d %d %d\n", i, j, R[i][j], kolory[i], kolory[j]);
                return;
            }
                
        }
    }
    printf("Kolorowanie poprawne.\n");
}


int main(int argc, char** argv) {
    //clock_t start, end;
    //double czas;
    int i;
    int p;
    ileKolorow = 0;
    wczytaj();
    
    double start = omp_get_wtime();
    macierz_podobienstw();    
    kolory = (int*)malloc(V * sizeof(int));
    for(i = 0; i<V; i++)
        kolory[i] = 0;
    for(p = maxC(); p >=0; p--){
        koloruj(p);
    }
    double end = omp_get_wtime();
    //czas = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Liczba wierzchołków: %d \n", V);    
    //wypisz();
    printf("Liczba kolorów: %d \n", ileKolorow);
    //wypisuje kolory wierzchołków
    if(V<20)
        for(i=0; i<V; i++){
            //if(kolory[i]==0)
                printf("kolory[%d] = %d ", i, kolory[i]);        
        }
    printf("\n");
    printf("start = %.16g end = %.16g diff = %.16g \n",
        start, end, end - start);
    sprawdz();
    destruktor();
    return (EXIT_SUCCESS);
}

