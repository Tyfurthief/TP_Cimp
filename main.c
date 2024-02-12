#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.c"

// Fonction principale, pour choisir la transformation ou le filtre que l'on souhaite utiliser
int main()
{
    struct imageNB i1;
    int choice;
    // char* nomFichier;
    //readFile(&i1);
    loadPGM(&i1, "linux.pgm");

    // Sélection d'une image par l'utilisateur
    // printf("Indiquer l'image à traiter : \n");
    // Récupération du path donné par l'utilisateur
    // scanf("%s", nomFichier);

    // Choix de l'action sur l'image par l'utilisateur
    printf("Quelle action doit être effectuée sur l'image ? \n");
    printf("1: Négatif\n");
    printf("2: Miroir (Vertical)\n");
    printf("3: Miroir (Horizontal)\n");
    printf("4: Translation (Verticale)\n");
    printf("5: Translation (Horizontale)\n");
    printf("6: Seuillage\n");
    printf("7: Histogramme\n");
    printf("8: Scale\n");
    printf("9: Pixelisation\n");
    printf("10: Flou\n");
    printf("11: Contraste\n");
    printf("12: Luminosité\n");
    // printf("13: BONUS Vignetting\n");

    printf("Choix : ");
    scanf("%d", &choice);

    // Application du choix
    switch (choice)
    {
    case 1:
        negatif(&i1);
    break;

    case 2:
        miroirV(&i1);
    break;

    case 3:
        miroirH(&i1);
    break;

    case 4:         
        int decalage;
        printf("Choisir un décalage :\n");
        scanf("%d", &decalage);
        translateV(&i1, decalage);  
    break;

    case 5:         
        printf("Choisir un décalage :\n");
        scanf("%d", &decalage);
        translateH(&i1, decalage);  
    break;

    case 6:  
        int seuil;
        printf("Choisir un seuil :\n");
        scanf("%d", &seuil);
        seuillage(&i1, seuil);
    break;

    case 7:
        histogramme(&i1);
    break;

    case 8:
        float scaleLvl;
        printf("Choisir le niveau de redimensionnement :\n");
        scanf("%f", &scaleLvl);
        scaling(&i1, scaleLvl);
    break;

    case 9:
        int pixelLvl;
        printf("Choisir un niveau de pixelisation :\n");
        scanf("%d", &pixelLvl);
        pixelisation(&i1, pixelLvl);
    break;

    case 10:
        int degreFlou;
        printf("Choisir un degré de flou :\n");
        scanf("%d", &degreFlou);
        floutage(&i1, degreFlou);
    break;

    case 11:
        int contrastLvl;
        printf("Choisir un niveau de contraste :\n");
        scanf("%d", &contrastLvl);
        contraste(&i1, contrastLvl);
    break;

    case 12:
        int lightLvl;
        printf("Choisir un niveau de luminosité :\n");
        scanf("%d", &lightLvl);
        luminosite(&i1, lightLvl);
    break;

    // case 13:
    //     vignet(&i1);
    // break;

    default: 
    printf("L'action demandée n'existe pas");
    break;
    }
    return 0;
}