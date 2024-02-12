#ifndef _IMAGE_
#define _IMAGE_
#include <stdio.h>
#include <stdlib.h>

struct imageNB
{
    int width;
    int height;
    unsigned char** grey;
    int vmax;
};

// Lecture de l'image
void readFile(struct imageNB* i1, char* nomImage)
{
    FILE*fichier = fopen(nomImage, "r");
    char chaine [1000];

    if(fichier != NULL){
    int verif = fscanf(fichier,"%s", chaine);
    printf("%s\n", chaine);

    fscanf(fichier, "%d %d", &i1->width, &i1->height);
    printf("Height = %d\nWidth = %d\n", i1->height, i1->width);

    i1->grey=malloc(i1->height * sizeof(float));

    int i;
    int j;

    for (i = 0; i < i1->height; i++) {
        i1->grey[i]= malloc(i1->width * sizeof(float));
    };

    for (i=0; i < i1->width; i++ ){
        for (j=0; j<i1->height; j++){
            fscanf(fichier, "%f", &i1->grey[i][j]);

        }
    }
    printf("%f", i1->grey[0][0]);

    fclose(fichier);

    }else{
    printf("Fichier Null");
    }
}

// Fonction pour le chargement de l'image
void loadPGM(struct imageNB* img, char* nomImage)
{
    FILE*fichier = fopen(nomImage, "rb");

    if(fichier != NULL){
        char chaine [3];
        int verif = fscanf(fichier,"%s", chaine);

        if (chaine[1] == '5') // Egal au format p5
        {
            fscanf(fichier, "%d %d", &img->width, &img->height);
            fscanf(fichier, "%d", &img->vmax);

            img->grey=malloc(img->height * sizeof(unsigned char*));

            if(img->grey == NULL) printf("ERROR");

            for (int i = 0; i < img->height; i++) {
                img->grey[i]= malloc(img->width * sizeof(unsigned char));
                if(img->grey[i] == NULL) printf("ERROR %d",i);
            }

            printf("Height = %d\n Width = %d\n VMAX = %d\n", img->height, img->width, img->vmax);

            for (int i=0; i < img->width; i++ ){
                for (int j=0; j < img->height; j++){
                    unsigned char tmp;
                    fread(&tmp, sizeof(unsigned char), 1, fichier);
                    img->grey[i][j] = tmp;
                }
            }
            fclose(fichier);
        }
        else
        {
            printf("Format inconnu\n");
        }
    }
    else
    {
        printf("L'image  %s est introuvable \n", nomImage);
    }
}

// Fonction pour la sauvegarde de la nouvelle image générée.
void savePGM(struct imageNB* img, char* nomImage)
{
    FILE*fichier = fopen(nomImage, "wb");
    if(fichier != NULL){
        fprintf(fichier, "P5\n");
        fprintf(fichier, "%d %d\n%d\n", img->width, img->height, img->vmax);
        unsigned char tmp = 0;
        for (int i=0; i < img->width; i++ ){
            for (int j=0; j<img->height; j++){
                tmp = (unsigned char) (img->grey[i][j]);
                fwrite(&tmp, sizeof(unsigned char), 1, fichier);
            }
        }
        fclose(fichier);
    }
    else
    {
        printf("Création du fichier %s impossible", nomImage);
    }   
}

// Fonction pour le filtre négatif
void negatif(struct imageNB* img)
{
    // Nouvelle structure pour la nouvelle image
    struct imageNB negative;
    // On affecte les mêmes dimensions
    negative.width = img->width;
    negative.height = img->height;
    negative.vmax = img->vmax;

    // Allocaton de la bonne mémoire
    negative.grey=malloc(negative.height * sizeof(unsigned char*));
    for (int indexParcours = 0; indexParcours < negative.height; indexParcours++){
        negative.grey[indexParcours]= malloc(negative.width * sizeof(unsigned char));
    }
    printf("test %d %d \n", img-> width, img-> height);

    // On affecte l'inverse des pixels de l'image "img" à l'image "negative"
    for (int i=0; i < img->width; i++ ){
        for (int j=0; j<img->height; j++){
            negative.grey[i][j]= 1 - (img->grey[i][j]);
        }
    }

    // Enregistrement de la nouvelle image avec l'effet négatif'
    savePGM(&negative,"negatif.pgm");
}

