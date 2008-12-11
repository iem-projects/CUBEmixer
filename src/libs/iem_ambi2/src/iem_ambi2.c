/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_ambi2 written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2008 */

/* hier werden die Encodier-Terme rein rekursiv berechnet, dh. ohne Limit in der Ordnung
dazu wird definiert:

N..order;
0 <= n <= N ... n = Laufindex der Ordnung;
-n <= m <= +n ... m = Laufindex des Grades;

fuer die harmonischen Terme gelten folgende Rekursions-Vorschriften:

      /= cos(mx) fuer m >= 0
Hm(x)<
      \= sin(|m|x) fuer m < 0

sin(mx) = sin((m-1)x)cos(x) + cos((m-1)x)sin(x)
cos(mx) = cos((m-1)x)cos(x) - sin((m-1)x)sin(x)fuer m > 0;

****************************************************************

fuer die Legendreschen Terme gelten folgende Rekursions-Vorschriften:

Pn,n(x) = (2n-1)sqrt(1-x*x)Pn-1,n-1(x)
Pn,n-1(x) = x(2n-1)Pn-1,n-1(x)
Pn,m(x) = [x(2n-1)Pn-1,m(x) - (n+m-1)Pn-2,m(x)] / (n-m)fuer m <= n-2
{Pn,-m(x) = (n+m)! * Pn,m(x) / (n-m)!}

wobei:
sqrt(1-x*x) = cos(Theta)
x = sin(Theta)

Pn,n(si) = (2n-1)*co*Pn-1,n-1(si)
Pn,n-1(si) = si(2n-1)Pn-1,n-1(si)
Pn,m(si) = [si(2n-1)Pn-1,m(si) - (n+m-1)Pn-2,m(si)] / (n-m)fuer m <= n-2

TEST:
P0,0(x) = 1;

P1,1(x) = (2*1-1)*sqrt(1-x*x)*P0,0(x) = 1*sqrt(1-x*x)*1 = sqrt(1-x*x);
P1,0(x) = x(2*1-1)*P0,0(x) = x*1*1 = x;

P2,2(x) = (2*2-1)*sqrt(1-x*x)*P1,1(x) = 3*sqrt(1-x*x)*sqrt(1-x*x) = 3*(1-x*x);
P2,1(x) = x(2*2-1)*P1,1(x) = x*3*sqrt(1-x*x) = 3*x*sqrt(1-x*x);
P2,0(x) = [x(2*2-1)*P1,0(x)-(2+0-1)*P0,0(x)]/(2-0) = [x*3*x-1*1]/2 = 1/2*(3*x^2 - 1)

P3,3(x) = (2*3-1)*sqrt(1-x*x)*P2,2(x) = 5*sqrt(1-x*x)*3*(1-x*x) = 15*sqrt(1-x*x)^3;
P3,2(x) = x(2*3-1)*P2,2(x) = x*5*3*(1-x*x) = 15*x*(1-x*x);
P3,1(x) = [x(2*3-1)*P2,1(x)-(3+1-1)*P1,1(x)]/(3-1) = [x*5*3*x*sqrt(1-x*x)-3*sqrt(1-x*x)]/2 = 3/2*sqrt(1-x*x)*(5*x^2 - 1);
P3,0(x) = [x(2*3-1)*P2,0(x)-(3+0-1)*P1,0(x)]/(3-0) = [x*5*(3*x*x - 1)/2 - 2*x]/3 = 15/6*x^3 -5/6*x - 2/3*x = 1/2*x*(5x^2 - 3);

P4,4(x) = (2*4-1)*sqrt(1-x*x)*P3,3(x) = 7*sqrt(1-x*x)*15*sqrt(1-x*x)^3 = 105*(1-x*x)^2;
P4,3(x) = x(2*4-1)*P3,3(x) = x*7*15*sqrt(1-x*x)^3 = 105*x*sqrt(1-x*x)^3;
P4,2(x) = [x(2*4-1)*P3,2(x)-(4+2-1)*P2,2(x)]/(4-2) = [x*7*15*x*(1-x*x)-5*3*(1-x*x)]/2 = 15/2*(1-x*x)*(7*x^2 - 1);
P4,1(x) = [x(2*4-1)*P3,1(x)-(4+1-1)*P2,1(x)]/(4-1) = [x*7*3/2*sqrt(1-x*x)*(5*x^2 - 1)-4*3*x*sqrt(1-x*x)]/3 = 5/2*x*[7*x^2 - 3]*sqrt(1-x*x);
P4,0(x) = [x(2*4-1)*P3,0(x)-(4+0-1)*P2,0(x)]/(4-0) = [x*7*1/2*x*(5x^2 - 3)-3*1/2*(3*x^2 - 1)]/4 = (35x^4 - 30x^2 + 3)/8;


******************************************************************

Schmidtsche Halbnormalisierung

Nn,m = sqrt(eps(m)*(n-m)!/(n+m)!);  eps(m=0)=1 fuer m=0;  eps(m!=0)=2 fuer m!=0;


Nn,0 = 1;
Nn,n = Nn-1,n-1 / sqrt(2n(2n-1))                      fuer: n-1 > 0
Nn,n = Nn-1,n-1 / sqrt(2n(2n-1)/2)                    fuer: n-1 = 0
Nn,m = Nn-1,m * sqrt((n-m) / (n+m))


TEST:
N0,0(x) = 1;

N1,1(x) = N0,0/sqrt(2*1(2*1-1)/2) = 1/sqrt(2*1/2) = 1/sqrt(1) = 1; .... fuer: n-1 = 0.... einziges mal
N1,0(x) = 1;

N2,2(x) = N1,1/sqrt(2*2*(2*2-1)) = 1/sqrt(4*3) = sqrt(3)/6;
N2,1(x) = N1,1*sqrt((2-1)/(2+1)) = 1*sqrt(1/3) = sqrt(3)/3;
N2,0(x) = 1;

N3,3(x) = N2,2/sqrt(2*3*(2*3-1)) = sqrt(3)/(6*sqrt(6*5)) = 1/(6*sqrt(10)) = sqrt(10)/60;
N3,2(x) = N2,2*sqrt((3-2)/(3+2)) = sqrt(3)/6 * sqrt(1/5) = sqrt(15)/30;
N3,1(x) = N2,1*sqrt((3-1)/(3+1)) = sqrt(3)/3 * sqrt(2/4) = sqrt(6)/6;
N3,0(x) = 1;

N4,4(x) = N3,3/sqrt(2*4*(2*4-1)) = sqrt(10)/(60*sqrt(8*7)) = sqrt(2*5)/(60*2*sqrt(2*7)) = sqrt(5*7)/(120*7) = sqrt(35)/840;
N4,3(x) = N3,3*sqrt((4-3)/(4+3)) = sqrt(10)/60 * sqrt(1/7) = sqrt(7*10)/(60*7) = sqrt(70)/420;
N4,2(x) = N3,2*sqrt((4-2)/(4+2)) = sqrt(15)/30 * sqrt(2/6) = sqrt(30/6)/30 = sqrt(5)/30;
N4,1(x) = N3,1*sqrt((4-1)/(4+1)) = sqrt(6)/6 * sqrt(3/5) = sqrt(6*3/(5*6*6) = sqrt(3/30) = sqrt(10)/10;


volle orthonormalisierung:

NFn,m = NSn,m * sqrt((2*n+1)/(4*pi))

N=sqrt((2n+1)/(2(1+delta(m)pi))*(n-m)!/(n+m)!)
N=sqrt((2n+1)/(2pi))*(n-m)!/(n+m)!)  fuer m != 0;
N=sqrt((2n+1)/(4pi))  fuer m == 0;

Nn,0 = sqrt((2*n+1)/(4*pi));
Nn,n = Nn-1,n-1 * sqrt(2n(2n-1))                      fuer: n-1 > 0
Nn,n = Nn-1,n-1 / sqrt(2n(2n-1)/2)                    fuer: n-1 = 0
Nn,m = Nn-1,m * sqrt((n-m) / (n+m))

Nn,n=sqrt( (2n+1)/(2pi) * 1/(2n)! ) = sqrt( (2n+1)/(2n-1) * (2n-1)/(2pi) * 1/(2n*(2n-1)*(2n-2)!) ) = sqrt( (2n+1)/(2n*(2n-1)*(2n-1)) * (2n-1)/(2pi) * 1/(2n-2)! ) = sqrt( (2n+1)/(2n*(2n-1)*(2n-1)) * Nn-1,n-1) fuer: n-1 > 0
Nn,n = sqrt((2n+1)/(2n*(2n-1)^2))*Nn-1,n-1;

Nn,m = sqrt((2n+1)/(2n-1)*(2n-1)/(2pi))*(n-m)*(n-1-m)!/((n+m)(n-1+m))!) = sqrt((2n+1)/(2n-1)*(n-m)/(n+m)) * sqrt((2n-1)/(2pi)*(n-1-m)!/(n-1+m)!) = sqrt((2n+1)/(2n-1)*(n-m)/(n+m))*Nn-1,m

TEST:
N0,0(x) = sqrt(1/4pi);

N1,1(x) = sqrt(3/4pi); .... fuer: n-1 = 0.... einziges mal
N1,0(x) = sqrt(3/4pi);

N2,2(x) = sqrt((5)/(2pi))*1/24);        sqrt((5)/(4*(3)^2))*N1,1 = sqrt(5/36)*sqrt(3/4pi) = sqrt(5/12)*sqrt(1/4pi) = sqrt(5/48pi);
N2,1(x) = ;     sqrt((5)/(3)*(1)/(3))*sqrt(3/4pi)=sqrt(5/12pi);
N2,0(x) = sqrt(5/4pi);

N3,3(x) = sqrt((7)/(2pi))*1/720);       sqrt((7)/(6*(5)^2))*N2,2 = sqrt(7/150)*sqrt(5/48pi) = sqrt(7/30)*sqrt(1/48pi) = sqrt(7/1440pi);
N3,2(x) = ;     sqrt((7)/(5)*(1)/(5))*sqrt(5/48pi)=sqrt(7/240pi);
N3,1(x) = ;     sqrt((7)/(5)*(2)/(4))*sqrt(5/12pi)=sqrt(7/24pi);
N3,0(x) = sqrt(7/4pi);

N4,4(x) = sqrt((9)/(2pi))*1/40320);
N4,3(x) = ;
N4,2(x) = ;
N4,1(x) = ;
N4,0(x) = sqrt(9/4pi);

******************************************************************

Die Dreiecksstruktur:

.,.. .,.. .,.. .,.. .,.. 0,0 .,.. .,.. .,.. .,.. .,..
.,.. .,.. .,.. .,.. 1,-1 1,0 1,+1 .,.. .,.. .,.. .,..
.,.. .,.. .,.. 2,-2 2,-1 2,0 2,+1 2,+2 .,.. .,.. .,..
.,.. .,.. 3,-3 3,-2 3,-1 3,0 3,+1 3,+2 3,+3 .,.. .,..
.,.. 4,-4 4,-3 4,-2 4,-1 4,0 4,+1 4,+2 4,+3 4,+4 .,..
5,-5 5,-4 5,-3 5,-2 5,-1 5,0 5,+1 5,+2 5,+3 5,+4 5,+5

halbes Dreieck:

0,0 .,.. .,.. .,.. .,.. .,..
1,0 1,+1 .,.. .,.. .,.. .,..
2,0 2,+1 2,+2 .,.. .,.. .,..
3,0 3,+1 3,+2 3,+3 .,.. .,..
4,0 4,+1 4,+2 4,+3 4,+4 .,..
5,0 5,+1 5,+2 5,+3 5,+4 5,+5

Index-Reihenfolge des halben Dreiecks:

00
02 01
05 04 03
09 08 07 06
14 13 12 11 10
20 19 18 17 16 15

full:
00, 01c1, 01s1, 02, 03c2, 03s2, 04c1, 04s1, 05, 06c3, 06s3, 07c2, 07s2, 08c1, 08s1, 09, 10c4, 10s4, 11c3, 11s3, 12c2, 12s2, 13c1, 13s1, 14, ....

hemi:
00, 01c1, 01s1, xx, 03c2, 03s2, xx, xx, 05, 06c3, 06s3, xx, xx, 08c1, 08s1, xx, 10c4, 10s4, xx, xx, 12c2, 12s2, xx, xx, 14, ....


00
06 01
11 07 02
12 13 08 03
14 15 16 09 04
17 18 19 20 10 05

full:
00;  07c1, 07s1, 06;  14c2, 14s2, 13c1, 13s1, 12;  21c3, 21s3, 20c2, 20s2, 19c1, 19s1, 18;


******************************************************************

dh. wir haben folgende Speicher Strukturen:

wenn 3d
Harmonische Terme: 2 Register si und co und ein linearer Speicher, der 2*(N+1) sein muss;   
Legendre Terme: (N+1)*(N+1) wir sind grosszuegig, berechnen aber nur die positiven m
Normalisierungs Terme: (N+1)*(N+1) wir sind grosszuegig, berechnen aber nur die positiven m
atom kette: 

******************************************************************  */


