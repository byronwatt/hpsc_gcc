/* Definition of RISC-V target for GNU compiler.
   Copyright (C) 2016-2021 Free Software Foundation, Inc.
   Contributed by Andrew Waterman (andrew@sifive.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_RISCV_OPTS_H
#define GCC_RISCV_OPTS_H

enum riscv_abi_type {
  ABI_ILP32,
  ABI_ILP32E,
  ABI_ILP32F,
  ABI_ILP32D,
  ABI_LP64,
  ABI_LP64F,
  ABI_LP64D
};
extern enum riscv_abi_type riscv_abi;

enum riscv_code_model {
  CM_MEDLOW,
  CM_MEDANY,
  CM_COMPACT,
  CM_PIC
};
extern enum riscv_code_model riscv_cmodel;

enum riscv_isa_spec_class {
  ISA_SPEC_CLASS_NONE,

  ISA_SPEC_CLASS_2P2,
  ISA_SPEC_CLASS_20190608,
  ISA_SPEC_CLASS_20191213
};

extern enum riscv_isa_spec_class riscv_isa_spec;

enum riscv_fp16_eval_format_t {
  FP16_EVAL_NATIVE,
  FP16_EVAL_FLOAT
};

/* Keep this list in sync with define_attr "tune" in riscv.md.  */
enum riscv_microarchitecture_type {
  generic,
  sifive_7,
  sifive_7n,
  sifive_8
};
extern enum riscv_microarchitecture_type riscv_microarchitecture;

enum riscv_align_data {
  riscv_align_data_type_xlen,
  riscv_align_data_type_natural
};

/* Where to get the canary for the stack protector.  */
enum stack_protector_guard {
  SSP_TLS,			/* per-thread canary in TLS block */
  SSP_GLOBAL			/* global canary */
};

#define MASK_ZICSR    (1 << 0)
#define MASK_ZIFENCEI (1 << 1)
#define MASK_ZICCLSM (1 << 2)
#define MASK_ZICCIF (1 << 3)
#define MASK_ZICCAMOA (1 << 4)
#define MASK_ZICCRSE (1 << 5)
#define MASK_ZA64RS (1 << 6)
#define MASK_ZIC64B (1 << 7)
#define MASK_ZIHINTNTL (1 << 8)

#define TARGET_ZICSR    ((riscv_zi_subext & MASK_ZICSR) != 0)
#define TARGET_ZIFENCEI ((riscv_zi_subext & MASK_ZIFENCEI) != 0)
#define TARGET_ZICCLSM ((riscv_zi_subext & MASK_ZICCLSM) != 0)
#define TARGET_ZICCIF ((riscv_zi_subext & MASK_ZICCIF) != 0)
#define TARGET_ZICCAMOA ((riscv_zi_subext & MASK_ZICCAMOA) != 0)
#define TARGET_ZICCRSE ((riscv_zi_subext & MASK_ZICCRSE) != 0)
#define TARGET_ZA64RS ((riscv_zi_subext & MASK_ZA64RS) != 0)
#define TARGET_ZIC64B ((riscv_zi_subext & MASK_ZIC64B) != 0)
#define TARGET_ZIHINTNTL ((riscv_zi_subext & MASK_ZIHINTNTL) != 0)

#define MASK_ZBA (1 << 0)
#define MASK_ZBB (1 << 1)
#define MASK_ZBS (1 << 2)
#define MASK_ZBP (1 << 3)
#define MASK_ZBE (1 << 4)
#define MASK_ZBF (1 << 5)
#define MASK_ZBC (1 << 6)
#define MASK_ZBR (1 << 7)
#define MASK_ZBM (1 << 8)
#define MASK_ZBT (1 << 9)

#define TARGET_ZBA ((riscv_bitmanip_subext & MASK_ZBA) != 0)
#define TARGET_ZBB ((riscv_bitmanip_subext & MASK_ZBB) != 0)
#define TARGET_ZBS ((riscv_bitmanip_subext & MASK_ZBS) != 0)
#define TARGET_ZBP ((riscv_bitmanip_subext & MASK_ZBP) != 0)
#define TARGET_ZBE ((riscv_bitmanip_subext & MASK_ZBE) != 0)
#define TARGET_ZBF ((riscv_bitmanip_subext & MASK_ZBF) != 0)
#define TARGET_ZBC ((riscv_bitmanip_subext & MASK_ZBC) != 0)
#define TARGET_ZBR ((riscv_bitmanip_subext & MASK_ZBR) != 0)
#define TARGET_ZBM ((riscv_bitmanip_subext & MASK_ZBM) != 0)
#define TARGET_ZBT ((riscv_bitmanip_subext & MASK_ZBT) != 0)

#define MASK_ZVKNS  (1 << 0)
#define MASK_ZVKNF  (1 << 1)
#define MASK_ZVKNHA (1 << 2)
#define MASK_ZVKNHB (1 << 3)
#define MASK_ZVKB   (1 << 4)
#define MASK_ZVKG   (1 << 5)
#define MASK_ZVKSED (1 << 6)
#define MASK_ZVKSH  (1 << 7)

