/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc_zkt -mabi=lp64d -mcmodel=medlow -misa-spec=2.2" } */

int main () {

#ifndef __riscv_arch_test
#error "__riscv_arch_test"
#endif

#if __riscv_xlen != 64
#error "__riscv_xlen"
#endif

#if !defined(__riscv_i)
#error "__riscv_i"
#endif

#if !defined(__riscv_c)
#error "__riscv_c"
#endif

#if defined(__riscv_e)
#error "__riscv_e"
#endif

#if !defined(__riscv_a)
#error "__riscv_a"
#endif

#if !defined(__riscv_m)
#error "__riscv_m"
#endif

#if !defined(__riscv_f)
#error "__riscv_f"
#endif

#if !defined(__riscv_d)
#error "__riscv_d"
#endif

#if !defined(__riscv_zkt)
#error "__riscv_zkt"
#endif

  return 0;
}