#include "m_pd.h"
#include "iemlib.h"
#include <math.h>

/*
 volle orthonormalisierung:
 
 N=sqrt((2n+1)/(2(1+delta(m)pi))*(n-m)!/(n+m)!)
 */

static t_class *iem_ambi2_class;

void iem_ambi2_init_ortho_norm(double *array_ortho_norm, int n)
{
  int i, j, k, incr=n;
  double df, one_over_four_pi = 1.0 / (4.0 * 4.0 * atan(1.0)); // 1/(4pi)
  
  for(i=0,j=0; i<n; i++,j+=incr)
  {
    for(k=0; k<=i; k++)
      array_ortho_norm[j+k] = 1.0;
  }
  
  incr = n+1;
  for(i=2,j=2*incr; i<n; i++,j+=incr)
    array_ortho_norm[j] = array_ortho_norm[j-incr] / (double)(2*i*(2*i-1));
  
  incr = n;
  for(i=2,j=2*incr; i<n; i++,j+=incr)
  {
    for(k=0; k<i; k++)
      array_ortho_norm[j+k] = array_ortho_norm[j+k-incr] * (double)(i-k) / (double)(i+k);
  }
  
  for(i=0,j=0; i<n; i++,j+=incr)
  {
    df = (double)(2*i+1);
    for(k=0; k<=i; k++)
      array_ortho_norm[j+k] = sqrt(df*array_ortho_norm[j+k]*one_over_four_pi);
  }
}

