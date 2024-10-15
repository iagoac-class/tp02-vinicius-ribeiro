#include <stdlib.h>
#include <stdio.h>
#include "arvores.h"

/*Retorna a altura do nó "n", ou 0, caso n seja NULL.*/
int altura(noAVL *n){
    if(n == NULL) return 0;
    else return n->altura;
}

/*Retorna o maior valor entre n1 e n2.*/
int max(int n1, int n2){
    if(n1 > n2) return n1;
    return n2;
}

/*Realiza rotação à esquerda.*/
noAVL *rot_esq(noAVL *raiz){
    noAVL *nova_raiz = raiz->dir;
    raiz->dir = nova_raiz->esq;
    nova_raiz->esq = raiz;

    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    nova_raiz->altura = max(altura(nova_raiz->esq), altura(nova_raiz->dir)) + 1;
    return nova_raiz;
}

/*Realiza rotação à direita.*/
noAVL *rot_dir(noAVL *raiz){
    noAVL *nova_raiz = raiz->esq;
    raiz->esq = nova_raiz->dir;
    nova_raiz->dir = raiz;
    
    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    nova_raiz->altura = max(altura(nova_raiz->esq), altura(nova_raiz->dir)) + 1;
    return nova_raiz;
}

/*Deve ser chamada após uma remoção na árvore AVL.
Percorre na árvore o caminho até onde estaria o valor "val", recém-removido.
A partir do nó-folha, recalcula a altura de cada nó e realiza rotações, caso
haja algum nó desbalanceado.*/
noAVL *nivelar_AVL(noAVL *raiz, int val)
{
    int n1, n2;

    /*Percorre recursivamente a árvore da raiz às folhas, no mesmo caminho
    onde ocorreu a remoção anterior (indicado pelo parâmetro "val").*/
    if(raiz->esq && val < raiz->valor) raiz->esq = nivelar_AVL(raiz->esq, val);
    else if(raiz->dir && val > raiz->valor) raiz->dir = nivelar_AVL(raiz->dir, val);

    /*Recalculando a altura do nó.*/
    n1 = altura(raiz->esq);
    n2 = altura(raiz->dir);
    
    raiz->altura = max(n1, n2) + 1;

    /*Comparando a altura das subárvores do nó atual.*/
    n1 = n1 - n2;

    /*Caso árvore pendendo para a direita.
    Realiza rotação à esquerda.*/
    if(n1 < -1)
    {
        /*Decide entre rotação simples ou dupla a partir da comparação
        entre a altura da subárvore à esquerda do filho à direita do nó atual
        e a altura da subárvore à direita do filho à direita do nó atual.*/
        if(altura(raiz->dir->esq) > altura(raiz->dir->dir))
        {
            /*Primeiro passo da rotação dupla à esquerda.*/
            raiz->dir = rot_dir(raiz->dir);
        } 
        return rot_esq(raiz);
    }

    /*Caso árvore pendendo para a esquerda.
    Realiza rotação à direita.*/
    if(n1 > 1)
    {
        /*Decide entre rotação simples ou dupla a partir da comparação
        entre a altura da subárvore à esquerda do filho à esquerda do nó atual
        e a altura da subárvore à direita do filho à esquerda do nó atual.*/
        if(altura(raiz->esq->dir) > altura(raiz->esq->esq))
        {
            /*Primeiro passo da rotação dupla à direita.*/
            raiz->esq = rot_esq(raiz->esq);
        }
        return rot_dir(raiz);
    }

    return raiz;
}

/*Cria um novo nó-folha, com valor especificado pelo parâmetro
"item" e altura 1. Retorna o endereço do nó criado.*/
noAVL *cria_AVL(int item) 
{
    noAVL *no_criado = (noAVL*) malloc(sizeof(noAVL));
    no_criado->esq = NULL;
    no_criado->dir = NULL;
    no_criado->valor = item;
    no_criado->altura = 1;
    return (no_criado);
}

