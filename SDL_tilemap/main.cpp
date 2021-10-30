#include <SDL_image.h>
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

//Taille de mon écran de jeu (petit car petit tileset)
#define WIDTH 160 
#define HEIGHT 160

//Taile de ma tuile (16x16)
#define HTILE 16 
#define WTILE 16 

//Nombre de tuiles en ligne et colonnes
#define NBLIGNE 10
#define NBCOL 10

//Taille de mon tileset
#define WTILESET 10


//Paramètres
void genereNiveau(SDL_Texture* tileset, SDL_Renderer* rend, int table[][NBCOL], int largeurTile, int hauteurTile);


using namespace std;


//main
int main(int argc, char* argv[])
{
    //Initialisation de la fenêtre
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "error initializing SDL: %s\n" << SDL_GetError();
    }

    SDL_Window* win;
    SDL_Renderer* rend;

    //Création de la fenêtre
    win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    // Création du Renderer
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //Initialisation de mes map.txt
    ifstream myfile1("../map/map.txt");
    ifstream myfile2("../map/map2.txt");
    if (!myfile1 || !myfile2)
    {
        cout << "Error opening output file" << endl;
    }


    //Création de la texture
    SDL_Surface* image = IMG_Load("../assets/tilemapDT.png");
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
    }
    SDL_Texture* tileSet = SDL_CreateTextureFromSurface(rend, image);
    SDL_FreeSurface(image);

    int layer1[NBLIGNE][NBCOL];
    int layer2[NBLIGNE][NBCOL];


    //Initialisation de mes maps en 2 plans 
    for (int n_i = 0; n_i < NBLIGNE; n_i++)
    {
        for (int n_j = 0; n_j < NBCOL; n_j++)
        {
            myfile1 >> layer2[n_i][n_j];
            myfile2 >> layer1[n_i][n_j];
        }
    }

    myfile1.close();
    myfile2.close();

    int close = 0;


    while (!close)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type) {

            case SDL_QUIT:
                close = 1;
                break;
            }
        }

        SDL_RenderClear(rend);

        //Création de mes 2 maps superposées
        genereNiveau(tileSet, rend, layer1, WTILE, HTILE);
        genereNiveau(tileSet, rend, layer2, WTILE, HTILE);


        SDL_RenderPresent(rend);
    }


    return 1;
}

void genereNiveau(SDL_Texture* tileset, SDL_Renderer* rend, int table[][NBCOL], int largeurTile, int hauteurTile)
{
    int l, c;
    SDL_Rect rectSource, rectDest;

    rectDest.w = 10;
    rectDest.h = 10;
    rectSource.w = 16;
    rectSource.h = 16;

    for (l = 0; l < NBLIGNE; l++)
    {
        for (c = 0; c < NBCOL; c++)
        {
            rectDest.x = c * 10;
            rectDest.y = l * 10;

            //x_source = (numéro de la taille modulo Largeur de la taille du tileset) * (tilesize)
            //y_source = numéro de la tile / hauteur du tileset * tile size

            rectSource.x = (table[l][c] % WTILESET) * 16;
            rectSource.y = table[l][c] / WTILESET * 16;

            SDL_RenderCopy(rend, tileset, &rectSource, &rectDest);
        }
    }


}