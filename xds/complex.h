/* eliminated long double reference --- MSC */
#ifndef COMPLEX_H
#define COMPLEX_H

/* ANSI C 
 * Floating double complex operations
 * Modified from Numerical Recipes by Timothy Davis April, 1992
 */

typedef struct SCOMPLEX {short r,i;} scomplex;
typedef struct FCOMPLEX {float r,i;} fcomplex;
typedef struct DCOMPLEX {double r,i;} dcomplex;
typedef struct LCOMPLEX {double r, i;} lcomplex;
/* Set the complex type to the preferred of {fcomplex, dcomplex, lcomplex}; */
typedef dcomplex complex;
 
extern complex CComplex(double re, double im),
  Cadd(complex a, complex b),  Csub(complex a, complex b),
  Cmul(complex a, complex b),  Cdiv(complex a, complex b),
  Conjg(complex z),  Csqrt(complex z),  RCmul(double x, complex a);
extern double  Cabs(complex z),   Carg(complex z);

extern fcomplex FComplex(double re, double im),
  FCadd(fcomplex a, fcomplex b),  FCsub(fcomplex a, fcomplex b),
  FCmul(fcomplex a, fcomplex b),  FCdiv(fcomplex a, fcomplex b),
  FConjg(fcomplex z),  FCsqrt(fcomplex z),  FRCmul(double x, fcomplex a);
extern double  FCabs(fcomplex z),   FCarg(fcomplex z);

extern dcomplex DComplex(double re, double im),
  DCadd(dcomplex a, dcomplex b),  DCsub(dcomplex a, dcomplex b),
  DCmul(dcomplex a, dcomplex b),  DCdiv(dcomplex a, dcomplex b),
  DConjg(dcomplex z),  DCsqrt(dcomplex z),  DRCmul(double x, dcomplex a);
extern double  DCabs(dcomplex z),   DCarg(dcomplex z);

extern lcomplex LComplex(double re, double im),
  LCadd(lcomplex a, lcomplex b),  LCsub(lcomplex a, lcomplex b),
  LCmul(lcomplex a, lcomplex b),  LCdiv(lcomplex a, lcomplex b),
  LConjg(lcomplex z),  LCsqrt(lcomplex z),  LRCmul(double x, lcomplex a);
extern double  LCabs(lcomplex z),   LCarg(lcomplex z);


#define MComplex(Z,RE,IM) ((Z).r=(RE), (Z).i=(IM))
#define MConjg(Z,A) ((Z).r=(A).r, (Z).i= -(A).i)
#define MRCmul(Z,A,alpha) ((Z).r=(A).r*(alpha), (Z).i=(A).i*(alpha))
#define MCadd(Z,A,B) ((Z).r=(A).r+(B).r, (Z).i=(A).i+(B).i)
#define MCsub(Z,A,B) ((Z).r=(A).r-(B).r, (Z).i=(A).i-(B).i)
#define MCmul(Z,A,B) ((Z).r=(A).r*(B).r-(A).i*(B).i, \
                      (Z).i=(A).i*(B).r+(A).r*(B).i)

#define MCabs(Z) (hypot((Z).r,(Z).i))
#define MCarg(Z) (((Z).r==0 && (Z).i==0) || (isnan((Z).i) || isnan((Z).r)) ? (double)0 : atan2((Z).i,(Z).r))

#define MCdiv(Z,A,B) ((Z)=Cdiv((A),(B)))
#define MCsqrt(Z,A)  ((Z)=Csqrt((A)))

#endif /* COMPLEX_H */
