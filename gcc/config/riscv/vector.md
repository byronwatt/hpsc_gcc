;; Machine description for RISC-V 'V' Extension for GNU compiler.
;; Copyright (C) 2022-2023 Free Software Foundation, Inc.
;; Contributed by Juzhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; This file describes the RISC-V 'V' Extension, Version 1.0.
;;
;; This file include :
;;
;; - Intrinsics (https://github.com/riscv/rvv-intrinsic-doc)
;; - Auto-vectorization (TBD)
;; - Combine optimization (TBD)

(include "vector-iterators.md")

(define_constants [
   (INVALID_ATTRIBUTE            255)
   (X0_REGNUM                      0)
])

;; True if the type is RVV instructions that include VTYPE
;; global status register in the use op list.
;; We known VTYPE has 4 fields: SEW, LMUL, TA, MA.
;; The instruction need any of VTYPE field is set as true
;; in this attribute.
(define_attr "has_vtype_op" "false,true"
  (cond [(eq_attr "type" "vlde,vste,vldm,vstm,vlds,vsts,\
			  vldux,vldox,vstux,vstox,vldff,\
			  vialu,viwalu,vext,vicalu,vshift,vnshift,vicmp,viminmax,\
			  vimul,vidiv,viwmul,vimuladd,viwmuladd,vimerge,vimov,\
			  vsalu,vaalu,vsmul,vsshift,vnclip,\
			  vfalu,vfwalu,vfmul,vfdiv,vfwmul,vfmuladd,vfwmuladd,vfsqrt,vfrecp,\
			  vfcmp,vfminmax,vfsgnj,vfclass,vfmerge,vfmov,\
			  vfcvtitof,vfcvtftoi,vfwcvtitof,vfwcvtftoi,\
			  vfwcvtftof,vfncvtitof,vfncvtftoi,vfncvtftof,\
			  vired,viwred,vfred,vfredo,vfwred,vfwredo,\
			  vmalu,vmpop,vmffs,vmsfs,vmiota,vmidx,vimovvx,vimovxv,vfmovvf,vfmovfv,\
			  vislide,vislide1,vfslide1,vgather,vcompress")
	 (const_string "true")]
	(const_string "false")))

;; True if the type is RVV instructions that include VL
;; global status register in the use op list.
;; The instruction need vector length to be specified is set
;; in this attribute.
(define_attr "has_vl_op" "false,true"
  (cond [(eq_attr "type" "vlde,vste,vldm,vstm,vlds,vsts,\
			  vldux,vldox,vstux,vstox,vldff,\
			  vialu,viwalu,vext,vicalu,vshift,vnshift,vicmp,viminmax,\
			  vimul,vidiv,viwmul,vimuladd,viwmuladd,vimerge,vimov,\
			  vsalu,vaalu,vsmul,vsshift,vnclip,\
			  vfalu,vfwalu,vfmul,vfdiv,vfwmul,vfmuladd,vfwmuladd,vfsqrt,vfrecp,\
			  vfcmp,vfminmax,vfsgnj,vfclass,vfmerge,vfmov,\
			  vfcvtitof,vfcvtftoi,vfwcvtitof,vfwcvtftoi,\
			  vfwcvtftof,vfncvtitof,vfncvtftoi,vfncvtftof,\
			  vired,viwred,vfred,vfredo,vfwred,vfwredo,\
			  vmalu,vmpop,vmffs,vmsfs,vmiota,vmidx,vimovxv,vfmovfv,\
			  vislide,vislide1,vfslide1,vgather,vcompress")
	 (const_string "true")]
	(const_string "false")))

;; The default SEW of RVV instruction. This attribute doesn't mean the instruction
;; is necessary to require SEW check for example vlm.v which require ratio to
;; check. However, we need default value of SEW for vsetvl instruction since there
;; is no field for ratio in the vsetvl instruction encoding.
(define_attr "sew" ""
  (cond [(eq_attr "mode" "VNx1QI,VNx2QI,VNx4QI,VNx8QI,VNx16QI,VNx32QI,VNx64QI,\
			  VNx1BI,VNx2BI,VNx4BI,VNx8BI,VNx16BI,VNx32BI,VNx64BI")
	 (const_int 8)
	 (eq_attr "mode" "VNx1HI,VNx2HI,VNx4HI,VNx8HI,VNx16HI,VNx32HI")
	 (const_int 16)
	 (eq_attr "mode" "VNx1SI,VNx2SI,VNx4SI,VNx8SI,VNx16SI,\
			  VNx1SF,VNx2SF,VNx4SF,VNx8SF,VNx16SF")
	 (const_int 32)
	 (eq_attr "mode" "VNx1DI,VNx2DI,VNx4DI,VNx8DI,\
			  VNx1DF,VNx2DF,VNx4DF,VNx8DF")
	 (const_int 64)]
	(const_int INVALID_ATTRIBUTE)))

;; Ditto to LMUL.
(define_attr "vlmul" ""
  (cond [(eq_attr "mode" "VNx1QI,VNx1BI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx1QImode)")
	 (eq_attr "mode" "VNx2QI,VNx2BI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx2QImode)")
	 (eq_attr "mode" "VNx4QI,VNx4BI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx4QImode)")
	 (eq_attr "mode" "VNx8QI,VNx8BI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx8QImode)")
	 (eq_attr "mode" "VNx16QI,VNx16BI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx16QImode)")
	 (eq_attr "mode" "VNx32QI,VNx32BI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx32QImode)")
	 (eq_attr "mode" "VNx64QI,VNx64BI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx64QImode)")
	 (eq_attr "mode" "VNx1HI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx1HImode)")
	 (eq_attr "mode" "VNx2HI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx2HImode)")
	 (eq_attr "mode" "VNx4HI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx4HImode)")
	 (eq_attr "mode" "VNx8HI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx8HImode)")
	 (eq_attr "mode" "VNx16HI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx16HImode)")
	 (eq_attr "mode" "VNx32HI")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx32HImode)")
	 (eq_attr "mode" "VNx1SI,VNx1SF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx1SImode)")
	 (eq_attr "mode" "VNx2SI,VNx2SF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx2SImode)")
	 (eq_attr "mode" "VNx4SI,VNx4SF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx4SImode)")
	 (eq_attr "mode" "VNx8SI,VNx8SF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx8SImode)")
	 (eq_attr "mode" "VNx16SI,VNx16SF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx16SImode)")
	 (eq_attr "mode" "VNx1DI,VNx1DF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx1DImode)")
	 (eq_attr "mode" "VNx2DI,VNx2DF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx2DImode)")
	 (eq_attr "mode" "VNx4DI,VNx4DF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx4DImode)")
	 (eq_attr "mode" "VNx8DI,VNx8DF")
	   (symbol_ref "riscv_vector::get_vlmul(E_VNx8DImode)")]
	(const_int INVALID_ATTRIBUTE)))

;; It is valid for instruction that require sew/lmul ratio.
(define_attr "ratio" ""
  (cond [(eq_attr "type" "vimov,vfmov,vldux,vldox,vstux,vstox,\
			  vialu,vshift,vicmp,vimul,vidiv,vsalu,\
			  vext,viwalu,viwmul,vicalu,vnshift,\
			  vimuladd,vimerge,vaalu,vsmul,vsshift,\
			  vnclip,viminmax,viwmuladd")
	   (const_int INVALID_ATTRIBUTE)
	 (eq_attr "mode" "VNx1QI,VNx1BI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx1QImode)")
	 (eq_attr "mode" "VNx2QI,VNx2BI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx2QImode)")
	 (eq_attr "mode" "VNx4QI,VNx4BI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx4QImode)")
	 (eq_attr "mode" "VNx8QI,VNx8BI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx8QImode)")
	 (eq_attr "mode" "VNx16QI,VNx16BI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx16QImode)")
	 (eq_attr "mode" "VNx32QI,VNx32BI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx32QImode)")
	 (eq_attr "mode" "VNx64QI,VNx64BI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx64QImode)")
	 (eq_attr "mode" "VNx1HI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx1HImode)")
	 (eq_attr "mode" "VNx2HI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx2HImode)")
	 (eq_attr "mode" "VNx4HI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx4HImode)")
	 (eq_attr "mode" "VNx8HI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx8HImode)")
	 (eq_attr "mode" "VNx16HI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx16HImode)")
	 (eq_attr "mode" "VNx32HI")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx32HImode)")
	 (eq_attr "mode" "VNx1SI,VNx1SF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx1SImode)")
	 (eq_attr "mode" "VNx2SI,VNx2SF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx2SImode)")
	 (eq_attr "mode" "VNx4SI,VNx4SF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx4SImode)")
	 (eq_attr "mode" "VNx8SI,VNx8SF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx8SImode)")
	 (eq_attr "mode" "VNx16SI,VNx16SF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx16SImode)")
	 (eq_attr "mode" "VNx1DI,VNx1DF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx1DImode)")
	 (eq_attr "mode" "VNx2DI,VNx2DF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx2DImode)")
	 (eq_attr "mode" "VNx4DI,VNx4DF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx4DImode)")
	 (eq_attr "mode" "VNx8DI,VNx8DF")
	   (symbol_ref "riscv_vector::get_ratio(E_VNx8DImode)")]
	(const_int INVALID_ATTRIBUTE)))

;; The index of operand[] to get the merge op.
(define_attr "merge_op_idx" ""
	(cond [(eq_attr "type" "vlde,vimov,vfmov,vldm,vlds,vmalu,vldux,vldox,vicmp,\
				vialu,vshift,viminmax,vimul,vidiv,vsalu,vext,viwalu,\
				viwmul,vnshift,vaalu,vsmul,vsshift,vnclip")
	       (const_int 2)

	       (eq_attr "type" "vimerge")
	       (const_int 1)

	       (eq_attr "type" "vimuladd,viwmuladd")
	       (const_int 5)]
	(const_int INVALID_ATTRIBUTE)))

;; The index of operand[] to get the avl op.
(define_attr "vl_op_idx" ""
  (cond [(eq_attr "type" "vlde,vste,vimov,vfmov,vldm,vstm,vmalu,vsts,vstux,\
			  vstox,vext")
	   (const_int 4)

	 ;; If operands[3] of "vlds" is not vector mode, it is pred_broadcast.
	 ;; wheras it is pred_strided_load if operands[3] is vector mode.
         (eq_attr "type" "vlds")
	   (if_then_else (match_test "VECTOR_MODE_P (GET_MODE (operands[3]))")
             (const_int 5)
             (const_int 4))

	 (eq_attr "type" "vldux,vldox,vialu,vshift,viminmax,vimul,vidiv,vsalu,\
			  viwalu,viwmul,vnshift,vimerge,vaalu,vsmul,\
			  vsshift,vnclip")
	   (const_int 5)

	 (eq_attr "type" "vicmp")
	   (const_int 6)

	 (eq_attr "type" "vimuladd,viwmuladd")
	   (const_int 6)]
  (const_int INVALID_ATTRIBUTE)))

;; The tail policy op value.
(define_attr "ta" ""
  (cond [(eq_attr "type" "vlde,vimov,vfmov,vext")
	   (symbol_ref "riscv_vector::get_ta(operands[5])")

	 ;; If operands[3] of "vlds" is not vector mode, it is pred_broadcast.
	 ;; wheras it is pred_strided_load if operands[3] is vector mode.
	 (eq_attr "type" "vlds")
	   (if_then_else (match_test "VECTOR_MODE_P (GET_MODE (operands[3]))")
	     (symbol_ref "riscv_vector::get_ta(operands[6])")
	     (symbol_ref "riscv_vector::get_ta(operands[5])"))

	 (eq_attr "type" "vldux,vldox,vialu,vshift,viminmax,vimul,vidiv,vsalu,\
			  viwalu,viwmul,vnshift,vimerge,vaalu,vsmul,\
			  vsshift,vnclip")
	   (symbol_ref "riscv_vector::get_ta(operands[6])")

	 (eq_attr "type" "vimuladd,viwmuladd")
	   (symbol_ref "riscv_vector::get_ta(operands[7])")]
	(const_int INVALID_ATTRIBUTE)))

;; The mask policy op value.
(define_attr "ma" ""
  (cond [(eq_attr "type" "vlde,vext")
	   (symbol_ref "riscv_vector::get_ma(operands[6])")

	 ;; If operands[3] of "vlds" is not vector mode, it is pred_broadcast.
	 ;; wheras it is pred_strided_load if operands[3] is vector mode.
	 (eq_attr "type" "vlds")
	   (if_then_else (match_test "VECTOR_MODE_P (GET_MODE (operands[3]))")
	     (symbol_ref "riscv_vector::get_ma(operands[7])")
	     (symbol_ref "riscv_vector::get_ma(operands[6])"))

	 (eq_attr "type" "vldux,vldox,vialu,vshift,viminmax,vimul,vidiv,vsalu,\
			  viwalu,viwmul,vnshift,vaalu,vsmul,vsshift,\
			  vnclip,vicmp")
	   (symbol_ref "riscv_vector::get_ma(operands[7])")

	 (eq_attr "type" "vimuladd,viwmuladd")
	   (symbol_ref "riscv_vector::get_ma(operands[8])")]
	(const_int INVALID_ATTRIBUTE)))

;; The avl type value.
(define_attr "avl_type" ""
  (cond [(eq_attr "type" "vlde,vlde,vste,vimov,vimov,vimov,vfmov,vext,vimerge")
	   (symbol_ref "INTVAL (operands[7])")
	 (eq_attr "type" "vldm,vstm,vimov,vmalu,vmalu")
	   (symbol_ref "INTVAL (operands[5])")

	 ;; If operands[3] of "vlds" is not vector mode, it is pred_broadcast.
	 ;; wheras it is pred_strided_load if operands[3] is vector mode.
	 (eq_attr "type" "vlds")
	   (if_then_else (match_test "VECTOR_MODE_P (GET_MODE (operands[3]))")
	     (const_int INVALID_ATTRIBUTE)
	     (symbol_ref "INTVAL (operands[7])"))

	 (eq_attr "type" "vldux,vldox,vialu,vshift,viminmax,vimul,vidiv,vsalu,\
			  viwalu,viwmul,vnshift,vimuladd,vaalu,vsmul,vsshift,\
			  vnclip,vicmp")
	   (symbol_ref "INTVAL (operands[8])")
	 (eq_attr "type" "vstux,vstox")
	   (symbol_ref "INTVAL (operands[5])")

	 (eq_attr "type" "vimuladd,viwmuladd")
	   (symbol_ref "INTVAL (operands[9])")]
	(const_int INVALID_ATTRIBUTE)))

;; -----------------------------------------------------------------
;; ---- Miscellaneous Operations
;; -----------------------------------------------------------------

(define_insn "vundefined<mode>"
  [(set (match_operand:V 0 "register_operand" "=vr")
	(unspec:V [(const_int 0)] UNSPEC_VUNDEF))]
  "TARGET_VECTOR"
  "")

;; This pattern is used to hold the AVL operand for
;; RVV instructions that implicity use VLMAX AVL.
;; RVV instruction implicitly use GPR that is ultimately
;; defined by this pattern is safe for VSETVL pass emit
;; a vsetvl instruction modify this register after RA.
;; Case 1:
;;   vlmax_avl a5
;;   ... (across many blocks)
;;   vadd (implicit use a5)  ====> emit: vsetvl a5,zero
;; Case 2:
;;   vlmax_avl a5
;;   ... (across many blocks)
;;   mv a6,a5
;;   ... (across many blocks)
;;   vadd (implicit use a6)  ====> emit: vsetvl a6,zero
;; Case 3:
;;   vlmax_avl a5
;;   ... (across many blocks)
;;   store mem,a5 (spill)
;;   ... (across many blocks)
;;   load a7,mem (spill)
;;   ... (across many blocks)
;;   vadd (implicit use a7)  ====> emit: vsetvl a7,zero
;; Such cases are all safe for VSETVL PASS to emit a vsetvl
;; instruction that modifies the AVL operand.
(define_insn "@vlmax_avl<mode>"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P [(match_operand:P 1 "const_int_operand" "i")] UNSPEC_VLMAX))]
  "TARGET_VECTOR"
  "")

;; -----------------------------------------------------------------
;; ---- Moves Operations
;; -----------------------------------------------------------------

(define_expand "mov<mode>"
  [(set (match_operand:V 0 "reg_or_mem_operand")
	(match_operand:V 1 "general_operand"))]
  "TARGET_VECTOR"
{
  /* For whole register move, we transform the pattern into the format
     that excludes the clobber of scratch register.

     We include clobber of a scalar scratch register which is going to be
     used for emit of vsetvl instruction after reload_completed since we
     need vsetvl instruction to set VL/VTYPE global status for fractional
     vector load/store.

     For example:
       [(set (match_operand:VNx1QI v24)
	     (match_operand:VNx1QI (mem: a4)))
	     (clobber (scratch:SI a5))]
     ====>> vsetvl a5,zero,e8,mf8
     ====>> vle8.v v24,(a4)

     Philosophy:

       - Clobber a scalar scratch register for each mov<mode>.

       - Classify the machine_mode mode = <MODE>mode into 2 class:
	 Whole register move and fractional register move.

       - Transform and remove scratch clobber register for whole
	 register move so that we can avoid occupying the scalar
	 registers.

       - We can not leave it to TARGET_SECONDARY_RELOAD since it happens
	 before spilling. The clobber scratch is used by spilling fractional
	 registers in IRA/LRA so it's too early.  */

  if (riscv_vector::legitimize_move (operands[0], operands[1], <VM>mode))
    DONE;
})

