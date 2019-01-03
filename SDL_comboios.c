#include "comboios.h"

SDL_Window* g_pWindow = NULL;
SDL_Renderer* g_pRenderer = NULL;

/*------------------------------- */
/* JANELA GRAFICA */


void abreJanela(int dimJanela[], COMBOIO *todos[], FERROVIA *todas[], int cores[][DIMrgb]){

    SDL_Init(SDL_INIT_EVERYTHING);

	coords[2];
	//SDL_Point CheckPontos[]

    if(SDL_Init(SDL_INIT_EVERYTHING) >= 0)
    {

		g_pWindow = SDL_CreateWindow("ComboioSim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		dimJanela[coordX], dimJanela[coordY], SDL_WINDOW_SHOWN);
	//se inicializa com sucesso inicia o render
		if(g_pWindow != 0)
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);

	}else {
		exit(0);
	}

// eliminar        SDL_Error();

         /*Select the color for drawing. It is set to white here. */
        SDL_SetRenderDrawColor(g_pRenderer, cores[BRANCO][R], cores[BRANCO][G], cores[BRANCO][B], cores[BRANCO][ALPA] );


        /* Clear the entire screen to our selected color. */
        SDL_RenderClear(g_pRenderer);

        /* Up until now everything was drawn behind the scenes.
           This will show the new contents of the window. */
        SDL_RenderPresent(g_pRenderer);

		SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

		//funcao ir buscar pontos as ferrovias para fazer draw line de cada linha e render dos pontos entre essas posicoes, isto deve estar dentro dum for

		//outra funcao para passar ponto inicial de comboio em coordenadas, se estiver no primeiro ponto da linha este deve somar a dim do comboio a posicao inicial


		//outra funcao para por os comboios a andar ate detetar

        /* Give us time to see the window. */
        SDL_Delay(5000);

        /* Always be sure to clean up */
        SDL_DestroyRenderer(g_pRenderer);
        g_pRenderer=NULL;

        SDL_Quit();


        return;



	// "limpa" a função de texto
	gfxPrimitivesSetFont(NULL, 0, 0);
}

