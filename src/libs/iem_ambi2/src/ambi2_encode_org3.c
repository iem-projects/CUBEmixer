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
cos(mx) = cos((m-1)x)cos(x) - sin((m-1)x)sin(x)  fuer m > 0;

****************************************************************

fuer die Legendreschen Terme gelten folgende Rekursions-Vorschriften:

Pn,n(x) = (2n-1)sqrt(1-x*x)Pn-1,n-1(x)
Pn,n-1(x) = x(2n-1)Pn-1,n-1(x)
Pn,m(x) = [x(2n-1)Pn-1,m(x) - (n+m-1)Pn-2,m(x)] / (n-m)  fuer m <= n-2
{Pn,-m(x) = (n+m)! * Pn,m(x) / (n-m)!}

wobei:
sqrt(1-x*x) = cos(Theta)
x = sin(Theta)

  Pn,n(si) = (2n-1)*co*Pn-1,n-1(si)
  Pn,n-1(si) = si(2n-1)Pn-1,n-1(si)
  Pn,m(si) = [si(2n-1)Pn-1,m(si) - (n+m-1)Pn-2,m(si)] / (n-m)  fuer m <= n-2

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
                                                                                         
Nn,0 = 1;
Nn,n = Nn-1,n-1 / sqrt(2n(2n-1))                      fuer: n-1 > 0
Nn,n = Nn-1,n-1 / sqrt(2n(2n-1)/2)                    fuer: n-1 = 0
Nn,m = Nn-1,m * sqrt((n-m) / (n+m))


TEST:
N0,0(x) = 1;

N1,1(x) = N0,0/sqrt(2*1(2*1-1)/2) = 1/sqrt(2*1/2) = 1/sqrt(1) = 1; .... fuer: n-1 = 0  .... einziges mal
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
N4,0(x) = 1;

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
#include "iem_ambi2.h"

/* -------------------------- ambi2_encode ------------------------------ */

typedef struct _ambi2_encode
{
  t_object  x_obj;
  t_atom    *x_at;
  double    *x_si;
  double    *x_co;
  double    *x_legendre;
  double    *x_semi_norm;
  double    *x_ambi_order_weight;
  double    x_pi_over_180;
  int       x_n_order;
  int       x_size_si_co;
  int       x_size_legendre;
  int       x_size_at;
  int       x_size_2d;
  int       x_size_3d;
  int       x_col_row_index;
} t_ambi2_encode;

static t_class *ambi2_encode_class;

static void ambi2_encode_ambi_weight(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if(argc > x->x_n_order)
  {
    int i, n=x->x_n_order;
    
    for(i=0; i<=n; i++)
      x->x_ambi_order_weight[i] = atom_getfloat(argv++);
  }
  else
    post("ambi2_encode-ERROR: ambi_weight needs %d float weights", x->x_n_order+1);
}

static void ambi2_encode_do_2d(t_ambi2_encode *x, t_floatarg phi)
{
  int i, n=x->x_n_order+1;
  t_atom *at=x->x_at;
  
  phi *= x->x_pi_over_180;
  
  iem_ambi2_calc_co_si(x->x_co, x->x_si, x->x_n_order+1, phi);
  SETFLOAT(at, (t_float)x->x_col_row_index);
  at++;
  SETFLOAT(at, x->x_ambi_order_weight[0]); // cos(0) = 1.0; W-Channel;
  at++;
  for(i=1; i<n; i++)
  {
    SETFLOAT(at, x->x_co[i] * x->x_ambi_order_weight[i]);
    at++;
    SETFLOAT(at, x->x_si[i] * x->x_ambi_order_weight[i]);
    at++;
  }
}

static void ambi2_encode_do_3d(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  double theta, phi;
  int i, j, k, n=x->x_n_order+1;
  t_atom *at=x->x_at;
 
  theta = (double)atom_getfloat(argv++) * x->x_pi_over_180;
  phi = (double)atom_getfloat(argv) * x->x_pi_over_180;
  
  iem_ambi2_calc_legendre(x->x_legendre, x->x_n_order+1, theta);
  iem_ambi2_calc_co_si(x->x_co, x->x_si, x->x_n_order+1, phi);
  
  SETFLOAT(at, (t_float)x->x_col_row_index);
  at++;
  SETFLOAT(at, x->x_ambi_order_weight[0]); // cos(0) = 1.0; W-Channel;
  at++;
  for(i=1,j=n; i<n; i++,j+=n)
  {
    for(k=i; k>0; k--)
    {
//      post("cos[%d]=%f, sin[%d]=%f, leg[%d][%d]=%f, norm[%d][%d]=%f, weight[%d]=%f", k, x->x_co[k], k, x->x_si[k], i, k, x->x_legendre[j+k], i, k, x->x_semi_norm[j+k], i, x->x_ambi_order_weight[i]);
      SETFLOAT(at, x->x_semi_norm[j+k]*x->x_legendre[j+k]*x->x_co[k]*x->x_ambi_order_weight[i]);
      at++;
      SETFLOAT(at, x->x_semi_norm[j+k]*x->x_legendre[j+k]*x->x_si[k]*x->x_ambi_order_weight[i]);
      at++;
    }
    SETFLOAT(at, x->x_legendre[j]*x->x_ambi_order_weight[i]);
    at++;
  }
}