;; This pattern is used for code-gen for whole register load/stores.
;; Also applicable for all register moves.
;; Fractional vector modes load/store are not allowed to match this pattern.
;; Mask modes load/store are not allowed to match this pattern.
;; We seperate "*mov<mode>" into "*mov<mode>_whole" and "*mov<mode>_fract" because
;; we don't want to include fractional load/store in "*mov<mode>" which will
;; create unexpected patterns in LRA.
;; For example:
;; ira rtl:
;;   (insn 20 19 9 2 (set (reg/v:VNx2QI 97 v1 [ v1 ])
;;      (reg:VNx2QI 134 [ _1 ])) "rvv.c":9:22 571 {*movvnx2qi_fract}
;;   (nil))
;; When the value of pseudo register 134 of the insn above is discovered already
;; spilled in the memory during LRA.
;; LRA will reload this pattern into a memory load instruction pattern.
;; Because VNx2QI is a fractional vector, we want LRA reload this pattern into
;;  (insn 20 19 9 2 (parallel [
;;       (set (reg:VNx2QI 98 v2 [orig:134 _1 ] [134])
;;           (mem/c:VNx2QI (reg:SI 13 a3 [155]) [1 %sfp+[-2, -2] S[2, 2] A8]))
;;       (clobber (reg:SI 14 a4 [149]))])
;; So that we could be able to emit vsetvl instruction using clobber sratch a4.
;; To let LRA generate the expected pattern, we should exclude fractional vector
;; load/store in "*mov<mode>_whole". Otherwise, it will reload this pattern into:
;;  (insn 20 19 9 2 (set (reg:VNx2QI 98 v2 [orig:134 _1 ] [134])
;;           (mem/c:VNx2QI (reg:SI 13 a3 [155]) [1 %sfp+[-2, -2] S[2, 2] A8])))
;; which is not the pattern we want.
;; According the facts above, we make "*mov<mode>_whole" includes load/store/move for whole
;; vector modes according to '-march' and "*mov<mode>_fract" only include fractional vector modes.
(define_insn "*mov<mode>_whole"
  [(set (match_operand:V_WHOLE 0 "reg_or_mem_operand" "=vr, m,vr")
	(match_operand:V_WHOLE 1 "reg_or_mem_operand" "  m,vr,vr"))]
  "TARGET_VECTOR"
  "@
   vl%m1re<sew>.v\t%0,%1
   vs%m1r.v\t%1,%0
   vmv%m1r.v\t%0,%1"
  [(set_attr "type" "vldr,vstr,vmov")
   (set_attr "mode" "<MODE>")])

(define_insn "*mov<mode>_fract"
  [(set (match_operand:V_FRACT 0 "register_operand" "=vr")
	(match_operand:V_FRACT 1 "register_operand" " vr"))]
  "TARGET_VECTOR"
  "vmv1r.v\t%0,%1"
  [(set_attr "type" "vmov")
   (set_attr "mode" "<MODE>")])

(define_expand "mov<mode>"
  [(set (match_operand:VB 0 "reg_or_mem_operand")
	(match_operand:VB 1 "general_operand"))]
  "TARGET_VECTOR"
{
  if (riscv_vector::legitimize_move (operands[0], operands[1], <MODE>mode))
    DONE;
})

(define_insn "*mov<mode>"
  [(set (match_operand:VB 0 "register_operand" "=vr")
	(match_operand:VB 1 "register_operand" " vr"))]
  "TARGET_VECTOR"
  "vmv1r.v\t%0,%1"
  [(set_attr "type" "vmov")
   (set_attr "mode" "<MODE>")])

;; -----------------------------------------------------------------
;; ---- Duplicate Operations
;; -----------------------------------------------------------------

;; According to GCC internal:
;; This pattern only handles duplicates of non-constant inputs.
;; Constant vectors go through the movm pattern instead.
;; So "direct_broadcast_operand" can only be mem or reg, no CONSTANT.
(define_expand "vec_duplicate<mode>"
  [(set (match_operand:V 0 "register_operand")
	(vec_duplicate:V
	  (match_operand:<VEL> 1 "direct_broadcast_operand")))]
  "TARGET_VECTOR"
  {
    riscv_vector::emit_vlmax_op (code_for_pred_broadcast (<MODE>mode),
		operands[0], operands[1], <VM>mode);
    DONE;
  }
)

;; -----------------------------------------------------------------
;; ---- 6. Configuration-Setting Instructions
;; -----------------------------------------------------------------
;; Includes:
;; - 6.1 vsetvli/vsetivl/vsetvl instructions
;; -----------------------------------------------------------------

;; we dont't define vsetvli as unspec_volatile which has side effects.
;; This instruction can be scheduled by the instruction scheduler.
;; This means these instructions will be deleted when
;; there is no instructions using vl or vtype in the following.
;; rd  | rs1 | AVL value | Effect on vl
;; -   | !x0 | x[rs1]    | Normal stripmining
;; !x0 | x0  | ~0        | Set vl to VLMAX
;; operands[0]: VL.
;; operands[1]: AVL.
;; operands[2]: SEW
;; operands[3]: LMUL
;; operands[4]: Tail policy 0 or 1 (undisturbed/agnostic)
;; operands[5]: Mask policy 0 or 1 (undisturbed/agnostic)

;; We define 2 types of "vsetvl*" instruction patterns:

;; -  "@vsetvl<mode>" is a parallel format which has side effects.

;; -  "@vsetvl<mode>_no_side_effects" has no side effects.

;; -  "@vsetvl<mode>" is used by "vsetvl" intrinsics and "insert-vsetvl" PASS.

;; -  "@vsetvl<mode>_no_side_effects" is used by GCC standard patterns.

;; -  "@vsetvl<mode>" includes VL/VTYPE global registers status (define set)
;; and each RVV instruction includes VL/VTYPE global registers status (use)
;; so that we can guarantee each RVV instruction can execute with correct
;; VL/VTYPE global registers status after "insert-vsetvl" PASS.

;; -  "@vsetvl<mode>_no_side_effects" has no side effects and excludes VL/VTYPE
;; global registers status (define set). It's only used by GCC standard pattern
;; expansion. For example: "mov<mode>" pattern for fractional vector modes which
;; need to set VL/VTYPE. Then we could manually call this pattern to gain benefits
;; from the optimization of each GCC internal PASS.

;; 1. void foo (float *in, float *out)
;;    {
;;      vfloat32mf2_t v = *(vfloat32mf2_t*)in;
;;      *(vfloat32mf2_t*)out = v;
;;    }
;; We could eliminate the second "vsetvl" by calling "@vsetvl<mode>_no_side_effects".
;;
;; "@vsetvl<mode>":               ;; "@vsetvl<mode>_no_side_effects":
;; vsetvli a4,zero,e32,mf2,ta,ma  ;; vsetvli a4,zero,e32,mf2,ta,ma
;; vle32.v v24,(a0)               ;; vle32.v v24,(a0)
;; vsetvli a4,zero,e32,mf2,ta,ma  ;; --
;; vse32.v v24,(a1)               ;; vse32.v v24,(a1)
;; ret                            ;; ret

;; 2. void foo (int8_t *in, int8_t *out, int M)
;;    {
;;      for (int i = 0; i < M; i++){
;;        vint8mf2_t v = *(vint8mf2_t*)(in + i);
;;        *(vint8mf2_t*)(out + i) = v;
;;      }
;;    }
;;
;; Hoist "vsetvl" instruction in LICM:
;; "@vsetvl<mode>":                  ;; "@vsetvl<mode>_no_side_effects":
;; -                                 ;;   vsetvli a4,zero,e32,mf2,ta,ma
;; LOOP:                             ;; LOOP:
;;   vsetvli a4,zero,e32,mf2,ta,ma   ;; -
;;   vle32.v v24,(a0)                ;;   vle32.v v24,(a0)
;;   vsetvli a4,zero,e32,mf2,ta,ma   ;; -
;;   vse32.v v24,(a1)                ;;   vse32.v v24,(a1)

;; However, it may produce wrong codegen if we exclude VL/VTYPE in "vsevl<mode>".
;; 3. void foo (int8_t *in, int8_t *out, int32_t *in2, int32_t *out2, int M)
;;    {
;;      for (int i = 0; i < M; i++){
;;        vint8mf2_t v = *(vint8mf2_t*)(in + i);
;;        vint32mf2_t v2 = *(vint32mf2_t*)(in + i + i);
;;        *(vint8mf2_t*)(out + i) = v;
;;        *(vint32mf2_t*)(out + i + i) = v2;
;;      }
;;    }
;;
;; vsetvli a6,zero,e8,mf2,ta,ma
;; vsetvli a2,zero,e32,mf2,ta,ma
;; LOOP:
;;   vle8.v  v25,(a0)
;;   vle32.v v24,(a5)
;;   addi    a0,a0,1
;;   vse8.v  v25,(a1)
;;   vse32.v v24,(a3)
;;
;; Both vle8.v and vle32.v are using the wrong VL/VTYPE status.
;; We leave it to "insert-vsetvl" PASS to correct this situation.

;; The "insert-vsetvl" PASS mechanism:
;; 1. Before "insert-vsetvl" PASS, only RVV instructions are generated
;;    by GCC standard pattern expansion has the corresponding "vsetvl".
;;    We exploit each GCC internal optimization pass to optimize the "vsetvl".
;; 2. Correct the VL/VTYPE status for each GCC standard pattern RVV instructions.
;;    Insert vsetvl for each RVV instructions that has no VL/VTYPE status if necessary.
;;    For example: RVV intrinsics.
;; 3. Optimize "vsetvl" instructions.

(define_insn "@vsetvl<mode>"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P [(match_operand:P 1 "csr_operand" "rK")
		   (match_operand 2 "const_int_operand" "i")
		   (match_operand 3 "const_int_operand" "i")
		   (match_operand 4 "const_int_operand" "i")
		   (match_operand 5 "const_int_operand" "i")] UNSPEC_VSETVL))
   (set (reg:SI VL_REGNUM)
	(unspec:SI [(match_dup 1)
		    (match_dup 2)
		    (match_dup 3)] UNSPEC_VSETVL))
   (set (reg:SI VTYPE_REGNUM)
	(unspec:SI [(match_dup 2)
		    (match_dup 3)
		    (match_dup 4)
		    (match_dup 5)] UNSPEC_VSETVL))]
  "TARGET_VECTOR"
  "vset%i1vli\t%0,%1,e%2,%m3,t%p4,m%p5"
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "<MODE>")
   (set (attr "sew") (symbol_ref "INTVAL (operands[2])"))
   (set (attr "vlmul") (symbol_ref "INTVAL (operands[3])"))
   (set (attr "ta") (symbol_ref "INTVAL (operands[4])"))
   (set (attr "ma") (symbol_ref "INTVAL (operands[5])"))])

;; vsetvl zero,zero,vtype instruction.
;; This pattern has no side effects and does not set X0 register.
(define_insn "vsetvl_vtype_change_only"
  [(set (reg:SI VTYPE_REGNUM)
	(unspec:SI
	  [(match_operand 0 "const_int_operand" "i")
	   (match_operand 1 "const_int_operand" "i")
	   (match_operand 2 "const_int_operand" "i")
	   (match_operand 3 "const_int_operand" "i")] UNSPEC_VSETVL))]
  "TARGET_VECTOR"
  "vsetvli\tzero,zero,e%0,%m1,t%p2,m%p3"
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "SI")])

;; vsetvl zero,rs1,vtype instruction.
;; The reason we need this pattern since we should avoid setting X0 register
;; in vsetvl instruction pattern.
(define_insn "@vsetvl_discard_result<mode>"
  [(set (reg:SI VL_REGNUM)
	(unspec:SI [(match_operand:P 0 "csr_operand" "rK")
		    (match_operand 1 "const_int_operand" "i")
		    (match_operand 2 "const_int_operand" "i")] UNSPEC_VSETVL))
   (set (reg:SI VTYPE_REGNUM)
	(unspec:SI [(match_dup 1)
		    (match_dup 2)
		    (match_operand 3 "const_int_operand" "i")
		    (match_operand 4 "const_int_operand" "i")] UNSPEC_VSETVL))]
  "TARGET_VECTOR"
  "vset%i0vli\tzero,%0,e%1,%m2,t%p3,m%p4"
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "<MODE>")
   (set (attr "sew") (symbol_ref "INTVAL (operands[1])"))
   (set (attr "vlmul") (symbol_ref "INTVAL (operands[2])"))
   (set (attr "ta") (symbol_ref "INTVAL (operands[3])"))
   (set (attr "ma") (symbol_ref "INTVAL (operands[4])"))])

;; It's emit by vsetvl/vsetvlmax intrinsics with no side effects.
;; Since we have many optmization passes from "expand" to "reload_completed",
;; such pattern can allow us gain benefits of these optimizations.
(define_insn_and_split "@vsetvl<mode>_no_side_effects"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P [(match_operand:P 1 "csr_operand" "rK")
		   (match_operand 2 "const_int_operand" "i")
		   (match_operand 3 "const_int_operand" "i")
		   (match_operand 4 "const_int_operand" "i")
		   (match_operand 5 "const_int_operand" "i")] UNSPEC_VSETVL))]
  "TARGET_VECTOR"
  "#"
  "&& epilogue_completed"
  [(parallel
    [(set (match_dup 0)
	  (unspec:P [(match_dup 1) (match_dup 2) (match_dup 3)
		     (match_dup 4) (match_dup 5)] UNSPEC_VSETVL))
     (set (reg:SI VL_REGNUM)
	  (unspec:SI [(match_dup 1) (match_dup 2) (match_dup 3)] UNSPEC_VSETVL))
     (set (reg:SI VTYPE_REGNUM)
	  (unspec:SI [(match_dup 2) (match_dup 3) (match_dup 4)
		      (match_dup 5)] UNSPEC_VSETVL))])]
  ""
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "SI")])

