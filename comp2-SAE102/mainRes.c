#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// CONSTANTES
#define N 4
#define TAILLE (N*N)

// STRUCTURE
typedef struct {
    int c_valeur;
    int c_candidats[TAILLE];
    int c_nbCandidats;
} t_Case1;

// TYPES
typedef t_Case1 tGrille[TAILLE][TAILLE];

// PROTOTYPES
int chargerGrille(tGrille g);
bool backtracking(tGrille g, int numeroCase);
void afficherGrille(tGrille g);
bool possible(tGrille g, int lig, int col, int val);

void initialiserCandidats(tGrille g);
void ajouterCandidat(t_Case1 *laCase,int val);
void retirerCandidat(t_Case1 *laCase, int val);
bool estCandidat(t_Case1 laCase, int val);
int nbCandidats(t_Case1 laCase);

void PairesCacheesLigne(tGrille g, int ligne);
void PairesCacheesBloc(tGrille g, int blocLigne, int blocColonne);
void PairesCacheesColonne(tGrille g, int colonne);

void PairesNuesLigne(tGrille g, int ligne);
void PairesNuesColonne(tGrille g, int colonne);
void PairesCacheesBloc(tGrille g, int blocLigne, int blocColonne);

void afficherCandidats (t_Case1 c);

int main(){

    // DECLARATION VARIABLES
    tGrille g;
    int numCase = 0;

    bool progression;
    int nbCasesVides;
    int count;

    progression = true;
    nbCasesVides = chargerGrille(g);
    afficherGrille(g);

    initialiserCandidats(g);
    printf("%d", g[0][0].c_candidats[0]);

    clock_t begin = clock();
    
    while((nbCasesVides!=0) && progression == true){
        progression = false;

        for( int lig = 0; lig < TAILLE ; lig++){
            for(int col = 0; col < TAILLE; col++){

                // Singleton caché
                for (int valeur = 1; valeur <= TAILLE; valeur++) {
                    count = 0;
                    
                    int candidatLigne = -1;
                    int candidatColonne = -1;

                    for (int i = 0; i < TAILLE; i++) {
                        // Ligne
                        if(g[lig][i].c_valeur == 0 && estCandidat(g[lig][i], valeur)){
                            count++;
                            candidatLigne = lig;
                            candidatColonne = i;
                        }

                        //Col
                        if(g[i][col].c_valeur == 0 && estCandidat(g[i][col], valeur)){
                            count++;
                            candidatLigne = i;
                            candidatColonne = col;
                        }

                        // // Bloc
                        for (int j = 0; j < TAILLE; j++) {
                            if(g[i%N + N*(lig/N)][j%N + N*(col/N)].c_valeur == 0 && estCandidat(g[i%N + N*(lig/N)][j%N + N*(col/N)], valeur)){
                                count++;
                                candidatLigne = N*(lig/N);
                                candidatColonne = N*(col/N);
                            }
                        }
                    }

                    // Si la valeur n'est qu'une seule fois candidat dans la ligne la colonne et le bloc alors c'est un Singleton Caché
                    if (count == 1) {
                        g[candidatLigne][candidatColonne].c_valeur = valeur;
                        initialiserCandidats(g);
                    }
                }

                // Marche mais perd du temps car pas assez de PaireCache dans la grille pour que ca soit rentable
                // Paires
                // for (int i = 0; i < TAILLE; i++) {
                //     PairesCacheesLigne(g, i);
                //     PairesCacheesColonne(g, i);
                //     PairesCacheesBloc(g, i/N, i%N);
                // }


                // Singleton nue
                if (g[lig][col].c_valeur == 0) {

                    if(g[lig][col].c_nbCandidats == 1){
                        
                        for (int k = 1; k <= TAILLE; k++) {
                            if (estCandidat(g[lig][col], k)) {
                                retirerCandidat(&g[lig][col], k);
                                g[lig][col].c_valeur = k;
                                nbCasesVides--;
                                initialiserCandidats(g);
                                break;
                            }
                        }
                        progression = true;
                    }
                    
                }

            // Marche pas car un problème incohérent avec les candidats
            // for (int i = 0; i < TAILLE; i++) {
            //     PairesNuesLigne(g, i);
            //     PairesNuesColonne(g, i);
            //     PairesNuesBloc(g, i/N, i%N);
            // }



            }
        }  
    }

    afficherGrille(g);

    backtracking(g,numCase);
    clock_t end = clock();
    double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;

    printf("\n\n");
    afficherGrille(g);
    printf( "Temps résolution de la grille = %.6f secondes\n\n",tmpsCPU);
    
    return 0;
}

