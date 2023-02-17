/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gcv_zvknf_zvl128b -mabi=lp64d -mcmodel=medlow -misa-spec=2.2" } */

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

#if !defined(__riscv_v)
#error "__riscv_v"
#endif

#if !defined(__riscv_zvl128b)
#error "__riscv_zvl128b"
#endif

#if !defined(__riscv_zvl64b)
#error "__riscv_zvl64b"
#endif

#if !defined(__riscv_zvl32b)
#error "__riscv_zvl32b"
#endif

#if !defined(__riscv_zvknf)
#error "__riscv_zvknf"
#endif

#if !defined(__riscv_vector)
#error "__riscv_vector"
#endif

  return 0;
}