;; RVV machine description matching format
;; (define_insn ""
;;   [(set (match_operand:MODE 0)
;; 	(if_then_else:MODE
;; 	  (unspec:<MODE:VM>
;; 	    [(match_operand:<VM> 1 "vector_mask_operand")
;; 	     (match_operand N + 4 "vector_length_operand")
;; 	     (match_operand N + 5 "const_int_operand")
;; 	     (match_operand N + 6 "const_int_operand")
;; 	     (reg:SI VL_REGNUM)
;; 	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
;; 	  (instruction operation:MODE
;; 	     (match_operand 3
;; 	     (match_operand 4
;; 	     (match_operand 5
;;           ................
;; 	     (match_operand N + 3)
;; 	  (match_operand:MODE 2 "vector_reg_or_const0_operand")))]
;;
;; (unspec:[........] UNSPEC_VPREDICATE) is a predicate wrapper.
;; Include mask predicate && length predicate && vector policy.

;; -------------------------------------------------------------------------------
;; ---- Predicated Mov
;; -------------------------------------------------------------------------------
;; Includes:
;; - 7.4. Vector Unit-Stride Instructions
;; - 11.15 Vector Integer Merge Instructions
;; - 11.16 Vector Integer Move Instructions
;; - 13.16 Vector Floating-Point Move Instruction
;; - 15.1 Vector Mask-Register Logical Instructions
;; -------------------------------------------------------------------------------

;; vle.v/vse.v/vmv.v.v/vmv.v.x/vmv.v.i/vfmv.v.f.
;; For vle.v/vmv.v.v/vmv.v.x/vmv.v.i/vfmv.v.f, we may need merge and mask operand.
;; For vse.v, we don't need merge operand, so it should always match "vu".
;; constraint alternative 0 ~ 1 match vle.v.
;; constraint alternative 2 match vse.v.
;; constraint alternative 3 match vmv.v.v.
;; constraint alternative 4 match vmv.v.i.
;; For vmv.v.i, we allow 2 following cases:
;;    1. (const_vector:VNx1QI repeat [
;;                (const_int:QI N)]), -15 <= N < 16.
;;    2. (const_vector:VNx1SF repeat [
;;                (const_double:SF 0.0 [0x0.0p+0])]).

;; We add "MEM_P (operands[0]) || MEM_P (operands[3]) || CONST_VECTOR_P (operands[1])" here to
;; make sure we don't want CSE to generate the following pattern:
;; (insn 17 8 19 2 (set (reg:VNx1HI 134 [ _1 ])
;;       (if_then_else:VNx1HI (unspec:VNx1BI [
;;                   (reg/v:VNx1BI 137 [ mask ])
;;                   (reg:DI 151)
;;                   (const_int 0 [0]) repeated x3
;;                   (reg:SI 66 vl)
;;                   (reg:SI 67 vtype)
;;               ] UNSPEC_VPREDICATE)
;;           (const_vector:VNx1HI repeat [
;;                   (const_int 0 [0])
;;               ])
;;           (reg/v:VNx1HI 140 [ merge ]))) "rvv.c":8:12 608 {pred_movvnx1hi}
;;    (expr_list:REG_DEAD (reg:DI 151)
;;       (expr_list:REG_DEAD (reg/v:VNx1HI 140 [ merge ])
;;           (expr_list:REG_DEAD (reg/v:VNx1BI 137 [ mask ])
;;               (nil)))))
;; Since both vmv.v.v and vmv.v.i doesn't have mask operand.
(define_insn_and_split "@pred_mov<mode>"
  [(set (match_operand:V 0 "nonimmediate_operand"      "=vr,    vr,    vd,     m,    vr,    vr")
    (if_then_else:V
      (unspec:<VM>
        [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm, vmWc1,   Wc1,   Wc1")
         (match_operand 4 "vector_length_operand"    "   rK,    rK,    rK,    rK,    rK,    rK")
         (match_operand 5 "const_int_operand"        "    i,     i,     i,     i,     i,     i")
         (match_operand 6 "const_int_operand"        "    i,     i,     i,     i,     i,     i")
         (match_operand 7 "const_int_operand"        "    i,     i,     i,     i,     i,     i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
      (match_operand:V 3 "vector_move_operand"       "    m,     m,     m,    vr,    vr, viWc0")
      (match_operand:V 2 "vector_merge_operand"      "    0,    vu,    vu,    vu,   vu0,   vu0")))]
  "TARGET_VECTOR && (MEM_P (operands[0]) || MEM_P (operands[3])
   || CONST_VECTOR_P (operands[1]))"
  "@
   vle<sew>.v\t%0,%3%p1
   vle<sew>.v\t%0,%3
   vle<sew>.v\t%0,%3,%1.t
   vse<sew>.v\t%3,%0%p1
   vmv.v.v\t%0,%3
   vmv.v.i\t%0,%v3"
  "&& register_operand (operands[0], <MODE>mode)
   && register_operand (operands[3], <MODE>mode)
   && satisfies_constraint_vu (operands[2])
   && INTVAL (operands[7]) == riscv_vector::VLMAX"
  [(set (match_dup 0) (match_dup 3))]
  ""
  [(set_attr "type" "vlde,vlde,vlde,vste,vimov,vimov")
   (set_attr "mode" "<MODE>")])

;; Dedicated pattern for vse.v instruction since we can't reuse pred_mov pattern to include
;; memory operand as input which will produce inferior codegen.
(define_insn "@pred_store<mode>"
  [(set (match_operand:V 0 "memory_operand"                 "+m")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
	     (match_operand 3 "vector_length_operand"    "   rK")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operand:V 2 "register_operand"         "    vr")
	  (match_dup 0)))]
  "TARGET_VECTOR"
  "vse<sew>.v\t%2,%0%p1"
  [(set_attr "type" "vste")
   (set_attr "mode" "<MODE>")
   (set (attr "avl_type") (symbol_ref "riscv_vector::NONVLMAX"))
   (set_attr "vl_op_idx" "3")])

;; vlm.v/vsm.v/vmclr.m/vmset.m.
;; constraint alternative 0 match vlm.v.
;; constraint alternative 1 match vsm.v.
;; constraint alternative 3 match vmclr.m.
;; constraint alternative 4 match vmset.m.
(define_insn_and_split "@pred_mov<mode>"
  [(set (match_operand:VB 0 "nonimmediate_operand"               "=vr,   m,  vr,  vr,  vr")
	(if_then_else:VB
	  (unspec:VB
	    [(match_operand:VB 1 "vector_all_trues_mask_operand" "Wc1, Wc1, Wc1, Wc1, Wc1")
	     (match_operand 4 "vector_length_operand"            " rK,  rK,  rK,  rK,  rK")
	     (match_operand 5 "const_int_operand"                "  i,   i,   i,   i,   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operand:VB 3 "vector_move_operand"              "  m,  vr,  vr, Wc0, Wc1")
	  (match_operand:VB 2 "vector_undef_operand"             " vu,  vu,  vu,  vu,  vu")))]
  "TARGET_VECTOR"
  "@
   vlm.v\t%0,%3
   vsm.v\t%3,%0
   #
   vmclr.m\t%0
   vmset.m\t%0"
  "&& register_operand (operands[0], <MODE>mode)
   && register_operand (operands[3], <MODE>mode)"
  [(set (match_dup 0) (match_dup 3))]
  ""
  [(set_attr "type" "vldm,vstm,vimov,vmalu,vmalu")
   (set_attr "mode" "<MODE>")])

;; Dedicated pattern for vsm.v instruction since we can't reuse pred_mov pattern to include
;; memory operand as input which will produce inferior codegen.
(define_insn "@pred_store<mode>"
  [(set (match_operand:VB 0 "memory_operand"                      "+m")
	(if_then_else:VB
	  (unspec:VB
	    [(match_operand:VB 1 "vector_all_trues_mask_operand" "Wc1")
	     (match_operand 3 "vector_length_operand"            " rK")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operand:VB 2 "register_operand"                 " vr")
	  (match_dup 0)))]
  "TARGET_VECTOR"
  "vsm.v\t%2,%0"
  [(set_attr "type" "vstm")
   (set_attr "mode" "<MODE>")
   (set (attr "avl_type") (symbol_ref "riscv_vector::NONVLMAX"))
   (set_attr "vl_op_idx" "3")])

(define_insn "@pred_merge<mode>"
  [(set (match_operand:V 0 "register_operand"            "=vd,    vd")
    (if_then_else:V
      (match_operand:<VM> 4 "register_operand"         "   vm,    vm")
      (if_then_else:V
        (unspec:<VM>
          [(match_dup 4)
           (match_operand 5 "vector_length_operand"    "   rK,    rK")
           (match_operand 6 "const_int_operand"        "    i,     i")
           (match_operand 7 "const_int_operand"        "    i,     i")
           (reg:SI VL_REGNUM)
           (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
        (match_operand:V 3 "vector_arith_operand"      "   vr,    vi")
        (match_operand:V 2 "register_operand"          "   vr,    vr"))
      (match_operand:V 1 "vector_merge_operand"        "  0vu,   0vu")))]
  "TARGET_VECTOR"
  "vmerge.v%o3m\t%0,%2,%v3,%4"
  [(set_attr "type" "vimerge")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_merge<mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"   "=vd")
    (if_then_else:VI_QHS
      (match_operand:<VM> 4 "register_operand"       " vm")
      (if_then_else:VI_QHS
        (unspec:<VM>
          [(match_dup 4)
           (match_operand 5 "vector_length_operand"  " rK")
           (match_operand 6 "const_int_operand"      "  i")
           (match_operand 7 "const_int_operand"      "  i")
           (reg:SI VL_REGNUM)
           (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	(vec_duplicate:VI_QHS
          (match_operand:<VEL> 3 "register_operand"  "  r"))
        (match_operand:VI_QHS 2 "register_operand"   " vr"))
      (match_operand:VI_QHS 1 "vector_merge_operand" "0vu")))]
  "TARGET_VECTOR"
  "vmerge.vxm\t%0,%2,%3,%4"
  [(set_attr "type" "vimerge")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_merge<mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
    (if_then_else:VI_D
      (match_operand:<VM> 4 "register_operand")
      (if_then_else:VI_D
        (unspec:<VM>
          [(match_dup 4)
           (match_operand 5 "vector_length_operand")
           (match_operand 6 "const_int_operand")
           (match_operand 7 "const_int_operand")
           (reg:SI VL_REGNUM)
           (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	(vec_duplicate:VI_D
          (match_operand:<VEL> 3 "reg_or_int_operand"))
        (match_operand:VI_D 2 "register_operand"))
      (match_operand:VI_D 1 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::simm5_p (operands[3]))
      operands[3] = force_reg (<VEL>mode, operands[3]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[3]))
	  operands[3] = gen_rtx_SIGN_EXTEND (<VEL>mode,
		force_reg (Pmode, operands[3]));
	else
	  {
	    if (CONST_INT_P (operands[3]))
	      operands[3] = force_reg (<VEL>mode, operands[3]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[3], operands[5], <VM>mode);
	    emit_insn (gen_pred_merge<mode> (operands[0], operands[1],
			operands[2], v, operands[4],operands[5],
			operands[6], operands[7]));
	    DONE;
	  }
      }
    else
      operands[3] = force_reg (<VEL>mode, operands[3]);
  })

(define_insn "*pred_merge<mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"     "=vd")
    (if_then_else:VI_D
      (match_operand:<VM> 4 "register_operand"       " vm")
      (if_then_else:VI_D
        (unspec:<VM>
          [(match_dup 4)
           (match_operand 5 "vector_length_operand"  " rK")
           (match_operand 6 "const_int_operand"      "  i")
           (match_operand 7 "const_int_operand"      "  i")
           (reg:SI VL_REGNUM)
           (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	(vec_duplicate:VI_D
          (match_operand:<VEL> 3 "register_operand" "  r"))
        (match_operand:VI_D 2 "register_operand"     " vr"))
      (match_operand:VI_D 1 "vector_merge_operand"   "0vu")))]
  "TARGET_VECTOR"
  "vmerge.vxm\t%0,%2,%3,%4"
  [(set_attr "type" "vimerge")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_merge<mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"     "=vd")
    (if_then_else:VI_D
      (match_operand:<VM> 4 "register_operand"       " vm")
      (if_then_else:VI_D
        (unspec:<VM>
          [(match_dup 4)
           (match_operand 5 "vector_length_operand"  " rK")
           (match_operand 6 "const_int_operand"      "  i")
           (match_operand 7 "const_int_operand"      "  i")
           (reg:SI VL_REGNUM)
           (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	(vec_duplicate:VI_D
	  (sign_extend:<VEL>
            (match_operand:<VSUBEL> 3 "register_operand" "  r")))
        (match_operand:VI_D 2 "register_operand"     " vr"))
      (match_operand:VI_D 1 "vector_merge_operand"   "0vu")))]
  "TARGET_VECTOR"
  "vmerge.vxm\t%0,%2,%3,%4"
  [(set_attr "type" "vimerge")
   (set_attr "mode" "<MODE>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated Broadcast
;; -------------------------------------------------------------------------------
;; Includes:
;; - 7.5. Vector Strided Instructions (zero stride)
;; - 11.16 Vector Integer Move Instructions (vmv.v.x)
;; - 13.16 Vector Floating-Point Move Instruction (vfmv.v.f)
;; -------------------------------------------------------------------------------

;; According to RVV ISA, vector-scalar instruction doesn't support
;; operand fetched from 2 consecutive registers, so we should use
;; vlse.v which is a memory access to broadcast a DImode scalar into a vector.
;;
;; Since the optimization flow in GCC is as follows:
;; expand --> LICM (Loop invariant) --> split.
;; To use LICM optimization, we postpone generation of vlse.v to split stage since
;; a memory access instruction can not be optimized by LICM (Loop invariant).
(define_insn_and_split "@pred_broadcast<mode>"
  [(set (match_operand:V 0 "register_operand"                 "=vr,  vr,  vr,  vr")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      " Wc1, Wc1, vm, Wc1")
	     (match_operand 4 "vector_length_operand"         " rK,  rK,  rK,  rK")
	     (match_operand 5 "const_int_operand"             "  i,   i,   i,   i")
	     (match_operand 6 "const_int_operand"             "  i,   i,   i,   i")
	     (match_operand 7 "const_int_operand"             "  i,   i,   i,   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (vec_duplicate:V
	    (match_operand:<VEL> 3 "direct_broadcast_operand" "  r,   f, Wdm, Wdm"))
	  (match_operand:V 2 "vector_merge_operand"           "0vu, 0vu, 0vu, 0vu")))]
  "TARGET_VECTOR"
  "@
   vmv.v.x\t%0,%3
   vfmv.v.f\t%0,%3
   vlse<sew>.v\t%0,%3,zero,%1.t
   vlse<sew>.v\t%0,%3,zero"
  "!FLOAT_MODE_P (<MODE>mode) && register_operand (operands[3], <VEL>mode)
  && GET_MODE_BITSIZE (<VEL>mode) > GET_MODE_BITSIZE (Pmode)"
  [(set (match_dup 0)
	(if_then_else:V (unspec:<VM> [(match_dup 1) (match_dup 4)
	     (match_dup 5) (match_dup 6) (match_dup 7)
	     (reg:SI VL_REGNUM) (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (vec_duplicate:V (match_dup 3))
	  (match_dup 2)))]
  {
    gcc_assert (can_create_pseudo_p ());
    rtx m = assign_stack_local (<VEL>mode, GET_MODE_SIZE (<VEL>mode),
				GET_MODE_ALIGNMENT (<VEL>mode));
    m = validize_mem (m);
    emit_move_insn (m, operands[3]);
    m = gen_rtx_MEM (<VEL>mode, force_reg (Pmode, XEXP (m, 0)));
    operands[3] = m;
  }
  [(set_attr "type" "vimov,vfmov,vlds,vlds")
   (set_attr "mode" "<MODE>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated Strided loads/stores
;; -------------------------------------------------------------------------------
;; Includes:
;; - 7.5. Vector Strided Instructions
;; -------------------------------------------------------------------------------

(define_insn "@pred_strided_load<mode>"
  [(set (match_operand:V 0 "register_operand"              "=vr,    vr,    vd")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm")
	     (match_operand 5 "vector_length_operand"    "   rK,    rK,    rK")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V
	    [(match_operand:V 3 "memory_operand"         "    m,     m,     m")
	     (match_operand 4 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")] UNSPEC_STRIDED)
	  (match_operand:V 2 "vector_merge_operand"      "    0,    vu,    vu")))]
  "TARGET_VECTOR"
  "vlse<sew>.v\t%0,%3,%z4%p1"
  [(set_attr "type" "vlds")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_strided_store<mode>"
  [(set (match_operand:V 0 "memory_operand"                 "+m")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V
	    [(match_operand 2 "pmode_reg_or_0_operand"   "   rJ")
	     (match_operand:V 3 "register_operand"       "   vr")] UNSPEC_STRIDED)
	  (match_dup 0)))]
  "TARGET_VECTOR"
  "vsse<sew>.v\t%3,%0,%z2%p1"
  [(set_attr "type" "vsts")
   (set_attr "mode" "<MODE>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated indexed loads/stores
;; -------------------------------------------------------------------------------
;; Includes:
;; - 7.6. Vector Indexed Instructions
;; -------------------------------------------------------------------------------

(define_insn "@pred_indexed_<order>load<VNX1_QHSD:mode><VNX1_QHSDI:mode>"
  [(set (match_operand:VNX1_QHSD 0 "register_operand"      "=&vr")
	(if_then_else:VNX1_QHSD
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  "vmWc1")
	     (match_operand 5 "vector_length_operand"     "   rK")
	     (match_operand 6 "const_int_operand"         "    i")
	     (match_operand 7 "const_int_operand"         "    i")
	     (match_operand 8 "const_int_operand"         "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VNX1_QHSD
	    [(match_operand 3 "pmode_register_operand"    "    r")
	     (mem:BLK (scratch))
	     (match_operand:VNX1_QHSDI 4 "register_operand" " vr")] ORDER)
	  (match_operand:VNX1_QHSD 2 "vector_merge_operand" "0vu")))]
  "TARGET_VECTOR"
  "vl<order>xei<VNX1_QHSDI:sew>.v\t%0,(%3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<VNX1_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>store<VNX1_QHSD:mode><VNX1_QHSDI:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_register_operand"      "   r")
	   (match_operand:VNX1_QHSDI 2 "register_operand" "  vr")
	   (match_operand:VNX1_QHSD 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_VECTOR"
  "vs<order>xei<VNX1_QHSDI:sew>.v\t%3,(%1),%2%p0"
  [(set_attr "type" "vst<order>x")
   (set_attr "mode" "<VNX1_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>load<VNX2_QHSD:mode><VNX2_QHSDI:mode>"
  [(set (match_operand:VNX2_QHSD 0 "register_operand"      "=&vr")
	(if_then_else:VNX2_QHSD
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  "vmWc1")
	     (match_operand 5 "vector_length_operand"     "   rK")
	     (match_operand 6 "const_int_operand"         "    i")
	     (match_operand 7 "const_int_operand"         "    i")
	     (match_operand 8 "const_int_operand"         "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VNX2_QHSD
	    [(match_operand 3 "pmode_register_operand"    "    r")
	     (mem:BLK (scratch))
	     (match_operand:VNX2_QHSDI 4 "register_operand" " vr")] ORDER)
	  (match_operand:VNX2_QHSD 2 "vector_merge_operand" "0vu")))]
  "TARGET_VECTOR"
  "vl<order>xei<VNX2_QHSDI:sew>.v\t%0,(%3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<VNX2_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>store<VNX2_QHSD:mode><VNX2_QHSDI:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_register_operand"      "   r")
	   (match_operand:VNX2_QHSDI 2 "register_operand" "  vr")
	   (match_operand:VNX2_QHSD 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_VECTOR"
  "vs<order>xei<VNX2_QHSDI:sew>.v\t%3,(%1),%2%p0"
  [(set_attr "type" "vst<order>x")
   (set_attr "mode" "<VNX2_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>load<VNX4_QHSD:mode><VNX4_QHSDI:mode>"
  [(set (match_operand:VNX4_QHSD 0 "register_operand"      "=&vr")
	(if_then_else:VNX4_QHSD
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  "vmWc1")
	     (match_operand 5 "vector_length_operand"     "   rK")
	     (match_operand 6 "const_int_operand"         "    i")
	     (match_operand 7 "const_int_operand"         "    i")
	     (match_operand 8 "const_int_operand"         "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VNX4_QHSD
	    [(match_operand 3 "pmode_register_operand"    "    r")
	     (mem:BLK (scratch))
	     (match_operand:VNX4_QHSDI 4 "register_operand" " vr")] ORDER)
	  (match_operand:VNX4_QHSD 2 "vector_merge_operand" "0vu")))]
  "TARGET_VECTOR"
  "vl<order>xei<VNX4_QHSDI:sew>.v\t%0,(%3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<VNX4_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>store<VNX4_QHSD:mode><VNX4_QHSDI:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_register_operand"      "   r")
	   (match_operand:VNX4_QHSDI 2 "register_operand" "  vr")
	   (match_operand:VNX4_QHSD 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_VECTOR"
  "vs<order>xei<VNX4_QHSDI:sew>.v\t%3,(%1),%2%p0"
  [(set_attr "type" "vst<order>x")
   (set_attr "mode" "<VNX4_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>load<VNX8_QHSD:mode><VNX8_QHSDI:mode>"
  [(set (match_operand:VNX8_QHSD 0 "register_operand"      "=&vr")
	(if_then_else:VNX8_QHSD
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  "vmWc1")
	     (match_operand 5 "vector_length_operand"     "   rK")
	     (match_operand 6 "const_int_operand"         "    i")
	     (match_operand 7 "const_int_operand"         "    i")
	     (match_operand 8 "const_int_operand"         "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VNX8_QHSD
	    [(match_operand 3 "pmode_register_operand"    "    r")
	     (mem:BLK (scratch))
	     (match_operand:VNX8_QHSDI 4 "register_operand" " vr")] ORDER)
	  (match_operand:VNX8_QHSD 2 "vector_merge_operand" "0vu")))]
  "TARGET_VECTOR"
  "vl<order>xei<VNX8_QHSDI:sew>.v\t%0,(%3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<VNX8_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>store<VNX8_QHSD:mode><VNX8_QHSDI:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_register_operand"      "   r")
	   (match_operand:VNX8_QHSDI 2 "register_operand" "  vr")
	   (match_operand:VNX8_QHSD 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_VECTOR"
  "vs<order>xei<VNX8_QHSDI:sew>.v\t%3,(%1),%2%p0"
  [(set_attr "type" "vst<order>x")
   (set_attr "mode" "<VNX8_QHSD:MODE>")])

(define_insn "@pred_indexed_<order>load<VNX16_QHS:mode><VNX16_QHSI:mode>"
  [(set (match_operand:VNX16_QHS 0 "register_operand"      "=&vr")
	(if_then_else:VNX16_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  "vmWc1")
	     (match_operand 5 "vector_length_operand"     "   rK")
	     (match_operand 6 "const_int_operand"         "    i")
	     (match_operand 7 "const_int_operand"         "    i")
	     (match_operand 8 "const_int_operand"         "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VNX16_QHS
	    [(match_operand 3 "pmode_register_operand"    "    r")
	     (mem:BLK (scratch))
	     (match_operand:VNX16_QHSI 4 "register_operand" " vr")] ORDER)
	  (match_operand:VNX16_QHS 2 "vector_merge_operand" "0vu")))]
  "TARGET_VECTOR"
  "vl<order>xei<VNX16_QHSI:sew>.v\t%0,(%3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<VNX16_QHS:MODE>")])

(define_insn "@pred_indexed_<order>store<VNX16_QHS:mode><VNX16_QHSI:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_register_operand"      "   r")
	   (match_operand:VNX16_QHSI 2 "register_operand" "  vr")
	   (match_operand:VNX16_QHS 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_VECTOR"
  "vs<order>xei<VNX16_QHSI:sew>.v\t%3,(%1),%2%p0"
  [(set_attr "type" "vst<order>x")
   (set_attr "mode" "<VNX16_QHS:MODE>")])

(define_insn "@pred_indexed_<order>load<VNX32_QH:mode><VNX32_QHI:mode>"
  [(set (match_operand:VNX32_QH 0 "register_operand"      "=&vr")
	(if_then_else:VNX32_QH
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  "vmWc1")
	     (match_operand 5 "vector_length_operand"     "   rK")
	     (match_operand 6 "const_int_operand"         "    i")
	     (match_operand 7 "const_int_operand"         "    i")
	     (match_operand 8 "const_int_operand"         "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VNX32_QH
	    [(match_operand 3 "pmode_register_operand"    "    r")
	     (mem:BLK (scratch))
	     (match_operand:VNX32_QHI 4 "register_operand"  " vr")] ORDER)
	  (match_operand:VNX32_QH 2 "vector_merge_operand"  "0vu")))]
  "TARGET_VECTOR"
  "vl<order>xei<VNX32_QHI:sew>.v\t%0,(%3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<VNX32_QH:MODE>")])

(define_insn "@pred_indexed_<order>store<VNX32_QH:mode><VNX32_QHI:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_register_operand"      "   r")
	   (match_operand:VNX32_QHI 2 "register_operand"  "  vr")
	   (match_operand:VNX32_QH 3 "register_operand"   "  vr")] ORDER))]
  "TARGET_VECTOR"
  "vs<order>xei<VNX32_QHI:sew>.v\t%3,(%1),%2%p0"
  [(set_attr "type" "vst<order>x")
   (set_attr "mode" "<VNX32_QH:MODE>")])

(define_insn "@pred_indexed_<order>load<VNX64_Q:mode><VNX64_Q:mode>"
  [(set (match_operand:VNX64_Q 0 "register_operand"      "=&vr")
	(if_then_else:VNX64_Q
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  "vmWc1")
	     (match_operand 5 "vector_length_operand"     "   rK")
	     (match_operand 6 "const_int_operand"         "    i")
	     (match_operand 7 "const_int_operand"         "    i")
	     (match_operand 8 "const_int_operand"         "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VNX64_Q
	    [(match_operand 3 "pmode_register_operand"    "    r")
	     (mem:BLK (scratch))
	     (match_operand:VNX64_Q 4 "register_operand"    " vr")] ORDER)
	  (match_operand:VNX64_Q 2 "vector_merge_operand"   "0vu")))]
  "TARGET_VECTOR"
  "vl<order>xei<VNX64_Q:sew>.v\t%0,(%3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<VNX64_Q:MODE>")])

(define_insn "@pred_indexed_<order>store<VNX64_Q:mode><VNX64_Q:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_register_operand"      "   r")
	   (match_operand:VNX64_Q 2 "register_operand"    "  vr")
	   (match_operand:VNX64_Q 3 "register_operand"    "  vr")] ORDER))]
  "TARGET_VECTOR"
  "vs<order>xei<VNX64_Q:sew>.v\t%3,(%1),%2%p0"
  [(set_attr "type" "vst<order>x")
   (set_attr "mode" "<VNX64_Q:MODE>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated integer binary operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 11.1 Vector Single-Width Integer Add and Subtract
;; - 11.4 Vector Integer Add-with-Carry/Subtract-with-Borrow Instructions
;; - 11.5 Vector Bitwise Logical Instructions
;; - 11.6 Vector Single-Width Bit Shift Instructions
;; - 11.9 Vector Integer Min/Max Instructions
;; - 11.10 Vector Single-Width Integer Multiply Instructions
;; - 11.11 Vector Integer Divide Instructions
;; - 12.1 Vector Single-Width Saturating Add and Subtract
;; -------------------------------------------------------------------------------

(define_insn "@pred_<optab><mode>"
  [(set (match_operand:VI 0 "register_operand"           "=vd, vr, vd, vr, vd, vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1, vm,Wc1, vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK, rK, rK, rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i,  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i,  i,  i,  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i,  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_int_binop:VI
	    (match_operand:VI 3 "<binop_rhs1_predicate>" "<binop_rhs1_constraint>")
	    (match_operand:VI 4 "<binop_rhs2_predicate>" "<binop_rhs2_constraint>"))
	  (match_operand:VI 2 "vector_merge_operand"     "0vu,0vu,0vu,0vu,0vu,0vu")))]
  "TARGET_VECTOR"
  "@
   v<insn>.vv\t%0,%3,%4%p1
   v<insn>.vv\t%0,%3,%4%p1
   v<binop_vi_variant_insn>\t%0,<binop_vi_variant_op>%p1
   v<binop_vi_variant_insn>\t%0,<binop_vi_variant_op>%p1
   v<binop_reverse_vi_variant_insn>\t%0,<binop_reverse_vi_variant_op>%p1
   v<binop_reverse_vi_variant_insn>\t%0,<binop_reverse_vi_variant_op>%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

;; vx instructions patterns.
;; Note: Unlike vv patterns, we should split them since they are variant.
;; For vsll.vx/vsra.vx/vsrl.vx the scalar mode should be Pmode wheras the
;; scalar mode is inner mode of the RVV mode for other vx patterns.
(define_insn "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI 0 "register_operand"            "=vd, vr, vd, vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "  vm,Wc1, vm,Wc1")
	     (match_operand 5 "vector_length_operand"    "  rK, rK, rK, rK")
	     (match_operand 6 "const_int_operand"        "   i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"        "   i,  i,  i,  i")
	     (match_operand 8 "const_int_operand"        "   i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_shift:VI
	    (match_operand:VI 3 "register_operand"        " vr, vr, vr, vr")
	    (match_operand 4 "pmode_reg_or_uimm5_operand" "  r,  r,  K,  K"))
	  (match_operand:VI 2 "vector_merge_operand"      "0vu,0vu,0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.v%o4\t%0,%3,%4%p1"
  [(set_attr "type" "vshift")
   (set_attr "mode" "<MODE>")])

;; Handle GET_MODE_INNER (mode) = QImode, HImode, SImode.
(define_insn "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"       "=vd, vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_commutative_binop:VI_QHS
	    (vec_duplicate:VI_QHS
	      (match_operand:<VEL> 4 "reg_or_0_operand"  " rJ, rJ"))
	    (match_operand:VI_QHS 3 "register_operand"   " vr, vr"))
	  (match_operand:VI_QHS 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"       "=vd, vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_non_commutative_binop:VI_QHS
	    (match_operand:VI_QHS 3 "register_operand"   " vr, vr")
	    (vec_duplicate:VI_QHS
	      (match_operand:<VEL> 4 "reg_or_0_operand"  " rJ, rJ")))
	  (match_operand:VI_QHS 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_sub<mode>_reverse_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"       "=vd, vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (minus:VI_QHS
	    (vec_duplicate:VI_QHS
	      (match_operand:<VEL> 4 "register_operand"  "  r,  r"))
	    (match_operand:VI_QHS 3 "register_operand"   " vr, vr"))
	  (match_operand:VI_QHS 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "vrsub.vx\t%0,%3,%4%p1"
  [(set_attr "type" "vialu")
   (set_attr "mode" "<MODE>")])

;; Handle GET_MODE_INNER (mode) = DImode. We need to split them since
;; we need to deal with SEW = 64 in RV32 system.
(define_expand "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_commutative_binop:VI_D
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "reg_or_int_operand"))
	    (match_operand:VI_D 3 "register_operand"))
	  (match_operand:VI_D 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::has_vi_variant_p (<CODE>, operands[4]))
      operands[4] = force_reg (<VEL>mode, operands[4]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[4]))
	  {
	    if (!rtx_equal_p (operands[4], const0_rtx))
	      operands[4] = force_reg (Pmode, operands[4]);
	    operands[4] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[4]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[4]))
	      operands[4] = force_reg (<VEL>mode, operands[4]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[4], operands[5], <VM>mode);
	    emit_insn (gen_pred_<optab><mode> (operands[0], operands[1],
			operands[2], operands[3], v, operands[5],
			operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      {
	if (!rtx_equal_p (operands[4], const0_rtx))
	  operands[4] = force_reg (<VEL>mode, operands[4]);
      }
  })

(define_insn "*pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"         "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_commutative_binop:VI_D
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "reg_or_0_operand"  " rJ, rJ"))
	    (match_operand:VI_D 3 "register_operand"     " vr, vr"))
	  (match_operand:VI_D 2 "vector_merge_operand"   "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<optab><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"             "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"     " vm,Wc1")
	     (match_operand 5 "vector_length_operand"        " rK, rK")
	     (match_operand 6 "const_int_operand"            "  i,  i")
	     (match_operand 7 "const_int_operand"            "  i,  i")
	     (match_operand 8 "const_int_operand"            "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_commutative_binop:VI_D
	    (vec_duplicate:VI_D
	      (sign_extend:<VEL>
	        (match_operand:<VSUBEL> 4 "reg_or_0_operand" " rJ, rJ")))
	    (match_operand:VI_D 3 "register_operand"         " vr, vr"))
	  (match_operand:VI_D 2 "vector_merge_operand"       "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_non_commutative_binop:VI_D
	    (match_operand:VI_D 3 "register_operand")
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "reg_or_int_operand")))
	  (match_operand:VI_D 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::has_vi_variant_p (<CODE>, operands[4]))
      operands[4] = force_reg (<VEL>mode, operands[4]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[4]))
	  {
	    if (!rtx_equal_p (operands[4], const0_rtx))
	      operands[4] = force_reg (Pmode, operands[4]);
	    operands[4] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[4]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[4]))
	      operands[4] = force_reg (<VEL>mode, operands[4]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[4], operands[5], <VM>mode);
	    emit_insn (gen_pred_<optab><mode> (operands[0], operands[1],
			operands[2], operands[3], v, operands[5],
			operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      {
	if (!rtx_equal_p (operands[4], const0_rtx))
	  operands[4] = force_reg (<VEL>mode, operands[4]);
      }
  })

(define_insn "*pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"         "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_non_commutative_binop:VI_D
	    (match_operand:VI_D 3 "register_operand"     " vr, vr")
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "reg_or_0_operand"  " rJ, rJ")))
	  (match_operand:VI_D 2 "vector_merge_operand"   "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<optab><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"             "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"     " vm,Wc1")
	     (match_operand 5 "vector_length_operand"        " rK, rK")
	     (match_operand 6 "const_int_operand"            "  i,  i")
	     (match_operand 7 "const_int_operand"            "  i,  i")
	     (match_operand 8 "const_int_operand"            "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_non_commutative_binop:VI_D
	    (match_operand:VI_D 3 "register_operand"         " vr, vr")
	    (vec_duplicate:VI_D
	      (sign_extend:<VEL>
	        (match_operand:<VSUBEL> 4 "reg_or_0_operand" " rJ, rJ"))))
	  (match_operand:VI_D 2 "vector_merge_operand"       "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_sub<mode>_reverse_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (minus:VI_D
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "reg_or_int_operand"))
	    (match_operand:VI_D 3 "register_operand"))
	  (match_operand:VI_D 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::neg_simm5_p (operands[4]))
      operands[4] = force_reg (<VEL>mode, operands[4]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[4]))
	  operands[4] = gen_rtx_SIGN_EXTEND (<VEL>mode,
		force_reg (Pmode, operands[4]));
	else
	  {
	    if (CONST_INT_P (operands[4]))
	      operands[4] = force_reg (<VEL>mode, operands[4]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[4], operands[5], <VM>mode);
	    emit_insn (gen_pred_sub<mode> (operands[0], operands[1],
			operands[2], operands[3], v, operands[5],
			operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      operands[4] = force_reg (<VEL>mode, operands[4]);
  })

(define_insn "*pred_sub<mode>_reverse_scalar"
  [(set (match_operand:VI_D 0 "register_operand"         "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (minus:VI_D
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "register_operand"  "  r,  r"))
	    (match_operand:VI_D 3 "register_operand"     " vr, vr"))
	  (match_operand:VI_D 2 "vector_merge_operand"   "0vu,0vu")))]
  "TARGET_VECTOR"
  "vrsub.vx\t%0,%3,%4%p1"
  [(set_attr "type" "vialu")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_sub<mode>_extended_reverse_scalar"
  [(set (match_operand:VI_D 0 "register_operand"             "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"     " vm,Wc1")
	     (match_operand 5 "vector_length_operand"        " rK, rK")
	     (match_operand 6 "const_int_operand"            "  i,  i")
	     (match_operand 7 "const_int_operand"            "  i,  i")
	     (match_operand 8 "const_int_operand"            "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (minus:VI_D
	    (vec_duplicate:VI_D
	      (sign_extend:<VEL>
	        (match_operand:<VSUBEL> 4 "register_operand" "  r,  r")))
	    (match_operand:VI_D 3 "register_operand"         " vr, vr"))
	  (match_operand:VI_D 2 "vector_merge_operand"       "0vu,0vu")))]
  "TARGET_VECTOR"
  "vrsub.vx\t%0,%3,%4%p1"
  [(set_attr "type" "vialu")
   (set_attr "mode" "<MODE>")])