int chargerGrille(tGrille g){

    int valCase;
    int nbCasesVides = 0;
    char nomFichier[30];
    FILE * f;


    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    printf("\n");

    f = fopen(nomFichier, "rb");

    if (f==NULL){
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } 
    else {
        for(int i = 0; i < TAILLE ; i++){
            for(int j = 0; j < TAILLE; j++){
                fread(&valCase, sizeof(int),1, f);
                g[i][j].c_valeur = valCase;
                if(valCase==0){
                    nbCasesVides++;
                }
            }
        }
    }

    fclose(f);
    return nbCasesVides;
}

bool backtracking(tGrille g, int numeroCase){

    int lig;
    int col;
    bool res;

    res = false;

    if(numeroCase == TAILLE*TAILLE){
        //on a traité toutes les cases, la grille est résolue
        res = true;
    }
    else{
        // On récupère les "coordonnées" de la case
        lig = numeroCase / TAILLE;
        col = numeroCase % TAILLE;
        if(g[lig][col].c_valeur!=0){
            res = backtracking(g, numeroCase+1);
        }
        else{
            for (int i = 1; i <= TAILLE; i++){
                if(possible(g,lig,col,i)){
                    // Si la valeur est autorisée on l'inscrit
                    //dans la case..
                    g[lig][col].c_valeur = i;
                    // ... et on passe à la suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if(backtracking(g,numeroCase+1)){
                        res = true;
                    }
                    else{
                        g[lig][col].c_valeur = 0;
                    }
                }
            }
        }
    }
    return res;
}

void tracerLigne() {
    printf("\n    ");
    for (int j = 0; j < TAILLE; j++) {
        if (j%N*N == 0) {
            printf("+");
        }
        printf("----");
    }
    printf("+\n");
}

void afficherIndices() {
    printf("    ");
    for (int i = 0; i < TAILLE; i++) {
        printf(" %2d ", i+1);
        if ((i+1)%N == 0 && i != 0 && i < 10) {
            printf("  ");
        }
        else if ((i+1)%N == 0 && i != 0) {
            printf(" ");
        }
    }  
}

void afficherGrille(tGrille g) {
    // Indices horizontaux
    afficherIndices();
    
    // Parcours le tableau g
    for (int i = 0; i < TAILLE; i++) {

        // Sépare les lignes verticalement
        if (i % N == 0)
            tracerLigne();
        else
            printf("\n");

        // Indices vérticaux
        printf("%2d  |", i+1); 

        // Affiche les valeurs de la grille   
        for (int j = 0; j < TAILLE; j++) {
            if (g[i][j].c_valeur != 0){
                printf(" %2d ", (g[i][j].c_valeur));
            }
            else {
                printf(" %2c ", '.');
            }

            // Sépare verticalement les cases
            if ((j+1) % N == 0 && j != 0) {
                printf("|");
            }
        }
    }

    // Dernière ligne
    tracerLigne();
}

bool possible(tGrille g, int lig, int col, int val){
    
    // verifie si la valeur est dans la ligne
    for (int i = 0; i < TAILLE; i++) {
            if(g[lig][i].c_valeur == val){
                return false;
            }
        }
    // verifie si la valeur est dans la colonne
    for (int j = 0; j < TAILLE; j++){
        if(g[j][col].c_valeur == val){
                return false;
            }
    }

    // verifie si valeur est dans le bloc
    for (int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){

            // le i et le j permettent de parcourir les cases du bloc
            if(g[i + N*(lig/N)][ j + N*(col/N)].c_valeur == val){
                return false;
            }
        }
    } 

    return true;
}


void initialiserCandidats(tGrille g){
    for( int i = 0; i < TAILLE; i++ ){
        for(int j = 0; j < TAILLE; j++){

            // Met tout à false
            g[i][j].c_nbCandidats = 0;
            for(int k = 0; k <= TAILLE; k++){
                g[i][j].c_candidats[k] = false;
            }

            // Recherches les candidats des cases qui n'ont pas de valeur 
            if(g[i][j].c_valeur == 0 ){
                g[i][j].c_nbCandidats = 0;

                for(int k = 1; k <= TAILLE; k++){
                    if(possible(g ,i,j, k)){
                        ajouterCandidat(&g[i][j], k);
                    }
                }
                afficherCandidats(g[i][j]);
            }
        }
    }
}


void ajouterCandidat(t_Case1 *laCase, int val) {
    if (laCase->c_nbCandidats <= TAILLE){
        laCase->c_candidats[val] = true;
        laCase->c_nbCandidats++;
    }
}

