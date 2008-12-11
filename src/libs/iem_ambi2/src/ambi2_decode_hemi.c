/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_ambi written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2008 */

#include "m_pd.h"
#include "iemlib.h"
#include "iem_ambi2.h"
#include <math.h>


/* -------------------------- ambi2_decode_hemi ------------------------------ */


typedef struct _ambi2_decode_hemi
{
	t_object	x_obj;
	t_atom		*x_at;
  double    *x_si;
  double    *x_co;
  double    *x_legendre;
  double    *x_semi_norm;
  double    *x_ambi_order_weight;
  double    x_pi_over_180;
	double		*x_inv_work1;
	double		*x_inv_work2;
	double		*x_inv_buf2;
	double		*x_transp;
	double		*x_ls_encode;
	double		*x_prod;
	double		x_sing_range;
  int       x_size_si_co;
  int       x_size_legendre;
	int				x_n_ambi;
	int				x_n_order;
	int				x_n_real_ls;
	int				x_n_pht_ls;
	t_symbol	*x_s_matrix;
} t_ambi2_decode_hemi;

static t_class *ambi2_decode_hemi_class;

static void ambi2_decode_hemi_copy_matrixrow_to_rowbuffer(t_ambi2_decode_hemi *x, int row)
{
	int n_ambi2 = 2*x->x_n_ambi;
	int i;
	double *dw=x->x_inv_work2;
	double *db=x->x_inv_buf2;

	dw += row*n_ambi2;
	for(i=0; i<n_ambi2; i++)
		*db++ = *dw++;
}

static void ambi2_decode_hemi_copy_rowbuffer_to_matrixrow(t_ambi2_decode_hemi *x, int row)
{
	int n_ambi2 = 2*x->x_n_ambi;
	int i;
	double *dw=x->x_inv_work2;
	double *db=x->x_inv_buf2;

	dw += row*n_ambi2;
	for(i=0; i<n_ambi2; i++)
		*dw++ = *db++;
}

static void ambi2_decode_hemi_copy_matrixrow_to_matrixrow(t_ambi2_decode_hemi *x, int src_row, int dst_row)
{
	int n_ambi2 = 2*x->x_n_ambi;
	int i;
	double *dw_src=x->x_inv_work2;
	double *dw_dst=x->x_inv_work2;

	dw_src += src_row*n_ambi2;
	dw_dst += dst_row*n_ambi2;
	for(i=0; i<n_ambi2; i++)
		*dw_dst++ = *dw_src++;
}

static void ambi2_decode_hemi_exchange_two_matrixrows(t_ambi2_decode_hemi *x, int row1, int row2)
{
	ambi2_decode_hemi_copy_matrixrow_to_rowbuffer(x, row1);
	ambi2_decode_hemi_copy_matrixrow_to_matrixrow(x, row2, row1);
	ambi2_decode_hemi_copy_rowbuffer_to_matrixrow(x, row2);
}

static void ambi2_decode_hemi_multiply_matrixrow_by_scalar(t_ambi2_decode_hemi *x, int row, double mul)
{
	int n_ambi2 = 2*x->x_n_ambi;
	int i;
	double *dw=x->x_inv_work2;

	dw += row*n_ambi2;
	for(i=0; i<n_ambi2; i++)
	{
		(*dw) *= mul;
		dw++;
	}
}

static void ambi2_decode_hemi_multiply_rowbuffer_by_scalar_and_add_to_matrixrow(t_ambi2_decode_hemi *x, double mul, int row)
{
	int n_ambi2 = 2*x->x_n_ambi;
	int i;
	double *dw=x->x_inv_work2;
	double *db=x->x_inv_buf2;

	dw += row*n_ambi2;
	for(i=0; i<n_ambi2; i++)
	{
		*dw += (*db)*mul;
		dw++;
		db++;
	}
}

static int ambi2_decode_hemi_which_element_of_column_is_not_equal_zero(t_ambi2_decode_hemi *x, int col, int start_row)
{
	int n_ambi = x->x_n_ambi;
	int n_ambi2 = 2*n_ambi;
	int i, j;
	double *dw=x->x_inv_work2;
	double singrange=x->x_sing_range;
	int ret=-1;

	dw += start_row*n_ambi2 + col;
	j = 0;
	for(i=start_row; i<n_ambi; i++)
	{
		if((*dw > singrange) || (*dw < -singrange))
		{
			ret = i;
			i = n_ambi+1;
		}
		dw += n_ambi2;
	}
	return(ret);
}

