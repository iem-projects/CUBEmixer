/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_ambi2 written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2008 */


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
  int       x_row_col_index;
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
    post("ambi2_encode ERROR: ambi_weight needs a list of %d <float> weights [-]", x->x_n_order+1);
}

static void ambi2_encode_do_2d(t_ambi2_encode *x, t_floatarg phi)
{
  int i, n=x->x_n_order+1;
  t_atom *at=x->x_at;
  
  phi *= x->x_pi_over_180;
  
  iem_ambi2_calc_co_si(x->x_co, x->x_si, x->x_n_order+1, phi);
  
  SETFLOAT(at, 1.0f);
  at++;
  SETFLOAT(at, (t_float)x->x_row_col_index);
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
  
  SETFLOAT(at, 1.0f);
  at++;
  SETFLOAT(at, (t_float)x->x_row_col_index);
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
  x->x_row_col_index = -1;
  ambi2_encode_do_2d(x, phi);
  outlet_list(x->x_obj.ob_outlet, &s_list, x->x_size_2d, x->x_at+2);
}

static void ambi2_encode_list(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if((argc <= 0) || (argc > 2))
  {
    post("ambi2_encode ERROR: list needs 2 angles: <float> elevation_theta [degree] and <float> azimuth_phi [degree]");
    return;
  }
  else if(argc == 1)
  {
    ambi2_encode_float(x, atom_getfloat(argv));
  }
  else
  {
    x->x_row_col_index = -1;
    ambi2_encode_do_3d(x, &s_list, 2, argv);
    outlet_list(x->x_obj.ob_outlet, &s_list, x->x_size_3d, x->x_at+2);
  }
}

static void ambi2_encode_row(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if(argc == 2)
  {
    x->x_row_col_index = (int)atom_getint(argv++);
    ambi2_encode_do_2d(x, atom_getfloat(argv));
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_2d+1, x->x_at+1);
  }
  else if(argc == 3)
  {
    x->x_row_col_index = (int)atom_getint(argv++);
    ambi2_encode_do_3d(x, &s_list, 2, argv);
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_3d+1, x->x_at+1);
  }
  else
  {
    post("ambi2_encode ERROR: row needs either 1 index + 2 angles: <float> row_index [-] and <float> elevation_theta [degree] and <float> azimuth_phi [degree]/n         or 1 index + 1 angle: <float> row_index [-] and <float> azimuth_phi [degree]");
  }
}

static void ambi2_encode_col(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if(argc == 2)
  {
    x->x_row_col_index = (int)atom_getint(argv++);
    ambi2_encode_do_2d(x, atom_getfloat(argv));
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_2d+1, x->x_at+1);
  }
  else if(argc == 3)
  {
    x->x_row_col_index = (int)atom_getint(argv++);
    ambi2_encode_do_3d(x, &s_list, 2, argv);
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_3d+1, x->x_at+1);
  }
  else
  {
    post("ambi2_encode ERROR: col needs either 1 index + 2 angles: <float> column_index [-] and <float> elevation_theta [degree] and <float> azimuth_phi [degree]/n         or 1 index + 1 angle: <float> column_index [-] and <float> azimuth_phi [degree]");
  }
}

static void ambi2_encode_matrix(t_ambi2_encode *x, t_symbol *s, int argc, t_atom *argv)
{
  if(argc == 1)
  {
    t_atom help_at;
    
    x->x_row_col_index = x->x_size_2d;
    ambi2_encode_do_2d(x, atom_getfloat(argv));
    help_at = x->x_at[0];
    x->x_at[0] = x->x_at[1];
    x->x_at[1] = help_at;
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_2d+2, x->x_at);
  }
  else if(argc == 2)
  {
    t_atom help_at;
    
    x->x_row_col_index = x->x_size_3d;
    ambi2_encode_do_3d(x, &s_list, 2, argv);
    help_at = x->x_at[0];
    x->x_at[0] = x->x_at[1];
    x->x_at[1] = help_at;
    outlet_anything(x->x_obj.ob_outlet, s, x->x_size_3d+2, x->x_at);
  }
  else
  {
    post("ambi2_encode ERROR: matrix needs either 2 angles: <float> elevation_theta [degree] and <float> phi [degree]/n         or 1 angle: <float> azimuth_phi [degree]");
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
  x->x_size_at = x->x_size_3d + 2;// for matrix row col
  
  x->x_at = (t_atom *)getbytes(x->x_size_at * sizeof(t_atom));
  x->x_si = (double *)getbytes(x->x_size_si_co * sizeof(double));
  x->x_co = (double *)getbytes(x->x_size_si_co * sizeof(double));
  x->x_legendre = (double *)getbytes(x->x_size_legendre * sizeof(double));
  x->x_semi_norm = (double *)getbytes(x->x_size_legendre * sizeof(double));
  x->x_ambi_order_weight = (double *)getbytes(x->x_size_si_co * sizeof(double));
  
  /* init */
  x->x_pi_over_180 = 4.0 * atan(1.0) / 180.0;
  x->x_row_col_index = -1;
  SETFLOAT(&x->x_at[0], 1.0f);/*optional row index of matrix-message */
  SETFLOAT(&x->x_at[1], -1.0f);/*optional row of row-message, col index of col-message or matrix message */
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
  class_addmethod(ambi2_encode_class, (t_method)ambi2_encode_matrix, gensym("matrix"), A_GIMME, 0);
  class_addmethod(ambi2_encode_class, (t_method)ambi2_encode_ambi_weight, gensym("ambi_weight"), A_GIMME, 0);
}