/*Insere um nó na árvore e realiza a rotação necessária, caso a 
inserção desbalanceie a árvore.*/
noAVL *insere_AVL(noAVL *raiz, int valor)
{
    int n1, n2;

    /*Caso esteja no fim da árvore, cria e insere o novo nó como nó-folha na árvore.*/
    if(raiz == NULL) return (cria_AVL(valor));

    /*Avança até o fim da árvore em busca do lugar correto de inserção.*/
    if(raiz->valor > valor){
        raiz->esq = insere_AVL(raiz->esq, valor);
    } else if(raiz->valor < valor){
        raiz->dir = insere_AVL(raiz->dir, valor);
    } else {
        return (raiz);
    }

    /*Recalcula a altura de cada nó da aŕvore no caminho
    percorrido para inserção.*/
    n1 = altura(raiz->esq);
    n2 = altura(raiz->dir);

    raiz->altura = max(n1, n2) + 1;

    /*Gerando o fator de balanceamento do nó e o armazenando em "n1"*/
    n1 = n1 - n2;

    /*Rotação simples à esquerda.*/
    if(n1 < -1 && raiz->dir->valor < valor)
        return rot_esq(raiz);

    /*Rotação simples à direita.*/
    if(n1 > 1 && raiz->esq->valor > valor)
        return rot_dir(raiz);

    /*Rotação dupla à esquerda.*/
    if(n1 < -1 && raiz->dir->valor > valor){
        raiz->dir = rot_dir(raiz->dir);
        return rot_esq(raiz);
    }

    /*Rotação dupla à direita.*/
    if(n1 > 1 && raiz->esq->valor < valor){
        raiz->esq = rot_esq(raiz->esq);
        return rot_dir(raiz);
    }

    return (raiz);
}

/*Busca um valor na árvore. Retorna o endereço do nó, caso este
seja encontrado, ou NULL, caso o valor não esteja na árvore.*/
noAVL *busca_AVL(noAVL *n, int item)
{
    while(n != NULL && n->valor != item)
    {
        if(n->valor > item) n = n->esq;
        else n = n->dir;
    }
    return (n);
}

/*Retorna o endereço do pai do nó "busca" na árvore cuja raiz é "n".
Caso "n" seja a raiz, retorna NULL.*/
noAVL *busca_pai_AVL(noAVL *n, noAVL *busca)
{
    if(n->valor == busca->valor) return NULL;
    while(n->esq != busca && n->dir != busca)
    {
        if(busca->valor > n->valor) n = n->dir;
        else n = n->esq;
    }
    return (n);
}

/*Libera a memória alocada por uma árvore.*/
void libera_AVL(noAVL *raiz)
{
    if(raiz->esq != NULL) libera_AVL(raiz->esq);
    if(raiz->dir != NULL) libera_AVL(raiz->dir);
    free(raiz);
    return;
}

noAVL *rm_raiz_AVL(noAVL *rm)
{
    noAVL *ant, *n;
    ant = rm;
    n = rm->esq;
    if(n)
    {
        /*Caso o nó a ser removido tenha filho à esquerda, avançar o máximo à direita.*/
        while(n->dir != NULL)
        {
            ant = n;
            n = n->dir;
        }
        if(ant != rm) 
        {
            /*Caso "n" não seja filho à esquerda de "rm", subir o filho à esquerda de "n"
            e fazer o filho à esquerda de "rm" se tornar filho à esquerda de "n".*/
            ant->dir = n->esq;
            n->esq = rm->esq;
        }
        /*Filho à direita de "rm" se torna filho à direita de "n".*/
        n->dir = rm->dir;
    }
    else
    {
        /*Caso o nó a ser removido não tenha filho à esquerda, subir seu filho à direita.*/
        n = rm->dir;
    }
    /*Libera a memória alocada pelo nó.*/
    free(rm);
    /*"n" substitui "rm".*/
    return (n);
}

noAVL *remove_AVL(noAVL *raiz, int item)
{
    /*Busca o item a ser removido e armazena seu endereço em "rm".*/
    noAVL *rm = busca_AVL(raiz, item);
    if(rm)
    {
        /*Busca o pai do nó a ser removido.*/
        noAVL *rm_pai = busca_pai_AVL(raiz, rm);
        if(rm_pai)
        {
            /*Atualiza o ponteiro que armazenava o endereço do nó a ser removido.*/
            if(rm_pai->dir == rm) 
                rm_pai->dir = rm_raiz_AVL(rm);
            else rm_pai->esq = rm_raiz_AVL(rm);
        }
        /*Caso o nó a ser removido seja a raiz, atualiza "raiz".*/
        else raiz = rm_raiz_AVL(rm);
    }
    /*Chama a função para balancear a árvore após a remoção.*/
    if(raiz) raiz = nivelar_AVL(raiz, item);
    return (raiz);
}

void manip_AVL(noAVL *raiz, const char *caminho)
{
    char operacao;
    int valor;
    FILE *arq = fopen(caminho, "r");

    if(!arq) 
    {
        puts("Erro na abertura do arquivo.");
        return;
        /*Retorna caso o arquivo não possa ser aberto.*/
    }

    while(fscanf(arq, "%c %d", &operacao, &valor) != EOF)
    {
        /*Lê e executa todas as operações especificadas no arquivo.
        Execução é interrompida ao chegar ao fim do arquivo.*/
        switch(operacao)
        {
            case 'I':
            raiz = insere_AVL(raiz, valor);
            break;

            case 'R':
            raiz = remove_AVL(raiz, valor);
            break;
        }
    }
    fclose(arq);
}