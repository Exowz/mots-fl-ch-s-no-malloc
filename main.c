#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define TAILLE_GRILLE_MAX 20

// Variables globales
int TAILLE_GRILLE = 15;

// Declarations de fonctions
void generer_grille(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], char mots[][TAILLE_GRILLE_MAX], int nb_mots);
void remplir_espaces_vides(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX]);
void afficher_grille(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int curseur_x, int curseur_y, int selectionne[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int valide[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], char mots_a_trouver[][TAILLE_GRILLE_MAX], int nb_mots, char mots_trouves[][TAILLE_GRILLE_MAX], int nb_trouves);
void copier_liste_mots(char liste_mots[][TAILLE_GRILLE_MAX], char mots_a_trouver[][TAILLE_GRILLE_MAX], int nb_mots);
int mots_restants(char mots_a_trouver[][TAILLE_GRILLE_MAX], int nb_mots);
void choisir_theme(char *theme);
char generer_lettre_aleatoire();
void jouer_jeu(char liste_mots[][TAILLE_GRILLE_MAX], int nb_mots);
void definir_couleur(int couleur, int x, int y);
int valider_mot(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int selectionne[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], char liste_mots[][TAILLE_GRILLE_MAX], int nb_mots, char mots_trouves[][TAILLE_GRILLE_MAX], int *nb_trouves);
int est_mot_dans_liste(char *mot, char liste_mots[][TAILLE_GRILLE_MAX], int nb_mots);
int peut_placer_mot(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int ligne, int col, int direction, char *mot);
void menu_principal();
void definir_taille_grille();
void ajouter_liste_mots();
void confirmer_sortie(int *sortie_jeu);

// le main
int main() {
    menu_principal();
    return 0;
}

// Fonction pour choisir un theme
void choisir_theme(char *theme) { 
    int choix;
    printf("Choisissez un theme:\n");
    printf("1. Programmation\n");
    printf("2. Jeux videos\n");
    printf("3. Animaux\n");
    printf("4. Pays\n");
    scanf("%d", &choix);
    switch (choix) { // Selection du theme choisi par l'utilisateur
        case 1:
            strcpy(theme, "Programmation");
            break;
        case 2:
            strcpy(theme, "Jeux videos");
            break;
        case 3:
            strcpy(theme, "Animaux");
            break;
        case 4:
            strcpy(theme, "Pays");
            break;
        default:
            printf("Choix invalide. Par defaut: Programmation.\n");
            strcpy(theme, "Programmation");
            break;
    }
}

// Fonction pour jouer au jeu
void jouer_jeu(char liste_mots[][TAILLE_GRILLE_MAX], int nb_mots) {
    char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX] = {0}; // Grille vide
    char mots_a_trouver[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX]; // Liste de mots
    char mots_trouves[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX] = {0}; // Liste des mots trouvés
    int nb_trouves = 0; // Nombre de mots trouvés

    copier_liste_mots(liste_mots, mots_a_trouver, nb_mots); // Copie de la liste de mots

    generer_grille(grille, mots_a_trouver, nb_mots); // Grille avec des espaces vides

    int curseur_x = 0, curseur_y = 0; // Position du curseur
    int selectionne[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX] = {0}; // Indicateur de selection
    int valide[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX] = {0}; // Indicateur de valide

    int continuer_jeu = 1; // Indicateur de continuer le jeu

    // Afficher la grille une fois au début pour éviter les appels fréquents à system("cls")
    afficher_grille(grille, curseur_x, curseur_y, selectionne, valide, mots_a_trouver, nb_mots, mots_trouves, nb_trouves);

    while (continuer_jeu && mots_restants(mots_a_trouver, nb_mots)) { // Boucle de jeu
        char ch = getch(); // Touche appuyée
        int besoin_maj = 0; // Indicateur de besoin de mise à jour de l'affichage

        switch (ch) {
            case 'w': // haut
                curseur_y = (curseur_y > 0) ? curseur_y - 1 : TAILLE_GRILLE - 1; // Decalage de la grille
                besoin_maj = 1; // Mise à jour de l'affichage
                break;
            case 's': // bas
                curseur_y = (curseur_y < TAILLE_GRILLE - 1) ? curseur_y + 1 : 0; // Decalage de la grille 
                besoin_maj = 1; // Mise à jour de l'affichage
                break;
            case 'a': // gauche
                curseur_x = (curseur_x > 0) ? curseur_x - 1 : TAILLE_GRILLE - 1; // Decalage de la grille
                besoin_maj = 1; // Mise à jour de l'affichage
                break;
            case 'd': // droite
                curseur_x = (curseur_x < TAILLE_GRILLE - 1) ? curseur_x + 1 : 0; // Decalage de la grille
                besoin_maj = 1; // Mise à jour de l'affichage
                break;
            case ' ': // selection/deselection
                selectionne[curseur_y][curseur_x] = !selectionne[curseur_y][curseur_x]; // inverse le statut de la case
                besoin_maj = 1;
                break;
            case '\r': // entrer pour valider
                if (valider_mot(grille, selectionne, mots_a_trouver, nb_mots, mots_trouves, &nb_trouves)) {
                    // si le mot est valide, marquer les lettres comme validees
                    for (int i = 0; i < TAILLE_GRILLE; i++) { // Boucle pour marquer les lettres comme validees
                        for (int j = 0; j < TAILLE_GRILLE; j++) { // Boucle pour marquer les lettres comme validees
                            if (selectionne[i][j]) { // Marquer les lettres comme validees
                                valide[i][j] = 1; // Marquer les lettres comme validees
                                selectionne[i][j] = 0; // Marquer les lettres comme validees 
                            }
                        }
                    }
                } else {
                    // si le mot n'est pas valide, deselectionner les lettres
                    for (int i = 0; i < TAILLE_GRILLE; i++) { // Boucle pour deselectionner les lettres
                        for (int j = 0; j < TAILLE_GRILLE; j++) { // Boucle pour deselectionner les lettres
                            selectionne[i][j] = 0; // Deselectionner les lettres
                        }
                    }
                }
                besoin_maj = 1; // Mise à jour de l'affichage
                break;
            case 'q': // quitter
                confirmer_sortie(&continuer_jeu); // Confirmer la sortie
                break;
        }

        // Si une mise à jour est nécessaire, on réaffiche la grille
        if (besoin_maj) {
            afficher_grille(grille, curseur_x, curseur_y, selectionne, valide, mots_a_trouver, nb_mots, mots_trouves, nb_trouves);
        }
    }

    // Afficher le message de fin de jeu
    if (continuer_jeu) { 
        system("cls");
        printf("GG ! Vous avez trouve tous les mots YIPEEEE!\n");
        printf("\n");
        printf("Appuyez sur une touche pour retourner au menu principal...\n");
        getch();
    }

    menu_principal();

    return 0;
}