;; Saturating Add and Subtract
(define_insn "@pred_<optab><mode>"
  [(set (match_operand:VI 0 "register_operand"                   "=vd, vr, vd, vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"         " vm,Wc1, vm,Wc1")
	     (match_operand 5 "vector_length_operand"            " rK, rK, rK, rK")
	     (match_operand 6 "const_int_operand"                "  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"                "  i,  i,  i,  i")
	     (match_operand 8 "const_int_operand"                "  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_sat_int_binop:VI
	    (match_operand:VI 3 "<binop_rhs1_predicate>"         " vr, vr, vr, vr")
	    (match_operand:VI 4 "<binop_rhs2_predicate>" "<binop_rhs2_constraint>"))
	  (match_operand:VI 2 "vector_merge_operand"             "0vu,0vu,0vu,0vu")))]
  "TARGET_VECTOR"
  "@
   v<insn>.vv\t%0,%3,%4%p1
   v<insn>.vv\t%0,%3,%4%p1
   v<binop_vi_variant_insn>\t%0,<binop_vi_variant_op>%p1
   v<binop_vi_variant_insn>\t%0,<binop_vi_variant_op>%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

;; Handle GET_MODE_INNER (mode) = QImode, HImode, SImode.
(define_insn "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"       "=vd, vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_plus_binop:VI_QHS
	    (vec_duplicate:VI_QHS
	      (match_operand:<VEL> 4 "register_operand"  "  r,  r"))
	    (match_operand:VI_QHS 3 "register_operand"   " vr, vr"))
	  (match_operand:VI_QHS 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"       "=vd, vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_minus_binop:VI_QHS
	    (match_operand:VI_QHS 3 "register_operand"   " vr, vr")
	    (vec_duplicate:VI_QHS
	      (match_operand:<VEL> 4 "register_operand"  "  r,  r")))
	  (match_operand:VI_QHS 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_plus_binop:VI_D
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "reg_or_int_operand"))
	    (match_operand:VI_D 3 "register_operand"))
	  (match_operand:VI_D 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::has_vi_variant_p (<CODE>, operands[4]))
      operands[4] = force_reg (<VEL>mode, operands[4]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[4]))
	  operands[4] = gen_rtx_SIGN_EXTEND (<VEL>mode,
		force_reg (Pmode, operands[4]));
	else
	  {
	    if (CONST_INT_P (operands[4]))
	      operands[4] = force_reg (<VEL>mode, operands[4]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[4], operands[5], <VM>mode);
	    emit_insn (gen_pred_<optab><mode> (operands[0], operands[1],
			operands[2], operands[3], v, operands[5],
			operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      operands[4] = force_reg (<VEL>mode, operands[4]);
  })

(define_insn "*pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"         "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_plus_binop:VI_D
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "register_operand"  "  r,  r"))
	    (match_operand:VI_D 3 "register_operand"     " vr, vr"))
	  (match_operand:VI_D 2 "vector_merge_operand"   "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<optab><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"             "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"     " vm,Wc1")
	     (match_operand 5 "vector_length_operand"        " rK, rK")
	     (match_operand 6 "const_int_operand"            "  i,  i")
	     (match_operand 7 "const_int_operand"            "  i,  i")
	     (match_operand 8 "const_int_operand"            "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_plus_binop:VI_D
	    (vec_duplicate:VI_D
	      (sign_extend:<VEL>
	        (match_operand:<VSUBEL> 4 "register_operand" "  r,  r")))
	    (match_operand:VI_D 3 "register_operand"         " vr, vr"))
	  (match_operand:VI_D 2 "vector_merge_operand"       "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_minus_binop:VI_D
	    (match_operand:VI_D 3 "register_operand")
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "reg_or_int_operand")))
	  (match_operand:VI_D 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::has_vi_variant_p (<CODE>, operands[4]))
      operands[4] = force_reg (<VEL>mode, operands[4]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[4]))
	  operands[4] = gen_rtx_SIGN_EXTEND (<VEL>mode,
		force_reg (Pmode, operands[4]));
	else
	  {
	    if (CONST_INT_P (operands[4]))
	      operands[4] = force_reg (<VEL>mode, operands[4]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[4], operands[5], <VM>mode);
	    emit_insn (gen_pred_<optab><mode> (operands[0], operands[1],
			operands[2], operands[3], v, operands[5],
			operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      operands[4] = force_reg (<VEL>mode, operands[4]);
  })

(define_insn "*pred_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"         "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_minus_binop:VI_D
	    (match_operand:VI_D 3 "register_operand"     " vr, vr")
	    (vec_duplicate:VI_D
	      (match_operand:<VEL> 4 "register_operand"  "  r,  r")))
	  (match_operand:VI_D 2 "vector_merge_operand"   "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<optab><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"             "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"     " vm,Wc1")
	     (match_operand 5 "vector_length_operand"        " rK, rK")
	     (match_operand 6 "const_int_operand"            "  i,  i")
	     (match_operand 7 "const_int_operand"            "  i,  i")
	     (match_operand 8 "const_int_operand"            "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (sat_int_minus_binop:VI_D
	    (match_operand:VI_D 3 "register_operand"         " vr, vr")
	    (vec_duplicate:VI_D
	      (sign_extend:<VEL>
	        (match_operand:<VSUBEL> 4 "register_operand" "  r,  r"))))
	  (match_operand:VI_D 2 "vector_merge_operand"       "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.vx\t%0,%3,%4%p1"
  [(set_attr "type" "<int_binop_insn_type>")
   (set_attr "mode" "<MODE>")])

;; Multiply High instructions.
(define_insn "@pred_mulh<v_su><mode>"
  [(set (match_operand:VFULLI 0 "register_operand"       "=vd, vr")
	(if_then_else:VFULLI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VFULLI
	    [(match_operand:VFULLI 3 "register_operand"  " vr, vr")
	     (match_operand:VFULLI 4 "register_operand"  " vr, vr")] VMULH)
	  (match_operand:VFULLI 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "vmulh<v_su>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "vimul")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_mulh<v_su><mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"       "=vd, vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_QHS
	    [(vec_duplicate:VI_QHS
	       (match_operand:<VEL> 4 "reg_or_0_operand"  " rJ, rJ"))
	     (match_operand:VI_QHS 3 "register_operand"   " vr, vr")] VMULH)
	  (match_operand:VI_QHS 2 "vector_merge_operand"  "0vu,0vu")))]
  "TARGET_VECTOR"
  "vmulh<v_su>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "vimul")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_mulh<v_su><mode>_scalar"
  [(set (match_operand:VFULLI_D 0 "register_operand")
	(if_then_else:VFULLI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VFULLI_D
	    [(vec_duplicate:VFULLI_D
	       (match_operand:<VEL> 4 "reg_or_int_operand"))
	     (match_operand:VFULLI_D 3 "register_operand")] VMULH)
	  (match_operand:VFULLI_D 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[4]))
	  {
	    if (!rtx_equal_p (operands[4], const0_rtx))
	      operands[4] = force_reg (Pmode, operands[4]);
	    operands[4] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[4]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[4]))
	      operands[4] = force_reg (<VEL>mode, operands[4]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[4], operands[5], <VM>mode);
	    emit_insn (gen_pred_mulh<v_su><mode> (operands[0], operands[1],
			operands[2], operands[3], v, operands[5],
			operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      {
	if (!rtx_equal_p (operands[4], const0_rtx))
	  operands[4] = force_reg (<VEL>mode, operands[4]);
      }
  })

(define_insn "*pred_mulh<v_su><mode>_scalar"
  [(set (match_operand:VFULLI_D 0 "register_operand"       "=vd, vr")
	(if_then_else:VFULLI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"   " vm,Wc1")
	     (match_operand 5 "vector_length_operand"      " rK, rK")
	     (match_operand 6 "const_int_operand"          "  i,  i")
	     (match_operand 7 "const_int_operand"          "  i,  i")
	     (match_operand 8 "const_int_operand"          "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VFULLI_D
	    [(vec_duplicate:VFULLI_D
	       (match_operand:<VEL> 4 "reg_or_0_operand"   " rJ, rJ"))
	     (match_operand:VFULLI_D 3 "register_operand"  " vr, vr")] VMULH)
	  (match_operand:VFULLI_D 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "vmulh<v_su>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "vimul")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_mulh<v_su><mode>_extended_scalar"
  [(set (match_operand:VFULLI_D 0 "register_operand"          "=vd, vr")
	(if_then_else:VFULLI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      " vm,Wc1")
	     (match_operand 5 "vector_length_operand"         " rK, rK")
	     (match_operand 6 "const_int_operand"             "  i,  i")
	     (match_operand 7 "const_int_operand"             "  i,  i")
	     (match_operand 8 "const_int_operand"             "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VFULLI_D
	    [(vec_duplicate:VFULLI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 4 "reg_or_0_operand" " rJ, rJ")))
	     (match_operand:VFULLI_D 3 "register_operand"     " vr, vr")] VMULH)
	  (match_operand:VFULLI_D 2 "vector_merge_operand"    "0vu,0vu")))]
  "TARGET_VECTOR"
  "vmulh<v_su>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "vimul")
   (set_attr "mode" "<MODE>")])

;; Vector Integer Add-with-Carry / Subtract-with-Borrow Instructions
(define_insn "@pred_adc<mode>"
  [(set (match_operand:VI 0 "register_operand"            "=&vr, &vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"     "  rK,  rK")
	     (match_operand 6 "const_int_operand"         "   i,   i")
	     (match_operand 7 "const_int_operand"         "   i,   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI
	     [(plus:VI
	       (match_operand:VI 2 "register_operand"     "  vr,  vr")
	       (match_operand:VI 3 "vector_arith_operand" "  vr,  vi"))
	     (match_operand:<VM> 4 "register_operand"     "  vm,  vm")] UNSPEC_VADC)
	  (match_operand:VI 1 "vector_merge_operand"      " 0vu, 0vu")))]
  "TARGET_VECTOR"
  "vadc.v%o3m\t%0,%2,%v3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_insn "@pred_sbc<mode>"
  [(set (match_operand:VI 0 "register_operand"            "=&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"     "  rK")
	     (match_operand 6 "const_int_operand"         "   i")
	     (match_operand 7 "const_int_operand"         "   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI
	     [(minus:VI
	       (match_operand:VI 2 "register_operand"     "  vr")
	       (match_operand:VI 3 "register_operand"     "  vr"))
	      (match_operand:<VM> 4 "register_operand"    "  vm")] UNSPEC_VSBC)
	  (match_operand:VI 1 "vector_merge_operand"      " 0vu")))]
  "TARGET_VECTOR"
  "vsbc.vvm\t%0,%2,%3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_insn "@pred_adc<mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"        "=&vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"      " rK")
	     (match_operand 6 "const_int_operand"          "  i")
	     (match_operand 7 "const_int_operand"          "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_QHS
	     [(plus:VI_QHS
	       (vec_duplicate:VI_QHS
	         (match_operand:<VEL> 3 "register_operand" "  r"))
	       (match_operand:VI_QHS 2 "register_operand"  " vr"))
	     (match_operand:<VM> 4 "register_operand"      " vm")] UNSPEC_VADC)
	  (match_operand:VI_QHS 1 "vector_merge_operand"   "0vu")))]
  "TARGET_VECTOR"
  "vadc.vxm\t%0,%2,%3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_insn "@pred_sbc<mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"         "=&vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"      "  rK")
	     (match_operand 6 "const_int_operand"          "   i")
	     (match_operand 7 "const_int_operand"          "   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_QHS
	     [(minus:VI_QHS
	        (match_operand:VI_QHS 2 "register_operand" "  vr")
	        (vec_duplicate:VI_QHS
	          (match_operand:<VEL> 3 "reg_or_0_operand" " rJ")))
	      (match_operand:<VM> 4 "register_operand"      " vm")] UNSPEC_VSBC)
	  (match_operand:VI_QHS 1 "vector_merge_operand"   " 0vu")))]
  "TARGET_VECTOR"
  "vsbc.vxm\t%0,%2,%z3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_expand "@pred_adc<mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	     [(plus:VI_D
	        (vec_duplicate:VI_D
	          (match_operand:<VEL> 3 "reg_or_int_operand"))
	        (match_operand:VI_D 2 "register_operand"))
	      (match_operand:<VM> 4 "register_operand")] UNSPEC_VADC)
	  (match_operand:VI_D 1 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::simm5_p (operands[3]))
      operands[3] = force_reg (<VEL>mode, operands[3]);
    else if (!TARGET_64BIT)
      {
        rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[3]))
	  operands[3] = gen_rtx_SIGN_EXTEND (<VEL>mode,
		force_reg (Pmode, operands[3]));
        else
  	  {
  	    if (CONST_INT_P (operands[3]))
  	      operands[3] = force_reg (<VEL>mode, operands[3]);

  	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
  	  				    v, operands[3], operands[5],
  	  				    <VM>mode);
  	    emit_insn (gen_pred_adc<mode> (operands[0], operands[1], operands[2],
  	  				   v, operands[4], operands[5],
  	  				   operands[6], operands[7]));
  	    DONE;
  	  }
      }
    else
      operands[3] = force_reg (<VEL>mode, operands[3]);
  })