// Fonction pour le filtre miroir sur l'axe horizontal
void miroirH(struct imageNB* img)
{
    struct imageNB mirror;
    mirror.width = img->width;
    mirror.height = img->height;
    mirror.vmax = img->vmax;

    mirror.grey=malloc(mirror.height * sizeof(unsigned char*));
    for (int indexParcours = 0; indexParcours < mirror.height; indexParcours++){
        mirror.grey[indexParcours]= malloc(mirror.width * sizeof(unsigned char));
    }
    printf("test %d %d \n", img-> width, img-> height);

    //
    for (int i=0; i < img->height; i++ ){
        for (int j=0; j <= img->width/2; j++){
            unsigned char tmp = img->grey[i][j];
            mirror.grey[i][j] = img->grey[i][img->height-j];
            mirror.grey[i][img->height-j] = tmp;
        }
    }

    // Enregistrement de la nouvelle image avec le miroir horizontal
    savePGM(&mirror,"miroirH.pgm");
}

// Fonction pour le filtre miroir sur l'axe vertical
void miroirV(struct imageNB* img)
{
    struct imageNB mirror;
    mirror.width = img->width;
    mirror.height = img->height;
    mirror.vmax = img->vmax;

    mirror.grey=malloc(mirror.height * sizeof(unsigned char*));
    for (int indexParcours = 0; indexParcours < mirror.height; indexParcours++){
        mirror.grey[indexParcours]= malloc(mirror.width * sizeof(unsigned char));
    }
    printf("test %d %d \n", img-> width, img-> height);

    //
    for (int i=0; i < mirror.height/2; i++ ){
        for (int j=0; j < mirror.width; j++){
            unsigned char tmp = img->grey[i][j];
            mirror.grey[i][j] = img->grey[mirror.height - 1 - i][j];
            mirror.grey[mirror.height - 1 - i][j] = tmp;
        }
    }

    // Enregistrement de la nouvelle image avec le miroir vertical
    savePGM(&mirror,"miroirV.pgm");
}

// Fonction pour la translation verticale d'une image, decal correspond au décalage souhaité de l'image
void translateV(struct imageNB* img, int decal)
{
    struct imageNB tr;
    tr.width = img->width;
    tr.height = img->height;
    tr.vmax = img->vmax;

    tr.grey=malloc(tr.height * sizeof(unsigned char*));
    for (int indexParcours = 0; indexParcours < tr.height; indexParcours++){
        tr.grey[indexParcours]= malloc(tr.width * sizeof(unsigned char));
    }
    printf("test %d %d \n", img-> width, img-> height);

    // On parcourt chaque pixel de l'image
    // Première boucle pour les lignes
    for(int i=0; i < img->width; i++){
        // Deuxième boucle pour chaque colonne d'une ligne
        for(int j=0; j < img->height; j++){
            // TRANSLATION
            // On applique la translation classique si on ne dépasse pas l'image
            // ex : Si on est a 15/30, qu'on décalle de 10, on arrive à 25
            if(i+decal < img->width){
                tr.grey[i+decal][j] = img->grey[i][j];
            }
            // Sinon on dépasse l'image, donc on doit enlever la taille de l'image
            // ex : Si on est à 25/30, qu'on décalle de 10, alors ou doit arriver à 5, soit 35-30 = 5 
            else{
                tr.grey[i+decal-img->width][j] = img->grey[i][j];
            }
        }
    }

    // Enregistrement de la nouvelle image avec la translation horizontale
    savePGM(&tr,"translationV.pgm");
}

// Fonction pour la translation horizontale d'une image, decal correspond au décalage souhaité de l'image
void translateH(struct imageNB* img, int decal)
{
    struct imageNB tr;
    tr.width = img->width;
    tr.height = img->height;
    tr.vmax = img->vmax;

    tr.grey=malloc(tr.height * sizeof(unsigned char*));
    for (int indexParcours = 0; indexParcours < tr.height; indexParcours++){
        tr.grey[indexParcours]= malloc(tr.width * sizeof(unsigned char));
    }
    printf("test %d %d \n", img-> width, img-> height);

    // On parcourt chaque pixel de l'image
    // Première boucle pour les colonnes
    for(int i=0; i < img->height; i++){
        // Deuxième boucle pour chaque ligne d'une colonne
        for(int j=0; j < img->width; j++){
            // TRANSLATION
            if(j+decal < img->width){
                tr.grey[i][j + decal] = img->grey[i][j];
            }
            else{
                tr.grey[i][j + decal - img->height] = img->grey[i][j];
            }
        }
    }

    // Enregistrement de la nouvelle image avec la translation verticale
    savePGM(&tr,"translationH.pgm");
}