// Fonction pour generer la grille
void generer_grille(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], char mots[][TAILLE_GRILLE_MAX], int nb_mots) {
    srand(time(NULL)); // initialisation de la graine
    for (int i = 0; i < nb_mots; i++) { // pour chaque mot
        int place = 0; // indique si le mot a été place
        while (!place) { // tant que le mot n'a pas été place
            int direction = rand() % 4; // direction aleatoire
            int longueur_mot = strlen(mots[i]); // longueur du mot
            int ligne, col; // coordonnées du mot

            switch (direction) { // direction du mot
                case 0: // horizontal gauche a droite
                    ligne = rand() % TAILLE_GRILLE; // ligne aleatoire
                    col = rand() % (TAILLE_GRILLE - longueur_mot); // colonne aleatoire
                    if (peut_placer_mot(grille, ligne, col, direction, mots[i])) { // si le mot peut être place
                        for (int j = 0; j < longueur_mot; j++) { // placer le mot
                            grille[ligne][col + j] = mots[i][j]; // placer le mot
                        }
                        place = 1; // le mot a été place
                    }
                    break; // fin du switch

                case 1: // horizontal droite a gauche
                    ligne = rand() % TAILLE_GRILLE; // ligne aleatoire
                    col = rand() % (TAILLE_GRILLE - longueur_mot) + longueur_mot - 1; // colonne aleatoire  
                    if (peut_placer_mot(grille, ligne, col, direction, mots[i])) { // si le mot peut être place
                        for (int j = 0; j < longueur_mot; j++) { // placer le mot
                            grille[ligne][col - j] = mots[i][j]; // placer le mot
                        }
                        place = 1; // le mot a été place
                    }
                    break;
                case 2: // vertical haut en bas
                    ligne = rand() % (TAILLE_GRILLE - longueur_mot);
                    col = rand() % TAILLE_GRILLE;
                    if (peut_placer_mot(grille, ligne, col, direction, mots[i])) {
                        for (int j = 0; j < longueur_mot; j++) {
                            grille[ligne + j][col] = mots[i][j];
                        }
                        place = 1;
                    }
                    break;
                case 3: // vertical bas en haut
                    ligne = rand() % (TAILLE_GRILLE - longueur_mot) + longueur_mot - 1;
                    col = rand() % TAILLE_GRILLE;
                    if (peut_placer_mot(grille, ligne, col, direction, mots[i])) {
                        for (int j = 0; j < longueur_mot; j++) {
                            grille[ligne - j][col] = mots[i][j];
                        }
                        place = 1;
                    }
                    break;
            }
        }
    }
    remplir_espaces_vides(grille); 
}

