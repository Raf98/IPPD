/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _FIBO_H_RPCGEN
#define _FIBO_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct operand {
	int num;
};
typedef struct operand operand;

#define FIBO_PROG 002
#define FIBO_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define FIBO 1
extern  int * fibo_1(operand *, CLIENT *);
extern  int * fibo_1_svc(operand *, struct svc_req *);
extern int fibo_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define FIBO 1
extern  int * fibo_1();
extern  int * fibo_1_svc();
extern int fibo_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_operand (XDR *, operand*);

#else /* K&R C */
extern bool_t xdr_operand ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_FIBO_H_RPCGEN */