#define TARGET_ZVKNS  ((riscv_zv_subext & MASK_ZVKNS) != 0)
#define TARGET_ZVKNF  ((riscv_zv_subext & MASK_ZVKNF) != 0)
#define TARGET_ZVKNHA ((riscv_zv_subext & MASK_ZVKNHA) != 0)
#define TARGET_ZVKNHB ((riscv_zv_subext & MASK_ZVKNHB) != 0)
#define TARGET_ZVKB   ((riscv_zv_subext & MASK_ZVKB) != 0)
#define TARGET_ZVKG   ((riscv_zv_subext & MASK_ZVKG) != 0)
#define TARGET_ZVKSED ((riscv_zv_subext & MASK_ZVKSED) != 0)
#define TARGET_ZVKSH  ((riscv_zv_subext & MASK_ZVKSH) != 0)

#define MASK_VECTOR_ELEN_32    (1 << 0)
#define MASK_VECTOR_ELEN_64    (1 << 1)
#define MASK_VECTOR_ELEN_FP_32 (1 << 2)
#define MASK_VECTOR_ELEN_FP_64 (1 << 3)

#define TARGET_VECTOR_ELEN_32 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_32) != 0)
#define TARGET_VECTOR_ELEN_64 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_64) != 0)
#define TARGET_VECTOR_ELEN_FP_32 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_FP_32) != 0)
#define TARGET_VECTOR_ELEN_FP_64 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_FP_64) != 0)

#define MASK_ZVL32B    (1 <<  0)
#define MASK_ZVL64B    (1 <<  1)
#define MASK_ZVL128B   (1 <<  2)
#define MASK_ZVL256B   (1 <<  3)
#define MASK_ZVL512B   (1 <<  4)
#define MASK_ZVL1024B  (1 <<  5)
#define MASK_ZVL2048B  (1 <<  6)
#define MASK_ZVL4096B  (1 <<  7)
#define MASK_ZVL8192B  (1 <<  8)
#define MASK_ZVL16384B (1 <<  9)
#define MASK_ZVL32768B (1 << 10)
#define MASK_ZVL65536B (1 << 11)

#define TARGET_ZVL32B    ((riscv_zvl_flags & MASK_ZVL32B) != 0)
#define TARGET_ZVL64B    ((riscv_zvl_flags & MASK_ZVL64B) != 0)
#define TARGET_ZVL128B   ((riscv_zvl_flags & MASK_ZVL128B) != 0)
#define TARGET_ZVL256B   ((riscv_zvl_flags & MASK_ZVL256B) != 0)
#define TARGET_ZVL512B   ((riscv_zvl_flags & MASK_ZVL512B) != 0)
#define TARGET_ZVL1024B  ((riscv_zvl_flags & MASK_ZVL1024B) != 0)
#define TARGET_ZVL2048B  ((riscv_zvl_flags & MASK_ZVL2048B) != 0)
#define TARGET_ZVL4096B  ((riscv_zvl_flags & MASK_ZVL4096B) != 0)
#define TARGET_ZVL8192B  ((riscv_zvl_flags & MASK_ZVL8192B) != 0)
#define TARGET_ZVL16384B ((riscv_zvl_flags & MASK_ZVL16384B) != 0)
#define TARGET_ZVL32768B ((riscv_zvl_flags & MASK_ZVL32768B) != 0)
#define TARGET_ZVL65536B ((riscv_zvl_flags & MASK_ZVL65536B) != 0)

#define MASK_ZICBOZ   (1 << 0)
#define MASK_ZICBOM   (1 << 1)
#define MASK_ZICBOP   (1 << 2)

#define TARGET_ZICBOZ ((riscv_zicmo_subext & MASK_ZICBOZ) != 0)
#define TARGET_ZICBOM ((riscv_zicmo_subext & MASK_ZICBOM) != 0)
#define TARGET_ZICBOP ((riscv_zicmo_subext & MASK_ZICBOP) != 0)

#define MASK_ZFHMIN   (1 << 0)
#define MASK_ZFH      (1 << 1)

#define TARGET_ZFHMIN ((riscv_zf_subext & MASK_ZFHMIN) != 0)
#define TARGET_ZFH    ((riscv_zf_subext & MASK_ZFH) != 0)

#define MASK_SVINVAL (1 << 0)
#define MASK_SVNAPOT (1 << 1)
#define MASK_SSCOFPMF (1 << 2)

#define TARGET_SVINVAL ((riscv_sv_subext & MASK_SVINVAL) != 0)
#define TARGET_SVNAPOT ((riscv_sv_subext & MASK_SVNAPOT) != 0)
#define TARGET_SSCOFPMF ((riscv_sv_subext & MASK_SSCOFPMF) != 0)

/* Bit of riscv_zvl_flags will set contintuly, N-1 bit will set if N-bit is
   set, e.g. MASK_ZVL64B has set then MASK_ZVL32B is set, so we can use
   popcount to caclulate the minimal VLEN.  */
#define TARGET_MIN_VLEN \
  ((riscv_zvl_flags == 0) \
   ? 0 \
   : 32 << (__builtin_popcount (riscv_zvl_flags) - 1))

/* RVV vector register sizes.  */
enum riscv_rvv_vector_bits_enum {
  RVV_SCALABLE,
  RVV_NOT_IMPLEMENTED = RVV_SCALABLE,
  RVV_64 = 64,
  RVV_128 = 128,
  RVV_256 = 256,
  RVV_512 = 512,
  RVV_1024 = 1024
};

#define MASK_ZKT (1 << 0)
#define TARGET_ZKT ((riscv_zk_subext & MASK_ZKT) != 0)

#endif /* ! GCC_RISCV_OPTS_H */