// Fonction pour verifier si un mot peut être place dans la grille
int peut_placer_mot(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int ligne, int col, int direction, char *mot) {
    int longueur_mot = strlen(mot); 
    switch (direction) {
        case 0: // horizontal gauche a droite
            for (int j = 0; j < longueur_mot; j++) {
                if (grille[ligne][col + j] != '\0' && grille[ligne][col + j] != mot[j]) {
                    return 0;
                }
            }
            break;
        case 1: // horizontal droite a gauche
            for (int j = 0; j < longueur_mot; j++) {
                if (grille[ligne][col - j] != '\0' && grille[ligne][col - j] != mot[j]) {
                    return 0;
                }
            }
            break;
        case 2: // vertical haut en bas
            for (int j = 0; j < longueur_mot; j++) {
                if (grille[ligne + j][col] != '\0' && grille[ligne + j][col] != mot[j]) {
                    return 0;
                }
            }
            break;
        case 3: // vertical bas en haut
            for (int j = 0; j < longueur_mot; j++) {
                if (grille[ligne - j][col] != '\0' && grille[ligne - j][col] != mot[j]) {
                    return 0;
                }
            }
            break;
    }
    return 1;
}

// Fonction pour remplir les espaces vides
void remplir_espaces_vides(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX]) {
    for (int i = 0; i < TAILLE_GRILLE; i++) { // parcourir la grille
        for (int j = 0; j < TAILLE_GRILLE; j++) { 
            if (grille[i][j] == '\0') { // si la case est vide  
                grille[i][j] = generer_lettre_aleatoire();
            }
        }
    }
}

// Fonction pour afficher la grille / mouvement
void afficher_grille(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int curseur_x, int curseur_y, int selectionne[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int valide[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], char mots_a_trouver[][TAILLE_GRILLE_MAX], int nb_mots, char mots_trouves[][TAILLE_GRILLE_MAX], int nb_trouves) {
    system("cls"); // Nettoyer l'affichage

    for (int i = 0; i < TAILLE_GRILLE; i++) { // Parcourir la grille
        for (int j = 0; j < TAILLE_GRILLE; j++) { // Parcourir la grille
            positionner_curseur(j * 3, i); // 3 espaces (esthethique)
            if (i == curseur_y && j == curseur_x) { // Affichage du curseur
                definir_couleur(1, i, j); // Curseur cyan

            } else if (valide[i][j]) { // Affichage des mots valides
                definir_couleur(3, i, j); // Valide

            } else if (selectionne[i][j]) { // Affichage des mots selectionnes
                definir_couleur(2, i, j); // Selection

            } else {
                definir_couleur(0, i, j); // Damier vert et gris
            }
            printf(" %c ", grille[i][j]); // Ajout d'espaces pour centrer les lettres
            printf("\033[0m"); // Reinitialiser les couleurs apres chaque cellule
        }
        printf("\n"); // Retour a la ligne apres chaque ligne de la grille
    }

    // Affichage des mots a rechercher
    int list_x = TAILLE_GRILLE * 3 + 5; // Position x a droite de la grille
    int list_y = 0; // Position y en haut de la grille
    positionner_curseur(list_x, list_y++); 
    printf("| %-40s |\n", "Mots a rechercher dans la grille"); // Affichage du tableau de mots 
    for (int i = 0; i < nb_mots; i++) { // Parcourir la liste de mots
        if (mots_a_trouver[i][0] != '\0') { // Verifier si le mot n'est pas vide
            positionner_curseur(list_x, list_y++);
            printf("| %-40s |\n", mots_a_trouver[i]); // Afficher le mot
        }
    }

    // Affichage des mots trouves
    list_y++; // Position y en bas de la grille
    positionner_curseur(list_x, list_y++);
    printf("| %-40s |\n", "Mots trouves");
    for (int i = 0; i < nb_trouves; i++) {
        positionner_curseur(list_x, list_y++);
        printf("| %-40s |\n", mots_trouves[i]);
    }
}

// Fonction pour copier la liste de mots afin de pouvoir les modifier
void copier_liste_mots(char liste_mots[][TAILLE_GRILLE_MAX], char mots_a_trouver[][TAILLE_GRILLE_MAX], int nb_mots) {
    for (int i = 0; i < nb_mots; i++) {
        strcpy(mots_a_trouver[i], liste_mots[i]);
    }
}

// Fonction pour verifier s'il reste des mots a trouver
int mots_restants(char mots_a_trouver[][TAILLE_GRILLE_MAX], int nb_mots) {
    for (int i = 0; i < nb_mots; i++) {
        if (mots_a_trouver[i][0] != '\0') {
            return 1;
        }
    }
    return 0;
}

// Fonction pour generer une lettre aleatoire
char generer_lettre_aleatoire() {
    return 'a' + rand() % 26;
}

// Fonction pour deplacer le curseur a une position donnee
void positionner_curseur(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1); // echapper de la pos puis bouger le curseur
}

