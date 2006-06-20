/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_spec2 written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2005 */

#ifdef NT
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
#endif


#include "m_pd.h"
#include "iemlib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define SPEC2LOGTEN 2.302585092994f

/* ------------------------ sigspec2_rmstodb~ ------------------------- */

static t_class *sigspec2_rmstodb_class;

typedef struct _sigspec2_rmstodb
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_rmstodb;

static t_int *sigspec2_rmstodb_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3]+1;

	for(; n--; in++, out++)
	{
		float f = *in;
		
		if(f <= 0.0f)
			*out = 0.0f;
		else
		{
			float g = 100.0f + 20.0f/SPEC2LOGTEN * log(f);
			*out = (g < 0.0f ? 0.0f : g);
		}
	}
	return(w+4);
}

static void sigspec2_rmstodb_dsp(t_sigspec2_rmstodb *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	dsp_add(sigspec2_rmstodb_perform, 3, sp[0]->s_vec, sp[0]->s_vec, n);
}

static void *sigspec2_rmstodb_new(void)
{
	t_sigspec2_rmstodb *x = (t_sigspec2_rmstodb *)pd_new(sigspec2_rmstodb_class);
	
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

void sigspec2_rmstodb_setup(void)
{
	sigspec2_rmstodb_class = class_new(gensym("spec2_rmstodb~"), (t_newmethod)sigspec2_rmstodb_new,
		0, sizeof(t_sigspec2_rmstodb), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_rmstodb_class, t_sigspec2_rmstodb, x_msi);
	class_addmethod(sigspec2_rmstodb_class, (t_method)sigspec2_rmstodb_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_rmstodb_class, gensym("iemhelp2/help-spec2_rmstodb~"));
}


/* ------------------------ sigspec2_powtodb~ ------------------------- */

static t_class *sigspec2_powtodb_class;

typedef struct _sigspec2_powtodb
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_powtodb;

static t_int *sigspec2_powtodb_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3]+1;

	for(; n--; in++, out++)
	{
		float f = *in;
		
		if(f <= 0.0f)
			*out = 0.0f;
		else
		{
			float g = 100.0f + 10.0f/SPEC2LOGTEN * log(f);
			*out = (g < 0.0f ? 0.0f : g);
		}
	}
	return(w+4);
}

static void sigspec2_powtodb_dsp(t_sigspec2_powtodb *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	dsp_add(sigspec2_powtodb_perform, 3, sp[0]->s_vec, sp[0]->s_vec, n);
}

static void *sigspec2_powtodb_new(void)
{
	t_sigspec2_powtodb *x = (t_sigspec2_powtodb *)pd_new(sigspec2_powtodb_class);
	
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

void sigspec2_powtodb_setup(void)
{
	sigspec2_powtodb_class = class_new(gensym("spec2_powtodb~"), (t_newmethod)sigspec2_powtodb_new,
		0, sizeof(t_sigspec2_powtodb), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_powtodb_class, t_sigspec2_powtodb, x_msi);
	class_addmethod(sigspec2_powtodb_class, (t_method)sigspec2_powtodb_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_powtodb_class, gensym("iemhelp2/help-spec2_powtodb~"));
}


/* ------------------------ sigspec2_dbtorms~ ------------------------- */

static t_class *sigspec2_dbtorms_class;

typedef struct _sigspec2_dbtorms
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_dbtorms;

static t_int *sigspec2_dbtorms_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3]+1;

	for (; n--; in++, out++)
	{	
		t_float f = *in;
		
		if(f <= 0.0f)
			*out = 0.0f;
		else
		{
			if(f > 485.0f)
				f = 485.0f;
			*out = exp((SPEC2LOGTEN * 0.05f) * (f-100.0f));
		}
	}
	return(w+4);
}

static void sigspec2_dbtorms_dsp(t_sigspec2_dbtorms *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	dsp_add(sigspec2_dbtorms_perform, 3, sp[0]->s_vec, sp[0]->s_vec, n);
}

static void *sigspec2_dbtorms_new(void)
{
	t_sigspec2_dbtorms *x = (t_sigspec2_dbtorms *)pd_new(sigspec2_dbtorms_class);
	
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

void sigspec2_dbtorms_setup(void)
{
	sigspec2_dbtorms_class = class_new(gensym("spec2_dbtorms~"), (t_newmethod)sigspec2_dbtorms_new,
		0, sizeof(t_sigspec2_dbtorms), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_dbtorms_class, t_sigspec2_dbtorms, x_msi);
	class_addmethod(sigspec2_dbtorms_class, (t_method)sigspec2_dbtorms_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_dbtorms_class, gensym("iemhelp2/help-spec2_dbtorms~"));
}



/* ------------------------ sigspec2_dbtopow~ ------------------------- */

static t_class *sigspec2_dbtopow_class;

typedef struct _sigspec2_dbtopow
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_dbtopow;

static t_int *sigspec2_dbtopow_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3]+1;

	for (; n--; in++, out++)
	{	
		t_float f = *in;
		
		if(f <= 0.0f)
			*out = 0.0f;
		else
		{
			if(f > 870.0f)
				f = 870.0f;
			*out = exp((SPEC2LOGTEN * 0.1f) * (f-100.0f));
		}
	}
	return(w+4);
}

static void sigspec2_dbtopow_dsp(t_sigspec2_dbtopow *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	dsp_add(sigspec2_dbtopow_perform, 3, sp[0]->s_vec, sp[0]->s_vec, n);
}

static void *sigspec2_dbtopow_new(void)
{
	t_sigspec2_dbtopow *x = (t_sigspec2_dbtopow *)pd_new(sigspec2_dbtopow_class);
	
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

void sigspec2_dbtopow_setup(void)
{
	sigspec2_dbtopow_class = class_new(gensym("spec2_dbtopow~"), (t_newmethod)sigspec2_dbtopow_new,
		0, sizeof(t_sigspec2_dbtopow), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_dbtopow_class, t_sigspec2_dbtopow, x_msi);
	class_addmethod(sigspec2_dbtopow_class, (t_method)sigspec2_dbtopow_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_dbtopow_class, gensym("iemhelp2/help-spec2_dbtopow~"));
}
