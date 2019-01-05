void menuSDL(char *opcao){
	char linha[MAX];
	char teste;
	getchar();
	printf ("\nEscolha uma das opções \n"
				"0 - Sair da janela gráfica \n"
				"1 - Mostrar a informação de uma ferrovia \n"
				"2 - Eliminar uma ferrovia \n"
				"3 - Mostrar a informação de um comboio \n"
				"4 - Eliminar um comboio \n"
				"5 - Criar um comboio \n"
				"Para voltar aos comboios, clicar continuar na janela de SDL e click enter aqui (sem anda) \n"
				"Opção: ");
	fgets(linha, MAX, stdin);
	sscanf(linha, "%c%c", opcao, &teste);
	printf("\n");

	if((teste!='\n' || *opcao>'5' || *opcao<'1')&&(*opcao!='\n')){
		printf("Insira apenas 1 numero entre 0 e 5, clique enter");
		menuSDL(opcao);
	}

}

void SDLsuspenso(COMBOIO *todos[], FERROVIA *todas[], int cores[][DIMrgb], SDL_Event *event){
	char opcao;
	int i;
	
	do{
		menuSDL(opcao);
		switch(opcao)
		{
			case '1': // MOSTRA FERROVIA
				mostraLinha(todas);
				break;
			case '2': // ELIMINA FERROVIA
                eliminaLinha(todas);
				break;
			case '3': // MOSTRA COMBOIOS
				mostraComboio(todos);
				break;
			case '4': // ELIMINA COMBOIOS
                // Lista para depois eliminar por ID
                eliminaComboio(todos);
 				break;
			case '5':
				criarComboio(todos, todas);
				for(i=0; i<MAX && todos[i]!=NULL; ++i){}
				for(temp=todos[i];temp!=NULL; temp=temp->prox, d--){
					temp->PARACOMBOIO = false;
					PosInicial(temp, todas);
					for(z= d * 2 * (temp->cart.DimBOLAS); z>0; z--){
						moveCarr(temp, todas);
					}
					filledCircleRGBA(g_pRenderer,temp->cart.PosiNoGraf[coordX],temp->cart.PosiNoGraf[coordY],temp->cart.DimBOLAS,cores[temp->cart.cor][R],cores[temp->cart.cor][G],cores[temp->cart.cor][B],cores[temp->cart.cor][ALPA]);
				}
					
				if(verificaColisoes(todos)==1){
					printf("ERRO, COMBOIOS INICIADOS PARA ALÉM DA DISTÂNCIA DE SEGURANÇA \n");

					SDL_DestroyRenderer(g_pRenderer);
					g_pRenderer=NULL;

					SDL_Quit();
					gfxPrimitivesSetFont(NULL, 0, 0);
					
					break;
				}
				break;


	}while(opcao!='0');
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}