// Fonction pour le seuillage. Le seuillage défini une valeur de gris à ne pas dépasser. 
// Les valeurs dépassant le seuil seront aligner sur le seuil en question.
void seuillage(struct imageNB* img, int seuil)
{
    struct imageNB seu;
    seu.width = img->width;
    seu.height = img->height;
    seu.vmax = img->vmax;

    seu.grey=malloc(seu.height * sizeof(unsigned char*));
     for (int indexParcours = 0; indexParcours < seu.height; indexParcours++) {
        seu.grey[indexParcours]= malloc(seu.width * sizeof(unsigned char));
    }
     for(int i=0;i< img->width;i++)
    {
        for(int j=0;j<img->height;j++)
        {
            if(img->grey[i][j] > seuil)
            {
                seu.grey[i][j] = seuil;
            }
            else
            {
                seu.grey[i][j] = img->grey[i][j];
            }
        }
    }

    // Enregistrement de la nouvelle image avec le seuillage demandé
     savePGM(&seu,"seuillage.pgm");
}

//Fonction pour l'histogramme. Qui permet d'afficher le nombre de pixel pour chaque valeur de gris.
void histogramme(struct imageNB* image) {
    FILE* file = fopen("histogramme.pgm", "w");
    if (!file) {
        fprintf(stderr, "Erreur lors de la création du fichier PGM pour l'histogramme.\n");
        return;
    }

    fprintf(file, "P2\n");
    fprintf(file, "256 256\n");
    fprintf(file, "255\n"); 

    // Calculer l'histogramme
    int pixels[256] = {0};
    for (int i = 0; i < image->width; ++i) {
        for (int j = 0; j < image->height; ++j) {
            pixels[image->grey[j][i]]++;
        }
    }

    // Trouver la valeur maximale de l'histogramme
    int maxFrequency = 0;
    for (int i = 0; i < 256; ++i) {
        if (pixels[i] > maxFrequency) {
            maxFrequency = pixels[i];
        }
    }

    // Normaliser et écrire l'histogramme dans le fichier PGM
    for (int j = 0; j < 256; ++j) {
        for (int i = 0; i <= 255; ++i) {
            int normalizedHeight = (pixels[i] * 255) / maxFrequency;
            if (j >= 255 - normalizedHeight) {
                fprintf(file, "0 ");  // attribuer 0 pour les pixels représentant l'histogramme
            } else {
                fprintf(file, "255 ");  // attribuer 255 pour les pixels qui ne représentent pas l'histogramme
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


//Fonction pour la pixélisation. Qui permet de pixeliser une image en l'affichant à une résolution inférieure.
void pixelisation(struct imageNB* img, int pixelLvl)
{
    struct imageNB pixel;
    pixel.width = img->width;
    pixel.height = img->height;
    pixel.vmax = img->vmax;
    pixel.grey = malloc(pixel.height * sizeof(unsigned char*));

    for (int indexParcours = 0; indexParcours < pixel.height; indexParcours++) {
        pixel.grey[indexParcours] = malloc(pixel.width * sizeof(unsigned char));
    }

    // Pixelisation de l'image d'origine
    for (int i = 0; i < img->width; i += pixelLvl) {
        for (int j = 0; j < img->height; j += pixelLvl) {
            int sum = 0;
            int count = 0;

            // Calcul de la moyenne des intensités des pixels
            for (int ki = 0; ki < pixelLvl && i + ki < img->width; ki++) {
                for (int kj = 0; kj < pixelLvl && j + kj < img->height; kj++) {
                    sum += img->grey[j + kj][i + ki];
                    count++;
                }
            }

            unsigned char avgIntensity = (unsigned char)(sum / count);

            // Remplissage de l'image pixelisée avec la moyenne d'intensité
            for (int ki = 0; ki < pixelLvl && i + ki < img->width; ki++) {
                for (int kj = 0; kj < pixelLvl && j + kj < img->height; kj++) {
                    pixel.grey[j + kj][i + ki] = avgIntensity;
                }
            }
        }
    }

    // Sauvegarde de l'image pixelisée
    savePGM(&pixel, "pixel.pgm");
}

//Fonction pour le floutage. Qui permet d'estomper les détails de l'image selon un certain degré.
void floutage(struct imageNB* img, int degreFlou)
{
    struct imageNB flou;
    flou.width = img->width;
    flou.height = img->height;
    flou.vmax = img->vmax;
    flou.grey = malloc(flou.height * sizeof(unsigned char*));

    for (int indexParcours = 0; indexParcours < flou.height; indexParcours++) {
        flou.grey[indexParcours] = malloc(flou.width * sizeof(unsigned char));
    }

    // Calcul de la taille du Noyau (Kernel) en fonction du degré de flou
    int kernelSize = 2 * degreFlou + 1;

    // Allocation dynamique de la mémoire pour le Noyau
    int** kernel = malloc(kernelSize * sizeof(int*));
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = malloc(kernelSize * sizeof(int));
    }

    // Fill the blur kernel with 1's
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] = 1;
        }
    }

    // Convolution
    for (int i = degreFlou; i < img->width - degreFlou; i++) {
        for (int j = degreFlou; j < img->height - degreFlou; j++) {
            int sum = 0;
            for (int ki = -degreFlou; ki <= degreFlou; ki++) {
                for (int kj = -degreFlou; kj <= degreFlou; kj++) {
                    sum += img->grey[j + kj][i + ki] * kernel[kj + degreFlou][ki + degreFlou];
                }
            }
            // Normalisation
            flou.grey[j][i] = sum / (kernelSize * kernelSize);
        }
    }

    // Libération de la mémoire qui avait été alloué pour Kernel
    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }
    free(kernel);

    savePGM(&flou, "flou.pgm");
}

