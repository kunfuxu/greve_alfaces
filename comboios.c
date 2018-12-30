#include "comboios.h"
SDL_Window* g_pWindow = NULL;
SDL_Renderer* g_pRenderer = NULL;

void menu(char *opcao){
	char linha[MAX];
	char teste;
	printf ("\nEscolha uma das opções \n"
				"0 - Terminar programa \n"
				"1 - Mostrar a informação de uma ferrovia \n"
				"2 - Eliminar uma ferrovia \n"
				"3 - Mostrar a informação de um comboio \n"
				"4 - Eliminar um comboio \n"
				"5 - Criar um comboio \n"
				"6 - Simulação dos comboios \n"
				"Opção: ");
	fgets(linha, MAX, stdin);
	sscanf(linha, "%c%c", opcao, &teste);
	printf("\n");

	if(teste!='\n' || *opcao>'6' || *opcao<'0' ){
		printf("Insira apenas 1 numero entre 0 e 6");
		menu(opcao);
	}
	/*if(*opcao>'6' || *opcao<'0'){
		printf("Insira apenas 1 numero entre 0 e 6");
		menu(opcao);
	}*/
}

/*-----------------COMBOIOS----------------------------*/

COMBOIO* inic_Comboios(CARRUAGEM dados){
	int i;

	COMBOIO *head = (COMBOIO*)calloc(1,sizeof(COMBOIO));
    if (head==NULL){
     printf("Falha na aquisiçao de bloco de memória, função inic comboio \n");
     exit(0);
    }

	head->prox=NULL;
	head->cart=dados;
	printf("%d cor car \n",dados.cor);

	for(i=0; i<3; ++i){

		dados.cor = rand()%10;
		printf("%d cor car \n",dados.cor);
		head = addi_Comboio(head, dados);
	}
    return head;
}

COMBOIO* addi_Comboio(COMBOIO* head,CARRUAGEM dados){
    COMBOIO* temp = head;

    COMBOIO* novo = (COMBOIO*) calloc(1,sizeof(COMBOIO) );

    if (novo == NULL){
        printf("Falha na aquisiçao de bloco de memória \n");
        return head;
    }
    novo->cart=dados;
    novo->prox=NULL;

    while (head->prox!=NULL ) //pretendemos inserir a carruagem sempre depois da anterior, ou seja, inserior sempre no fim da lista.
        head=head->prox;
    head->prox = novo;

    return temp;
}
void elimina_comboio(COMBOIO* lista[]/*,char cident[]*/){
    COMBOIO* temp;
    COMBOIO* head;
    char cident[MAX];
    int i,j;

    if (ListaComboio(lista)==0){
        return;
    }
    /*Procura de COMBOIO A ELIMINAR*/
    printf("Inserir ID do comboio a eliminar \n");
    scanf("%s", cident);

    for(i=0 ; i<MAX; i++){

        if(lista[i]==NULL){
           // se nao existirem linhas, esta condição é sempre verificada.
           printf("ERRO, ID de linha nao encontrado, verificar ficheiro de config!");
           return;
        }else if(strcmp(cident,lista[i]->cart.cident)==0){
           head=lista[i];
           printf("MATCH \n");
           break;
        }
        printf("i = %d -> cident = %s \n",i,lista[i]->cart.cident);
      }
    /*REORDENAÇÃO*/
    printf("Reordenação \n");
    for(j=0;j<MAX;j++){
        if(lista[j]==NULL)
            break;
    }
    if(j!=0){
         lista[i]=lista[j-1];
         lista[j-1]=NULL;
    }
    /*-----------------*/

    printf("Libertação \n");

    while(head!=NULL){
        temp = head;
        head=head->prox;
        free(temp);
    }
    printf("COMBOIO %s ELIMINADO EBIC STYLE B) \n", cident);
    printf("No lugar deste ficou : %s \n",lista[i]->cart.cident);// debug
}
int ListaComboio(COMBOIO* lista[]){
    int i;
    if (lista[0]==NULL){
        printf("É dia de greve da CP sem serviços mínimos (TODOS os comboios suprimidos)! \n");
        return 0;
    }
    printf("Lista de Comboios: ");
    for (i=0;i<MAX&&lista[i]!=NULL;i++){

        printf("%s %p ",lista[i]->cart.cident, (void*)lista[i]);
    }
    printf("\nTotal Comboios %d\n",i);
    return 1;
}
void mostraComboio(COMBOIO* lista[]) {
    //WIP
     if (ListaComboio(lista)==0){
        return;
    }
    int i;
    char cident[MAX];
    COMBOIO* topo = NULL;

    printf("Inserir ID do Comboio a mostrar \n");
    scanf("%s",cident);
    printf("cident : %s \n", cident);

    for (i=0; i<MAX&&lista[i]!=NULL ;i++){
         if ( strcmp (cident, lista[i]->cart.cident)==0 ){
             topo=lista[i];
             break;
         }
    }

    if (topo==NULL){
        printf("Comboio inexistente ! Verificar ID \n \n");
        return;
    }

   for(; topo!=NULL; topo=topo->prox)
    {
       //Poderao ser adicionadas outras coisas
       printf("ID : %s nCOR: %d \n",topo->cart.cident, topo->cart.cor);
    }
}
/*------------------LINHAS-------------------*/
FERROVIA* inic_Linha(char lident[], PONTOS dados){
    FERROVIA*head = (FERROVIA*)calloc(1,sizeof(FERROVIA));
    if (head==NULL){
     printf("Falha na aquisiçao de bloco de memória, função inic_Linha \n");
     exit(0);
    }
    strcpy(head->lident, lident);
    head->RA=NULL;
    head->RB=NULL;


    head->pont = dados;

    return head;
}

