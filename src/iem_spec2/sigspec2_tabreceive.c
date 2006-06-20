/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_spec2 written by Thomas Musil (c) IEM KUG Graz Austria 2002 - 2005 */

#ifdef NT
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
#endif


#include "m_pd.h"
#include "iemlib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


/* ------------------------ sigspec2_tabreceive~ ------------------------- */

static t_class *sigspec2_tabreceive_class;

typedef struct _sigspec2_tabreceive
{
	t_object	x_obj;
	t_float		*x_vec;
	t_symbol	*x_arrayname;
} t_sigspec2_tabreceive;

static void sigspec2_tabreceive_symbol(t_sigspec2_tabreceive *x, t_symbol *s)
{
	x->x_arrayname = s;
}

static t_int *sigspec2_tabreceive_perform(t_int *w)
{
	t_sigspec2_tabreceive *x = (t_sigspec2_tabreceive *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3]+1;
	t_float *vec = x->x_vec;

	if(vec)
		while(n--)
			*out++ = *vec++;
	else
		while(n--)
			*out++ = 0.0f;
	return(w+4);
}

static t_int *sigspec2_tabreceive_perf16(t_int *w)
{
	t_sigspec2_tabreceive *x = (t_sigspec2_tabreceive *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3];
	t_float *vec = x->x_vec;

	if(vec)
	{
		while(n)
		{
			out[0] = vec[0];
			out[1] = vec[1];
			out[2] = vec[2];
			out[3] = vec[3];
			out[4] = vec[4];
			out[5] = vec[5];
			out[6] = vec[6];
			out[7] = vec[7];
			out[8] = vec[8];
			out[9] = vec[9];
			out[10] = vec[10];
			out[11] = vec[11];
			out[12] = vec[12];
			out[13] = vec[13];
			out[14] = vec[14];
			out[15] = vec[15];

			vec += 16;
			out += 16;
			n -= 16;
		}
		out[0] = vec[0];
	}
	else
	{
		while(n)
		{
			out[0] = 0.0f;
			out[1] = 0.0f;
			out[2] = 0.0f;
			out[3] = 0.0f;
			out[4] = 0.0f;
			out[5] = 0.0f;
			out[6] = 0.0f;
			out[7] = 0.0f;
			out[8] = 0.0f;
			out[9] = 0.0f;
			out[10] = 0.0f;
			out[11] = 0.0f;
			out[12] = 0.0f;
			out[13] = 0.0f;
			out[14] = 0.0f;
			out[15] = 0.0f;

			out += 16;
			n -= 16;
		}
		out[0] = 0.0f;
	}

	return(w+4);
}

static void sigspec2_tabreceive_dsp(t_sigspec2_tabreceive *x, t_signal **sp)
{
	t_garray *a;
	int vecsize;
    
	if(!(a = (t_garray *)pd_findbyclass(x->x_arrayname, garray_class)))
	{
		if(*x->x_arrayname->s_name)
			error("spec2_tabreceive~: %s: no such array", x->x_arrayname->s_name);
	}
	else if(!garray_getfloatarray(a, &vecsize, &x->x_vec))
		error("%s: bad template for spec2_tabreceive~", x->x_arrayname->s_name);
	else 
	{
		int n = sp[0]->s_n;

		if(n < vecsize)
			vecsize = n;
		vecsize /= 2;
		if(vecsize&15)
			dsp_add(sigspec2_tabreceive_perform, 3, x, sp[0]->s_vec, vecsize);
		else
			dsp_add(sigspec2_tabreceive_perf16, 3, x, sp[0]->s_vec, vecsize);
	}
}

static void *sigspec2_tabreceive_new(t_symbol *s)
{
	t_sigspec2_tabreceive *x = (t_sigspec2_tabreceive *)pd_new(sigspec2_tabreceive_class);
	
	x->x_arrayname = s;
	outlet_new(&x->x_obj, &s_signal);
	return (x);
}

void sigspec2_tabreceive_setup(void)
{
	sigspec2_tabreceive_class = class_new(gensym("spec2_tabreceive~"), (t_newmethod)sigspec2_tabreceive_new,
		0, sizeof(t_sigspec2_tabreceive), 0, A_DEFSYM, 0);
	class_addmethod(sigspec2_tabreceive_class, (t_method)sigspec2_tabreceive_dsp, gensym("dsp"), 0);
	class_addsymbol(sigspec2_tabreceive_class, (t_method)sigspec2_tabreceive_symbol);
	class_sethelpsymbol(sigspec2_tabreceive_class, gensym("iemhelp/help-spec2_tabreceive~"));
}
