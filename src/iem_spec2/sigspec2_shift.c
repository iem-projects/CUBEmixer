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


/* -- spec2_shift~ - shift spectral bins to left (lower, negative) or to right (higher, positiv) --- */

typedef struct sigspec2_shift
{
	t_object	x_obj;
	int				x_blocksize;
	float			x_add;
	float			*x_spec;
	float			x_msi;
} t_sigspec2_shift;

t_class *sigspec2_shift_class;

static t_int *sigspec2_shift_perform(t_int *w)
{
	float *in = (float *)(w[1]);
	float *out = (float *)(w[2]);
	t_sigspec2_shift *x = (t_sigspec2_shift *)(w[3]);
	int i, j, n = (t_int)(w[4])+1;
	float *spec=x->x_spec;
	float add=x->x_add;

	if((add >= n) || (add <= -n))
	{
		for(i=0; i<n; i++)/* clear residal of spec-buffer */
			out[i] = 0.0f;
	}
	else
	{
		for(i=0; i<n; i++)/* copy spec into buffer */
			spec[i] = in[i];

		if(add >= 0)
		{
			for(i=0; i<add; i++)/* clear residal of spec-buffer */
				out[i] = 0.0f;
			for(j=0; i<n; i++, j++)/* copy spec into buffer */
				out[i] = spec[j];
		}
		else
		{
			add *= -1;
			for(i=0, j=add; j<n; i++, j++)/* copy spec into buffer */
				out[i] = spec[j];
			for(; i<n; i++)/* clear residal of spec-buffer */
				out[i] = 0.0f;
		}
	}
	return(w+5);
}

static void sigspec2_shift_add(t_sigspec2_shift *x, t_floatarg add)
{
	x->x_add = add;
}

static void sigspec2_shift_dsp(t_sigspec2_shift *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(!x->x_blocksize)
	{
		x->x_spec = (t_float *)getbytes((n+1)*sizeof(t_float));
		x->x_blocksize = n;
	}
	else if(x->x_blocksize != n)
	{
		x->x_spec = (t_float *)resizebytes(x->x_spec, (x->x_blocksize+1)*sizeof(t_float), (n+1)*sizeof(t_float));
		x->x_blocksize = n;
	}
	dsp_add(sigspec2_shift_perform, 4, sp[0]->s_vec, sp[1]->s_vec, x, n);
}

static void *sigspec2_shift_new(t_floatarg add)
{
	t_sigspec2_shift *x = (t_sigspec2_shift *)pd_new(sigspec2_shift_class);

	outlet_new(&x->x_obj, &s_signal);
	x->x_blocksize = 0;
	x->x_add = add;
	x->x_spec = (float *)0;
	return (x);
}

static void sigspec2_shift_free(t_sigspec2_shift *x)
{
	if(x->x_spec)
		freebytes(x->x_spec, (x->x_blocksize+1) * sizeof(float));
}

void sigspec2_shift_setup(void)
{
	sigspec2_shift_class = class_new(gensym("spec2_shift~"), (t_newmethod)sigspec2_shift_new,
				0, sizeof(t_sigspec2_shift), 0, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(sigspec2_shift_class, t_sigspec2_shift, x_msi);
	class_addmethod(sigspec2_shift_class, (t_method)sigspec2_shift_dsp, gensym("dsp"), 0);
	class_addfloat(sigspec2_shift_class, (t_method)sigspec2_shift_add);
	class_sethelpsymbol(sigspec2_shift_class, gensym("iemhelp/help-spec2_shift~"));
}