// Fonction pour changer la couleur du texte
void definir_couleur(int couleur, int x, int y) {
    switch (couleur) {
        case 0: // Damier vert clair et gris clair
            if ((x + y) % 2 == 0) { // si (x + y) est pair
                printf("\033[102m\033[30m"); // fond vert clair, texte noir
            } 
            
            else { // si (x + y) est impair
                printf("\033[47m\033[30m"); // fond gris clair, texte noir
            }
            break;

        case 1: // Curseur cyan
            printf("\033[46m\033[30m"); // fond cyan, texte noir
            break;
        case 2: // selection
            printf("\033[40m\033[31m"); // fond noir, texte rouge
            break;
        case 3: // valide
            printf("\033[42m\033[30m"); // fond vert, texte noir
            break;
    }
}

// Fonction pour verifier si un mot est dans la liste des mots
int est_mot_dans_liste(char *mot, char liste_mots[][TAILLE_GRILLE_MAX], int nb_mots) {
    for (int i = 0; i < nb_mots; i++) {
        if (liste_mots[i][0] != '\0' && strcmp(mot, liste_mots[i]) == 0) { // Verifier si le mot est dans la liste
            liste_mots[i][0] = '\0'; // marque le mot comme trouve
            return 1;
        }
    }
    return 0;
}

// Fonction pour valider un mot selectionne dans la grille
// cad le menu ou on peut creer notre propre grille
int valider_mot(char grille[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], int selectionne[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX], char liste_mots[][TAILLE_GRILLE_MAX], int nb_mots, char mots_trouves[][TAILLE_GRILLE_MAX], int *nb_trouves) {
    char mot_selectionne[TAILLE_GRILLE_MAX] = ""; // Mot selectionne
    int index = 0; // Index du mot selectionne

    // Construire le mot selectionne
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        for (int j = 0; j < TAILLE_GRILLE; j++) {
            if (selectionne[i][j]) { // Verifier si la case est selectionne
                mot_selectionne[index++] = grille[i][j]; // Ajouter la lettre au mot selectionne
            }
        }
    }
    mot_selectionne[index] = '\0'; // Marquer la fin du mot selectionne

    // Verifier le mot dans toutes les directions
    if (est_mot_dans_liste(mot_selectionne, liste_mots, nb_mots)) {
        strcpy(mots_trouves[*nb_trouves], mot_selectionne);
        (*nb_trouves)++;
        return 1;
    }

    // Inverser le mot selectionne et verifier a nouveau
    for (int i = 0; i < index / 2; i++) { // Inverser le mot
        char temp = mot_selectionne[i]; // Copier la lettre
        mot_selectionne[i] = mot_selectionne[index - i - 1]; // Copier la lettre
        mot_selectionne[index - i - 1] = temp; // Copier la lettre
    }
    
    if (est_mot_dans_liste(mot_selectionne, liste_mots, nb_mots)) { // Verifier
        strcpy(mots_trouves[*nb_trouves], mot_selectionne); // copier
        (*nb_trouves)++; // Ajouter
        return 1;
    }

    return 0;
}

