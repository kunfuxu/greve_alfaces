#include "comboios.h"

SDL_Window* g_pWindow = NULL;
SDL_Renderer* g_pRenderer = NULL;
/* JANELA GRAFICA */
void PosInicial(COMBOIO *temp, FERROVIA* todas[]){
	temp->cart.linha_actual = procuraID(todas,temp->cart.lident,temp->cart.pident);//inicializa linha_actual
    temp->cart.PosiNoGraf[coordX] = temp->cart.linha_actual->pont.coord[coordX];//inicializa coordenadas da carruagem
    temp->cart.PosiNoGraf[coordY] = temp->cart.linha_actual->pont.coord[coordY];
}
int SDL_inicComboios(COMBOIO *todos[],int cores[][DIMrgb], FERROVIA* todas[]){
	int i, d, z;
	COMBOIO *temp;
    //inicializa todos os comboios no vetor de endereços COMBOIO
	for(i=0;i<MAX&&todos[i]!=NULL;++i){
		for(temp=todos[i], d=4;temp!=NULL; temp=temp->prox, d--){//cada carruagem vai andar menos para a frente ao ser incializada
            temp->PARACOMBOIO = false;//Comboios começam a andar
			PosInicial(temp, todas);
			for(z= d * 2 * (temp->cart.DimBOLAS); z>0; z--){//como e um raio DimBOLAS tem de ser multiplicado por 2
				moveCarr(temp, todas);
			}
			filledCircleRGBA(g_pRenderer,temp->cart.PosiNoGraf[coordX],temp->cart.PosiNoGraf[coordY],temp->cart.DimBOLAS,cores[temp->cart.cor][R],cores[temp->cart.cor][G],cores[temp->cart.cor][B],cores[temp->cart.cor][ALPA]);
            circleRGBA(g_pRenderer,temp->cart.PosiNoGraf[coordX],temp->cart.PosiNoGraf[coordY],temp->cart.DimBOLAS,0,0,0,255);
		}
	}
	if(verificaColisoes(todos)==1){
        return 1;
	}
	else
        return 0;
}
void moveCarr(COMBOIO *temp, FERROVIA* todas[]){//move carruagem ao longo da linha
    float X1, X2, Y1, Y2, m, b;

    if(temp->cart.locomotiva == 0 && temp->cart.cor==temp->cart.linha_actual->pont.cor && strcmp(temp->cart.linha_actual->pont.TipoDePonto,"EST")==0) {
        temp->cart.cor=CINZENTO; //muda a cor das carruagens nao locomotivas para cinzento quando passam por uma estaçao da sua cor
    }

	if(temp->cart.linha_actual->RA != NULL){
		X1=temp->cart.linha_actual->pont.coord[coordX];
		Y1=temp->cart.linha_actual->pont.coord[coordY];
		X2=temp->cart.linha_actual->RA->pont.coord[coordX];
		Y2=temp->cart.linha_actual->RA->pont.coord[coordY];

		if (X2-X1 != 0) {//se o declive existir
			m=(Y2-Y1)/(X2-X1);//calcula o declive da Ferrovia onde esta
			b=Y1-m*X1;//equação afim

			if(X2>X1)
				(temp->cart.PosiNoGraf[coordX])++;//vai para a frente no X
			else
				(temp->cart.PosiNoGraf[coordX])--;//diminui o X
			temp->cart.PosiNoGraf[coordY]=m*temp->cart.PosiNoGraf[coordX]+b;//desloca a carruagem ao longo da linha
			if(temp->cart.PosiNoGraf[coordX] == temp->cart.linha_actual->RA->pont.coord[coordX]){
				temp->cart.linha_actual = temp->cart.linha_actual->RA;//se chegar ao proximo ponto, troca de ponto atual
			}
		}else{
			//aumenta o Y apenas pois o declive é nulo
			if(Y2>Y1)
				(temp->cart.PosiNoGraf[coordY])++;
			else
				(temp->cart.PosiNoGraf[coordY])--;

			if(temp->cart.PosiNoGraf[coordY] == temp->cart.linha_actual->RA->pont.coord[coordY]){
				temp->cart.linha_actual = temp->cart.linha_actual->RA;//se chegar ao proximo ponto troca de ponto atual
			}
        }

    }else{
        temp->cart.nservico--;//chegou ao fim da linha, diminui numero de servicos, vai para pos inicial
		PosInicial(temp, todas);
    }
}
void trajectoriaComb(COMBOIO* todo, int cores[][DIMrgb], FERROVIA* todas[]){//desloca um comboio inteiro
    int corTemp, randoma;
    COMBOIO* temp;
	for(temp=todo; temp!=NULL; temp=temp->prox){
        corTemp=temp->cart.cor;//para piscar

        if(temp->PARACOMBOIO==false){// se nao houve colisoes move o comboio
            moveCarr(temp, todas);

        }else if (temp->cart.locomotiva==1){//pisca a cor do comboio quando esta parado
            randoma = rand()%2;
            if(randoma==1)
                temp->cart.cor=BRANCO;
        }
		if(temp->cart.nservico>0){
				filledCircleRGBA(g_pRenderer,temp->cart.PosiNoGraf[coordX],temp->cart.PosiNoGraf[coordY],temp->cart.DimBOLAS,cores[temp->cart.cor][R],cores[temp->cart.cor][G],cores[temp->cart.cor][B],cores[temp->cart.cor][ALPA]);
                circleRGBA(g_pRenderer,temp->cart.PosiNoGraf[coordX],temp->cart.PosiNoGraf[coordY],temp->cart.DimBOLAS,0,0,0,255);
		}else{//limpar a posição do comboio quando desaparece, para que nao haja uma colisão "fantasma" no seu ultimo ponto.
            temp->cart.PosiNoGraf[coordX] = 50000; //valor muito maior do que ecra com resolucao 16K
            temp->cart.PosiNoGraf[coordY] = 50000;
		}
		if(temp->cart.locomotiva==1){
			temp->cart.cor = corTemp;//a locomotiva nunca muda de cor, caso tenha mudado por estar parada, volta a inicial
		}
    }
}
void abreJanela(int dimJanela[], COMBOIO *todos[], FERROVIA *todas[], int cores[][DIMrgb]){
    SDL_Init(SDL_INIT_EVERYTHING);
    COMBOIO *temp = NULL;
    int end_game = 1, i;

    if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){//se SDL inicializado corretamente
		g_pWindow = SDL_CreateWindow("ComboioSim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dimJanela[coordX], dimJanela[coordY], SDL_WINDOW_SHOWN);
		if(g_pWindow != 0)
            g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
    }else{
		exit(0);
	}
	SDL_SetRenderDrawColor(g_pRenderer, cores[BRANCO][R], cores[BRANCO][G], cores[BRANCO][B], cores[BRANCO][ALPA]);
    SDL_RenderClear(g_pRenderer);//limpa janela com branco
	SDL_Event event;//inicializa evento
	SDL_unepontos(todas);//usa for_pontos para escrever linhas
	SDL_escrevePontos(todas,cores);//mostra pontos das linhas
	if(SDL_inicComboios(todos, cores, todas)==1){
        printf("ERRO, COMBOIOS INICIADOS PARA ALÉM DA DISTÂNCIA DE SEGURANÇA \n");
        SDL_DestroyRenderer(g_pRenderer);
        g_pRenderer=NULL;
        SDL_Quit();
        return;//se comboios incializados para alem de distancia sai do SDL
	}
	SDL_RenderPresent(g_pRenderer);
	SDL_Delay(1000);
    do{
        SDL_SetRenderDrawColor(g_pRenderer, cores[BRANCO][R], cores[BRANCO][G], cores[BRANCO][B], cores[BRANCO][ALPA]);
        SDL_RenderClear(g_pRenderer);// pinta o ecra da cor acima
		SDL_desenhaButoesHUD(dimJanela);
		SDL_unepontos(todas);
        SDL_escrevePontos(todas,cores);
        verificaColisoes(todos);//verifica colisoes entre todos os comboios
		for(i=0;i<MAX&&todos[i]!=NULL;i++){//para mover todos os comboios
			trajectoriaComb(todos[i], cores, todas);
		}
		SDL_RenderPresent(g_pRenderer);
		SDL_Delay(15);//velocidade de simulação
		while(SDL_PollEvent(&event)){//recebe primeiro evento da queue
			end_game = RegistoDeEventos(todas, todos, &event, dimJanela, cores);
		}
    }while(end_game);//ate fazer SDL_QUIT
    SDL_DestroyRenderer(g_pRenderer);//limpar SDL
    g_pRenderer=NULL;
    SDL_Quit();
    gfxPrimitivesSetFont(NULL, 0, 0);
    /*Para que as vidas dos comboios voltem ao numero inicial ao sair do SDL*/
    for(i=0;todos[i]!=NULL && i<MAX; i++){
        for(temp = todos[i]; temp !=NULL ; temp = temp->prox){
            if(temp->cart.locomotiva==0)
                temp->cart.cor = rand()%DIMCores;//da.para.jogar.outra.vez.com.comboios
            temp->cart.nservico= todos[i]->nServicoInicial;
        }
    }
    return;
}
void SDL_unepontos(FERROVIA *todas[]){
	int i;
	FERROVIA *head1,*head2;

	for(i=0; todas[i]!=NULL && i<MAX; ++i){
        head1=(todas[i]);
        head2=(todas[i]->RA);
		for_pontos(head1, head2);
	}//para unir dois pontos
}
void SDL_escrevePontos(FERROVIA *todas[],int cores[][DIMrgb]){
	int i;
	char lident[MAX];
	FERROVIA *head;
	for(i=0; i<MAX&&todas[i]!=NULL; ++i){
		strcpy(lident, todas[i]->lident);//para so escrever na mesma linha
        for(head = todas[i];head!=NULL && strcmp(lident, head->lident)==0; head=head->RA){
            filledCircleRGBA(g_pRenderer, head->pont.coord[coordX], head->pont.coord[coordY], head->pont.Dimensao, cores[head->pont.cor][R], cores[head->pont.cor][G], cores[head->pont.cor][B], cores[head->pont.cor][ALPA]);
            circleRGBA(g_pRenderer, head->pont.coord[coordX], head->pont.coord[coordY], head->pont.Dimensao,0,0,0,255);// contorno dos pontos, fica bonito

            if(head->RB!=NULL && strcmp(lident, head->RB->lident)==0){//se RB pertencer à linha
                head = head->RB;
                filledCircleRGBA(g_pRenderer, head->pont.coord[coordX], head->pont.coord[coordY], head->pont.Dimensao, cores[head->pont.cor][R], cores[head->pont.cor][G], cores[head->pont.cor][B], cores[head->pont.cor][ALPA]);
                circleRGBA(g_pRenderer, head->pont.coord[coordX], head->pont.coord[coordY], head->pont.Dimensao,0,0,0,255);
            }
        }
    }
}
void for_pontos(FERROVIA *head1, FERROVIA* head2){
	FERROVIA *head3, *headtemp;
	char lident[MAX];
	strcpy(lident, head1->lident);

	for(; head1!=NULL && head2!=NULL && (strcmp(head1->lident, lident) == 0); head1 = head1->RA, head2 = head2->RA){
		SDL_SetRenderDrawColor( g_pRenderer, 0, 0, 0, 255 );//se unir por RA a linha é preta(por onde passa comboio)
		SDL_RenderDrawLine(g_pRenderer, head1->pont.coord[coordX], head1->pont.coord[coordY], head2->pont.coord[coordX], head2->pont.coord[coordY]);
		
		if(head1->RB!=NULL){
			head3 = head1->RB;
			SDL_SetRenderDrawColor( g_pRenderer, 255, 0, 0, 255 );//se unir por RB linha é vermelha
			SDL_RenderDrawLine(g_pRenderer, head1->pont.coord[coordX], head1->pont.coord[coordY], head3->pont.coord[coordX], head3->pont.coord[coordY]);

			if((strcmp(head1->lident,head3->lident)==0) &&(head3->RA!=NULL)) {
				headtemp= head3;
				head3 = head3 ->RA;
				for_pontos(headtemp, head3);//para continuar a unir caso linhas estejam trocadas
            }
        }
	}
}
void SDL_desenhaButoesHUD(int dimJanela[]){
    SDL_Rect r1;
		//declara as 4 variáveis do retangulo
		r1.x = dimJanela[coordX] - 100;//para serem independentes da janela
		r1.y = dimJanela[coordY] - 45;
		r1.w = 80;
		r1.h = 20;
    SDL_Rect r2;
		r2.x = dimJanela[coordX] - 100;
		r2.y = dimJanela[coordY] - 75;
		r2.w = 80;
		r2.h = 20;
	SDL_Rect r3;
		r3.x = dimJanela[coordX] - 100;
		r3.y = dimJanela[coordY] - 105;
		r3.w = 80;
		r3.h = 20;

    SDL_SetRenderDrawColor( g_pRenderer, 200, 255, 100, 255 );
    SDL_RenderFillRect( g_pRenderer, &r1 );
    SDL_RenderFillRect( g_pRenderer, &r2 );
    SDL_RenderFillRect( g_pRenderer, &r3 );

    SDL_SetRenderDrawColor( g_pRenderer, 0, 0, 0, 255 );
	SDL_RenderDrawRect( g_pRenderer, &r1 );
    SDL_RenderDrawRect( g_pRenderer, &r2 );
    SDL_RenderDrawRect( g_pRenderer, &r3 );

    stringRGBA( g_pRenderer, r3.x+2, r3.y+4,"CONTINUAR",0,0,0,255 );
    stringRGBA( g_pRenderer, r2.x+2, r2.y+4,"SUSPENDER",0,0,0,255 );
    stringRGBA( g_pRenderer, r1.x+2, r1.y+4,"SDL QUIT",0,0,0,255 );
}
int RegistoDeEventos(FERROVIA *todas[], COMBOIO *todos[], SDL_Event *event, int dimJanela[], int cores[][DIMrgb]){
	int i, x, y;
	switch(event->type){//um event type, mas ja que tinhamos isto preparado para mais...
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&x, &y);
			if(x>dimJanela[coordX]-100 && x<dimJanela[coordX]-20){//se clicar num dos botoes
				if(y>dimJanela[coordY]-45 && y<dimJanela[coordY]-25){//sair SDL
					return 0;
				}
				else if(y>dimJanela[coordY]-75 && y<dimJanela[coordY]-55){//suspender
                    while(SDLsuspenso(todos, todas, dimJanela, cores, event));
				}
			}
			for(i=0;i<MAX && todos[i]!=NULL; ++i){//para verificar se clicou em alguma das locomotivas
				if(x<=(todos[i]->cart.PosiNoGraf[coordX] + todos[i]->cart.DimBOLAS) && x>=(todos[i]->cart.PosiNoGraf[coordX] - todos[i]->cart.DimBOLAS)
				&& y<=(todos[i]->cart.PosiNoGraf[coordY] + todos[i]->cart.DimBOLAS) && y>=(todos[i]->cart.PosiNoGraf[coordY] - todos[i]->cart.DimBOLAS)){
					clicaParaAnda(todas, todos[i], cores);//stop de comboio
				}
			}
			for(i=0;i<MAX && todas[i]!=NULL; ++i){//para verificar em todas as linhas
				clicaPonto(todas[i], x, y,todos);
			}
	}
	return 1;
}
void clicaParaAnda(FERROVIA *todas[], COMBOIO *todo, int cores[][DIMrgb]){
	COMBOIO* temp;
	int i;

	if(todo->PARACOMBOIO==false){
		for(temp=todo;temp!=NULL;temp=temp->prox){
			temp->PARACOMBOIO = true;//comboio para se a andar
		}
	}
	else if(todo->PARACOMBOIO == true){
		for(temp=todo;temp!=NULL;temp=temp->prox){
			temp->PARACOMBOIO = false;//comboio anda se tiver parado
		}
		for(i=0;i<5;i++){//para andar 5 vezes
			trajectoriaComb(todo, cores, todas);
		}
	}
}
void clicaPonto(FERROVIA *toda, int x, int y, COMBOIO *todos[]){
	for(;toda!=NULL && toda->RA!=NULL && strcmp(toda->lident, toda->RA->lident)==0;toda = toda->RA){//se a mesma linha continuar por RA
		if(toda->pont.nSaidas == 2){//se clicar num ponto com 2 saidas
			if(x<=(toda->pont.coord[coordX] + toda->pont.Dimensao) && x>=(toda->pont.coord[coordX] - toda->pont.Dimensao) &&
               y<=(toda->pont.coord[coordY] + toda->pont.Dimensao) && y>=(toda->pont.coord[coordY] - toda->pont.Dimensao) ){
				trocaCarris(toda, todos);
			}
		}
	}
	for(;toda!=NULL && toda->RB!=NULL && strcmp(toda->lident, toda->RB->lident)==0;toda = toda->RB){//se mesma linha continuar por RB
		if(toda->pont.nSaidas == 2){
			if(x<=(toda->pont.coord[coordX] + toda->pont.Dimensao) && x>=(toda->pont.coord[coordX] - toda->pont.Dimensao)
			&& y<=(toda->pont.coord[coordY] + toda->pont.Dimensao) && y>=(toda->pont.coord[coordY] - toda->pont.Dimensao)){
				trocaCarris(toda,todos);
			}
		}
	}
}
void menuSDL(char *opcao, SDL_Event *event, int dimJanela[]){
	char linha[MAX];
	char teste;
	printf("Press enter");
	getchar();
	printf ("\nEscolha uma das opções \n"
                "0 - Fechar menu (clicar continuar na janela SDL para continuar simulação)\n"
				"1 - Mostrar a informação de uma ferrovia \n"
				"2 - Eliminar uma ferrovia \n"
				"3 - Mostrar a informação de um comboio \n"
				"4 - Eliminar um comboio \n"
				"5 - Criar um comboio \n"
				"Opção: ");

	fgets(linha, MAX, stdin);
	sscanf(linha, "%c%c", opcao, &teste);
	printf("\n");

	if((teste!='\n' || *opcao>'5' || *opcao<'0')){
		printf("Insira apenas 1 numero entre 0 e 5 \n");
		menuSDL(opcao, event, dimJanela);
	}
}
int SDLsuspenso(COMBOIO *todos[], FERROVIA *todas[], int dimJanela[], int cores[][DIMrgb], SDL_Event *event){
	char opcao;
	int i,d,z;
	COMBOIO *temp;
	//se suspenso faz o MENU_SDL e verifica opcoes
	do{
		menuSDL(&opcao, event, dimJanela);
		switch(opcao)
		{
			case '1': // MOSTRA FERROVIA
				mostraLinha(todas);
				break;
			case '2': // ELIMINA FERROVIA
                eliminaLinha(todas,todos);
				break;
			case '3': // MOSTRA COMBOIOS
				mostraComboio(todos);
				break;
			case '4': // ELIMINA COMBOIOS
                eliminaComboio(todos); // Lista para depois eliminar por ID
 				break;
			case '5':
				criarComboio(todos, todas);
				for(i=0; i<MAX && todos[i]!=NULL; ++i){} // para avançar o i
                //inicializa o comboio adicionado
 				for(temp=todos[i-1], d=4;temp!=NULL; temp=temp->prox, d--){
					temp->PARACOMBOIO = false;
					PosInicial(temp, todas);

					for(z = d * 2 * (temp->cart.DimBOLAS); z>0; z--){
						moveCarr(temp, todas);
					}
					filledCircleRGBA(g_pRenderer,temp->cart.PosiNoGraf[coordX],temp->cart.PosiNoGraf[coordY],temp->cart.DimBOLAS,cores[temp->cart.cor][R],cores[temp->cart.cor][G],cores[temp->cart.cor][B],cores[temp->cart.cor][ALPA]);
                    circleRGBA(g_pRenderer,temp->cart.PosiNoGraf[coordX],temp->cart.PosiNoGraf[coordY],temp->cart.DimBOLAS,0,0,0,255);
				}
				if(verificaColisoes(todos)==1){// se o comboio adicionado estiver em colisao com os comboios existentes, o programa fecha
					printf("ERRO, COMBOIOS INICIADOS PARA ALÉM DA DISTÂNCIA DE SEGURANÇA \n");
					exit(0);
				}
				break;
		}
	}while(opcao!='0');//se igual a 0 iremos querer voltar a janela
	printf("Clique continue na JANELA!\n");
    while(SDL_verificaContinua(dimJanela, event));//após escolher opção zero verificar o click no butao continua
    return 0;
}
int SDL_verificaContinua(int dimJanela[], SDL_Event *event){//verifica se clicou continuar
     int x, y;
     while(SDL_WaitEvent(event)) {
        switch(event->type){
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);
                if(y>dimJanela[coordY]-105 && y<dimJanela[coordY]-85){//continuar
                    return 0;
                }
        }
     }
     return 2;
}