void retirerCandidat(t_Case1 *laCase, int val) {
    if (laCase->c_candidats[val] == true) {
        laCase->c_candidats[val] = false;
        laCase->c_nbCandidats--;
    }
}

bool estCandidat(t_Case1 laCase, int val) {
    return laCase.c_candidats[val];
}

int nbCandidats(t_Case1 laCase) {
    return laCase.c_nbCandidats;
}
void PairesCacheesLigne(tGrille g, int ligne) {
    for (int val = 1; val <= TAILLE; val++) {
        int count = 0;
        int col1 = -1, col2 = -1;

        for (int col = 0; col < TAILLE; col++) {
            if (g[ligne][col].c_valeur == 0 && estCandidat(g[ligne][col], val)) {
                count++;
                if (col1 == -1) {
                    col1 = col;
                } else {
                    col2 = col;
                }
            }
        }

        // Si la valeur n'est que dans deux cases, c'est une paire cachée
        if (count == 2) {
            // Éliminer les candidats de la ligne sauf dans les deux colonnes de la paire
            for (int col = 0; col < TAILLE; col++) {
                if (col != col1 && col != col2) {
                    retirerCandidat(&g[ligne][col], val);
                }
            }
        }
    }
}
void PairesCacheesColonne(tGrille g, int colonne) {
    for (int val = 1; val <= TAILLE; val++) {
        int count = 0;
        int ligne1 = -1, ligne2 = -1;

        for (int ligne = 0; ligne < TAILLE; ligne++) {
            if (g[ligne][colonne].c_valeur == 0 && estCandidat(g[ligne][colonne], val)) {
                count++;
                if (ligne1 == -1) {
                    ligne1 = ligne;
                } else {
                    ligne2 = ligne;
                }
            }
        }

        // Si la valeur n'est que dans deux cases, c'est une paire cachée
        if (count == 2) {
            // Éliminer les candidats de la colonne sauf dans les deux lignes de la paire
            for (int ligne = 0; ligne < TAILLE; ligne++) {
                if (ligne != ligne1 && ligne != ligne2) {
                    retirerCandidat(&g[ligne][colonne], val);
                }
            }
        }
    }
}
void PairesCacheesBloc(tGrille g, int blocLigne, int blocColonne) {
    for (int val = 1; val <= TAILLE; val++) {
        int count = 0;
        int ligne1 = -1, ligne2 = -1;
        int colonne1 = -1, colonne2 = -1;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int ligne = blocLigne * N + i;
                int colonne = blocColonne * N + j;

                if (g[ligne][colonne].c_valeur == 0 && estCandidat(g[ligne][colonne], val)) {
                    count++;
                    if (ligne1 == -1) {
                        ligne1 = ligne;
                        colonne1 = colonne;
                    } else {
                        ligne2 = ligne;
                        colonne2 = colonne;
                    }
                }
            }
        }

        // Si la valeur n'est que dans deux cases, c'est une paire cachée
        if (count == 2) {
            // Éliminer les candidats du bloc sauf dans les deux cellules de la paire
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    int ligne = blocLigne * N + i;
                    int colonne = blocColonne * N + j;

                    if (ligne != ligne1 && ligne != ligne2 && colonne != colonne1 && colonne != colonne2) {
                        retirerCandidat(&g[ligne][colonne], val);
                    }
                }
            }
        }
    }
}
void PairesNuesColonne(tGrille g, int colonne) {
    for (int ligne1 = 0; ligne1 < TAILLE - 1; ligne1++) {
        for (int ligne2 = ligne1 + 1; ligne2 < TAILLE; ligne2++) {
            if (nbCandidats(g[ligne1][colonne]) == 2 && nbCandidats(g[ligne2][colonne]) == 2) {
                int candidat1a = 0, candidat1b = 0;
                int candidat2a = 0, candidat2b = 0;

                // Obtenez les deux candidats de la première case
                for (int val = 1; val <= TAILLE; val++) {
                    if (estCandidat(g[ligne1][colonne], val)) {
                        if (candidat1a == 0) {
                            candidat1a = val;
                        } else {
                            candidat1b = val;
                        }
                    }
                }

                // Obtenez les deux candidats de la deuxième case
                for (int val = 1; val <= TAILLE; val++) {
                    if (estCandidat(g[ligne2][colonne], val)) {
                        if (candidat2a == 0) {
                            candidat2a = val;
                        } else {
                            candidat2b = val;
                        }
                    }
                }

                // Comparez manuellement les candidats
                if ((candidat1a == candidat2a && candidat1b == candidat2b) ||
                    (candidat1a == candidat2b && candidat1b == candidat2a)) {
                    // Paire nue trouvée, éliminer les autres candidats dans la colonne
                    for (int ligne = 0; ligne < TAILLE; ligne++) {
                        if (ligne != ligne1 && ligne != ligne2) {
                            for (int val = 1; val <= TAILLE; val++) {
                                if (estCandidat(g[ligne][colonne], val)) {
                                    retirerCandidat(&g[ligne][colonne], val);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void PairesNuesLigne(tGrille g, int ligne) {
    for (int col1 = 0; col1 < TAILLE - 1; col1++) {
        for (int col2 = col1 + 1; col2 < TAILLE; col2++) {
            if (nbCandidats(g[ligne][col1]) == 2 && nbCandidats(g[ligne][col2]) == 2) {
                int candidat1a = 0, candidat1b = 0;
                int candidat2a = 0, candidat2b = 0;

                // Obtenez les deux candidats de la première case
                for (int val = 1; val <= TAILLE; val++) {
                    if (estCandidat(g[ligne][col1], val)) {
                        if (candidat1a == 0) {
                            candidat1a = val;
                        } else {
                            candidat1b = val;
                        }
                    }
                }

                // Obtenez les deux candidats de la deuxième case
                for (int val = 1; val <= TAILLE; val++) {
                    if (estCandidat(g[ligne][col2], val)) {
                        if (candidat2a == 0) {
                            candidat2a = val;
                        } else {
                            candidat2b = val;
                        }
                    }
                }

                // Comparez manuellement les candidats
                if ((candidat1a == candidat2a && candidat1b == candidat2b) ||
                    (candidat1a == candidat2b && candidat1b == candidat2a)) {
                    // Paire nue trouvée, éliminer les autres candidats dans la ligne
                    for (int colp = 0; colp < TAILLE; colp++) {
                        if (colp != col1 && colp != col2) {
                            for (int val = 1; val <= TAILLE; val++) {
                                if (estCandidat(g[ligne][colp], val)) {
                                    retirerCandidat(&g[ligne][colp], val);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void PairesNuesBloc(tGrille g, int blocLigne, int blocColonne) {
    for (int i1 = 0; i1 < N - 1; i1++) {
        for (int j1 = i1 + 1; j1 < N; j1++) {
            int ligne1 = blocLigne * N + i1;
            int ligne2 = blocLigne * N + j1;

            for (int i2 = 0; i2 < N; i2++) {
                for (int j2 = 0; j2 < N; j2++) {
                    int col1 = blocColonne * N + i2;
                    int col2 = blocColonne * N + j2;

                    if (nbCandidats(g[ligne1][col1]) == 2 && nbCandidats(g[ligne2][col2]) == 2) {
                        int candidat1a = 0, candidat1b = 0;
                        int candidat2a = 0, candidat2b = 0;

                        // Obtenez les deux candidats de la première case
                        for (int val = 1; val <= TAILLE; val++) {
                            if (estCandidat(g[ligne1][col1], val)) {
                                if (candidat1a == 0) {
                                    candidat1a = val;
                                } else {
                                    candidat1b = val;
                                }
                            }
                        }

                        // Obtenez les deux candidats de la deuxième case
                        for (int val = 1; val <= TAILLE; val++) {
                            if (estCandidat(g[ligne2][col2], val)) {
                                if (candidat2a == 0) {
                                    candidat2a = val;
                                } else {
                                    candidat2b = val;
                                }
                            }
                        }

                        // Comparez manuellement les candidats
                        if ((candidat1a == candidat2a && candidat1b == candidat2b) ||
                            (candidat1a == candidat2b && candidat1b == candidat2a)) {
                            // Paire nue trouvée, éliminer les autres candidats dans le bloc
                            for (int m = 0; m < N; m++) {
                                for (int n = 0; n < N; n++) {
                                    int ligne = blocLigne * N + m;
                                    int col = blocColonne * N + n;

                                    if (ligne != ligne1 && ligne != ligne2 && col != col1 && col != col2) {
                                        for (int val = 1; val <= TAILLE; val++) {
                                            if (estCandidat(g[ligne][col], val)) {
                                                retirerCandidat(&g[ligne][col], val);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void afficherCandidats (t_Case1 c){
    if (c.c_nbCandidats > 0) {
        printf("Nb candidat : %d\n", c.c_nbCandidats);
        for(int i =1; i <= 15; i++){
            if (c.c_candidats[i])
            
                printf(".%d ",i);
        }
        printf("\n");

    }
}