static void ambi2_decode_hemi_transpose_back_to_ls_encode(t_ambi2_decode_hemi *x)
{
	double *vec, *transp=x->x_transp;
	double *straight=x->x_ls_encode;
	int n_ls=x->x_n_real_ls+x->x_n_pht_ls;
	int n_ambi=x->x_n_ambi;
	int i, j;
  
	for(j=0; j<n_ambi; j++)
	{
		vec = transp;
		for(i=0; i<n_ls; i++)
		{
			*straight++ = *vec;
			vec += n_ambi;
		}
		transp++;
	}
}

static void ambi2_decode_hemi_multiply_ls_encode_by_its_transposed_matrix(t_ambi2_decode_hemi *x)
{
	double *vec1, *beg1=x->x_ls_encode;
	double *vec2, *beg2=x->x_ls_encode;
	double *inv=x->x_inv_work1;
	double sum;
	int n_ls=x->x_n_real_ls+x->x_n_pht_ls;
	int n_ambi=x->x_n_ambi;
	int i, j, k;

	for(k=0; k<n_ambi; k++)
	{
		beg2=x->x_ls_encode;
		for(j=0; j<n_ambi; j++)
		{
			sum = 0.0;
			vec1 = beg1;
			vec2 = beg2;
			for(i=0; i<n_ls; i++)
			{
				sum += *vec1++ * *vec2++;
			}
			beg2 += n_ls;
			*inv++ = sum;
		}
		beg1 += n_ls;
	}
}

static void ambi2_decode_hemi_invert_matrix_via_gauss(t_ambi2_decode_hemi *x)
{
	int n_ambi = x->x_n_ambi;
	int n_ambi2 = 2*n_ambi;
	int i, j, nz;
	int r,c;
	double *src=x->x_inv_work1;
	double *db=x->x_inv_work2;
	double rcp, *dv;

	dv = db;
	for(i=0; i<n_ambi; i++) /* init */
	{
		for(j=0; j<n_ambi; j++)
		{
			*dv++ = *src++;
		}
		for(j=0; j<n_ambi; j++)
		{
			if(j == i)
				*dv++ = 1.0;
			else
				*dv++ = 0.0;
		}
	}

		/* make 1 in main-diagonale, and 0 below */
	for(i=0; i<n_ambi; i++)
	{
		nz = ambi2_decode_hemi_which_element_of_column_is_not_equal_zero(x, i, i);
		if(nz < 0)
		{
			post("ambi2_decode_hemi ERROR: matrix_inverse singular !!!!");
			return;
		}
		else
		{
			if(nz != i)
				ambi2_decode_hemi_exchange_two_matrixrows(x, i, nz);
			dv = db + i*n_ambi2 + i;
			rcp = 1.0 /(*dv);
			ambi2_decode_hemi_multiply_matrixrow_by_scalar(x, i, rcp);
			ambi2_decode_hemi_copy_matrixrow_to_rowbuffer(x, i);
			for(j=i+1; j<n_ambi; j++)
			{
				dv += n_ambi2;
				rcp = -(*dv);
				ambi2_decode_hemi_multiply_rowbuffer_by_scalar_and_add_to_matrixrow(x, rcp, j);
			}
		}
	}

			/* make 0 above the main diagonale */
	for(i=n_ambi-1; i>=0; i--)
	{
		dv = db + i*n_ambi2 + i;
		ambi2_decode_hemi_copy_matrixrow_to_rowbuffer(x, i);
		for(j=i-1; j>=0; j--)
		{
			dv -= n_ambi2;
			rcp = -(*dv);
			ambi2_decode_hemi_multiply_rowbuffer_by_scalar_and_add_to_matrixrow(x, rcp, j);
		}
	}

	post("ambi2_decode_hemi OK: matrix_inverse regular");
}

static void ambi2_decode_hemi_multiply_transposed_ls_encode_by_inverted_matrix_and_weight_it(t_ambi2_decode_hemi *x)
{
	int n_ls=x->x_n_real_ls+x->x_n_pht_ls;
	int n_ambi=x->x_n_ambi;
	int n_ambi2=2*n_ambi;
	int i, j, k;
	double *vec1, *beg1=x->x_transp;
	double *vec2, *beg2=x->x_inv_work2+n_ambi;
	double *vec3=x->x_prod;
	double *acw_vec=x->x_ambi_order_weight;
	double sum;
  
	for(k=0; k<n_ls; k++)
	{
		beg2=x->x_inv_work2+n_ambi;
		for(j=0; j<n_ambi; j++)
		{
			sum = 0.0;
			vec1 = beg1;
			vec2 = beg2;
			for(i=0; i<n_ambi; i++)
			{
				sum += *vec1++ * *vec2;
				vec2 += n_ambi2;
			}
			beg2++;
			*vec3++ = sum * acw_vec[j];
		}
		beg1 += n_ambi;
	}
}

