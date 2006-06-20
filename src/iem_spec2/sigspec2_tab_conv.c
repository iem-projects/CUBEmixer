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


/* -- spec2_tab_conv~ - convolute a spectrum with a table --- */

typedef struct sigspec2_tab_conv
{
	t_object	x_obj;
	t_float		*x_spec;
	t_float		*x_beg_array;
	int				x_blocksize;
	int				x_winsize;
	int				x_has_changed;
	t_symbol	*x_sym_array;
	t_float		x_msi;
} t_sigspec2_tab_conv;

t_class *sigspec2_tab_conv_class;

static void sigspec2_tab_conv_set(t_sigspec2_tab_conv *x, t_symbol *s, int argc, t_atom *argv)
{
	if(argc >= 2)
	{
		x->x_sym_array = (t_symbol *)(atom_getsymbol(argv));
		argv++;
		x->x_winsize = (int)(atom_getint(argv));
		x->x_has_changed = 1;
	}
}

static t_int *sigspec2_tab_conv_perform(t_int *w)
{
	float *in = (float *)(w[1]);
	float *out = (float *)(w[2]);
	t_sigspec2_tab_conv *x = (t_sigspec2_tab_conv *)(w[3]);
	float sum=0.0f;
	float *vec1, *vec2, *vec3, *win;
	int i, m, n = (int)(w[4])+1;
	int j, ws=x->x_winsize;

	vec2 = x->x_spec + n;
	vec1 = vec2;
	vec3 = vec2 + 2*n - 2;

	for(i=0; i<n; i++)
	{
		sum = in[i];
		*vec2++ = sum;
		*vec1-- = sum;
		*vec3-- = sum;
	}
	vec2 = x->x_spec + n - ws/2;
	win = x->x_beg_array;

	for(i=0; i<n; i++)
	{
		sum = 0.0f;
		for(j=0; j<ws; j++)
			sum += win[j] * vec2[j];
		out[i] = sum;
		vec2++;
	}
	return(w+5);
}

static void sigspec2_tab_conv_dsp(t_sigspec2_tab_conv *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;
	t_garray *a;
	int n_points;

	if(x->x_has_changed)
	{
		x->x_has_changed = 0;
		if(!(a = (t_garray *)pd_findbyclass(x->x_sym_array, garray_class)))
		{
			if(*x->x_sym_array->s_name)
				error("spec2_tab_conv~: %s: no such array", x->x_sym_array->s_name);
		}
		else if(!garray_getfloatarray(a, &n_points, &x->x_beg_array))
			error("%s: bad template for spec2_tab_conv~", x->x_sym_array->s_name);
		else 
		{
			if(n_points > (n+1))
				n_points = n+1;
			if(x->x_winsize < 0)
				x->x_winsize = 0;
			if(x->x_winsize > n_points)
				x->x_winsize = n_points;
		}
	}

	if(!x->x_blocksize)
	{
		x->x_spec = (t_float *)getbytes(3*(n+1)*sizeof(t_float));
		x->x_blocksize = n;
	}
	else if(x->x_blocksize != n)
	{
		x->x_spec = (t_float *)resizebytes(x->x_spec, 3*(x->x_blocksize+1)*sizeof(t_float), 3*(n+1)*sizeof(t_float));
		x->x_blocksize = n;
	}

	dsp_add(sigspec2_tab_conv_perform, 4, sp[0]->s_vec, sp[1]->s_vec, x, n);
}

static void sigspec2_tab_conv_free(t_sigspec2_tab_conv *x)
{
	if(x->x_spec)
		freebytes(x->x_spec, 3*(x->x_blocksize+1)*sizeof(t_float));
}

static void *sigspec2_tab_conv_new(t_symbol *s, int argc, t_atom *argv)
{
	t_sigspec2_tab_conv *x = (t_sigspec2_tab_conv *)pd_new(sigspec2_tab_conv_class);

	if(argc >= 2)
	{
		x->x_sym_array = (t_symbol *)(atom_getsymbol(argv));
		argv++;
		x->x_winsize = (int)(atom_getint(argv));
		x->x_spec = (t_float *)0;
		x->x_beg_array = (t_float *)0;
		x->x_blocksize = 0;
		x->x_has_changed = 1;
		outlet_new(&x->x_obj, &s_signal);
		x->x_msi = 0.0f;
		return(x);
	}
	else
	{
		post("spec2_tab_conv-ERROR: needs 2 args: <sym> convolution-array-name + <float> convolution-array-size !!!");
		return(0);
	}
}

void sigspec2_tab_conv_setup(void)
{
	sigspec2_tab_conv_class = class_new(gensym("spec2_tab_conv~"), (t_newmethod)sigspec2_tab_conv_new,
		(t_method)sigspec2_tab_conv_free, sizeof(t_sigspec2_tab_conv), 0, A_GIMME, 0);
	CLASS_MAINSIGNALIN(sigspec2_tab_conv_class, t_sigspec2_tab_conv, x_msi);
	class_addmethod(sigspec2_tab_conv_class, (t_method)sigspec2_tab_conv_dsp, gensym("dsp"), 0);
	class_addmethod(sigspec2_tab_conv_class, (t_method)sigspec2_tab_conv_set, gensym("set"), A_GIMME, 0);
	class_sethelpsymbol(sigspec2_tab_conv_class, gensym("iemhelp/help-spec2_tab_conv~"));
}