FERROVIA* addi_Linha(FERROVIA* head,PONTOS dados){

    FERROVIA* temp=head;
    FERROVIA* novo=(FERROVIA*)calloc(1,sizeof(FERROVIA));

    if (novo==NULL){
        printf("Falha na alocação de memória, addi_linha \n");
        return 0;
    }

    novo->pont=dados;
    // significa que a linha já existe.
    novo->pont.nEntradas = 1;

    novo->RA=NULL;
    novo->RB=NULL;
    strcpy(novo->lident, head->lident);

    while(head->RA != NULL){
        head=head->RA;
    }
    head->RA=novo;
    head->pont.nSaidas = 1;
    return temp;

}

void KonnectLinhas(FERROVIA* lista[], char lident_Sai[], char lident_Recebe[],char ID_Sai[],char ID_Entra[]) {
    // W-MUITO-IP
    /*FAZER VERIFICAÇAO DE NUMERO DE SAIDAS E ENTRADAS*/
    printf("(connect linhas)ID saida: %s , ID entrada:  %s \n", ID_Sai, ID_Entra);

    FERROVIA* ligaEntrada = NULL;
    FERROVIA* ligaSaida = NULL;
    //printf("ID saida: %p , ID entrada:  %p \n", (void*)ligaSaida, (void*)ligaEntrada);
    ligaEntrada = ProcuraID(lista,lident_Recebe,ID_Entra);
    ligaSaida = ProcuraID(lista,lident_Sai,ID_Sai);

    if ( ligaSaida->pont.nSaidas == 2){
        printf("ERRO, Capacidade de saídas do ponto %s excedido \n",ID_Sai);
        return;
    }else if(ligaEntrada->pont.nEntradas==2){
        printf("ERRO, Capacidade de entradas do ponto %s excedido \n",ID_Entra);
        return;
    }

    //printf("ID saida: %p , ID entrada:  %p \n", ligaSaida, ligaEntrada);

    if (ligaSaida -> RA==NULL) {
        printf("ligou por RA \n");
        ligaSaida -> RA = ligaEntrada;
        ligaSaida ->pont.nSaidas++;
        ligaEntrada ->pont.nEntradas++;

    }else {
        printf("ligou por RB \n");
        ligaSaida -> RB = ligaEntrada;
        ligaSaida ->pont.nSaidas++;
        ligaEntrada ->pont.nEntradas++;

    }
}

int ListaFerrovias(FERROVIA* lista[]){
    int i;
    printf("Lista de Ferrovias: ");

    if (lista[0]==NULL){
        printf("Não existem linhas! \n");
        return 0;
    }

    for (i=0;i<MAX&&lista[i]!=NULL;i++){
        printf("%s ",lista[i]->lident);
    }
    printf("\nTotal de ferrovias: %d\n",i);
    return 1;

}

