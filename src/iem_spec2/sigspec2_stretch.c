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


/* -- spec2_stretch~ - stretch spectral bins --- */

typedef struct sigspec2_stretch
{
	t_object	x_obj;
	int				x_blocksize;
	float			x_mul;
	float			*x_spec;
	float			x_msi;
} t_sigspec2_stretch;

t_class *sigspec2_stretch_class;

static t_int *sigspec2_stretch_perform(t_int *w)
{
	float *in = (float *)(w[1]);
	float *out = (float *)(w[2]);
	t_sigspec2_stretch *x = (t_sigspec2_stretch *)(w[3]);
	int i, j, m, n = (t_int)(w[4])+1;
	float yn0, yn1, fract;
	float *spec=x->x_spec;
	float mul=x->x_mul;
	float rcp_mul = 1.0f / mul;

	for(i=0; i<n; i++)/* copy spec into buffer */
		spec[i] = in[i];

	m = (int)((float)n * mul);
	if(m > n)
		m = n;
	for(i=0; i<m; i++)/* stretch spec-buffer */
	{
		fract = (float)i * rcp_mul;
		j = (int)fract;
		fract -= (float)j;
		yn0 = spec[j];
		yn1 = spec[j+1];
		out[i] = (yn1 - yn0)*fract + yn0;
	}
	for(i=m; i<n; i++)/* clear residal of spec-buffer */
		out[i] = 0.0f;

	return(w+5);
}

static void sigspec2_stretch_mul(t_sigspec2_stretch *x, t_floatarg mul)
{
	if(mul <= 0.0f)
		mul = 1.0f;
	x->x_mul = mul;
}

static void sigspec2_stretch_dsp(t_sigspec2_stretch *x, t_signal **sp)
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
	dsp_add(sigspec2_stretch_perform, 4, sp[0]->s_vec, sp[1]->s_vec, x, n);
}

static void *sigspec2_stretch_new(t_floatarg mul)
{
	t_sigspec2_stretch *x = (t_sigspec2_stretch *)pd_new(sigspec2_stretch_class);

	outlet_new(&x->x_obj, &s_signal);
	if(mul <= 0.0f)
		mul = 1.0f;
	x->x_blocksize = 0;
	x->x_mul = mul;
	x->x_spec = (float *)0;
	return (x);
}

static void sigspec2_stretch_free(t_sigspec2_stretch *x)
{
	if(x->x_spec)
		freebytes(x->x_spec, (x->x_blocksize+1) * sizeof(float));
}

void sigspec2_stretch_setup(void)
{
	sigspec2_stretch_class = class_new(gensym("spec2_stretch~"), (t_newmethod)sigspec2_stretch_new,
				0, sizeof(t_sigspec2_stretch), 0, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(sigspec2_stretch_class, t_sigspec2_stretch, x_msi);
	class_addmethod(sigspec2_stretch_class, (t_method)sigspec2_stretch_dsp, gensym("dsp"), 0);
	class_addfloat(sigspec2_stretch_class, (t_method)sigspec2_stretch_mul);
	class_sethelpsymbol(sigspec2_stretch_class, gensym("iemhelp/help-spec2_stretch~"));
}
