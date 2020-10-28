/*
RAFAEL RUTZ DOS SANTOS - 17102565 - ENGENHARIA DE COMPUTAÇÃO

O programa está funcionando corretamente.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <ctime>

#include <iostream>
#include <list>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

/*
Crie um programa no modelo produtor/consumidor. Este
programa deve receber 3 valores numéricos, informando o
número de threads produtores e threads consumidores
que devem ser criados e o número de itens a serem
produzidos. O programa deve manipular um buffer
compartilhado e permitir, com auxílio de variável de
condição, garantir coordenação no acesso ao buffer
compartilhado.
*/

//classe que engloba os dados de buffer e o valor V a ser inserido no buffer, passados para produtores e consumidores
class Input
{
public:
    std::list<int> buffer;
     int nItems;            //numero total de itens
    int itemsToInsert;     //numero de itens que ainda serao inseridos
    int itemsToRemove;     //numero de itens que ainda serao removidos
    int maxBufferSize;     //numero maximo de itens no buffer compartilhado
};

void *insertBuffer(void *Input);
void *deleteBuffer(void *buffer);

pthread_mutex_t mutex;    //variavel de exclusao mutua
pthread_cond_t condProds; //variavel de condicao para funcionamento dos produtores
pthread_cond_t condCons;  //variavel de condicao para funcionamento dos consumidores

//usados como na especificacao do trabalho

int main(int argc, char const *argv[])
{
    clock_t time = clock();

    Input *input = new Input();

    int nProds, nCons, nItems;

    if (argc < 4)
    {
        cout << "Input error! The input should have this format:\n <producers> <consumers> <items>" << endl;
        exit(1);
    }

    nProds = atoi(argv[1]);                         //numero de produtores
    nCons = atoi(argv[2]);                          //numero de consumidores
    nItems = atoi(argv[3]);                         //numero de itens

    input->nItems = nItems;
    input->maxBufferSize = nItems;
    input->itemsToInsert = nItems;
    input->itemsToRemove = nItems;

    pthread_t producers[nProds];                    //threads produtoras
    pthread_t consumers[nCons];                     //threads consumidoras

    pthread_mutex_init(&mutex, NULL);               //inicializa mutex
    pthread_cond_init(&condProds, NULL);            //inicializa variavel de condicao condProds
    pthread_cond_init(&condCons, NULL);             //inicializa variavel de condicao condCons

    int i;

    //Criacao dos produtores
    for (i = 0; i < nProds; i++)
    {
        pthread_create(&(producers[i]), NULL, insertBuffer, (void *)input);
        cout << "Created producer " << i << endl;
    }

    //Criacao dos consumidores
    for (i = 0; i < nCons; i++)
    {
        pthread_create(&(consumers[i]), NULL, deleteBuffer, (void *)input);
        cout << "Created consumer " << i << endl;
    }

    //Sincronizacao dos produtores
    for (i = 0; i < nProds; i++)
    {
        pthread_join(producers[i], NULL);
        cout << "Synchronizing producer " << i << "\n\n";
    }

    //Sincronizacao dos consumidores
    for (i = 0; i < nCons; i++)
    {
        pthread_join(consumers[i], NULL);
        cout << "Synchronizing consumer " << i << "\n\n";
    }

    delete input;

    time = clock() - time;
    cout << "Total execution time: " << (float)time / CLOCKS_PER_SEC << " seconds" << endl;

    return 0;
}

//Verifica se o buffer atingiu seu tamanho maximo
bool isMaxSize(Input input)
{
    if (input.buffer.size() == input.maxBufferSize)
        return true;
    return false;
}

//insere inteiros randomicos no buffer
void *insertBuffer(void *inputFormal)
{
    //cast do ponteiro void passado para ponteiro Input a ser usado
    Input *input = (Input *)inputFormal;

    int itemNum;

    //seed dos numeros randomicos
    srand(time(NULL));

    //insere numero randomico no buffer e decrementa o numero de itens a serem produzidos
    for (;;)
    {
        /*inicio da secao critica*/
        pthread_mutex_lock(&mutex);

        if (input->itemsToInsert == 0)
        {
            pthread_cond_signal(&condCons);
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        itemNum = rand() % 4096;
        input->buffer.push_front(itemNum);
        input->itemsToInsert--;
        cout << "Producer ID: " << pthread_self() << "; Item inserted! Number " << itemNum << endl;
        cout << "Process ID: " << pthread_self() << "; Items left: " << input->itemsToInsert << "\n\n";

        //acorda um thread consumidora na espera da variavel de condicao condCons, se houver
        pthread_cond_signal(&condCons);

        pthread_mutex_unlock(&mutex);
        /*fim da secao critica*/
    }
}

//deleta inteiros do buffer
void *deleteBuffer(void *inputFormal)
{
    //cast do ponteiro void passado a funcao para o tipo Input a ser usado
    Input *input = (Input *)inputFormal;

    int itemNum;

    for (;;)
    {
        /*inicio da secao critica*/
        pthread_mutex_lock(&mutex);

        //bloqueia a thread consumidora corrente enquanto o buffer esta vazio
        while (input->buffer.empty()) /*&& input->buffer.size() < lastIndex*/
        {
            if (input->itemsToRemove == 0)
            {
                //como todos os itens ja foram removidos, aplica broadcast nas variaveis de condiçao
                //para que todas as threads consumidoras restantes sincronizem
                pthread_cond_broadcast(&condCons);
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }

            pthread_cond_wait(&condCons, &mutex);
        }

        //retira os itens do buffer em FIFO
        itemNum = input->buffer.back();
        input->buffer.pop_back();
        input->itemsToRemove--;

        cout << "Consumer ID: " << pthread_self() << "\nItem being removed! Number " << itemNum << "; " << (input->buffer.size()) << " items to be removed!"
             << "\n\n";

        pthread_mutex_unlock(&mutex);
        /*fim da secao critica*/
    }
}