void iem_ambi2_init_semi_norm(double *array_semi_norm, int n)
{
  int i, j, k, incr=n;
  
  for(i=0,j=0; i<n; i++,j+=incr)
  {
    for(k=0; k<=i; k++)
      array_semi_norm[j+k] = 1.0;
  }
  
  incr = n+1;
  for(i=2,j=2*incr; i<n; i++,j+=incr)
    array_semi_norm[j] = array_semi_norm[j-incr] / (double)(2*i*(2*i-1));
  
  incr = n;
  for(i=2,j=2*incr; i<n; i++,j+=incr)
  {
    for(k=0; k<i; k++)
      array_semi_norm[j+k] = array_semi_norm[j+k-incr] * (double)(i-k) / (double)(i+k);
  }
  
  for(i=0,j=0; i<n; i++,j+=incr)
  {
    for(k=0; k<=i; k++)
      array_semi_norm[j+k] = sqrt(array_semi_norm[j+k]);
  }
}

/*void iem_ambi2_init_semi_norm(double *array_semi_norm, int n)
{
  int i, j, k, incr;
  
  array_semi_norm[0] = 1;
  if(n > 1)
  {
    array_semi_norm[n] = 1;
    array_semi_norm[n+1] = 1;
  }
  
  incr = n+1;
  for(i=2,j=2*incr; i<n; i++,j+=incr)
    array_semi_norm[j] = array_semi_norm[j-incr] / sqrt( (double)(2*i*(2*i-1)) );
  
  incr = n;
  for(i=2,j=2*incr; i<n; i++,j+=incr)
  {
    for(k=0; k<i; k++)
      array_semi_norm[j+k] = array_semi_norm[j+k-incr] * sqrt( (double)(i-k) / (double)(i+k) );
  }
}*/

