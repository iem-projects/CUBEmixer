/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_ambi2 written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2008 */

#include "m_pd.h"
#include "iemlib.h"
#include <math.h>
#include <stdio.h>
#include "iem_ambi2.h"

/* -------------------------- ambi2_rotate ------------------------------ */

typedef struct _ambi2_rotate
{
  t_object  x_obj;
  t_atom    *x_at;
  double    *x_si;
  double    *x_co;
  int       x_n_order;
  int       x_size_si_co;
  int       x_size_at;
  t_float   x_pi_over_180;
} t_ambi2_rotate;

static t_class *ambi2_rotate_class;

static void ambi2_rotate_float(t_ambi2_rotate *x, t_floatarg rho_z) /* = ambi2_rotate_z(); */
{
  t_atom *at=x->x_at;
  int i, n=x->x_n_order+1;
  
  rho_z *= x->x_pi_over_180;
  iem_ambi2_calc_co_si(x->x_co, x->x_si, x->x_n_order+1, rho_z);
  SETFLOAT(at+2, 2.0);
  SETFLOAT(at+3, 2.0);
  for(i=1; i<n; i++)
  {
    SETFLOAT(at+0, (t_float)i);
    SETFLOAT(at+4, x->x_co[i]);
    SETFLOAT(at+5, -x->x_si[i]);
    SETFLOAT(at+6, x->x_si[i]);
    SETFLOAT(at+7, x->x_co[i]);
    outlet_list(x->x_obj.ob_outlet, &s_list, 8, x->x_at);
  }
}

static void ambi2_rotate_free(t_ambi2_rotate *x)
{
  freebytes(x->x_at, x->x_size_at * sizeof(t_atom));
  freebytes(x->x_si, x->x_size_si_co * sizeof(double));
  freebytes(x->x_co, x->x_size_si_co * sizeof(double));
}

static void *ambi2_rotate_new(t_floatarg forder)
{
  t_ambi2_rotate *x = (t_ambi2_rotate *)pd_new(ambi2_rotate_class);
  int norder=(int)forder;
  
  if(norder < 0)
    norder = 0;
  x->x_n_order = norder;
  x->x_size_si_co = x->x_n_order + 1;
  x->x_size_at = 8;
  x->x_at = (t_atom *)getbytes(x->x_size_at * sizeof(t_atom));
  x->x_si = (double *)getbytes(x->x_size_si_co * sizeof(double));
  x->x_co = (double *)getbytes(x->x_size_si_co * sizeof(double));
  x->x_pi_over_180  = (t_float)(4.0 * atan(1.0) / 180.0);
  
  x->x_co[0] = 1.0;
  x->x_si[0] = 0.0;
  SETSYMBOL(x->x_at+1, gensym("matrix"));
  outlet_new(&x->x_obj, &s_list);
  return (x);
}

void ambi2_rotate_setup(void)
{
  ambi2_rotate_class = class_new(gensym("ambi2_rotate"), (t_newmethod)ambi2_rotate_new, (t_method)ambi2_rotate_free,
    sizeof(t_ambi2_rotate), 0, A_DEFFLOAT, 0);
  class_addfloat(ambi2_rotate_class, (t_method)ambi2_rotate_float);
}
