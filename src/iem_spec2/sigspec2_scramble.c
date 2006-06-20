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


/* -- spec2_scramble~ - shift spectral bins to left (lower, negative) or to right (higher, positiv) --- */

typedef struct sigspec2_scramble
{
	t_object	x_obj;
	int				x_blocksize;
	float			x_scramble_width;
	float			*x_spec;
	float			x_msi;
} t_sigspec2_scramble;

t_class *sigspec2_scramble_class;

static t_int *sigspec2_scramble_perform(t_int *w)
{
	float *in = (float *)(w[1]);
	float *out = (float *)(w[2]);
	t_sigspec2_scramble *x = (t_sigspec2_scramble *)(w[3]);
	int i, j, n = (t_int)(w[4]);
	float *spec=x->x_spec;
	float scramble_width=x->x_scramble_width;

	if(scramble_width < 0)
	{
		scramble_width = 0;
		x->x_scramble_width = 0;
	}
	if(scramble_width > n)
	{
		scramble_width = n;
		x->x_scramble_width = n;
	}
	
	for(i=1, j=0; i<n; i++, j++)/* copy spec into buffer */
		spec[i] = in[m-i];

	if(add >= 0)
	{
		for(i=0; i<add; i++)/* clear residal of spec-buffer */
			out[i] = 0.0f;
		for(i=add, j=0; i<n; i++, j++)/* copy spec into buffer */
			out[i] = spec[j];
	}
	else
	{
		for(i=0, j=add; i<n; i++, j++)/* copy spec into buffer */
			out[i] = spec[j];
		for(i=add; i<n; i++)/* clear residal of spec-buffer */
			out[i] = 0.0f;
	}
	return(w+5);
}

static void sigspec2_scramble_scramble_width(t_sigspec2_scramble *x, t_floatarg scramble_width)
{
	x->x_scramble_width = scramble_width;
}

static void sigspec2_scramble_dsp(t_sigspec2_scramble *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(!x->x_blocksize)
	{
		x->x_spec = (t_float *)getbytes(3*n*sizeof(t_float));
		x->x_blocksize = n;
	}
	else if(x->x_blocksize != n)
	{
		x->x_spec = (t_float *)resizebytes(x->x_spec, 3*x->x_blocksize*sizeof(t_float), 3*n*sizeof(t_float));
		x->x_blocksize = n;
	}
	dsp_add(sigspec2_scramble_perform, 4, sp[0]->s_vec, sp[1]->s_vec, x, n);
}

static void *sigspec2_scramble_new(t_floatarg scramble_width)
{
	t_sigspec2_scramble *x = (t_sigspec2_scramble *)pd_new(sigspec2_scramble_class);

	outlet_new(&x->x_obj, &s_signal);
	x->x_blocksize = 0;
	x->x_scramble_width = scramble_width;
	x->x_spec = (float *)0;
	return (x);
}

static void sigspec2_scramble_free(t_sigspec2_scramble *x)
{
	if(x->x_spec)
		freebytes(x->x_spec, 3 * x->x_blocksize * sizeof(float));
}

void sigspec2_scramble_setup(void)
{
	sigspec2_scramble_class = class_new(gensym("spec2_scramble~"), (t_newmethod)sigspec2_scramble_new,
				0, sizeof(t_sigspec2_scramble), 0, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(sigspec2_scramble_class, t_sigspec2_scramble, x_msi);
	class_addmethod(sigspec2_scramble_class, (t_method)sigspec2_scramble_dsp, gensym("dsp"), 0);
	class_addfloat(sigspec2_scramble_class, (t_method)sigspec2_scramble_scramble_width);
	class_sethelpsymbol(sigspec2_scramble_class, gensym("iemhelp/help-spec2_scramble~"));
}
