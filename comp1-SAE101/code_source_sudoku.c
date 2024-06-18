#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// CONSTANTES

#define N 3 
#define TAILLE (N*N)

typedef int tGrille[TAILLE][TAILLE];


// PROTOTYPES

void chargerGrille(tGrille g);
void afficherGrille(tGrille grille1);
void saisir(int *val);
bool possible(tGrille grille1, int lig, int col, int val);
bool estPleine(tGrille grille1);
void ligneGrille();

/*****************************************************
*               PROGRAMME PRINCIPAL                  *
*****************************************************/


/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme ( 0 : sortie normale).
* 
* Le programme principal permet d'afficher une grille de sudoku et d'y jouer jusqu'à ce que 
* la grille soit pleine, en demandant de saisir la ligne, la colonne et la valeur à placer
*/

int main(){

    tGrille grille1;
    
    int numLigne;
    int numColonne;
    int valeur;

        chargerGrille(grille1);

        // Boucle de jeu
        while(grille1!=0){

            afficherGrille(grille1);
            
            printf("Indices de la case ? \n");
            printf("numéro de ligne : \n");
            saisir(&numLigne);
            numLigne = numLigne-1;
            printf("numéro de colonne : \n");
            saisir(&numColonne);
            numColonne = numColonne-1;

            if(grille1[numLigne][numColonne]!=0){
                printf("IMPOSSIBLE, la case n'est pas libre.\n");
            }

            else{
                printf("Valeur à insérer ? \n");
                saisir(&valeur);
                if(possible(grille1, numLigne, numColonne, valeur)==true){
                    grille1[numLigne][numColonne] = valeur;
                }
            }   
        }

    printf("Grille pleine, fin de partie"); 
    return EXIT_SUCCESS;
 }


/*****************************************************
*              PROCÉDURES ET FONCTIONS               *
*****************************************************/


/**
* \fn void chargerGrille(tGrille g)
* \brief procédure qui charge une grille sous format fichier .sud .
* \param g paramètre d'entrée/sortie qui représente le tableau qui contiendra la grille à charger.
*
* Demande à l'utilisateur de saisir le d'un nom de fichier .sud . 
* Les valeurs présente dans le fichier .sud seront assigné au tableau tGrille g.
*/

void chargerGrille(tGrille g){

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
    fread(g, sizeof(int), TAILLE*TAILLE, f);
    }
    fclose(f);
}


/**
* \fn void ligneGrille()
* \brief Procédure qui affiche une ligne formée avec des "+" et des "-" 
*
* Affiche N fois :  N*N tirets puis un "+"
*/

void ligneGrille(){
    printf("   "); 
    for(int i = 0; i < N; i++){
        printf("+");

        // affiche N*N tirets
        for(int j = 0; j < TAILLE ; j++){
            printf("-");
        }
       
    }
    printf("+"); // affiche le "+" de fin de ligne
    printf("\n");
}


/**
* \fn void chargerGrille(tGrille g)
* \brief procédure qui charge une grille sous format fichier .sud .
* \param g paramètre d'entrée/sortie qui représente le tableau qui contiendra la grille à charger.
*
* Demande à l'utilisateur de saisir le d'un nom de fichier .sud . 
* Les valeurs présente dans le fichier .sud seront assigné au tableau tGrille g.
*/

void afficherGrille(tGrille grille1){


    printf("    ");

    // afficher numéro colonne
    for(int i = 0; i < TAILLE; i++){ 
        printf(" %d ",i+1);
        if((i+1) % N == 0){
            printf(" ");
        }
    }
    printf("\n\n");

    ligneGrille(); // première ligne de séparation horizontale 

    // Boucle principal pour affichage de la grille
    for (int i = 0; i < TAILLE; i++) {

        // affiche numero de ligne
        printf("%d  ",i+1); 

        // Apparence de la grille
        for (int j = 0; j <= TAILLE; j++) {

            // affiche séparation verticale de bloc
            if((j % N == 0)){ 
                printf("|");
            }
            
            // remplace les valeurs nulles de la grille par un point
            if((j<TAILLE)&&(grille1[i][j] == 0)){ 
                printf(" . ");
            }

            // affiche les valeurs déja contenues dans la grille du fichier .sud
            else{
                printf(" %d ", grille1[i][j]);
            } 

        }
        printf("\n");

        // affichage des lignes de séparation horizontale de la grille
        if((i+1) % N == 0){
            ligneGrille(); 
        }
    
    }
}   


/**
* \fn  void saisir(int *val){
* \brief procédure qui demande à l'utilisateur de saisir une valeur et de l'affecter à une variable.
* \param val paramètre d'entrée/sortie 

*/

void saisir(int *val){

    int x; // variable temporaire qui stock la saisie de l'utilisateur pour la vérification
    char ch[10];  
    int erreur;

    erreur = 1;
    
    // tant qu'il y a une erreur
    while(erreur !=0){
        scanf("%s",ch);

        // vérifie si la valeur rentrée est un nombre entier
        if(sscanf(ch, "%d", &x)==0){
        printf("ERREUR, veuillez saisir un entier\n");
        }

        // vérifie si la valeur rentrée est un nombre entier compris entre 1 et N*N
        else{
            if( (x < 1) || (x > TAILLE) ){
                printf("Erreur, valeur doit être comprise entre 1 et 9 inclus\n");
            }
            else{
                erreur = 0;
            }
        }
    }

    /*
    * Si tous les tests ont été passés
    * alors la valeur de x est assigné à la variable val.
    */
    *val = x; 
    
}


/**
* \fn bool possible(tGrille grille1, int lig, int col, int val)
* \brief procédure qui vérifie si l'utilisateur peut placer une valeur à tel emplacement ou non
* \param grille1 paramètre d'entrée qui reprrésente la grille de sudoku utilisée
* \param lig paramètre d'entrée qui représente le numéro de la ligne de la grille
* \param col paramètre d'entrée qui représente le numéro de la colonne de la grille
* \param val paramètre d'entrée qui représente la valeur saisie à placer dans la grille
* \return true quand la valeur n''est présente ni dans le bloc, la ligne ou la colonne, false sinon
*
*la fonction test si la valeur saisie est présente quelque part dans la ligne, dans la colonne ou dans le bloc 
*/

bool possible(tGrille grille1, int lig, int col, int val){
    
    // verifie si la valeur est dans la ligne
    for (int i = 0; i < TAILLE; i++) {
            if(grille1[lig][i]==val){
                printf("valeur déja présente dans la ligne \n");
                return false;
            }
        }
    // verifie si la valeur est dans la colonne
    for (int j = 0; j < TAILLE; j++){
        if(grille1[j][col]==val){

                printf("valeur déja présente dans la colonne\n");
                return false;
            }
    }

    // verifie si valeur est dans le bloc
    for (int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){

            if(grille1[i + N*(lig/N)][ j + N*(col/N)]==val){
                printf("valeur est déjà présente dans le bloc\n");
                return false;
            }
        }
    } 
    return true;
    
}


/**
* \fn bool possible(tGrille grille1)
* \brief procédure qui vérifie si la grille est pleine
* \param grille1 paramètre d'entrée qui reprrésente la grille de sudoku utilisée
* \return true quand la grille ne contient plus aucune valeur nulle, false sinon
*
* la fonction vérifie si la grille est pleine ou non 
*/

bool estPleine(tGrille grille1){
    for (int i = 0; i < TAILLE; i++){ // parcours les lignes
        for (int j = 0; j < TAILLE; j++){ // parcours les colonnes
            if(grille1[i][j]==0){
                return false;
            }
        }
    }
    return true;
}