void mostraLinha(FERROVIA* lista[]) {
    //WIP
    if(ListaFerrovias(lista)==0){
        return;
    }
    char lident[MAX];
    printf("Escreva o ID da Ferrovia a apresentar sem espaços! \n");
    scanf("%100s",lident);//fazer isto em todos os scanf
    FERROVIA* head = NULL;
    int i;
    for( i=0 ; i<MAX ; i++){
        printf("i =%d \n",i);

        if (lista[i]!=NULL){
            printf("lident = %s \n",lista[i]->lident);

            if(strcmp(lident,lista[i]->lident)==0){
               head=lista[i];
               break;
           }
        }
     }
    if (head==NULL){
        // se nao existirem linhas, esta condição é sempre verificada.
       printf("ERRO, ID de linha nao encontrado! Verificar se o ID foi introduzido correctamente\n");
       return;
    }

    while(head!=NULL && strcmp(lident,head->lident) == 0 )

    {
       printf("endereço :  %p \n",(void*)head);
       printf("IDENTIFICADOR : %s \n",head->pont.pident);
       printf("nEntradas %d  nSaidas %d \n\n", head->pont.nEntradas,head->pont.nSaidas);
       head=head->RA;

    }


}
void elimina_linha(FERROVIA* lista[]){
    FERROVIA* temp = NULL;
    FERROVIA* head = NULL;
    char lident[MAX];
    int i,j;

    if (ListaFerrovias(lista)==0){
        return;
    };// Lista as ferrovias por ID

    printf("Escreva o ID da Ferrovia a eliminar sem espaços! \n");
    scanf("%s",lident);

    for( i=0 ; i<MAX ; i++){ //Procura da linha com ID = lident
        printf("i =%d \n",i);

        if (lista[i]!=NULL){
            printf("lident = %s \n",lista[i]->lident);

            if(strcmp(lident,lista[i]->lident)==0){
               head=lista[i];
               break;
           }
        }
    }
    if (head==NULL){
        // se nao existirem linhas, esta condição é sempre verificada.
       printf("ERRO, ID de linha nao encontrado");
       return;
    }
    verificaAcessos(lista,lident);//ajusta o nEntradas e nSaidas
    printf("Reordenação \n");
    for(j=0;j<MAX;j++){

        if(lista[j]==NULL)
            break;
    }
    if(j!=0){
         lista[i]=lista[j-1]; // colocamos o ultimo elemento da lista no lugar do elemento a ser eliminado.
         lista[j-1]=NULL; // limpamos a anterior última célula com dados úteis.
    }
    printf("Libertação \n");//free no bloco de memória

    while((head!=NULL)&&(strcmp(head->lident,lident)==0)){/*para que apenas a linha com o ID passado seja eliminada*/
        temp = head;
        head=head->RA;
        free(temp);
        temp = NULL;
    }
    printf("FERROVIA %s ELIMINADA EBIC STYLE B) \n", lident);
}
/* funções de apoio e debug*/
void verificaAcessos(FERROVIA* lista[],char ident[]){
    //queremos que a todas as linhas com saída para a linha com ID "ident" seja reduzida 1 saída.
    //queremos que a todas as linhas com entradas provenientes da linha com ID "ident" seja reduzida 1 entrada.
    int k,p,l;
    FERROVIA* temp;
    //É preciso só verificar um numero p de pontos que será o numero total de pontos da linha K.

    for(k=0, temp=lista[k]; lista[k] != NULL && k<MAX ; k++,temp=lista[k]) {
        // percorremos todas as linhas existentes.
        printf("OK \n");
        fflush(stdout);

        l=nPontos(lista[k]);// aqui para só correr uma vez e não ~p vezes no while

        for(p=0; p < l;p++) {

           // printf("%p =?= %p", temp->RA, lista[k]);
            printf("OK i= %d ID : %s \n", k, temp->RA->lident);
            printf("OK i= %d ID RB : %s \n", k, temp->RB->lident);
            if ( strcmp(temp->RA->lident,ident) == 0 && strcmp(temp->lident,ident)!=0 ){//se a proximo ponto(RA) pertencer à estação a eliminar e não estivermos sobre ela
                printf("Retirando saída %s, %s \n", temp->lident, temp->pont.pident);
                temp->pont.nSaidas--;

            }else if(strcmp(temp->lident,ident)==0 && strcmp(temp->RA->lident,ident) != 0) {//estamos sobre a estaçao a eliminar mas o prox ponto nao pertence a ela
                printf("Retirando entrada %s, %s \n", temp->lident, temp->pont.pident);
                temp->RA->pont.nEntradas--;
            }

            if (temp->RB != NULL){//nem sempre existe
                if ( strcmp(temp->RB->lident,ident) == 0 && strcmp(temp->lident,ident) != 0){//se a proximo ponto(RB) pertencer à estação a eliminar e não estivermos sobre ela
                    printf("Retirando saída RB %s, %s \n", temp->lident, temp->pont.pident);
                    temp ->pont.nSaidas --;

                }else if(strcmp(temp->lident,ident)== 0 && strcmp(temp->RB->lident,ident) != 0) {//estamos sobre a estaçao a eliminar mas o prox ponto nao pertence a ela
                    printf("Retirando entrada RB %s, %s \n", temp->lident, temp->pont.pident);
                    temp->RB->pont.nEntradas--;
                }
            }
            temp=temp->RA;
        }
    }
}

