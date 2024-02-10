#include<stdio.h>
#include<stdlib.h>

typedef struct arvore
{
	int chave;
	struct arvore *esquerda;
	struct arvore *direita;
}Arvore;

typedef struct no
{
    Arvore *raiz;
    struct no *proximo;
}No;

typedef struct fila
{
    No *inicio;
    No *fim;
    int tamanho;
}Fila;

typedef struct pilha
{
    No *topo;
    int tamanho;
}Pilha;

Pilha* pilhaIniciar()
{
    Pilha *pilha = (Pilha*) malloc(sizeof(Pilha));
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

void pilhaEmpilhar(Pilha *pilha,Arvore *raiz)
{
    No *no = (No*) malloc(sizeof(No));
    no->raiz = raiz;
    no->proximo = pilha->topo;
    pilha->topo = no;
    pilha->tamanho++;
}

Arvore* pilhaDesempilhar(Pilha *pilha)
{
    Arvore *raiz;
    No *aux = pilha->topo;
    pilha->topo = aux->proximo;
    pilha->tamanho--;
    raiz = aux->raiz;
    free(aux);
    return raiz;
}

Fila* filaIniciar()
{
    Fila *fila = (Fila*) malloc(sizeof(Fila));
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    return fila;
}

void filaAdicionar(Fila *fila,Arvore *raiz)
{
    fila->tamanho++;
    if(fila->inicio==NULL)
    {
        fila->inicio = (No*) malloc(sizeof(No));
        fila->inicio->raiz = raiz;
        fila->inicio->proximo = NULL;
        fila->fim = fila->inicio;
        return;
    }
    No *aux = (No*) malloc(sizeof(No));
    aux->raiz = raiz;
    fila->fim->proximo = aux;
    fila->fim = aux;
}

void filaAdicionarOrdemCrescente(Fila *fila,Arvore *raiz)
{
    fila->tamanho++;
    if(fila->inicio==NULL)
    {
        fila->inicio = (No*) malloc(sizeof(No));
        fila->inicio->raiz = raiz;
        fila->inicio->proximo = NULL;
        fila->fim = fila->inicio;
        return;
    }
    int valor = raiz->chave;

    No *anterior = fila->inicio;
    No *aux = anterior;
    if(aux->raiz->chave>valor)
    {
        aux = (No*) malloc(sizeof(No));
        aux->raiz = raiz;
        aux->proximo = fila->inicio;
        fila->inicio = aux;
        return;
    }
    for(int i=0;i<fila->tamanho-2;i++)
    {
        anterior = aux;
        aux = aux->proximo;
        if(aux->raiz->chave>valor)
        {
            aux = (No*) malloc(sizeof(No));
            aux->raiz = raiz;
            aux->proximo = anterior->proximo;
            anterior->proximo = aux;
            return;
        }
    }
    aux = (No*) malloc(sizeof(No));
    aux->raiz = raiz;
    fila->fim->proximo = aux;
    fila->fim = aux;
}

Arvore* filaRemover(Fila *fila)
{
    fila->tamanho--;
    No *aux = (No*) malloc(sizeof(No));
    if(fila->inicio==fila->fim)
    {
        aux = fila->inicio;
        Arvore *raiz = aux->raiz;
        fila->inicio = NULL;
        fila->fim = NULL;
        free(aux);
        return raiz;
    }
    aux = fila->inicio;
    fila->inicio = aux->proximo;
    Arvore *raiz = aux->raiz;
    free(aux);
    return raiz;
}

void percursoEmNivel(Arvore **arvore)
{
    Arvore *atual;
    atual = *arvore;
    Fila *fila = filaIniciar(fila);
    filaAdicionar(fila,atual);
    while(fila->tamanho>0)
    {
        atual = filaRemover(fila);
        printf("%d ",atual->chave);
        if(atual->esquerda!=NULL)
            filaAdicionar(fila,atual->esquerda);
        if(atual->direita!=NULL)
            filaAdicionar(fila,atual->direita);
    }
    free(fila);
    printf("\n");
}

void percursoEmOrdemSimetrica(Arvore **arvore)
{
    Arvore *atual;
    Fila *aux = filaIniciar(aux);
    Fila *fila = filaIniciar(fila);
    filaAdicionar(aux,*arvore);
    filaAdicionarOrdemCrescente(fila,*arvore);
    while(aux->tamanho>0)
    {
        atual = filaRemover(aux);
        if(atual->esquerda!=NULL)
        {
            filaAdicionar(aux,atual->esquerda);
            filaAdicionarOrdemCrescente(fila,atual->esquerda);
        }

        if(atual->direita!=NULL)
        {
            filaAdicionar(aux,atual->direita);
            filaAdicionarOrdemCrescente(fila,atual->direita);
        }
    }
    free(aux);
    while(fila->tamanho>0)
    {
        atual = filaRemover(fila);
        printf("%d ",atual->chave);
    }
    free(fila);
    printf("\n");
}

void percursoEmPreOrdem(Arvore **arvore)
{
    Arvore *atual;
    atual = *arvore;
    Pilha *pilha = pilhaIniciar();
    while(1)
    {
        printf("%d ",atual->chave);
        if(pilha->tamanho==0&&atual->esquerda==NULL)
        {
            free(pilha);
            printf("\n");
            return;
        }
        if(atual->direita!=NULL)
            pilhaEmpilhar(pilha,atual->direita);
        if(atual->esquerda==NULL)
            atual = pilhaDesempilhar(pilha);
        else
            atual = atual->esquerda;
    }
}

void percursoEmPosOrdem(Arvore **arvore)
{
    Arvore *atual,*anterior;
    int stop = 1;
    atual = *arvore;
    Pilha *pilha = pilhaIniciar();
    while(stop)
    {
        while(1)
        {
            pilhaEmpilhar(pilha,atual);
            if(atual->direita!=NULL)
                pilhaEmpilhar(pilha,atual->direita);
            if(atual->esquerda==NULL)
                break;
            atual = atual->esquerda;
        }
        while(1)
        {
            anterior = atual;
            atual = pilhaDesempilhar(pilha);
            if(atual->esquerda!=anterior&&atual->direita!=anterior)
            {
                if(atual->direita!=NULL||atual->esquerda!=NULL)
                    break;
            }
            printf("%d ",atual->chave,pilha->tamanho);
            if(pilha->tamanho==0)
            {
                stop = 0;
                break;
            }
        }
    }
    printf("\n");
}

Arvore* criarNovoNodo()
{
    Arvore *arvore = (Arvore*) malloc(sizeof(Arvore));
    arvore->esquerda = NULL;
    arvore->direita = NULL;
    return arvore;
}

void inserirElementoArvore(Arvore **raiz,int chave)
{
    if(*raiz==NULL)
    {
        *raiz=criarNovoNodo();
        (*raiz)->chave = chave;
        return;
    }
    Arvore *anterior,*atual;
    anterior = *raiz;
    atual = *raiz;
    int lado = 0;
    while(atual!=NULL)
    {
        anterior = atual;
        if(chave<atual->chave)
        {
            atual=atual->esquerda;
            lado = 0;
        }
        else
        {
            atual=atual->direita;
            lado = 1;
        }
    }
    atual = criarNovoNodo();
    atual->chave = chave;
    if(lado)
        anterior->direita = atual;
    else
        anterior->esquerda = atual;
}

Arvore* buscarElementoArvore(Arvore **raiz,int chave)
{
    Arvore *aux = *raiz;
    while(1)
    {
        if(aux==NULL)
            return NULL;
        if(aux->chave==chave)
            return aux;
        if(chave<aux->chave)
            aux = aux->esquerda;
        else
            aux = aux->direita;
    }
}

int removerElementoArvore(Arvore **raiz,int chave)
{
    if(buscarElementoArvore(&(*raiz),chave)==NULL)
        return 0;
    Arvore *retorno;
    Arvore *anterior,*atual;
    anterior = *raiz;
    atual = *raiz;
    while(atual->chave!=chave)
    {
        anterior = atual;
        if(chave<atual->chave)
            atual = atual->esquerda;
        else
            atual = atual->direita;
        if(atual==NULL)
            return 1;
    }
    if(atual->direita==NULL && atual->esquerda==NULL)
    {
        if((*raiz)->chave==chave)
        {
            *raiz=NULL;
            free(atual);
            return 1;
        }
        if(anterior->direita==atual)
            anterior->direita=NULL;
        else
            anterior->esquerda=NULL;
        free(atual);
        return 1;
    }
    //----------------------------------------------//
    if(atual->direita==NULL)
    {
        if((*raiz)->chave==chave)
            *raiz=(*raiz)->esquerda;

        if(anterior->direita==atual)
            anterior->direita=atual->esquerda;
        else
            anterior->esquerda=atual->esquerda;
        free(atual);
        return 1;
    }
    if(atual->esquerda==NULL)
    {
        if((*raiz)->chave==chave)
            *raiz=(*raiz)->direita;

        if(anterior->direita==atual)
            anterior->direita=atual->direita;
        else
            anterior->esquerda=atual->direita;
        free(atual);
        return 1;
    }
    //----------------------------------------------//
    //Certeza de ter 2 folhas
    Arvore *aux;
    anterior = atual;
    aux = atual->esquerda;
    while(aux->direita!=NULL)
    {
        anterior = aux;
        aux=aux->direita;
    }
    atual->chave=aux->chave;
    if(anterior->direita==aux)
        anterior->direita=aux->esquerda;
    if(anterior->esquerda==aux)
        anterior->esquerda=aux->esquerda;
    free(aux);
    return 1;
}

void inicializarArvore(Arvore **arvore)
{
	*arvore = NULL;
}

int main()
{
	Arvore *arvore;
	inicializarArvore(&arvore);
    int op=1,numero;
	while(op!=-1)
    {
        scanf("%d",&op);
        switch(op)
        {
        case 1:
            scanf("%d",&numero);
            inserirElementoArvore(&arvore,numero);
            printf("inserido\n");
            break;
        case 2:
            scanf("%d",&numero);
            if(removerElementoArvore(&arvore,numero))
                printf("removido\n");
            else
                printf("nao encontrado\n");
            break;
        case 3:
            scanf("%d",&numero);
            if(buscarElementoArvore(&arvore,numero)!=NULL)
                printf("encontrado\n");
            else
                printf("nao encontrado\n");
            break;
        case 4:
            percursoEmNivel(&arvore);
            break;
        case 5:
            percursoEmOrdemSimetrica(&arvore);
            break;
        case 6:
            percursoEmPreOrdem(&arvore);
            break;
        case 7:
            percursoEmPosOrdem(&arvore);
            break;
        }
    }
	return 0;
}

/*inserirElementoArvore(&arvore,35);
	inserirElementoArvore(&arvore,80);
	inserirElementoArvore(&arvore,73);
	inserirElementoArvore(&arvore,7);
	inserirElementoArvore(&arvore,11);
	inserirElementoArvore(&arvore,15);
	inserirElementoArvore(&arvore,12);
	inserirElementoArvore(&arvore,6);
	inserirElementoArvore(&arvore,9);
	inserirElementoArvore(&arvore,1);
	inserirElementoArvore(&arvore,2);
	inserirElementoArvore(&arvore,13);
	inserirElementoArvore(&arvore,71);
	inserirElementoArvore(&arvore,70);
	inserirElementoArvore(&arvore,74);
    //removerElementoArvore(&arvore,15);
    //printf("%d ",arvore->direita->chave);
    //percursoEmOrdemSimetrica(&arvore);
    //percursoEmPreOrdem(&arvore);
    percursoEmPosOrdem(&arvore);
    //percursoEmNivel(&arvore);*/