//Fonction pour le contraste. Qui permet d'estomper les détails de l'image selon un certain degré.
void contraste(struct imageNB* img, float contrastLvl)
{
    struct imageNB contrast;
    contrast.width = img->width;
    contrast.height = img->height;
    contrast.vmax = img->vmax;
    contrast.grey = malloc(contrast.height * sizeof(unsigned char*));

    for (int indexParcours = 0; indexParcours < contrast.height; indexParcours++) {
        contrast.grey[indexParcours] = malloc(contrast.width * sizeof(unsigned char));
    }

    // Ajustement du contraste pour chaque pixel
    for (int i = 0; i < img->width; i++) {
        for (int j = 0; j < img->height; j++) {
            float adjustedIntensity = (img->grey[j][i] - img->vmax / 2.0) * contrastLvl + img->vmax / 2.0;

            // Vérification que l'intensité reste dans la bonne plage
            adjustedIntensity = (adjustedIntensity < 0) ? 0 : adjustedIntensity;
            adjustedIntensity = (adjustedIntensity > img->vmax) ? img->vmax : adjustedIntensity;

            contrast.grey[j][i] = (unsigned char)adjustedIntensity;
        }
    }

    savePGM(&contrast, "contrast.pgm");
}

//Fonction pour le scale. Qui permet redimensionner l'image selon une échelle choisie.
void scaling(struct imageNB* img, float scaleLvl)
{
    // Calcul des nouvelles dimensions de l'image
    int newWidth = (int)(img->width * scaleLvl);
    int newHeight = (int)(img->height * scaleLvl);

    struct imageNB scale;
    scale.width = newWidth;
    scale.height = newHeight;
    scale.vmax = img->vmax;

    scale.grey = malloc(newHeight * sizeof(unsigned char*));
    for (int indexParcours = 0; indexParcours < newHeight; indexParcours++) {
        scale.grey[indexParcours] = malloc(newWidth * sizeof(unsigned char));
    }

    // Redimension de l'image
    for (int i = 0; i < newWidth; i++) {
        for (int j = 0; j < newHeight; j++) {
            int originalX = (int)(i / scaleLvl);
            int originalY = (int)(j / scaleLvl);

            // Vérification des indices qui doivent rester dans les limites de l'image d'origine
            originalX = (originalX < 0) ? 0 : originalX;
            originalY = (originalY < 0) ? 0 : originalY;
            originalX = (originalX >= img->width) ? img->width - 1 : originalX;
            originalY = (originalY >= img->height) ? img->height - 1 : originalY;

            scale.grey[j][i] = img->grey[originalY][originalX];
        }
    }

    savePGM(&scale, "scale.pgm");
}

//Fonction pour changer la luminosité de l'image, selon le niveau choisi.
void luminosite(struct imageNB* img, int lightLvl)
{
    for (int i = 0; i < img->width; i++) {
        for (int j = 0; j < img->height; j++) {
            // Ajout de la valeur de luminosité à chaque pixel
            int newPixelValue = img->grey[i][j] + lightLvl;

            // Vérification que la valeur reste dans la plage. Entre 0 et vmax.
            newPixelValue = (newPixelValue < 0) ? 0 : (newPixelValue > img->vmax) ? img->vmax : newPixelValue;

            // Remplacement de la valeur du pixel par la nouvelle valeur
            img->grey[i][j] = newPixelValue;
        }
    }

    savePGM(img, "light.pgm");
}

#endif