static void ambi2_decode_hemi_begin_pseudo_inverse(t_ambi2_decode_hemi *x)
{
	t_atom *at=x->x_at;
	int i, n=x->x_n_real_ls*x->x_n_ambi;
	double *dv1=x->x_prod;

	ambi2_decode_hemi_transpose_back_to_ls_encode(x);
	ambi2_decode_hemi_multiply_ls_encode_by_its_transposed_matrix(x);
	ambi2_decode_hemi_invert_matrix_via_gauss(x);
	ambi2_decode_hemi_multiply_transposed_ls_encode_by_inverted_matrix_and_weight_it(x);
	at += 2;
	for(i=0; i<n; i++)
	{
		SETFLOAT(at, (t_float)(*dv1));
		dv1++;
		at++;
	}
}

static void ambi2_decode_hemi_ipht_ireal_muladd(t_ambi2_decode_hemi *x, t_symbol *s, int argc, t_atom *argv)
{
	t_atom *at=x->x_at;
	int i, n=x->x_n_ambi;
	int pht_index, real_index;
	double mw;
	t_float dat1;
	double *dv2=x->x_prod;

  if(argc != 3)
	{
    post("ambi2_decode_hemi ERROR: ipht_ireal_muladd needs 2 indices + 1 mirror_weight: <float> phantom_ls_index [-] and <float> real_ls_index [-] and <float> mirror_weight_element [-]");
		return;
	}
	pht_index = (int)atom_getint(argv++) - 1;
	real_index = (int)atom_getint(argv++) - 1;
	mw = (double)atom_getfloat(argv);

	if(pht_index < 0)
		pht_index = 0;
	if(real_index < 0)
		real_index = 0;
	if(real_index >= x->x_n_real_ls)
		real_index = x->x_n_real_ls - 1;
	if(pht_index >= x->x_n_pht_ls)
		pht_index = x->x_n_pht_ls - 1;

	at += 2 + (real_index)*x->x_n_ambi;
	dv2 += (x->x_n_real_ls+pht_index)*x->x_n_ambi;
	for(i=0; i<n; i++)
	{
		dat1 = atom_getfloat(at);
		SETFLOAT(at, dat1 + (t_float)(*dv2*mw));
		dv2++;
		at++;
	}
}

static void ambi2_decode_hemi_end_pseudo_inverse(t_ambi2_decode_hemi *x)
{
	outlet_anything(x->x_obj.ob_outlet, x->x_s_matrix, x->x_n_ambi*x->x_n_real_ls+2, x->x_at);
}

static void ambi2_decode_hemi_encode_ls_3d(t_ambi2_decode_hemi *x, int argc, t_atom *argv, int mode)
{// store it in transposed encoded loudspeaker matrix
  double theta, phi, *dw = x->x_transp;
  int index, i, j, k, n=x->x_n_order+1;
  
  if(argc != 3)
	{
    if(mode == AMBI2_LS_REAL)
		  post("ambi2_decode_hemi ERROR: real_ls needs 1 index and 2 angles: <float> real_ls_index [-] and <float> elevation_theta [degree] and <float> azimuth_phi [degree]");
    else
      post("ambi2_decode_hemi ERROR: pht_ls needs 1 index and 2 angles: <float> phantom_ls_index [-] and <float> elevation_theta [degree] and <float> azimuth_phi [degree]");
		return;
	}
	index = (int)atom_getint(argv++) - 1;
  theta = (double)atom_getfloat(argv++) * x->x_pi_over_180;
  phi = (double)atom_getfloat(argv) * x->x_pi_over_180;
  
  if(index < 0)
		index = 0;
	
	if(mode == AMBI2_LS_REAL)
	{
		if(index >= x->x_n_real_ls)
			index = x->x_n_real_ls - 1;
	}
	else if(mode == AMBI2_LS_PHT)
	{
		if(x->x_n_pht_ls)
		{
      if(index > x->x_n_real_ls)
				index -= x->x_n_real_ls;
			if(index >= x->x_n_pht_ls)
				index = x->x_n_pht_ls - 1;
			index += x->x_n_real_ls;
		}
		else
			return;
	}
	else
		return;
  
  dw += index * x->x_n_ambi;
  
  iem_ambi2_calc_legendre(x->x_legendre, x->x_n_order+1, theta);
  iem_ambi2_calc_co_si(x->x_co, x->x_si, x->x_n_order+1, phi);
  
  *dw++ = 1.0; // cos(0) = 1.0; W-Channel;
  for(i=1,j=n; i<n; i++,j+=n)
  {
    for(k=i; k>0; k-=2)
    {
      *dw++ = x->x_semi_norm[j+k]*x->x_legendre[j+k]*x->x_co[k];
      *dw++ = x->x_semi_norm[j+k]*x->x_legendre[j+k]*x->x_si[k];
    }
    if(k==0)
      *dw++ = x->x_legendre[j];
  }
}