static void ambi2_encode_float(t_ambi2_encode *x, t_floatarg phi)
{
  x->x_col_row_index = -1;
  ambi2_encode_do_2d(x, phi);
  outlet_list(x->x_obj.ob_outlet, &s_list, x->x_size_2d, x->x_at+1);
}

static void ambi2_encode_list(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if(argc <= 0)
  {
    post("ambi2_encode ERROR: list-input needs 2 angles: delta [rad] and phi [rad]");
    return;
  }
  else if(argc == 1)
  {
    ambi2_encode_float(x, atom_getfloat(argv));
  }
  else
  {
    x->x_col_row_index = -1;
    ambi2_encode_do_3d(x, &s_list, 2, argv);
    outlet_list(x->x_obj.ob_outlet, &s_list, x->x_size_3d, x->x_at+1);
  }
}

static void ambi2_encode_row(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if(argc == 2)
  {
    x->x_col_row_index = (int)atom_getint(argv++);
    ambi2_encode_do_2d(x, atom_getfloat(argv));
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_2d+1, x->x_at);
  }
  else if(argc >= 3)
  {
    x->x_col_row_index = (int)atom_getint(argv++);
    ambi2_encode_do_3d(x, &s_list, 2, argv);
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_3d+1, x->x_at);
  }
  else
  {
    post("ambi2_encode-ERROR: row needs <float> row-index + <float> angle ( + <float> angle)");
  }
}

static void ambi2_encode_col(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if(argc == 2)
  {
    x->x_col_row_index = (int)atom_getint(argv++);
    ambi2_encode_do_2d(x, atom_getfloat(argv));
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_2d+1, x->x_at);
  }
  else if(argc >= 3)
  {
    x->x_col_row_index = (int)atom_getint(argv++);
    ambi2_encode_do_3d(x, &s_list, 2, argv);
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_3d+1, x->x_at);
  }
  else
  {
    post("ambi2_encode-ERROR: col needs <float> col-index + <float> angle ( + <float> angle)");
  }
}

static void ambi2_encode_free(t_ambi2_encode *x)
{
  freebytes(x->x_at, x->x_size_at * sizeof(t_atom));
  freebytes(x->x_si, x->x_size_si_co * sizeof(double));
  freebytes(x->x_co, x->x_size_si_co * sizeof(double));
  freebytes(x->x_legendre, x->x_size_legendre * sizeof(double));
  freebytes(x->x_semi_norm, x->x_size_legendre * sizeof(double));
  freebytes(x->x_ambi_order_weight, x->x_size_si_co * sizeof(double));
}

static void *ambi2_encode_new(t_floatarg forder)
{
  t_ambi2_encode *x = (t_ambi2_encode *)pd_new(ambi2_encode_class);
  int i, norder=(int)forder;
  
  if(norder < 0)
    norder = 0;
  x->x_n_order = norder;
  x->x_size_2d = 2*x->x_n_order + 1;
  x->x_size_3d = (x->x_n_order + 1)*(x->x_n_order + 1);
  x->x_size_legendre = x->x_size_3d;
  x->x_size_si_co = x->x_n_order + 1;
  x->x_size_at = x->x_size_3d + 1;// for matrix row col
  
  x->x_at = (t_atom *)getbytes(x->x_size_at * sizeof(t_atom));
  x->x_si = (double *)getbytes(x->x_size_si_co * sizeof(double));
  x->x_co = (double *)getbytes(x->x_size_si_co * sizeof(double));
  x->x_legendre = (double *)getbytes(x->x_size_legendre * sizeof(double));
  x->x_semi_norm = (double *)getbytes(x->x_size_legendre * sizeof(double));
  x->x_ambi_order_weight = (double *)getbytes(x->x_size_si_co * sizeof(double));
  
  /* init */
  x->x_pi_over_180 = 4.0 * atan(1.0) / 180.0;
  x->x_col_row_index = 0;
  SETFLOAT(&x->x_at[0], -1.0f);/*optional row or col index*/
  SETFLOAT(&x->x_at[1], 1.0f);/*W channel*/
  x->x_co[0] = 1.0;
  x->x_si[0] = 0.0;
  x->x_legendre[0] = 1.0;
  iem_ambi2_init_semi_norm(x->x_semi_norm, x->x_n_order+1);
  for(i=0; i<x->x_size_si_co; i++)
    x->x_ambi_order_weight[i] = 1.0;

  outlet_new(&x->x_obj, &s_list);
  return (x);
}

void ambi2_encode_setup(void)
{
  ambi2_encode_class = class_new(gensym("ambi2_encode"), (t_newmethod)ambi2_encode_new, (t_method)ambi2_encode_free,
    sizeof(t_ambi2_encode), 0, A_DEFFLOAT, 0);
  class_addlist(ambi2_encode_class, (t_method)ambi2_encode_list);
  class_addfloat(ambi2_encode_class, (t_method)ambi2_encode_float);
  class_addmethod(ambi2_encode_class, (t_method)ambi2_encode_row, gensym("row"), A_GIMME, 0);
  class_addmethod(ambi2_encode_class, (t_method)ambi2_encode_col, gensym("col"), A_GIMME, 0);
  class_addmethod(ambi2_encode_class, (t_method)ambi2_encode_ambi_weight, gensym("ambi_weight"), A_GIMME, 0);
}