(define_insn "*pred_adc<mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"           "=&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"       " rK")
	     (match_operand 6 "const_int_operand"           "  i")
	     (match_operand 7 "const_int_operand"           "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	     [(plus:VI_D
	        (vec_duplicate:VI_D
	          (match_operand:<VEL> 3 "register_operand" "  r"))
	        (match_operand:VI_D 2 "register_operand"    " vr"))
	      (match_operand:<VM> 4 "register_operand"      " vm")] UNSPEC_VADC)
	  (match_operand:VI_D 1 "vector_merge_operand"      "0vu")))]
  "TARGET_VECTOR"
  "vadc.vxm\t%0,%2,%3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_insn "*pred_adc<mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"                "=&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"            " rK")
	     (match_operand 6 "const_int_operand"                "  i")
	     (match_operand 7 "const_int_operand"                "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	     [(plus:VI_D
	        (vec_duplicate:VI_D
	          (sign_extend:<VEL>
	            (match_operand:<VSUBEL> 3 "register_operand" "  r")))
	        (match_operand:VI_D 2 "register_operand"         " vr"))
	      (match_operand:<VM> 4 "register_operand"           " vm")] UNSPEC_VADC)
	  (match_operand:VI_D 1 "vector_merge_operand"           "0vu")))]
  "TARGET_VECTOR"
  "vadc.vxm\t%0,%2,%3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_expand "@pred_sbc<mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	     [(minus:VI_D
	        (match_operand:VI_D 2 "register_operand")
	        (vec_duplicate:VI_D
	          (match_operand:<VEL> 3 "reg_or_int_operand")))
	      (match_operand:<VM> 4 "register_operand")] UNSPEC_VSBC)
	  (match_operand:VI_D 1 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (!TARGET_64BIT)
      {
        rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[3]))
	  {
	    if (!rtx_equal_p (operands[3], const0_rtx))
	      operands[3] = force_reg (Pmode, operands[3]);
	    operands[3] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[3]);
	  }
        else
  	  {
  	    if (CONST_INT_P (operands[3]))
  	      operands[3] = force_reg (<VEL>mode, operands[3]);

  	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
  	  				    v, operands[3], operands[5],
  	  				    <VM>mode);
  	    emit_insn (gen_pred_sbc<mode> (operands[0], operands[1], operands[2],
  	  				   v, operands[4], operands[5],
  	  				   operands[6], operands[7]));
  	    DONE;
  	  }
      }
    else
      {
	if (!rtx_equal_p (operands[3], const0_rtx))
	  operands[3] = force_reg (<VEL>mode, operands[3]);
      }
  })

(define_insn "*pred_sbc<mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"           "=&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"       " rK")
	     (match_operand 6 "const_int_operand"           "  i")
	     (match_operand 7 "const_int_operand"           "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	     [(minus:VI_D
	        (match_operand:VI_D 2 "register_operand"    " vr")
	        (vec_duplicate:VI_D
	          (match_operand:<VEL> 3 "reg_or_0_operand" " rJ")))
	      (match_operand:<VM> 4 "register_operand"      " vm")] UNSPEC_VSBC)
	  (match_operand:VI_D 1 "vector_merge_operand"      "0vu")))]
  "TARGET_VECTOR"
  "vsbc.vxm\t%0,%2,%z3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_insn "*pred_sbc<mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"                "=&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand 5 "vector_length_operand"            " rK")
	     (match_operand 6 "const_int_operand"                "  i")
	     (match_operand 7 "const_int_operand"                "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	     [(minus:VI_D
	        (match_operand:VI_D 2 "register_operand"         " vr")
	        (vec_duplicate:VI_D
	          (sign_extend:<VEL>
	            (match_operand:<VSUBEL> 3 "reg_or_0_operand" " rJ"))))
	      (match_operand:<VM> 4 "register_operand"           " vm")] UNSPEC_VSBC)
	  (match_operand:VI_D 1 "vector_merge_operand"           "0vu")))]
  "TARGET_VECTOR"
  "vsbc.vxm\t%0,%2,%z3,%4"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