static void ambi2_decode_hemi_real_ls(t_ambi2_decode_hemi *x, t_symbol *s, int argc, t_atom *argv)
{
	ambi2_decode_hemi_encode_ls_3d(x, argc, argv, AMBI2_LS_REAL);
}

static void ambi2_decode_hemi_pht_ls(t_ambi2_decode_hemi *x, t_symbol *s, int argc, t_atom *argv)
{
	ambi2_decode_hemi_encode_ls_3d(x, argc, argv, AMBI2_LS_PHT);
}

static void ambi2_decode_hemi_ambi_weight(t_ambi2_decode_hemi *x, t_symbol *s, int argc, t_atom *argv)
{
	if(argc > x->x_n_order)
	{
		int i, j, k=0, m, n=x->x_n_order;
		double d;

		x->x_ambi_order_weight[k] = (double)atom_getfloat(argv++);
		k++;
    for(i=1; i<=n; i++)
    {
      d = (double)atom_getfloat(argv++);
      m = i + 1;
      for(j=0; j<m; j++)
      {
        x->x_ambi_order_weight[k] = d;
        k++;
			}
		}
	}
	else
    post("ambi2_decode_hemi ERROR: ambi_weight needs a list of %d <float> weights [-]", x->x_n_order+1);
}

static void ambi2_decode_hemi_sing_range(t_ambi2_decode_hemi *x, t_floatarg f)
{
	if(f < 0.0f)
		x->x_sing_range = -(double)f;
	else
		x->x_sing_range = (double)f;
}

static void ambi2_decode_hemi_free(t_ambi2_decode_hemi *x)
{
	freebytes(x->x_inv_work1, x->x_n_ambi * x->x_n_ambi * sizeof(double));
	freebytes(x->x_inv_work2, 2 * x->x_n_ambi * x->x_n_ambi * sizeof(double));
	freebytes(x->x_inv_buf2, 2 * x->x_n_ambi * sizeof(double));
	freebytes(x->x_transp, (x->x_n_real_ls+x->x_n_pht_ls) * x->x_n_ambi * sizeof(double));
	freebytes(x->x_ls_encode, (x->x_n_real_ls+x->x_n_pht_ls) * x->x_n_ambi * sizeof(double));
	freebytes(x->x_prod, (x->x_n_real_ls+x->x_n_pht_ls) * x->x_n_ambi * sizeof(double));
	freebytes(x->x_ambi_order_weight, x->x_n_ambi * sizeof(double));
	freebytes(x->x_at, (x->x_n_real_ls * x->x_n_ambi + 2) * sizeof(t_atom));
  freebytes(x->x_si, x->x_size_si_co * sizeof(double));
  freebytes(x->x_co, x->x_size_si_co * sizeof(double));
  freebytes(x->x_legendre, x->x_size_legendre * sizeof(double));
  freebytes(x->x_semi_norm, x->x_size_legendre * sizeof(double));
}