FERROVIA* ProcuraID(FERROVIA* lista[],char lident[],char IDE_X[]){
    int i=0,k=0;

    FERROVIA* TempX=NULL;

    for(i=0;i<MAX;i++){
        if (lista[i]!=NULL){
             k = strcmp(lident,lista[i]->lident);
//             printf ("k = %d \n", k );

            if( k == 0 ) {
                //printf (" %d \n",strcmp(lident,lista[i]->lident)==0 );
                TempX=lista[i];
                printf("match em TempX = %p de lista[%d] \n",(void*)TempX,i);
    //          scanf("%c",&debug);
                break;
            }
        }
    }
    if (TempX==NULL){
       printf("ERRO, ID de linha nao encontrado, verificar ficheiro de config!");
       exit(0);
    }
    while( (strcmp(TempX->lident,lident)==0) && (strcmp( TempX->pont.pident,IDE_X ) != 0 ) ){

        if(TempX->RA != NULL){
            printf("IDENTIFICADOR : %s \n",TempX->pont.pident);
            TempX=TempX->RA;
        }else{
            printf("ERRO, ID de ponto nao encontrado, verificar ficheiro de config!");
            exit(0);
        }
    }
    printf("MATCH : %s \n",TempX->pont.pident);
    printf(" AT Address : %p \n",(void*)TempX);
    return TempX;
 }

void trocaCarris(FERROVIA* PercursoA){
    /*WIP*/
    /*É importante correr esta função duas vezes, uma vez para o comboio passar e segunda depois do comboio passar,
    porque se alguém apagar uma linha enquanto os carris estão trocados, em vez de apagar essa linha vai apagar uma mistura de linhas.
    */
    FERROVIA *temp1=PercursoA->RA;
    PercursoA->RA = PercursoA->RB;
    PercursoA->RB = temp1->RA;

 }

void mostracores(int cores[DIMCores][DIMrgb]){
    int i,j;
    for(i=0;i<DIMCores;i++){
        for(j=0;j<DIMrgb;j++)
            printf("%d, ", cores[i][j]);

        printf("\n");
    };
}

int ConvCor(char corestr[]){
    //printf("printei isto ::: === %s", corestr);

    if ( strcmp(corestr,"VERMELHO") == 0 ){
        return VERMELHO;

    }else if ( strcmp(corestr,"AMARELO")  == 0 ) {
        return AMARELO;

    }else if ( strcmp(corestr,"CYAN")  == 0 ) {
        return CYAN;

    }else if ( strcmp(corestr,"ROXO")  == 0 ) {
        return ROXO;

    }else if ( strcmp(corestr,"VERDE")  == 0 ) {
        return VERDE;

    }else if ( strcmp(corestr,"CASTANHO")  == 0 ) {
        return CASTANHO;

    }else if ( strcmp(corestr,"PRETO")  == 0 ) {
        return PRETO;

    }else if ( strcmp(corestr,"CINZENTO")  == 0 ) {
        return CINZENTO;

    }else if ( strcmp(corestr,"BRANCO")  == 0 ) {
        return BRANCO;

    }else if ( strcmp(corestr,"AZUL")  == 0 ) {
        return AZUL;

    } else {
        printf("Erro na conversão de cores, atenção utilizar só as cores especificadas:\n"
               "VERMELHO, AZUL, AMARELO, CYAN, ROXO, VERDE, CASTANHO, PRETO, CINZENTO, BRANCO \n\n");
        return 100;
    }
}

void InputRefinado(char cident[]){

//possivelmente desnecessário
   scanf("%s",cident);
//introduzir condiçoes necessárias

}

int nPontos(FERROVIA* lista){
    int counter=1;
    while (lista!=NULL&&strcmp(lista->lident,lista->RA->lident)==0){
        counter++;
        lista=lista->RA;
    }
    printf("ID:%s nPontos = %d\n", lista->lident, counter);
    return counter;
}
/*------------------------------- */
/* JANELA GRAFICA */


/*void AbreJanela(int dimJanela[]){

    SDL_Init(SDL_INIT_EVERYTHING);
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

        //SDL_Error();

         Select the color for drawing. It is set to red here. */
        //SDL_SetRenderDrawColor(g_pRenderer, 255, 0, 0, 255);

        /* Clear the entire screen to our selected color. */
        //SDL_RenderClear(g_pRenderer);

        /* Up until now everything was drawn behind the scenes.
           This will show the new, red contents of the window. */
        //SDL_RenderPresent(g_pRenderer);

        /* Give us time to see the window. */
        //SDL_Delay(5000);

        /* Always be sure to clean up */
        //SDL_DestroyRenderer(g_pRenderer);
        //g_pRenderer=NULL;

        //SDL_Quit();


    //    return;



	// "limpa" a função de texto
	//gfxPrimitivesSetFont(NULL, 0, 0);
//}