(define_insn "@pred_madc<mode>"
  [(set (match_operand:<VM> 0 "register_operand"        "=&vr, &vr")
	(unspec:<VM>
	   [(plus:VI
	     (match_operand:VI 1 "register_operand"     "  vr,  vr")
	     (match_operand:VI 2 "vector_arith_operand" "  vr,  vi"))
	    (match_operand:<VM> 3 "register_operand"    "  vm,  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand" "  rK,  rK")
	       (match_operand 5 "const_int_operand"     "   i,   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_VECTOR"
  "vmadc.v%o2m\t%0,%1,%v2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_insn "@pred_msbc<mode>"
  [(set (match_operand:<VM> 0 "register_operand"        "=&vr")
	(unspec:<VM>
	   [(minus:VI
	     (match_operand:VI 1 "register_operand"     "  vr")
	     (match_operand:VI 2 "register_operand"     "  vr"))
	    (match_operand:<VM> 3 "register_operand"    "  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand" "  rK")
	       (match_operand 5 "const_int_operand"     "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_VECTOR"
  "vmsbc.vvm\t%0,%1,%2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_insn "@pred_madc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "register_operand" "   r"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (match_operand:<VM> 3 "register_operand"     "  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  "  rK")
	       (match_operand 5 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_VECTOR"
  "vmadc.vxm\t%0,%1,%2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_insn "@pred_msbc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "reg_or_0_operand" "  rJ"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (match_operand:<VM> 3 "register_operand"     "  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  "  rK")
	       (match_operand 5 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_VECTOR"
  "vmsbc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_expand "@pred_madc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (match_operand:<VM> 3 "register_operand")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand")
	       (match_operand 5 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::simm5_p (operands[2]))
      operands[2] = force_reg (<VEL>mode, operands[2]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[2]))
	  operands[2] = gen_rtx_SIGN_EXTEND (<VEL>mode,
		force_reg (Pmode, operands[2]));
	else
	  {
	    if (CONST_INT_P (operands[2]))
	      operands[2] = force_reg (<VEL>mode, operands[2]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
					    v, operands[2], operands[5],
					    <VM>mode);
	    emit_insn (gen_pred_madc<mode> (operands[0], operands[1], v, operands[3],
					    operands[4], operands[5]));
	    DONE;
	  }
      }
    else
      operands[2] = force_reg (<VEL>mode, operands[2]);
  })

(define_insn "*pred_madc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "register_operand" "   r"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (match_operand:<VM> 3 "register_operand"     "  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  "  rK")
	       (match_operand 5 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_VECTOR"
  "vmadc.vxm\t%0,%1,%2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_insn "*pred_madc<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"             "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "register_operand" "   r")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (match_operand:<VM> 3 "register_operand"          "  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"       "  rK")
	       (match_operand 5 "const_int_operand"           "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_VECTOR"
  "vmadc.vxm\t%0,%1,%2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_expand "@pred_msbc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (match_operand:<VM> 3 "register_operand")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand")
	       (match_operand 5 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_VECTOR"
  {
    if (!TARGET_64BIT)
      {
        rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[2]))
	  {
	    if (!rtx_equal_p (operands[2], const0_rtx))
	      operands[2] = force_reg (Pmode, operands[2]);
	    operands[2] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[2]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[2]))
	      operands[2] = force_reg (<VEL>mode, operands[2]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
					    v, operands[2], operands[5],
					    <VM>mode);
	    emit_insn (gen_pred_msbc<mode> (operands[0], operands[1], v, operands[3],
					    operands[4], operands[5]));
	    DONE;
	  }
      }
    else
      {
	if (!rtx_equal_p (operands[2], const0_rtx))
	  operands[2] = force_reg (<VEL>mode, operands[2]);
      }
  })

(define_insn "*pred_msbc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_0_operand" "  rJ"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (match_operand:<VM> 3 "register_operand"     "  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  "  rK")
	       (match_operand 5 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_VECTOR"
  "vmsbc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_insn "*pred_msbc<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"             "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "reg_or_0_operand" "  rJ")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (match_operand:<VM> 3 "register_operand"          "  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"       "  rK")
	       (match_operand 5 "const_int_operand"           "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_VECTOR"
  "vmsbc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])

(define_insn "@pred_madc<mode>_overflow"
  [(set (match_operand:<VM> 0 "register_operand"        "=&vr, &vr")
	(unspec:<VM>
	   [(plus:VI
	     (match_operand:VI 1 "register_operand"     "  vr,  vr")
	     (match_operand:VI 2 "vector_arith_operand" "  vr,  vi"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand" "  rK,  rK")
	       (match_operand 4 "const_int_operand"     "   i,   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmadc.v%o2\t%0,%1,%v2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

(define_insn "@pred_msbc<mode>_overflow"
  [(set (match_operand:<VM> 0 "register_operand"        "=&vr")
	(unspec:<VM>
	   [(minus:VI
	     (match_operand:VI 1 "register_operand"     "  vr")
	     (match_operand:VI 2 "register_operand"     "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand" "  rK")
	       (match_operand 4 "const_int_operand"     "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmsbc.vv\t%0,%1,%2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

(define_insn "@pred_madc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "register_operand" "   r"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  "  rK")
	       (match_operand 4 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmadc.vx\t%0,%1,%2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

(define_insn "@pred_msbc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "reg_or_0_operand" "  rJ"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  "  rK")
	       (match_operand 4 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmsbc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

(define_expand "@pred_madc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand")
	       (match_operand 4 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  {
    if (riscv_vector::simm5_p (operands[2]))
      operands[2] = force_reg (<VEL>mode, operands[2]);
    else if (!TARGET_64BIT)
      {
        rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[2]))
	  operands[2] = gen_rtx_SIGN_EXTEND (<VEL>mode,
		force_reg (Pmode, operands[2]));
	else
	  {
	    if (CONST_INT_P (operands[2]))
	      operands[2] = force_reg (<VEL>mode, operands[2]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
					    v, operands[2], operands[3],
					    <VM>mode);
	    emit_insn (gen_pred_madc<mode>_overflow (operands[0], operands[1],
						     v, operands[3], operands[4]));
	    DONE;
	  }
      }
    else
      operands[2] = force_reg (<VEL>mode, operands[2]);
  })

(define_insn "*pred_madc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "register_operand" "   r"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  "  rK")
	       (match_operand 4 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmadc.vx\t%0,%1,%2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

(define_insn "*pred_madc<mode>_overflow_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"             "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "register_operand" "   r")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"       "  rK")
	       (match_operand 4 "const_int_operand"           "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmadc.vx\t%0,%1,%2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

(define_expand "@pred_msbc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand")
	       (match_operand 4 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  {
    if (!TARGET_64BIT)
      {
        rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[2]))
	  {
	    if (!rtx_equal_p (operands[2], const0_rtx))
	      operands[2] = force_reg (Pmode, operands[2]);
	    operands[2] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[2]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[2]))
	      operands[2] = force_reg (<VEL>mode, operands[2]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
					    v, operands[2], operands[3],
					    <VM>mode);
	    emit_insn (gen_pred_msbc<mode>_overflow (operands[0], operands[1],
						     v, operands[3], operands[4]));
	    DONE;
	  }
      }
    else
      {
	if (!rtx_equal_p (operands[2], const0_rtx))
	  operands[2] = force_reg (<VEL>mode, operands[2]);
      }
  })

(define_insn "*pred_msbc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_0_operand" "  rJ"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  "  rK")
	       (match_operand 4 "const_int_operand"      "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmsbc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

(define_insn "*pred_msbc<mode>_overflow_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"             "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "reg_or_0_operand" "  rJ")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"       "  rK")
	       (match_operand 4 "const_int_operand"           "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_VECTOR"
  "vmsbc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[4])"))])

;; -------------------------------------------------------------------------------
;; ---- Predicated integer unary operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - vneg.v/vnot.v
;; -------------------------------------------------------------------------------

(define_insn "@pred_<optab><mode>"
  [(set (match_operand:VI 0 "register_operand"           "=vd, vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 4 "vector_length_operand"    " rK, rK")
	     (match_operand 5 "const_int_operand"        "  i,  i")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_int_unop:VI
	    (match_operand:VI 3 "register_operand"       " vr, vr"))
	  (match_operand:VI 2 "vector_merge_operand"     "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<insn>.v\t%0,%3%p1"
  [(set_attr "type" "vialu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

;; -------------------------------------------------------------------------------
;; ---- Predicated integer widening binary operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 11.2 Vector Widening Integer Add/Subtract
;; - 11.3 Vector Integer Extension
;; - 11.12 Vector Widening Integer Multiply Instructions
;; -------------------------------------------------------------------------------

;; Vector Double-Widening Sign-extend and Zero-extend.
(define_insn "@pred_<optab><mode>_vf2"
  [(set (match_operand:VWEXTI 0 "register_operand"            "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"         "vmWc1")
	     (match_operand 4 "vector_length_operand"            "   rK")
	     (match_operand 5 "const_int_operand"                "    i")
	     (match_operand 6 "const_int_operand"                "    i")
	     (match_operand 7 "const_int_operand"                "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_extend:VWEXTI
	    (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	  (match_operand:VWEXTI 2 "vector_merge_operand"         "  0vu")))]
  "TARGET_VECTOR"
  "v<sz>ext.vf2\t%0,%3%p1"
  [(set_attr "type" "vext")
   (set_attr "mode" "<MODE>")])

;; Vector Quad-Widening Sign-extend and Zero-extend.
(define_insn "@pred_<optab><mode>_vf4"
  [(set (match_operand:VQEXTI 0 "register_operand"          "=&vr")
	(if_then_else:VQEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1")
	     (match_operand 4 "vector_length_operand"          "   rK")
	     (match_operand 5 "const_int_operand"              "    i")
	     (match_operand 6 "const_int_operand"              "    i")
	     (match_operand 7 "const_int_operand"              "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_extend:VQEXTI
	    (match_operand:<V_QUAD_TRUNC> 3 "register_operand" "   vr"))
	  (match_operand:VQEXTI 2 "vector_merge_operand"       "  0vu")))]
  "TARGET_VECTOR"
  "v<sz>ext.vf4\t%0,%3%p1"
  [(set_attr "type" "vext")
   (set_attr "mode" "<MODE>")])

;; Vector Oct-Widening Sign-extend and Zero-extend.
(define_insn "@pred_<optab><mode>_vf8"
  [(set (match_operand:VOEXTI 0 "register_operand"         "=&vr")
	(if_then_else:VOEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 4 "vector_length_operand"         "   rK")
	     (match_operand 5 "const_int_operand"             "    i")
	     (match_operand 6 "const_int_operand"             "    i")
	     (match_operand 7 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_extend:VOEXTI
	    (match_operand:<V_OCT_TRUNC> 3 "register_operand" "   vr"))
	  (match_operand:VOEXTI 2 "vector_merge_operand"      "  0vu")))]
  "TARGET_VECTOR"
  "v<sz>ext.vf8\t%0,%3%p1"
  [(set_attr "type" "vext")
   (set_attr "mode" "<MODE>")])

;; Vector Widening Add/Subtract/Multiply.
(define_insn "@pred_dual_widen_<any_widen_binop:optab><any_extend:su><mode>"
  [(set (match_operand:VWEXTI 0 "register_operand"                  "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK")
	     (match_operand 6 "const_int_operand"                  "    i")
	     (match_operand 7 "const_int_operand"                  "    i")
	     (match_operand 8 "const_int_operand"                  "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_widen_binop:VWEXTI
	    (any_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	    (any_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	  (match_operand:VWEXTI 2 "vector_merge_operand"           "  0vu")))]
  "TARGET_VECTOR"
  "vw<any_widen_binop:insn><any_extend:u>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "vi<widen_binop_insn_type>")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_dual_widen_<any_widen_binop:optab><any_extend:su><mode>_scalar"
  [(set (match_operand:VWEXTI 0 "register_operand"                  "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK")
	     (match_operand 6 "const_int_operand"                  "    i")
	     (match_operand 7 "const_int_operand"                  "    i")
	     (match_operand 8 "const_int_operand"                  "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_widen_binop:VWEXTI
	    (any_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	    (any_extend:VWEXTI
	      (vec_duplicate:<V_DOUBLE_TRUNC>
		(match_operand:<VSUBEL> 4 "reg_or_0_operand"       "   rJ"))))
	  (match_operand:VWEXTI 2 "vector_merge_operand"           "  0vu")))]
  "TARGET_VECTOR"
  "vw<any_widen_binop:insn><any_extend:u>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "vi<widen_binop_insn_type>")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_single_widen_<plus_minus:optab><any_extend:su><mode>"
  [(set (match_operand:VWEXTI 0 "register_operand"                  "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK")
	     (match_operand 6 "const_int_operand"                  "    i")
	     (match_operand 7 "const_int_operand"                  "    i")
	     (match_operand 8 "const_int_operand"                  "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VWEXTI
	    (match_operand:VWEXTI 3 "register_operand"             "   vr")
	    (any_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	  (match_operand:VWEXTI 2 "vector_merge_operand"           "  0vu")))]
  "TARGET_VECTOR"
  "vw<plus_minus:insn><any_extend:u>.wv\t%0,%3,%4%p1"
  [(set_attr "type" "vi<widen_binop_insn_type>")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_single_widen_<plus_minus:optab><any_extend:su><mode>_scalar"
  [(set (match_operand:VWEXTI 0 "register_operand"                  "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK")
	     (match_operand 6 "const_int_operand"                  "    i")
	     (match_operand 7 "const_int_operand"                  "    i")
	     (match_operand 8 "const_int_operand"                  "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VWEXTI
	    (match_operand:VWEXTI 3 "register_operand"             "   vr")
	    (any_extend:VWEXTI
	      (vec_duplicate:<V_DOUBLE_TRUNC>
		(match_operand:<VSUBEL> 4 "reg_or_0_operand"       "   rJ"))))
	  (match_operand:VWEXTI 2 "vector_merge_operand"           "  0vu")))]
  "TARGET_VECTOR"
  "vw<plus_minus:insn><any_extend:u>.wx\t%0,%3,%z4%p1"
  [(set_attr "type" "vi<widen_binop_insn_type>")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_widen_mulsu<mode>"
  [(set (match_operand:VWEXTI 0 "register_operand"                  "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK")
	     (match_operand 6 "const_int_operand"                  "    i")
	     (match_operand 7 "const_int_operand"                  "    i")
	     (match_operand 8 "const_int_operand"                  "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (mult:VWEXTI
	    (sign_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	    (zero_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	  (match_operand:VWEXTI 2 "vector_merge_operand"           "  0vu")))]
  "TARGET_VECTOR"
  "vwmulsu.vv\t%0,%3,%4%p1"
  [(set_attr "type" "viwmul")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_widen_mulsu<mode>_scalar"
  [(set (match_operand:VWEXTI 0 "register_operand"                  "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK")
	     (match_operand 6 "const_int_operand"                  "    i")
	     (match_operand 7 "const_int_operand"                  "    i")
	     (match_operand 8 "const_int_operand"                  "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (mult:VWEXTI
	    (sign_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	    (zero_extend:VWEXTI
	      (vec_duplicate:<V_DOUBLE_TRUNC>
		(match_operand:<VSUBEL> 4 "reg_or_0_operand"       "   rJ"))))
	  (match_operand:VWEXTI 2 "vector_merge_operand"           "  0vu")))]
  "TARGET_VECTOR"
  "vwmulsu.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "viwmul")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

;; vwcvt<u>.x.x.v
(define_insn "@pred_<optab><mode>"
  [(set (match_operand:VWEXTI 0 "register_operand"                  "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1")
	     (match_operand 4 "vector_length_operand"              "   rK")
	     (match_operand 5 "const_int_operand"                  "    i")
	     (match_operand 6 "const_int_operand"                  "    i")
	     (match_operand 7 "const_int_operand"                  "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus:VWEXTI
	    (any_extend:VWEXTI
	      (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	    (vec_duplicate:VWEXTI
	      (reg:<VEL> X0_REGNUM)))
	  (match_operand:VWEXTI 2 "vector_merge_operand"           "  0vu")))]
  "TARGET_VECTOR"
  "vwcvt<u>.x.x.v\t%0,%3%p1"
  [(set_attr "type" "viwalu")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

;; -------------------------------------------------------------------------------
;; ---- Predicated integer Narrowing operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 11.7 Vector Narrowing Integer Right Shift Instructions
;; -------------------------------------------------------------------------------

;; The destination EEW is smaller than the source EEW and the overlap is in the
;; lowest-numbered part of the source register group
;; e.g, when LMUL = 1, vnsrl.wi v0,v0,3 is legal but a destination of v1 is not.
(define_insn "@pred_narrow_<optab><mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=vd, vr,  &vr, vd, vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,Wc1,vmWc1, vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK, rK,   rK, rK, rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 8 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (truncate:<V_DOUBLE_TRUNC>
	    (any_shiftrt:VWEXTI
	     (match_operand:VWEXTI 3 "register_operand"                "  0,  0,   vr,  0,  0,   vr")
	     (match_operand:<V_DOUBLE_TRUNC> 4 "vector_shift_operand"  " vr, vr,   vr, vk, vk,   vk")))
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     "0vu,0vu,  0vu,0vu,0vu,  0vu")))]
  "TARGET_VECTOR"
  "vn<insn>.w%o4\t%0,%3,%v4%p1"
  [(set_attr "type" "vnshift")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_narrow_<optab><mode>_scalar"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=vd, vr,  &vr, vd, vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,Wc1,vmWc1, vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK, rK,   rK, rK, rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 8 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (truncate:<V_DOUBLE_TRUNC>
	    (any_shiftrt:VWEXTI
	     (match_operand:VWEXTI 3 "register_operand"                "  0,  0,   vr,  0,  0,   vr")
	     (match_operand 4 "pmode_reg_or_uimm5_operand"             "  r,  r,    r,  K,  K,    K")))
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     "0vu,0vu,  0vu,0vu,0vu,  0vu")))]
  "TARGET_VECTOR"
  "vn<insn>.w%o4\t%0,%3,%4%p1"
  [(set_attr "type" "vnshift")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

;; vncvt.x.x.w
(define_insn "@pred_trunc<mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=vd, vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,Wc1,vmWc1")
	     (match_operand 4 "vector_length_operand"                  " rK, rK,   rK")
	     (match_operand 5 "const_int_operand"                      "  i,  i,    i")
	     (match_operand 6 "const_int_operand"                      "  i,  i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (truncate:<V_DOUBLE_TRUNC>
	    (match_operand:VWEXTI 3 "register_operand"                 "  0,  0,   vr"))
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     "0vu,0vu,  0vu")))]
  "TARGET_VECTOR"
  "vncvt.x.x.w\t%0,%3%p1"
  [(set_attr "type" "vnshift")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[7])"))])

;; -------------------------------------------------------------------------------
;; ---- Predicated fixed-point operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 12.2 Vector Single-Width Aaveraging Add and Subtract
;; - 12.3 Vector Single-Width Fractional Multiply with Rounding and Saturation
;; - 12.5 Vector Single-Width Scaling Shift Instructions
;; - 12.6 Vector Narrowing Fixed-Point Clip Instructions
;; -------------------------------------------------------------------------------

(define_insn "@pred_<sat_op><mode>"
  [(set (match_operand:VI 0 "register_operand"           "=vd, vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI
	    [(match_operand:VI 3 "register_operand"      " vr, vr")
	     (match_operand:VI 4 "register_operand"      " vr, vr")] VSAT_OP)
	  (match_operand:VI 2 "vector_merge_operand"     "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<sat_op>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "<sat_insn_type>")
   (set_attr "mode" "<MODE>")])

;; Handle GET_MODE_INNER (mode) = QImode, HImode, SImode.
(define_insn "@pred_<sat_op><mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand"       "=vd, vr")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_QHS
	    [(match_operand:VI_QHS 3 "register_operand"  " vr, vr")
	     (match_operand:<VEL> 4 "reg_or_0_operand"   " rJ, rJ")] VSAT_ARITH_OP)
	  (match_operand:VI_QHS 2 "vector_merge_operand" "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<sat_op>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<sat_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_<sat_op><mode>_scalar"
  [(set (match_operand:VI 0 "register_operand"             "=vd, vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"   " vm,Wc1")
	     (match_operand 5 "vector_length_operand"      " rK, rK")
	     (match_operand 6 "const_int_operand"          "  i,  i")
	     (match_operand 7 "const_int_operand"          "  i,  i")
	     (match_operand 8 "const_int_operand"          "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI
	    [(match_operand:VI 3 "register_operand"        " vr, vr")
	     (match_operand 4 "pmode_reg_or_uimm5_operand" " rK, rK")] VSAT_SHIFT_OP)
	  (match_operand:VI 2 "vector_merge_operand"       "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<sat_op>.v%o4\t%0,%3,%4%p1"
  [(set_attr "type" "<sat_insn_type>")
   (set_attr "mode" "<MODE>")])

;; Handle GET_MODE_INNER (mode) = DImode. We need to split them since
;; we need to deal with SEW = 64 in RV32 system.
(define_expand "@pred_<sat_op><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 5 "vector_length_operand")
	     (match_operand 6 "const_int_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	    [(match_operand:VI_D 3 "register_operand")
	     (match_operand:<VEL> 4 "reg_or_int_operand")] VSAT_ARITH_OP)
	  (match_operand:VI_D 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[4]))
	  {
	    if (!rtx_equal_p (operands[4], const0_rtx))
	      operands[4] = force_reg (Pmode, operands[4]);
	    operands[4] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[4]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[4]))
	      operands[4] = force_reg (<VEL>mode, operands[4]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[4], operands[5], <VM>mode);
	    emit_insn (gen_pred_<sat_op><mode> (operands[0], operands[1],
			operands[2], operands[3], v, operands[5],
			operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      {
	if (!rtx_equal_p (operands[4], const0_rtx))
	  operands[4] = force_reg (<VEL>mode, operands[4]);
      }
  })

(define_insn "*pred_<sat_op><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand"         "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK")
	     (match_operand 6 "const_int_operand"        "  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	    [(match_operand:VI_D 3 "register_operand"    " vr, vr")
	     (match_operand:<VEL> 4 "reg_or_0_operand"   " rJ, rJ")] VSAT_ARITH_OP)
	  (match_operand:VI_D 2 "vector_merge_operand"   "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<sat_op>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<sat_insn_type>")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<sat_op><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"            "=vd, vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"    " vm,Wc1")
	     (match_operand 5 "vector_length_operand"       " rK, rK")
	     (match_operand 6 "const_int_operand"           "  i,  i")
	     (match_operand 7 "const_int_operand"           "  i,  i")
	     (match_operand 8 "const_int_operand"           "  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VI_D
	    [(match_operand:VI_D 3 "register_operand"       " vr, vr")
	     (sign_extend:<VEL>
	       (match_operand:<VSUBEL> 4 "reg_or_0_operand" " rJ, rJ"))] VSAT_ARITH_OP)
	  (match_operand:VI_D 2 "vector_merge_operand"      "0vu,0vu")))]
  "TARGET_VECTOR"
  "v<sat_op>.vx\t%0,%3,%z4%p1"
  [(set_attr "type" "<sat_insn_type>")
   (set_attr "mode" "<MODE>")])

;; CLIP
(define_insn "@pred_narrow_clip<v_su><mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=vd, vr,  &vr, vd, vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,Wc1,vmWc1, vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK, rK,   rK, rK, rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 8 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<V_DOUBLE_TRUNC>
	    [(match_operand:VWEXTI 3 "register_operand"                 "  0,  0,   vr,  0,  0,   vr")
	     (match_operand:<V_DOUBLE_TRUNC> 4 "vector_shift_operand"   " vr, vr,   vr, vk, vk,   vk")] VNCLIP)
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"      "0vu,0vu,  0vu,0vu,0vu,  0vu")))]
  "TARGET_VECTOR"
  "vnclip<v_su>.w%o4\t%0,%3,%v4%p1"
  [(set_attr "type" "vnclip")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_narrow_clip<v_su><mode>_scalar"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=vd, vr,  &vr, vd, vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,Wc1,vmWc1, vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK, rK,   rK, rK, rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (match_operand 8 "const_int_operand"                      "  i,  i,    i,  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<V_DOUBLE_TRUNC>
	    [(match_operand:VWEXTI 3 "register_operand"                "  0,  0,   vr,  0,  0,   vr")
	     (match_operand 4 "pmode_reg_or_uimm5_operand"             "  r,  r,    r,  K,  K,    K")] VNCLIP)
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     "0vu,0vu,  0vu,0vu,0vu,  0vu")))]
  "TARGET_VECTOR"
  "vnclip<v_su>.w%o4\t%0,%3,%4%p1"
  [(set_attr "type" "vnclip")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated comparison operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 11.8 Vector Integer Comparision Instructions
;; -------------------------------------------------------------------------------

(define_expand "@pred_cmp<mode>"
  [(set (match_operand:<VM> 0 "register_operand")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_ltge_operator"
	     [(match_operand:VI 4 "register_operand")
	      (match_operand:VI 5 "vector_arith_operand")])
	  (match_operand:<VM> 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {})

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_cmp<mode>"
  [(set (match_operand:<VM> 0 "register_operand"                "=vr,   vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_ltge_operator"
	     [(match_operand:VI 4 "register_operand"          "   vr,   vr")
	      (match_operand:VI 5 "vector_arith_operand"      "   vr,   vi")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu,  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_cmp<mode>_narrow"
  [(set (match_operand:<VM> 0 "register_operand"              "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_ltge_operator"
	     [(match_operand:VI 4 "register_operand"          "   vr,   vr")
	      (match_operand:VI 5 "vector_arith_operand"      "   vr,   vi")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu,  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_ltge<mode>"
  [(set (match_operand:<VM> 0 "register_operand")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "ltge_operator"
	     [(match_operand:VI 4 "register_operand")
	      (match_operand:VI 5 "vector_neg_arith_operand")])
	  (match_operand:<VM> 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {})

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_ltge<mode>"
  [(set (match_operand:<VM> 0 "register_operand"                "=vr,   vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "ltge_operator"
	     [(match_operand:VI 4 "register_operand"          "   vr,   vr")
	      (match_operand:VI 5 "vector_neg_arith_operand"  "   vr,   vj")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu,  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_ltge<mode>_narrow"
  [(set (match_operand:<VM> 0 "register_operand"              "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "ltge_operator"
	     [(match_operand:VI 4 "register_operand"          "   vr,   vr")
	      (match_operand:VI 5 "vector_neg_arith_operand"  "   vr,   vj")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu,  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_cmp<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_QHS 4 "register_operand")
	      (vec_duplicate:VI_QHS
	        (match_operand:<VEL> 5 "register_operand"))])
	  (match_operand:<VM> 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {})

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_cmp<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_QHS 4 "register_operand"      "   vr")
	      (vec_duplicate:VI_QHS
	        (match_operand:<VEL> 5 "register_operand"     "    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_cmp<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"               "=&vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_QHS 4 "register_operand"      "   vr")
	      (vec_duplicate:VI_QHS
	        (match_operand:<VEL> 5 "register_operand"     "    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_eqne<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_QHS
	        (match_operand:<VEL> 5 "register_operand"))
	      (match_operand:VI_QHS 4 "register_operand")])
	  (match_operand:<VM> 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {})

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_eqne<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_QHS
	        (match_operand:<VEL> 5 "register_operand"     "    r"))
	      (match_operand:VI_QHS 4 "register_operand"      "   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_eqne<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"               "=&vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_QHS
	        (match_operand:<VEL> 5 "register_operand"     "    r"))
	      (match_operand:VI_QHS 4 "register_operand"      "   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; Handle GET_MODE_INNER (mode) = DImode. We need to split them since
;; we need to deal with SEW = 64 in RV32 system.
(define_expand "@pred_cmp<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_D 4 "register_operand")
	      (vec_duplicate:VI_D
	        (match_operand:<VEL> 5 "reg_or_int_operand"))])
	  (match_operand:<VM> 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    enum rtx_code code = GET_CODE (operands[3]);

    if (riscv_vector::has_vi_variant_p (code, operands[5]))
      operands[5] = force_reg (<VEL>mode, operands[5]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[5]))
	  {
	    if (!rtx_equal_p (operands[5], const0_rtx))
	      operands[5] = force_reg (Pmode, operands[5]);
	    operands[5] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[5]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[5]))
	      operands[5] = force_reg (<VEL>mode, operands[5]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[5], operands[6], <VM>mode);
	    if (code == LT || code == LTU)
		emit_insn (gen_pred_ltge<mode> (operands[0], operands[1],
			   operands[2], operands[3], operands[4], v,
			   operands[6], operands[7], operands[8]));
	    else
		emit_insn (gen_pred_cmp<mode> (operands[0], operands[1],
			   operands[2], operands[3], operands[4], v,
			   operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      operands[5] = force_reg (<VEL>mode, operands[5]);
  })

(define_expand "@pred_eqne<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_D
	        (match_operand:<VEL> 5 "reg_or_int_operand"))
	      (match_operand:VI_D 4 "register_operand")])
	  (match_operand:<VM> 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
  {
    enum rtx_code code = GET_CODE (operands[3]);

    if (riscv_vector::has_vi_variant_p (code, operands[5]))
      operands[5] = force_reg (<VEL>mode, operands[5]);
    else if (!TARGET_64BIT)
      {
	rtx v = gen_reg_rtx (<MODE>mode);

	if (riscv_vector::simm32_p (operands[5]))
	  {
	    if (!rtx_equal_p (operands[5], const0_rtx))
	      operands[5] = force_reg (Pmode, operands[5]);
	    operands[5] = gen_rtx_SIGN_EXTEND (<VEL>mode, operands[5]);
	  }
	else
	  {
	    if (CONST_INT_P (operands[5]))
	      operands[5] = force_reg (<VEL>mode, operands[5]);

	    riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[5], operands[6], <VM>mode);
	    emit_insn (gen_pred_cmp<mode> (operands[0], operands[1],
		       operands[2], operands[3], operands[4], v,
		       operands[6], operands[7], operands[8]));
	    DONE;
	  }
      }
    else
      operands[5] = force_reg (<VEL>mode, operands[5]);
  })

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_cmp<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_D 4 "register_operand"        "    vr")
	      (vec_duplicate:VI_D
	        (match_operand:<VEL> 5 "register_operand"     "    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_cmp<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"               "=&vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_D 4 "register_operand"        "   vr")
	      (vec_duplicate:VI_D
	        (match_operand:<VEL> 5 "register_operand"     "    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_eqne<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_D
	        (match_operand:<VEL> 5 "register_operand"     "    r"))
	      (match_operand:VI_D 4 "register_operand"        "   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_eqne<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"               "=&vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK")
	     (match_operand 7 "const_int_operand"             "    i")
	     (match_operand 8 "const_int_operand"             "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_D
	        (match_operand:<VEL> 5 "register_operand"     "    r"))
	      (match_operand:VI_D 4 "register_operand"        "   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_cmp<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                 "=vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK")
	     (match_operand 7 "const_int_operand"              "    i")
	     (match_operand 8 "const_int_operand"              "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_D 4 "register_operand"         "   vr")
	      (vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r")))])
	  (match_operand:<VM> 2 "vector_merge_operand"         "  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_cmp<mode>_extended_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK")
	     (match_operand 7 "const_int_operand"              "    i")
	     (match_operand 8 "const_int_operand"              "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:VI_D 4 "register_operand"         "   vr")
	      (vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r")))])
	  (match_operand:<VM> 2 "vector_merge_operand"         "  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_eqne<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                 "=vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK")
	     (match_operand 7 "const_int_operand"              "    i")
	     (match_operand 8 "const_int_operand"              "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r")))
	      (match_operand:VI_D 4 "register_operand"         "   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"         "  0vu")))]
  "TARGET_VECTOR && known_le (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_eqne<mode>_extended_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK")
	     (match_operand 7 "const_int_operand"              "    i")
	     (match_operand 8 "const_int_operand"              "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r")))
	      (match_operand:VI_D 4 "register_operand"         "   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"         "  0vu")))]
  "TARGET_VECTOR && known_gt (GET_MODE_SIZE (<MODE>mode), BYTES_PER_RISCV_VECTOR)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; GE, vmsge.vx/vmsgeu.vx
;;
;; unmasked va >= x
;;  - pseudoinstruction: vmsge{u}.vx vd, va, x
;;  - expansion: vmslt{u}.vx vd, va, x; vmnand.mm vd, vd, vd
;;
;; masked va >= x, vd != v0
;;  - pseudoinstruction: vmsge{u}.vx vd, va, x, v0.t
;;  - expansion: vmslt{u}.vx vd, va, x, v0.t; vmxor.mm vd, vd, v0
;;
;; masked va >= x, vd == v0
;;  - pseudoinstruction: vmsge{u}.vx vd, va, x, v0.t, vt
;;  - expansion: vmslt{u}.vx vt, va, x;  vmandn.mm vd, vd, vt
(define_expand "@pred_ge<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "ge_operator"
	     [(match_operand:VI 4 "register_operand")
	      (vec_duplicate:VI
	        (match_operand:<VEL> 5 "reg_or_int_operand"))])
	  (match_operand:<VM> 2 "vector_merge_operand")))]
  "TARGET_VECTOR"
{
  enum rtx_code code = GET_CODE (operands[3]);
  rtx undef = RVV_VUNDEF (<VM>mode);
  if (code == GEU && rtx_equal_p (operands[5], const0_rtx))
    {
      /* If vmsgeu with 0 immediate, expand it to vmset.  */
      if (satisfies_constraint_Wc1 (operands[1]))
	emit_insn (
	  gen_pred_mov (<VM>mode, operands[0], CONSTM1_RTX (<VM>mode), undef,
			CONSTM1_RTX (<VM>mode), operands[6], operands[8]));
      else
	{
	  /* If vmsgeu_mask with 0 immediate, expand it to vmor mask, maskedoff.
	   */
	  if (rtx_equal_p (operands[1], operands[2]))
	    emit_move_insn (operands[0], operands[1]);
	  else if (register_operand (operands[2], <VM>mode))
	    emit_insn (gen_pred (IOR, <VM>mode, operands[0],
				 CONSTM1_RTX (<VM>mode), undef, operands[1],
				 operands[2], operands[6], operands[8]));
	  else
	    emit_insn (gen_pred (IOR, <VM>mode, operands[0],
				 CONSTM1_RTX (<VM>mode), undef, operands[1],
				 operands[1], operands[6], operands[8]));
	}
    }
  else if (riscv_vector::neg_simm5_p (operands[5]))
    emit_insn (
      gen_pred_ltge<mode> (operands[0], operands[1], operands[2], operands[3],
			   operands[4],
			   gen_const_vec_duplicate (<MODE>mode, operands[5]),
			   operands[6], operands[7], operands[8]));
  else
    {
      if (code == GE)
	operands[3] = gen_rtx_fmt_ee (LT, <VM>mode, XEXP (operands[3], 0),
				      XEXP (operands[3], 1));
      else
	operands[3] = gen_rtx_fmt_ee (LTU, <VM>mode, XEXP (operands[3], 0),
				      XEXP (operands[3], 1));
      if (GET_MODE_BITSIZE (<VEL>mode) <= GET_MODE_BITSIZE (Pmode))
	operands[5] = force_reg (<VEL>mode, operands[5]);

      if (satisfies_constraint_Wc1 (operands[1]))
	{
	  /* unmasked va >= x
	    - pseudoinstruction: vmsge{u}.vx vd, va, x
	    - expansion: vmslt{u}.vx vd, va, x; vmnand.mm vd, vd, vd.  */
	  emit_insn (
	    gen_pred_cmp<mode>_scalar (operands[0], operands[1], operands[2],
					operands[3], operands[4], operands[5],
					operands[6], operands[7], operands[8]));
	  emit_insn (gen_pred_nand<vm> (operands[0], CONSTM1_RTX (<VM>mode),
					undef, operands[0], operands[0],
					operands[6], operands[8]));
	}
      else
	{
	  if (rtx_equal_p (operands[1], operands[2]))
	    {
	      /* masked va >= x, vd == v0
		- pseudoinstruction: vmsge{u}.vx vd, va, x, v0.t, vt
		- expansion: vmslt{u}.vx vt, va, x;  vmandn.mm vd, vd, vt.  */
	      rtx reg = gen_reg_rtx (<VM>mode);
	      emit_insn (gen_pred_cmp<mode>_scalar (
		reg, CONSTM1_RTX (<VM>mode), undef, operands[3], operands[4],
		operands[5], operands[6], operands[7], operands[8]));
	      emit_insn (
		gen_pred_andn<vm> (operands[0], CONSTM1_RTX (<VM>mode), undef,
				   operands[1], reg, operands[6], operands[8]));
	    }
	  else
	    {
	      /* masked va >= x, vd != v0
		- pseudoinstruction: vmsge{u}.vx vd, va, x, v0.t
		- expansion: vmslt{u}.vx vd, va, x, v0.t; vmxor.mm vd, vd, v0.
	      */
	      emit_insn (gen_pred_cmp<mode>_scalar (
		operands[0], operands[1], operands[2], operands[3], operands[4],
		operands[5], operands[6], operands[7], operands[8]));
	      emit_insn (gen_pred (XOR, <VM>mode, operands[0],
				   CONSTM1_RTX (<VM>mode), undef, operands[0],
				   operands[1], operands[6], operands[8]));
	    }
	}
    }
  DONE;
})

;; -------------------------------------------------------------------------------
;; ---- Predicated integer ternary operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 11.13 Vector Single-Width Integer Multiply-Add Instructions
;; -------------------------------------------------------------------------------

(define_expand "@pred_mul_<optab><mode>"
  [(set (match_operand:VI 0 "register_operand")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (match_operand 9 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (match_operand:VI 2 "register_operand")
	      (match_operand:VI 3 "register_operand"))
	    (match_operand:VI 4 "register_operand"))
	  (match_operand:VI 5 "vector_merge_operand")))]
  "TARGET_VECTOR"
{
  /* Swap the multiplication operands if the fallback value is the
     second of the two.  */
  if (rtx_equal_p (operands[3], operands[5]))
    std::swap (operands[2], operands[3]);
})

(define_insn "pred_mul_<optab><mode>_undef_merge"
  [(set (match_operand:VI 0 "register_operand"           "=vd, vr, vd, vr, ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1, vm,Wc1,vmWc1")
	     (match_operand 6 "vector_length_operand"    " rK, rK, rK, rK,   rK")
	     (match_operand 7 "const_int_operand"        "  i,  i,  i,  i,    i")
	     (match_operand 8 "const_int_operand"        "  i,  i,  i,  i,    i")
	     (match_operand 9 "const_int_operand"        "  i,  i,  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (match_operand:VI 2 "register_operand"     " %0,  0, vr, vr,   vr")
	      (match_operand:VI 3 "register_operand"     " vr, vr, vr, vr,   vr"))
	    (match_operand:VI 4 "register_operand"       " vr, vr,  0,  0,   vr"))
	  (match_operand:VI 5 "vector_undef_operand"     " vu, vu, vu, vu,   vu")))]
  "TARGET_VECTOR"
  "@
   v<madd_nmsub>.vv\t%0,%3,%4%p1
   v<madd_nmsub>.vv\t%0,%3,%4%p1
   v<macc_nmsac>.vv\t%0,%2,%3%p1
   v<macc_nmsac>.vv\t%0,%2,%3%p1
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vv\t%0,%2,%3%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<madd_nmsub><mode>"
  [(set (match_operand:VI 0 "register_operand"           "=vd, vr, ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK,   rK")
	     (match_operand 6 "const_int_operand"        "  i,  i,    i")
	     (match_operand 7 "const_int_operand"        "  i,  i,    i")
	     (match_operand 8 "const_int_operand"        "  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (match_operand:VI 2 "register_operand"     "  0,  0,   vr")
	      (match_operand:VI 3 "register_operand"     " vr, vr,   vr"))
	    (match_operand:VI 4 "register_operand"       " vr, vr,   vr"))
	  (match_dup 2)))]
  "TARGET_VECTOR"
  "@
   v<madd_nmsub>.vv\t%0,%3,%4%p1
   v<madd_nmsub>.vv\t%0,%3,%4%p1
   vmv.v.v\t%0,%2\;v<madd_nmsub>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "4")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[8])"))])

(define_insn "*pred_<macc_nmsac><mode>"
  [(set (match_operand:VI 0 "register_operand"           "=vd, vr, ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    " rK, rK,   rK")
	     (match_operand 6 "const_int_operand"        "  i,  i,    i")
	     (match_operand 7 "const_int_operand"        "  i,  i,    i")
	     (match_operand 8 "const_int_operand"        "  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (match_operand:VI 2 "register_operand"     " vr, vr,   vr")
	      (match_operand:VI 3 "register_operand"     " vr, vr,   vr"))
	    (match_operand:VI 4 "register_operand"       "  0,  0,   vr"))
	  (match_dup 4)))]
  "TARGET_VECTOR"
  "@
   v<macc_nmsac>.vv\t%0,%2,%3%p1
   v<macc_nmsac>.vv\t%0,%2,%3%p1
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vv\t%0,%2,%3%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "2")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[8])"))])

(define_insn_and_rewrite "*pred_mul_<optab><mode>"
  [(set (match_operand:VI 0 "register_operand"            "=&vr,?&vr, ?&vr, ?&vr,  ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"    "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"        "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i,    i,    i,    i")
	     (match_operand 9 "const_int_operand"        "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (match_operand:VI 2 "register_operand"     "   vr,   vr,   vi,   vr,   vr")
	      (match_operand:VI 3 "register_operand"     "   vr,   vr,   vr,   vi,   vr"))
	    (match_operand:VI 4 "vector_arith_operand"   "   vr,   vi,   vr,   vr,   vr"))
	  (match_operand:VI 5 "register_operand"         "    0,   vr,   vr,   vr,   vr")))]
  "TARGET_VECTOR
   && !rtx_equal_p (operands[2], operands[5])
   && !rtx_equal_p (operands[3], operands[5])
   && !rtx_equal_p (operands[4], operands[5])"
  "@
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vv\t%0,%2,%3%p1
   #
   #
   #
   #"
  "&& reload_completed
   && !rtx_equal_p (operands[0], operands[5])"
  {
    if (satisfies_constraint_vi (operands[3]))
      std::swap (operands[2], operands[3]);

    if (satisfies_constraint_vi (operands[2]))
      {
        emit_insn (gen_pred_merge<mode> (operands[0], RVV_VUNDEF (<MODE>mode),
                	operands[5], operands[2], operands[1], operands[6],
			operands[7], operands[9]));
        operands[5] = operands[2] = operands[0];
      }
    else
      {
        emit_insn (gen_pred_merge<mode> (operands[0], RVV_VUNDEF (<MODE>mode),
                	operands[5], operands[4], operands[1], operands[6], 
			operands[7], operands[9]));
        operands[5] = operands[4] = operands[0];
      }
  }
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_mul_<optab><mode>_scalar"
  [(set (match_operand:VI_QHS 0 "register_operand")
	(if_then_else:VI_QHS
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (match_operand 9 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI_QHS
	    (mult:VI_QHS
	      (vec_duplicate:VI_QHS
	        (match_operand:<VEL> 2 "reg_or_int_operand"))
	      (match_operand:VI_QHS 3 "register_operand"))
	    (match_operand:VI_QHS 4 "register_operand"))
	  (match_operand:VI_QHS 5 "vector_merge_operand")))]
  "TARGET_VECTOR"
{
  operands[2] = force_reg (<VEL>mode, operands[2]);
})

(define_insn "*pred_mul_<optab><mode>_undef_merge_scalar"
  [(set (match_operand:VI 0 "register_operand"             "=vd, vr, vd, vr, ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"   " vm,Wc1, vm,Wc1,vmWc1")
	     (match_operand 6 "vector_length_operand"      " rK, rK, rK, rK,   rK")
	     (match_operand 7 "const_int_operand"          "  i,  i,  i,  i,    i")
	     (match_operand 8 "const_int_operand"          "  i,  i,  i,  i,    i")
	     (match_operand 9 "const_int_operand"          "  i,  i,  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (vec_duplicate:VI
	        (match_operand:<VEL> 2 "register_operand" "   r,  r,  r,  r,    r"))
	      (match_operand:VI 3 "register_operand"       "  0,  0, vr, vr,   vr"))
	    (match_operand:VI 4 "register_operand"         " vr, vr,  0,  0,   vr"))
	  (match_operand:VI 5 "vector_undef_operand"       " vu, vu, vu, vu,   vu")))]
  "TARGET_VECTOR"
  "@
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vx\t%0,%2,%3%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<madd_nmsub><mode>_scalar"
  [(set (match_operand:VI 0 "register_operand"            "=vd, vr, ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  " vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"     " rK, rK,   rK")
	     (match_operand 6 "const_int_operand"         "  i,  i,    i")
	     (match_operand 7 "const_int_operand"         "  i,  i,    i")
	     (match_operand 8 "const_int_operand"         "  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (vec_duplicate:VI
	        (match_operand:<VEL> 2 "register_operand" "  r,  r,   vr"))
	      (match_operand:VI 3 "register_operand"      "  0,  0,   vr"))
	    (match_operand:VI 4 "register_operand"        " vr, vr,   vr"))
	  (match_dup 3)))]
  "TARGET_VECTOR"
  "@
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   vmv.v.v\t%0,%2\;v<madd_nmsub>.vx\t%0,%2,%4%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "4")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[8])"))])

(define_insn "*pred_<macc_nmsac><mode>_scalar"
  [(set (match_operand:VI 0 "register_operand"            "=vd, vr, ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  " vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"     " rK, rK,   rK")
	     (match_operand 6 "const_int_operand"         "  i,  i,    i")
	     (match_operand 7 "const_int_operand"         "  i,  i,    i")
	     (match_operand 8 "const_int_operand"         "  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (vec_duplicate:VI
	        (match_operand:<VEL> 2 "register_operand" "  r,  r,   vr"))
	      (match_operand:VI 3 "register_operand"      " vr, vr,   vr"))
	    (match_operand:VI 4 "register_operand"        "  0,  0,   vr"))
	  (match_dup 4)))]
  "TARGET_VECTOR"
  "@
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vx\t%0,%2,%3%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "2")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[8])"))])

(define_insn_and_rewrite "*pred_mul_<optab><mode>_scalar"
  [(set (match_operand:VI 0 "register_operand"            "=&vr, ?&vr, ?&vr, ?&vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"    "   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"        "    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i,    i,    i")
	     (match_operand 9 "const_int_operand"        "    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI
	    (mult:VI
	      (vec_duplicate:VI
	        (match_operand:<VEL> 2 "register_operand" "    r,    r,    r,    r"))
	      (match_operand:VI 3 "register_operand"      "   vr,   vr,   vi,   vr"))
	    (match_operand:VI 4 "vector_arith_operand"    "   vr,   vi,   vr,   vr"))
	  (match_operand:VI 5 "register_operand"          "    0,   vr,   vr,   vr")))]
  "TARGET_VECTOR
   && !rtx_equal_p (operands[3], operands[5])
   && !rtx_equal_p (operands[4], operands[5])"
  "@
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vx\t%0,%2,%3%p1
   #
   #
   #"
  "&& reload_completed
   && !rtx_equal_p (operands[0], operands[5])"
  {
    if (satisfies_constraint_vi (operands[3]))
      {
        emit_insn (gen_pred_merge<mode> (operands[0], RVV_VUNDEF (<MODE>mode),
                	operands[5], operands[3], operands[1], operands[6],
			operands[7], operands[9]));
        operands[5] = operands[3] = operands[0];
      }
    else
      {
        emit_insn (gen_pred_merge<mode> (operands[0], RVV_VUNDEF (<MODE>mode),
                	operands[5], operands[4], operands[1], operands[6],
			operands[7], operands[9]));
        operands[5] = operands[4] = operands[0];
      }
  }
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_mul_<optab><mode>_scalar"
  [(set (match_operand:VI_D 0 "register_operand")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand")
	     (match_operand 6 "vector_length_operand")
	     (match_operand 7 "const_int_operand")
	     (match_operand 8 "const_int_operand")
	     (match_operand 9 "const_int_operand")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI_D
	    (mult:VI_D
	      (vec_duplicate:VI_D
	        (match_operand:<VEL> 2 "reg_or_int_operand"))
	      (match_operand:VI_D 3 "register_operand"))
	    (match_operand:VI_D 4 "register_operand"))
	  (match_operand:VI_D 5 "vector_merge_operand")))]
  "TARGET_VECTOR"
{
  if (!TARGET_64BIT)
    {
      rtx v = gen_reg_rtx (<MODE>mode);

      if (riscv_vector::simm32_p (operands[2]))
        operands[2] = gen_rtx_SIGN_EXTEND (<VEL>mode,
      		force_reg (Pmode, operands[2]));
      else
        {
          if (CONST_INT_P (operands[2]))
            operands[2] = force_reg (<VEL>mode, operands[2]);

	  riscv_vector::emit_nonvlmax_op (code_for_pred_broadcast (<MODE>mode),
			v, operands[2], operands[6], <VM>mode);
	  emit_insn (gen_pred_mul_<optab><mode> (operands[0], operands[1],
			v, operands[3], operands[4], operands[5], operands[6],
			operands[7], operands[8], operands[9]));
	  DONE;
	}
    }
   else
    operands[2] = force_reg (<VEL>mode, operands[2]);
})

(define_insn "*pred_mul_<optab><mode>_undef_merge_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"               "=vd, vr, vd, vr, ?&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       " vm,Wc1, vm,Wc1,vmWc1")
	     (match_operand 6 "vector_length_operand"          " rK, rK, rK, rK,   rK")
	     (match_operand 7 "const_int_operand"              "  i,  i,  i,  i,    i")
	     (match_operand 8 "const_int_operand"              "  i,  i,  i,  i,    i")
	     (match_operand 9 "const_int_operand"              "  i,  i,  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI_D
	    (mult:VI_D
	      (vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 2 "register_operand" "  r,  r,  r,  r,    r")))
	      (match_operand:VI_D 3 "register_operand"         "  0,  0, vr, vr,   vr"))
	    (match_operand:VI_D 4 "register_operand"           " vr, vr,  0,  0,   vr"))
	  (match_operand:VI_D 5 "vector_undef_operand"         " vu, vu, vu, vu,   vu")))]
  "TARGET_VECTOR"
  "@
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vx\t%0,%2,%3%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_<madd_nmsub><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"               "=vd, vr, ?&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       " vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"          " rK, rK,   rK")
	     (match_operand 6 "const_int_operand"              "  i,  i,    i")
	     (match_operand 7 "const_int_operand"              "  i,  i,    i")
	     (match_operand 8 "const_int_operand"              "  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI_D
	    (mult:VI_D
	      (vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 2 "register_operand" "  r,  r,   vr")))
	      (match_operand:VI_D 3 "register_operand"         "  0,  0,   vr"))
	    (match_operand:VI_D 4 "register_operand"           " vr, vr,   vr"))
	  (match_dup 3)))]
  "TARGET_VECTOR"
  "@
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   v<madd_nmsub>.vx\t%0,%2,%4%p1
   vmv.v.v\t%0,%2\;v<madd_nmsub>.vx\t%0,%2,%4%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "4")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[8])"))])

