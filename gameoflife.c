#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>  


void afficherAide(){
    printf("Usage: ./fileComile N M K\n");
    printf("  N : nombre de lignes du tableau\n");
    printf("  M : nombre de colonnes du tableau\n");
    printf("  K : nombre d'iterations\n");
}


int lireParametres(int argc, char *argv[], int *N, int *M, int *K) {
    if (argc != 4) { // Vérifie si le nombre d'arguments est incorrect
        printf("Erreur: nombre d'arguments invalide\n");
        afficherAide();
        return 0;
    }

    /* atoi convertit de char en int */
    *N = atoi(argv[1]);
    *M = atoi(argv[2]);
    *K = atoi(argv[3]);

    /* Vérification de la validité des paramètres */
    if (*N <= 0 || *M <= 0 || *K < 0) {
        printf("Erreur: N et M doivent être positifs et K >= 0.\n");
        return 0;
    }

    return 1;
}

/* Fonction pour allouer dynamiquement un tableau 2D (N x M) */
int** allouerTableau2D(int N, int M) {
    int** tableau = (int**)malloc(N * sizeof(int*)); // Correction ici
    if (!tableau) return NULL;

    for (int i = 0; i < N; i++) {
        tableau[i] = (int*)malloc(M * sizeof(int));
        if (!tableau[i]) {
            for (int j = 0; j < i; j++) free(tableau[j]);
            free(tableau);
            return NULL;
        }
    }
    return tableau;
}


/* Fonction pour libérer la mémoire allouée pour un tableau 2D */
void libererTableau2D(int** tableau, int N) {
    if (!tableau) return;
    for (int i = 0; i < N; i++) {
        free(tableau[i]);  // Libértion des lignes de la matrice
    }
    free(tableau); // Libération du tableau principal
}

/* Fonction pour initialiser le tableau de jeu avec des valeurs aléatoires */
void initialiserJeu(int** tableau, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // Chaque cellule a 30% de chance d'être vivante (1), sinon elle est morte (0)
            tableau[i][j] = (rand() % 100 < 30) ? 1 : 0;
        }
    }
}

/* Fonction pour compter les voisins vivants autour d'une cellule donnée */
int compterVoisinsVivants(int** tableau, int N, int M, int i, int j) {
    int compteur = 0;
    for (int di = -1; di <= 1; di++) { // Parcours des voisins dans les 8 directions autour de la cellule
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue; // On ignore la cellule elle-même
            int ni = i + di, nj = j + dj;
            if (ni >= 0 && ni < N && nj >= 0 && nj < M) compteur += tableau[ni][nj]; // Incrémentation si voisin vivant
        }
    }
    return compteur;
}

/* Fonction qui fait évoluer l'état du jeu d'une itération à l'autre */
void evoluerJeu(int** tableau, int** nouveauTableau, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int voisins = compterVoisinsVivants(tableau, N, M, i, j);
            // Règles du jeu de la vie : une cellule vivante reste vivante si elle a 2 ou 3 voisins vivants
            // Une cellule morte devient vivante si elle a exactement 3 voisins vivants
            nouveauTableau[i][j] = (tableau[i][j] == 1) ? (voisins == 2 || voisins == 3) : (voisins == 3);
        }
    }
}

/* Fonction qui copie le contenu d'un tableau dans un autre */
void copierTableau(int** source, int** destination, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            destination[i][j] = source[i][j]; // Copie de chaque valeur
        }
    }
}



void afficherJeu(int** tableau, int N, int M) {
    printf("+");
    for (int i = 0; i < M; i++) printf("-");
    printf("+\n");

    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < M; j++) {
            printf("%c", tableau[i][j] ? 'o' : ' '); 
        }
        printf("|\n");
    }

    printf("+");
    for (int i = 0; i < M; i++) printf("-");
    printf("+\n");
}

int main(int argc, char *argv[]) {
    int N, M, K;
    if (!lireParametres(argc, argv, &N, &M, &K)) return 1; // Lecture des paramètres

    srand(time(NULL)); // Initialisation de la génération aléatoire

    // Allocation des tableaux pour l'état actuel et l'état futur
    int** tableau = allouerTableau2D(N, M);
    int** nouveauTableau = allouerTableau2D(N, M);
    if (!tableau || !nouveauTableau) return 1; // Vérification de l'allocation

    // Initialisation du jeu et affichage de l'état initial
    initialiserJeu(tableau, N, M);
    printf("État initial :\n");
    afficherJeu(tableau, N, M);
    usleep(1000000); // Pause 1s 
    //  K itérations
    for (int iteration = 1; iteration <= K; iteration++) {
        evoluerJeu(tableau, nouveauTableau, N, M); // Calculer l'état suivant du jeu
        copierTableau(nouveauTableau, tableau, N, M); // Mettre à jour l'état actuel
        printf("Itération %d sur %d :\n", iteration, K);
        afficherJeu(tableau, N, M); 
        usleep(300000); // Pause de 300 ms 
    }

    // Libération de la mémoire allouée pour les tableaux (contrainte memory)
    libererTableau2D(tableau, N);
    libererTableau2D(nouveauTableau, N);
    
    return 0;
}
