#include <stdio.h>
#include "fibo.h"

int fiboRec(int n)
{
    if (n < 2)
        return n;
    else
        return fiboRec(n - 1) + fiboRec(n - 2);
}

/* implementação da função fibonacci */
int * fibo_1_svc(operand *argp, struct svc_req *rqstp)
{
    static int result;

    printf("Received call: fibonacci %d\n", argp->num);

    result = fiboRec(argp->num);
    return (&result);
}