(define_insn "*pred_<macc_nmsac><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"               "=vd, vr, ?&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       " vm,Wc1,vmWc1")
	     (match_operand 5 "vector_length_operand"          " rK, rK,   rK")
	     (match_operand 6 "const_int_operand"              "  i,  i,    i")
	     (match_operand 7 "const_int_operand"              "  i,  i,    i")
	     (match_operand 8 "const_int_operand"              "  i,  i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI_D
	    (mult:VI_D
	      (vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 2 "register_operand" "  r,  r,   vr")))
	      (match_operand:VI_D 3 "register_operand"         " vr, vr,   vr"))
	    (match_operand:VI_D 4 "register_operand"           "  0,  0,   vr"))
	  (match_dup 4)))]
  "TARGET_VECTOR"
  "@
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   v<macc_nmsac>.vx\t%0,%2,%3%p1
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vx\t%0,%2,%3%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "2")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[8])"))])

(define_insn_and_rewrite "*pred_mul_<optab><mode>_extended_scalar"
  [(set (match_operand:VI_D 0 "register_operand"                "=&vr, ?&vr, ?&vr, ?&vr")
	(if_then_else:VI_D
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"              "    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"              "    i,    i,    i,    i")
	     (match_operand 9 "const_int_operand"              "    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VI_D
	    (mult:VI_D
	      (vec_duplicate:VI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 2 "register_operand" "    r,    r,    r,    r")))
	      (match_operand:VI_D 3 "register_operand"         "   vr,   vr,   vr,   vr"))
	    (match_operand:VI_D 4 "vector_arith_operand"       "   vr,   vr,   vr,   vr"))
	  (match_operand:VI_D 5 "register_operand"             "    0,   vr,   vr,   vr")))]
  "TARGET_VECTOR
   && !rtx_equal_p (operands[3], operands[5])
   && !rtx_equal_p (operands[4], operands[5])"
  "@
   vmv.v.v\t%0,%4\;v<macc_nmsac>.vx\t%0,%2,%3%p1
   #
   #
   #"
  "&& reload_completed
   && !rtx_equal_p (operands[0], operands[5])"
  {
    if (satisfies_constraint_vi (operands[3]))
      {
        emit_insn (gen_pred_merge<mode> (operands[0], RVV_VUNDEF (<MODE>mode),
                	operands[5], operands[3], operands[1], operands[6],
			operands[7], operands[9]));
        operands[5] = operands[3] = operands[0];
      }
    else
      {
        emit_insn (gen_pred_merge<mode> (operands[0], RVV_VUNDEF (<MODE>mode),
                	operands[5], operands[4], operands[1], operands[6],
			operands[7], operands[9]));
        operands[5] = operands[4] = operands[0];
      }
  }
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated integer ternary operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 11.14 Vector Widening Integer Multiply-Add Instructions
;; -------------------------------------------------------------------------------

