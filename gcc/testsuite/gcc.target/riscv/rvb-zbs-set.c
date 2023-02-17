/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zbs -mabi=lp64 -O2" } */

/* sbset */
long
sub0 (long i, long j)
{
  return i | (1L << j);
}

/* sbset_mask */
long
sub1 (long i, long j)
{
  return i | (1L << (j & 0x3f));
}

/* sbset_1 */
long
sub2 (long i)
{
  return 1L << i;
}

/* sbset_1_mask */
long
sub3 (long i)
{
  return 1L << (i & 0x3f);
}

/* sbseti */
long
sub4 (long i)
{
  return i | (1L << 20);
}

/* sbsetiw */
int
sub5 (long i)
{
  return (int)i | (1 << 20);
}

/* { dg-final { scan-assembler-times "bset" 6 } } */
/* { dg-final { scan-assembler-not "andi" } } */