void iem_ambi2_calc_legendre(double *array_legendre, int n, double theta)
{
  double si, co;
  int i, j, k, incr;
  
  co = cos(theta); // sqrt(1 - zeta*zeta)
  si = sin(theta); // zeta
  
  incr = n+1;
  for(i=1,j=incr; i<n; i++,j+=incr)
    array_legendre[j] = (double)(2*i-1)*co*array_legendre[j-incr];
  
  for(i=1,j=n; i<n; i++,j+=incr)
    array_legendre[j] = (double)(2*i-1)*si*array_legendre[j-n];
  
  incr = n;
  for(i=2,j=2*incr; i<n; i++,j+=incr)
  {
    for(k=0; k<i-1; k++)
      array_legendre[j+k] = ( (double)(2*i-1)*si*array_legendre[j-incr+k] - (double)(i+k-1)*array_legendre[j-2*incr+k] ) / (double)(i-k);
  }
}

void iem_ambi2_calc_co_si(double *array_cosinus, double *array_sinus, int n, double phi)
{
  double si, co;
  int i;
  
  co = cos(phi);
  si = sin(phi);
  for(i=1; i<n; i++)
  {
    array_cosinus[i] = array_cosinus[i-1]*co - array_sinus[i-1]*si;
    array_sinus[i] = array_sinus[i-1]*co + array_cosinus[i-1]*si;
  }
}

static void *iem_ambi2_new(void)
{
	t_object *x = (t_object *)pd_new(iem_ambi2_class);
    
	return (x);
}

void ambi2_encode_setup(void);
void ambi2_decode_setup(void);
void ambi2_rotate_setup(void);
void ambi2_encode_hemi_setup(void);
void ambi2_decode_hemi_setup(void);
void ambi3_encode_setup(void);
void ambi3_decode_setup(void);
void ambi3_encode_hemi_setup(void);
void ambi3_decode_hemi_setup(void);

/* ------------------------ setup routine ------------------------- */

void iem_ambi2_setup(void)
{
	ambi2_encode_setup();
	ambi2_decode_setup();
  ambi2_rotate_setup();
  ambi2_encode_hemi_setup();
	ambi2_decode_hemi_setup();
  ambi3_encode_setup();
	ambi3_decode_setup();
  ambi3_encode_hemi_setup();
	ambi3_decode_hemi_setup();

  post("iem_ambi2 (R-1.18) library loaded!   (c) Thomas Musil 07.2008");
	post("   musil%ciem.at iem KUG Graz Austria", '@');
}