// Menu principal
void menu_principal() {
    int choix;
    char theme[20];
    // Liste de mots (theme de jeu)
    char mots_programmation[][TAILLE_GRILLE_MAX] = {"variable", "fonction", "compilation", "boucle", "tableau", "pointeur", "objet", "classe", "heritage", "polymorphisme", "recursivite", "algorithme", "debugger"};
    char mots_jeux_videos[][TAILLE_GRILLE_MAX] = {"mario", "zelda", "minecraft", "fortnite", "overwatch", "valorant", "cyberpunk", "halo", "destiny", "pokemon", "fallout", "skyrim"};
    char mots_animaux[][TAILLE_GRILLE_MAX] = {"alligator", "baleine", "castor", "elan", "grenouille", "lamantin", "okapi", "panthere", "pie", "ragondin", "taupe", "tortue", "yack"};
    char mots_pays[][TAILLE_GRILLE_MAX] = {"albanie", "bahamas", "chili", "danemark", "finlande", "georgie", "haiti", "inde", "japon", "kenya", "lituanie", "mexique", "norvege", "perou", "roumanie", "senegal", "thailande"};

    int sortie_jeu = 0;

    // Boucle du jeu
    while (!sortie_jeu) {
        system("cls");
        printf("Menu Principal:\n");
        printf("1. Choisir un theme\n");
        printf("2. Choisir la taille de la grille\n");
        printf("3. Ajouter une nouvelle liste de mots\n");
        printf("4. Quitter le jeu\n");
        printf("Entrez votre choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                choisir_theme(theme);
                if (strcmp(theme, "Programmation") == 0) { // Selection du theme choisi par l'utilisateur
                    jouer_jeu(mots_programmation, sizeof(mots_programmation) / sizeof(mots_programmation[0])); // Lancement du jeu
                } else if (strcmp(theme, "Jeux videos") == 0) {
                    jouer_jeu(mots_jeux_videos, sizeof(mots_jeux_videos) / sizeof(mots_jeux_videos[0]));
                } else if (strcmp(theme, "Animaux") == 0) {
                    jouer_jeu(mots_animaux, sizeof(mots_animaux) / sizeof(mots_animaux[0]));
                } else if (strcmp(theme, "Pays") == 0) {
                    jouer_jeu(mots_pays, sizeof(mots_pays) / sizeof(mots_pays[0]));
                }
                break;
            case 2:
                definir_taille_grille();
                break;
            case 3:
                ajouter_liste_mots();
                break;
            case 4:
                sortie_jeu = 1;
                break;
            default:
                printf("Choix invalide.\n");
                break;
        }
    }
}

// Fonction pour definir la taille de la grille
// la taille min est = a la taille du mot le plus long du theme
void definir_taille_grille() {
    printf("Entrez la taille de la grille (max %d): ", TAILLE_GRILLE_MAX);
    scanf("%d", &TAILLE_GRILLE); 
    if (TAILLE_GRILLE > TAILLE_GRILLE_MAX) {
        TAILLE_GRILLE = TAILLE_GRILLE_MAX;
    } else if (TAILLE_GRILLE < 5) {
        TAILLE_GRILLE = 5;
    }
    printf("Taille de la grille definie a %d\n", TAILLE_GRILLE); 
    system("pause");
}

// Fonction pour ajouter une nouvelle liste de mots
void ajouter_liste_mots() {
    int nb_mots;
    printf("Combien de mots voulez-vous ajouter? ");
    scanf("%d", &nb_mots);

    char nouvelle_liste_mots[TAILLE_GRILLE_MAX][TAILLE_GRILLE_MAX] = {0}; // initialisation de la nouvelle liste de mots
    char mot[TAILLE_GRILLE_MAX]; // initialisation du mot

    for (int i = 0; i < nb_mots; i++) { // boucle pour ajouter les mots
        printf("Entrez le mot %d: ", i + 1);
        scanf("%s", mot);
        strcpy(nouvelle_liste_mots[i], mot); // copie du mot dans la nouvelle liste
    }

    jouer_jeu(nouvelle_liste_mots, nb_mots); // lancement du jeu avec la nouvelle liste de mots
}

// Fonction pour confirmer la sortie (menu mid jeu)
void confirmer_sortie(int *sortie_jeu) {
    printf("\n");
    printf("Appuyez sur 'q' pour quitter au menu principal, ou sur une autre touche pour continuer le jeu.");
    char ch = getch(); // touche appuyée
    if (ch == 'q') {
        *sortie_jeu = 0; // sortie du jeu
    } else {
        *sortie_jeu = 1; // continuer le jeu
    }
}