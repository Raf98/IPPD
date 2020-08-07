/* fibo.x : definição da interface */
 
#define PROGRAM_NUMBER 002
#define VERSION_NUMBER 1
 
/* tipo de dado que será passado aos procedimentos remotos */
 
struct operand
{
  int num;
};


 
/* Definição da interface que será oferecida aos clientes */
 
program FIBO_PROG
{
  version FIBO_VERSION
  {
    int FIBO (operand) = 1;
  }
  = VERSION_NUMBER;
}
= PROGRAM_NUMBER;