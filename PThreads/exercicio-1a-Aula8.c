#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>

typedef struct { int s, begin, end, *v;
                 int *acum;
                 pthread_mutex_t *m;
} Input;

int acum;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* soma( void* par ) {
 int i, somaL;
 Input *in = (Input*) par;

 somaL = 0;

 for( i = in->begin ; i < in->end ; ++i )
  somaL += in->v[i];

 pthread_mutex_lock(&m);
 in->acum += somaL;
 acum += somaL;
 pthread_mutex_unlock(&m);

 return NULL;
}

int main( int argc, char **argv ) {
 int i, tam, nthreads, *vet;
 pthread_t *tid;
 Input *dta;

 acum = 0;

 if( argc != 3 ) {
  fprintf(stderr,"Uso: acumula <tamanhoVetor> <numeroThreads>\n");
  exit(1);
 }
 
 if( (tam = atoi(argv[1])) < 100 ) {
  fprintf(stderr,"O tamanho do vetor deve ser maior ou igual à 100.\n");
  exit(1);
 }
 if( (nthreads = atoi(argv[2])) < 2 ) {
  fprintf(stderr,"O numero de threads deve ser maior ou igual à 2.\n");
  exit(1);
 }

 tid = (pthread_t*) malloc( nthreads*sizeof(pthread_t) );
 dta = (Input*) malloc( nthreads*sizeof(Input) );
 vet = (int*) malloc( tam*sizeof(int) );

 for( i = 0 ; i < tam ; ++i ) vet[i] = i;

 for( i = 0 ; i < nthreads ; ++i ) {
  dta[i].s = tam;
  dta[i].v = vet;
  dta[i].begin = i*(tam/nthreads);
  dta[i].end   = (i+1)*(tam/nthreads);
  dta[i].acum  = &acum;
  dta[i].m     = &m;
  pthread_create( &(tid[i]), NULL, soma, (void*)&(dta[i]) );
 }

 for( i = 0 ; i < nthreads ; ++i )
  pthread_join( tid[i], (void**)NULL );

 fprintf(stdout,"Res = %d\n", acum);

 free(tid);
 free(dta);
 free(vet);

 return 0;
}  
