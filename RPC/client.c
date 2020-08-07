/* Arquivo client: um cliente RPC simples */

#include <stdio.h>
#include "fibo.h"

/* função que chama a RPC fibonacci_svc */
int fibo(CLIENT *clnt, int num)
{
   operand op;
   int *result;

   /* junta os parâmetros em um struct */
   op.num = num;

   /* chama a função remota */
   result = fibo_1(&op, clnt);
   if (result == NULL)
   {
      printf("Problems ocurred while calling remote function\n");
      exit(1);
   }

   return (*result);
}

int main(int argc, char *argv[])
{
   CLIENT *clnt;
   int num;

   /* verifica se o cliente foi chamado corretamente */
   if (argc != 3)
   {
      fprintf(stderr, "Usage: %s hostname num\n", argv[0]);
      exit(1);
   }

   printf("\n\n***************RECURSIVE FIBONACCI ON RPC***************\n\n");

   /* cria uma struct CLIENT que referencia uma interface RPC */
   clnt = clnt_create(argv[1], FIBO_PROG, FIBO_VERSION, "udp");

   /* verifica se a referência foi criada */
   if (clnt == (CLIENT *)NULL)
   {
      clnt_pcreateerror(argv[1]);
      exit(1);
   }

   /* obtém os dois inteiros que serão passados via RPC */
   num = atoi(argv[2]);

   if (num > 40)
   {
      printf("Number's too large, this is going to take too long! Set to 40.\n");
      num = 40;
   }
   else if (num < 0)
   {
      printf("This number is negative, it can be calculated!\n");
      return 1;
   }

   /* executa os procedimentos remotos */
   printf("Fibonacci's number of %d is %d\n", num, fibo(clnt, num));

   return (0);
}