(define_insn "@pred_widen_mul_plus<su><mode>"
  [(set (match_operand:VWEXTI 0 "register_operand"                    "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             "vmWc1")
	     (match_operand 6 "vector_length_operand"                "   rK")
	     (match_operand 7 "const_int_operand"                    "    i")
	     (match_operand 8 "const_int_operand"                    "    i")
	     (match_operand 9 "const_int_operand"                    "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus:VWEXTI
	    (mult:VWEXTI
	      (any_extend:VWEXTI
	        (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	      (any_extend:VWEXTI
	        (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	    (match_operand:VWEXTI 2 "register_operand"               "    0"))
	  (match_operand:VWEXTI 5 "vector_merge_operand"             "  0vu")))]
  "TARGET_VECTOR"
  "vwmacc<u>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "viwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_widen_mul_plus<su><mode>_scalar"
  [(set (match_operand:VWEXTI 0 "register_operand"                    "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             "vmWc1")
	     (match_operand 6 "vector_length_operand"                "   rK")
	     (match_operand 7 "const_int_operand"                    "    i")
	     (match_operand 8 "const_int_operand"                    "    i")
	     (match_operand 9 "const_int_operand"                    "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus:VWEXTI
	    (mult:VWEXTI
	      (any_extend:VWEXTI
	        (vec_duplicate:<V_DOUBLE_TRUNC>
	          (match_operand:<VSUBEL> 3 "register_operand"       "    r")))
	      (any_extend:VWEXTI
	        (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	    (match_operand:VWEXTI 2 "register_operand"               "    0"))
	  (match_operand:VWEXTI 5 "vector_merge_operand"             "  0vu")))]
  "TARGET_VECTOR"
  "vwmacc<u>.vx\t%0,%3,%4%p1"
  [(set_attr "type" "viwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_widen_mul_plussu<mode>"
  [(set (match_operand:VWEXTI 0 "register_operand"                    "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             "vmWc1")
	     (match_operand 6 "vector_length_operand"                "   rK")
	     (match_operand 7 "const_int_operand"                    "    i")
	     (match_operand 8 "const_int_operand"                    "    i")
	     (match_operand 9 "const_int_operand"                    "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus:VWEXTI
	    (mult:VWEXTI
	      (sign_extend:VWEXTI
	        (match_operand:<V_DOUBLE_TRUNC> 3 "register_operand" "   vr"))
	      (zero_extend:VWEXTI
	        (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	    (match_operand:VWEXTI 2 "register_operand"               "    0"))
	  (match_operand:VWEXTI 5 "vector_merge_operand"             "  0vu")))]
  "TARGET_VECTOR"
  "vwmaccsu.vv\t%0,%3,%4%p1"
  [(set_attr "type" "viwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_widen_mul_plussu<mode>_scalar"
  [(set (match_operand:VWEXTI 0 "register_operand"                    "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             "vmWc1")
	     (match_operand 6 "vector_length_operand"                "   rK")
	     (match_operand 7 "const_int_operand"                    "    i")
	     (match_operand 8 "const_int_operand"                    "    i")
	     (match_operand 9 "const_int_operand"                    "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus:VWEXTI
	    (mult:VWEXTI
	      (sign_extend:VWEXTI
	        (vec_duplicate:<V_DOUBLE_TRUNC>
	          (match_operand:<VSUBEL> 3 "register_operand"       "    r")))
	      (zero_extend:VWEXTI
	        (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	    (match_operand:VWEXTI 2 "register_operand"               "    0"))
	  (match_operand:VWEXTI 5 "vector_merge_operand"             "  0vu")))]
  "TARGET_VECTOR"
  "vwmaccsu.vx\t%0,%3,%4%p1"
  [(set_attr "type" "viwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_widen_mul_plusus<mode>_scalar"
  [(set (match_operand:VWEXTI 0 "register_operand"                    "=&vr")
	(if_then_else:VWEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             "vmWc1")
	     (match_operand 6 "vector_length_operand"                "   rK")
	     (match_operand 7 "const_int_operand"                    "    i")
	     (match_operand 8 "const_int_operand"                    "    i")
	     (match_operand 9 "const_int_operand"                    "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (plus:VWEXTI
	    (mult:VWEXTI
	      (zero_extend:VWEXTI
	        (vec_duplicate:<V_DOUBLE_TRUNC>
	          (match_operand:<VSUBEL> 3 "register_operand"       "    r")))
	      (sign_extend:VWEXTI
	        (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand" "   vr")))
	    (match_operand:VWEXTI 2 "register_operand"               "    0"))
	  (match_operand:VWEXTI 5 "vector_merge_operand"             "  0vu")))]
  "TARGET_VECTOR"
  "vwmaccus.vx\t%0,%3,%4%p1"
  [(set_attr "type" "viwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated BOOL mask operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 15.1 Vector Mask-Register Logical Instructions
;; -------------------------------------------------------------------------------

;; We keep this pattern same as pred_mov so that we can gain more optimizations.
;; For example, if we have vmxor.mm v1,v1,v1. It will be optmized as vmclr.m which
;; is generated by pred_mov.
(define_insn "@pred_<optab><mode>"
  [(set (match_operand:VB 0 "register_operand"                   "=vr")
	(if_then_else:VB
	  (unspec:VB
	    [(match_operand:VB 1 "vector_all_trues_mask_operand" "Wc1")
	     (match_operand 5 "vector_length_operand"            " rK")
	     (match_operand 6 "const_int_operand"                "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_bitwise:VB
	    (match_operand:VB 3 "register_operand"               " vr")
	    (match_operand:VB 4 "register_operand"               " vr"))
	  (match_operand:VB 2 "vector_undef_operand"             " vu")))]
  "TARGET_VECTOR"
  "vm<insn>.mm\t%0,%3,%4"
  [(set_attr "type" "vmalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "5")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[6])"))])

(define_insn "@pred_n<optab><mode>"
  [(set (match_operand:VB 0 "register_operand"                   "=vr")
	(if_then_else:VB
	  (unspec:VB
	    [(match_operand:VB 1 "vector_all_trues_mask_operand" "Wc1")
	     (match_operand 5 "vector_length_operand"            " rK")
	     (match_operand 6 "const_int_operand"                "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (not:VB
	    (any_bitwise:VB
	      (match_operand:VB 3 "register_operand"             " vr")
	      (match_operand:VB 4 "register_operand"             " vr")))
	  (match_operand:VB 2 "vector_undef_operand"             " vu")))]
  "TARGET_VECTOR"
  "vm<ninsn>.mm\t%0,%3,%4"
  [(set_attr "type" "vmalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "5")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[6])"))])

(define_insn "@pred_<optab>n<mode>"
  [(set (match_operand:VB 0 "register_operand"                   "=vr")
	(if_then_else:VB
	  (unspec:VB
	    [(match_operand:VB 1 "vector_all_trues_mask_operand" "Wc1")
	     (match_operand 5 "vector_length_operand"            " rK")
	     (match_operand 6 "const_int_operand"                "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (and_ior:VB
	    (match_operand:VB 3 "register_operand"               " vr")
	    (not:VB
	      (match_operand:VB 4 "register_operand"             " vr")))
	  (match_operand:VB 2 "vector_undef_operand"             " vu")))]
  "TARGET_VECTOR"
  "vm<insn>n.mm\t%0,%3,%4"
  [(set_attr "type" "vmalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "5")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[6])"))])

(define_insn "@pred_not<mode>"
  [(set (match_operand:VB 0 "register_operand"                   "=vr")
	(if_then_else:VB
	  (unspec:VB
	    [(match_operand:VB 1 "vector_all_trues_mask_operand" "Wc1")
	     (match_operand 4 "vector_length_operand"            " rK")
	     (match_operand 5 "const_int_operand"                "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (not:VB
	    (match_operand:VB 3 "register_operand"               " vr"))
	  (match_operand:VB 2 "vector_undef_operand"             " vu")))]
  "TARGET_VECTOR"
  "vmnot.mm\t%0,%3"
  [(set_attr "type" "vmalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type") (symbol_ref "INTVAL (operands[5])"))])