static void *ambi2_decode_hemi_new(t_symbol *s, int argc, t_atom *argv)
{
	t_ambi2_decode_hemi *x = (t_ambi2_decode_hemi *)pd_new(ambi2_decode_hemi_class);
	int order, i;
	int n_real_ls=0;/* number of loudspeakers */
	int n_pht_ls=0;/* number of phantom_loudspeakers */

	if((argc >= 3) &&
		IS_A_FLOAT(argv,0) &&
		IS_A_FLOAT(argv,1) &&
		IS_A_FLOAT(argv,2))
	{
		order = (int)atom_getint(argv++);
		n_real_ls = (int)atom_getint(argv++);
		n_pht_ls = (int)atom_getint(argv);

		if(order < 1)
			order = 1;
		
    x->x_n_ambi = (order + 1)*(order + 2)/2;
		
		x->x_n_order = order;
    x->x_size_legendre = (order + 1)*(order + 1);
    x->x_size_si_co = order + 1;
		if(n_real_ls < 1)
			n_real_ls = 1;
		if(n_pht_ls < 0)
			n_pht_ls = 0;
		if((n_real_ls + n_pht_ls) < x->x_n_ambi)
      post("ambi2_decode_hemi WARNING: Number of Loudspeakers is less than Number of Ambisonic-Channels !!!!");
		
		x->x_n_real_ls = n_real_ls;
		x->x_n_pht_ls = n_pht_ls;
		x->x_inv_work1 = (double *)getbytes(x->x_n_ambi * x->x_n_ambi * sizeof(double));
		x->x_inv_work2 = (double *)getbytes(2 * x->x_n_ambi * x->x_n_ambi * sizeof(double));
		x->x_inv_buf2 = (double *)getbytes(2 * x->x_n_ambi * sizeof(double));
		x->x_transp = (double *)getbytes((x->x_n_real_ls+x->x_n_pht_ls) * x->x_n_ambi * sizeof(double));
		x->x_ls_encode = (double *)getbytes((x->x_n_real_ls+x->x_n_pht_ls) * x->x_n_ambi * sizeof(double));
		x->x_prod = (double *)getbytes((x->x_n_real_ls+x->x_n_pht_ls) * x->x_n_ambi * sizeof(double));
		x->x_ambi_order_weight = (double *)getbytes(x->x_n_ambi * sizeof(double));
		x->x_at = (t_atom *)getbytes((x->x_n_real_ls * x->x_n_ambi + 2) * sizeof(t_atom));
    x->x_si = (double *)getbytes(x->x_size_si_co * sizeof(double));
    x->x_co = (double *)getbytes(x->x_size_si_co * sizeof(double));
    x->x_legendre = (double *)getbytes(x->x_size_legendre * sizeof(double));
    x->x_semi_norm = (double *)getbytes(x->x_size_legendre * sizeof(double));
		x->x_s_matrix = gensym("matrix");
		x->x_pi_over_180	= 4.0 * atan(1.0) / 180.0;
		x->x_sing_range = 1.0e-10;
    x->x_co[0] = 1.0;
    x->x_si[0] = 0.0;
    x->x_legendre[0] = 1.0;
    iem_ambi2_init_semi_norm(x->x_semi_norm, x->x_n_order+1);
		for(i=0; i<x->x_n_ambi; i++)
			x->x_ambi_order_weight[i] = 1.0;
    SETFLOAT(x->x_at, (t_float)x->x_n_real_ls);
		SETFLOAT(x->x_at+1, (t_float)x->x_n_ambi);
    
		outlet_new(&x->x_obj, &s_list);
		return (x);
	}
	else
	{
		post("ambi2_decode_hemi ERROR: inital arguments need a list of 3 <float> arguments: <float> ambi_order and <float> number_of_real_loudspeakers and <float> number_of_canceled_phantom_speakers");
		return(0);
	}
}

void ambi2_decode_hemi_setup(void)
{
	ambi2_decode_hemi_class = class_new(gensym("ambi2_decode_hemi"), (t_newmethod)ambi2_decode_hemi_new, (t_method)ambi2_decode_hemi_free,
				   sizeof(t_ambi2_decode_hemi), 0, A_GIMME, 0);
	class_addmethod(ambi2_decode_hemi_class, (t_method)ambi2_decode_hemi_real_ls, gensym("real_ls"), A_GIMME, 0);
	class_addmethod(ambi2_decode_hemi_class, (t_method)ambi2_decode_hemi_pht_ls, gensym("pht_ls"), A_GIMME, 0);
	class_addmethod(ambi2_decode_hemi_class, (t_method)ambi2_decode_hemi_ambi_weight, gensym("ambi_weight"), A_GIMME, 0);
	class_addmethod(ambi2_decode_hemi_class, (t_method)ambi2_decode_hemi_sing_range, gensym("sing_range"), A_DEFFLOAT, 0);
	class_addmethod(ambi2_decode_hemi_class, (t_method)ambi2_decode_hemi_begin_pseudo_inverse, gensym("begin_pseudo_inverse"), 0);
	class_addmethod(ambi2_decode_hemi_class, (t_method)ambi2_decode_hemi_ipht_ireal_muladd, gensym("ipht_ireal_muladd"), A_GIMME, 0);
	class_addmethod(ambi2_decode_hemi_class, (t_method)ambi2_decode_hemi_end_pseudo_inverse, gensym("end_pseudo_inverse"), 0);
}
