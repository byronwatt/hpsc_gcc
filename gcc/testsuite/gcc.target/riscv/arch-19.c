/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32i_zvfh -mabi=ilp32 -mcmodel=medlow" } */
int foo()
{
}
/* { dg-error ".'-march=rv32i_zvfh': Extension `zvfh' require either 'v' or 'zve\\*' extension." "" { target *-*-* } 0 } */
