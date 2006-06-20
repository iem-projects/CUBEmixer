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

/* -------------------------- spec2_add~ ------------------------------ */
static t_class *sigspec2_add_class;

typedef struct _sigspec2_add
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_add;

static t_int *sigspec2_add_perform(t_int *w)
{
	t_float *in1 = (float *)(w[1]);
	t_float *in2 = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int i, n = (t_int)(w[4]);

	for(i=0; i<=n; i++)
	{
		out[i] = in1[i] + in2[i];
	}
	return(w+5);
}

static t_int *sigspec2_add_perf16(t_int *w)
{
	t_float *in1 = (float *)(w[1]);
	t_float *in2 = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int n = (t_int)(w[4]);

	while(n)
	{
		out[0] = in1[0] + in2[0];
		out[1] = in1[1] + in2[1];
		out[2] = in1[2] + in2[2];
		out[3] = in1[3] + in2[3];
		out[4] = in1[4] + in2[4];
		out[5] = in1[5] + in2[5];
		out[6] = in1[6] + in2[6];
		out[7] = in1[7] + in2[7];
		out[8] = in1[8] + in2[8];
		out[9] = in1[9] + in2[9];
		out[10] = in1[10] + in2[10];
		out[11] = in1[11] + in2[11];
		out[12] = in1[12] + in2[12];
		out[13] = in1[13] + in2[13];
		out[14] = in1[14] + in2[14];
		out[15] = in1[15] + in2[15];
	

		in1 += 16;
		in2 += 16;
		out += 16;
		n -= 16;
	}
	out[0] = in1[0] + in2[0];
	return(w+5);
}

static void sigspec2_add_dsp(t_sigspec2_add *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(n&15)
		dsp_add(sigspec2_add_perform, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
	else
		dsp_add(sigspec2_add_perf16, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
}

static void *sigspec2_add_new(void)
{
	t_sigspec2_add *x = (t_sigspec2_add *)pd_new(sigspec2_add_class);

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_add_free(t_sigspec2_add *x)
{
}

void sigspec2_add_setup(void)
{
	sigspec2_add_class = class_new(gensym("spec2+~"), (t_newmethod)sigspec2_add_new, (t_method)sigspec2_add_free,
		sizeof(t_sigspec2_add), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_add_class, t_sigspec2_add, x_msi);
	class_addmethod(sigspec2_add_class, (t_method)sigspec2_add_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_add_class, gensym("iemhelp2/help-spec2_add~"));
}


/* -------------------------- spec2_add_scalar~ ------------------------------ */
static t_class *sigspec2_add_scalar_class;

typedef struct _sigspec2_add_scalar
{
	t_object	x_obj;
	t_float		x_f;
	t_float		x_msi;
} t_sigspec2_add_scalar;

static void sigspec2_add_scalar_ft1(t_sigspec2_add_scalar *x, t_floatarg f)
{
    x->x_f = f;
}

static t_int *sigspec2_add_scalar_perform(t_int *w)
{
	t_float *io = (float *)(w[1]);
	t_sigspec2_add_scalar *x = (t_sigspec2_add_scalar *)(w[2]);
	int i, n = (t_int)(w[3]);
	t_float f = x->x_f;

	for(i=0; i<=n; i++)
	{
		io[i] += f;
	}
	return(w+4);
}

static t_int *sigspec2_add_scalar_perf16(t_int *w)
{
	t_float *io = (float *)(w[1]);
	t_sigspec2_add_scalar *x = (t_sigspec2_add_scalar *)(w[2]);
	int n = (t_int)(w[3]);
	t_float f = x->x_f;

	while(n)
	{
		io[0] += f;
		io[1] += f;
		io[2] += f;
		io[3] += f;
		io[4] += f;
		io[5] += f;
		io[6] += f;
		io[7] += f;
		io[8] += f;
		io[9] += f;
		io[10] += f;
		io[11] += f;
		io[12] += f;
		io[13] += f;
		io[14] += f;
		io[15] += f;

		io += 16;
		n -= 16;
	}
	io[0] += f;
	return(w+4);
}

static void sigspec2_add_scalar_dsp(t_sigspec2_add_scalar *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(n&15)
		dsp_add(sigspec2_add_scalar_perform, 3, sp[0]->s_vec, x, n);
	else
		dsp_add(sigspec2_add_scalar_perf16, 3, sp[0]->s_vec, x, n);
}

static void *sigspec2_add_scalar_new(t_floatarg f)
{
	t_sigspec2_add_scalar *x = (t_sigspec2_add_scalar *)pd_new(sigspec2_add_scalar_class);

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("ft1"));
	outlet_new(&x->x_obj, &s_signal);
	x->x_f = f;
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_add_scalar_free(t_sigspec2_add_scalar *x)
{
}

void sigspec2_add_scalar_setup(void)
{
	sigspec2_add_scalar_class = class_new(gensym("spec2+s~"), (t_newmethod)sigspec2_add_scalar_new, (t_method)sigspec2_add_scalar_free,
		sizeof(t_sigspec2_add_scalar), 0, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(sigspec2_add_scalar_class, t_sigspec2_add_scalar, x_msi);
	class_addmethod(sigspec2_add_scalar_class, (t_method)sigspec2_add_scalar_ft1, gensym("ft1"), A_FLOAT, 0);
	class_addmethod(sigspec2_add_scalar_class, (t_method)sigspec2_add_scalar_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_add_scalar_class, gensym("iemhelp2/help-spec2_add_scalar~"));
}



/* -------------------------- spec2_mul~ ------------------------------ */
static t_class *sigspec2_mul_class;

typedef struct _sigspec2_mul
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_mul;

static t_int *sigspec2_mul_perform(t_int *w)
{
	t_float *in1 = (float *)(w[1]);
	t_float *in2 = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int i, n = (t_int)(w[4]);

	for(i=0; i<=n; i++)
	{
		out[i] = in1[i] * in2[i];
	}
	return(w+5);
}

static t_int *sigspec2_mul_perf16(t_int *w)
{
	t_float *in1 = (float *)(w[1]);
	t_float *in2 = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int n = (t_int)(w[4]);

	while(n)
	{
		out[0] = in1[0] * in2[0];
		out[1] = in1[1] * in2[1];
		out[2] = in1[2] * in2[2];
		out[3] = in1[3] * in2[3];
		out[4] = in1[4] * in2[4];
		out[5] = in1[5] * in2[5];
		out[6] = in1[6] * in2[6];
		out[7] = in1[7] * in2[7];
		out[8] = in1[8] * in2[8];
		out[9] = in1[9] * in2[9];
		out[10] = in1[10] * in2[10];
		out[11] = in1[11] * in2[11];
		out[12] = in1[12] * in2[12];
		out[13] = in1[13] * in2[13];
		out[14] = in1[14] * in2[14];
		out[15] = in1[15] * in2[15];
	

		in1 += 16;
		in2 += 16;
		out += 16;
		n -= 16;
	}
	out[0] = in1[0] * in2[0];
	return(w+5);
}

static void sigspec2_mul_dsp(t_sigspec2_mul *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(n&15)
		dsp_add(sigspec2_mul_perform, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
	else
		dsp_add(sigspec2_mul_perf16, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
}

static void *sigspec2_mul_new(void)
{
	t_sigspec2_mul *x = (t_sigspec2_mul *)pd_new(sigspec2_mul_class);

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_mul_free(t_sigspec2_mul *x)
{
}

void sigspec2_mul_setup(void)
{
	sigspec2_mul_class = class_new(gensym("spec2*~"), (t_newmethod)sigspec2_mul_new, (t_method)sigspec2_mul_free,
		sizeof(t_sigspec2_mul), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_mul_class, t_sigspec2_mul, x_msi);
	class_addmethod(sigspec2_mul_class, (t_method)sigspec2_mul_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_mul_class, gensym("iemhelp2/help-spec2_mul~"));
}


/* -------------------------- spec2_mul_scalar~ ------------------------------ */
static t_class *sigspec2_mul_scalar_class;

typedef struct _sigspec2_mul_scalar
{
	t_object	x_obj;
	t_float		x_f;
	t_float		x_msi;
} t_sigspec2_mul_scalar;

static void sigspec2_mul_scalar_ft1(t_sigspec2_mul_scalar *x, t_floatarg f)
{
    x->x_f = f;
}

static t_int *sigspec2_mul_scalar_perform(t_int *w)
{
	t_float *io = (float *)(w[1]);
	t_sigspec2_mul_scalar *x = (t_sigspec2_mul_scalar *)(w[2]);
	int i, n = (t_int)(w[3]);
	t_float f = x->x_f;

	for(i=0; i<=n; i++)
	{
		io[i] *= f;
	}
	return(w+4);
}

static t_int *sigspec2_mul_scalar_perf16(t_int *w)
{
	t_float *io = (float *)(w[1]);
	t_sigspec2_mul_scalar *x = (t_sigspec2_mul_scalar *)(w[2]);
	int n = (t_int)(w[3]);
	t_float f = x->x_f;

	while(n)
	{
		io[0] *= f;
		io[1] *= f;
		io[2] *= f;
		io[3] *= f;
		io[4] *= f;
		io[5] *= f;
		io[6] *= f;
		io[7] *= f;
		io[8] *= f;
		io[9] *= f;
		io[10] *= f;
		io[11] *= f;
		io[12] *= f;
		io[13] *= f;
		io[14] *= f;
		io[15] *= f;

		io += 16;
		n -= 16;
	}
	io[0] *= f;
	return(w+4);
}

static void sigspec2_mul_scalar_dsp(t_sigspec2_mul_scalar *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(n&15)
		dsp_add(sigspec2_mul_scalar_perform, 3, sp[0]->s_vec, x, n);
	else
		dsp_add(sigspec2_mul_scalar_perf16, 3, sp[0]->s_vec, x, n);
}

static void *sigspec2_mul_scalar_new(t_floatarg f)
{
	t_sigspec2_mul_scalar *x = (t_sigspec2_mul_scalar *)pd_new(sigspec2_mul_scalar_class);

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("ft1"));
	outlet_new(&x->x_obj, &s_signal);
	x->x_f = f;
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_mul_scalar_free(t_sigspec2_mul_scalar *x)
{
}

void sigspec2_mul_scalar_setup(void)
{
	sigspec2_mul_scalar_class = class_new(gensym("spec2*s~"), (t_newmethod)sigspec2_mul_scalar_new, (t_method)sigspec2_mul_scalar_free,
		sizeof(t_sigspec2_mul_scalar), 0, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(sigspec2_mul_scalar_class, t_sigspec2_mul_scalar, x_msi);
	class_addmethod(sigspec2_mul_scalar_class, (t_method)sigspec2_mul_scalar_ft1, gensym("ft1"), A_FLOAT, 0);
	class_addmethod(sigspec2_mul_scalar_class, (t_method)sigspec2_mul_scalar_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_mul_scalar_class, gensym("iemhelp2/help-spec2_mul_scalar~"));
}


/* -------------------------- spec2_sub~ ------------------------------ */
static t_class *sigspec2_sub_class;

typedef struct _sigspec2_sub
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_sub;

static t_int *sigspec2_sub_perform(t_int *w)
{
	t_float *in1 = (float *)(w[1]);
	t_float *in2 = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int i, n = (t_int)(w[4]);

	for(i=0; i<=n; i++)
	{
		out[i] = in1[i] - in2[i];
	}
	return(w+5);
}

static t_int *sigspec2_sub_perf16(t_int *w)
{
	t_float *in1 = (float *)(w[1]);
	t_float *in2 = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int n = (t_int)(w[4]);

	while(n)
	{
		out[0] = in1[0] - in2[0];
		out[1] = in1[1] - in2[1];
		out[2] = in1[2] - in2[2];
		out[3] = in1[3] - in2[3];
		out[4] = in1[4] - in2[4];
		out[5] = in1[5] - in2[5];
		out[6] = in1[6] - in2[6];
		out[7] = in1[7] - in2[7];
		out[8] = in1[8] - in2[8];
		out[9] = in1[9] - in2[9];
		out[10] = in1[10] - in2[10];
		out[11] = in1[11] - in2[11];
		out[12] = in1[12] - in2[12];
		out[13] = in1[13] - in2[13];
		out[14] = in1[14] - in2[14];
		out[15] = in1[15] - in2[15];
	

		in1 += 16;
		in2 += 16;
		out += 16;
		n -= 16;
	}
	out[0] = in1[0] - in2[0];
	return(w+5);
}

static void sigspec2_sub_dsp(t_sigspec2_sub *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(n&15)
		dsp_add(sigspec2_sub_perform, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
	else
		dsp_add(sigspec2_sub_perf16, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
}

static void *sigspec2_sub_new(void)
{
	t_sigspec2_sub *x = (t_sigspec2_sub *)pd_new(sigspec2_sub_class);

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_sub_free(t_sigspec2_sub *x)
{
}

void sigspec2_sub_setup(void)
{
	sigspec2_sub_class = class_new(gensym("spec2-~"), (t_newmethod)sigspec2_sub_new, (t_method)sigspec2_sub_free,
		sizeof(t_sigspec2_sub), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_sub_class, t_sigspec2_sub, x_msi);
	class_addmethod(sigspec2_sub_class, (t_method)sigspec2_sub_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_sub_class, gensym("iemhelp2/help-spec2_sub~"));
}


/* ------------------------ sigspec2_abs~ ------------------------- */

static t_class *sigspec2_abs_class;

typedef struct _sigspec2_abs
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_abs;

static t_int *sigspec2_abs_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3]+1;

	while(n--)
	{	
		*in++ = fabs(*out++);
	}
	return(w+4);
}

static t_int *sigspec2_abs_perf16(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3];

	while(n)
	{	
		in[0] = fabs(out[0]);
		in[1] = fabs(out[1]);
		in[2] = fabs(out[2]);
		in[3] = fabs(out[3]);
		in[4] = fabs(out[4]);
		in[5] = fabs(out[5]);
		in[6] = fabs(out[6]);
		in[7] = fabs(out[7]);
		in[8] = fabs(out[8]);
		in[9] = fabs(out[9]);
		in[10] = fabs(out[10]);
		in[11] = fabs(out[11]);
		in[12] = fabs(out[12]);
		in[13] = fabs(out[13]);
		in[14] = fabs(out[14]);
		in[15] = fabs(out[15]);

		in += 16;
		out += 16;
		n -= 16;
	}
	in[0] = fabs(out[0]);
	return(w+4);
}

static void sigspec2_abs_dsp(t_sigspec2_abs *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(n&15)
		dsp_add(sigspec2_abs_perform, 3, sp[0]->s_vec, sp[0]->s_vec, n);
	else
		dsp_add(sigspec2_abs_perf16, 3, sp[0]->s_vec, sp[0]->s_vec, n);
}

static void *sigspec2_abs_new(void)
{
	t_sigspec2_abs *x = (t_sigspec2_abs *)pd_new(sigspec2_abs_class);
	
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

void sigspec2_abs_setup(void)
{
	sigspec2_abs_class = class_new(gensym("spec2_abs~"), (t_newmethod)sigspec2_abs_new,
		0, sizeof(t_sigspec2_abs), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_abs_class, t_sigspec2_abs, x_msi);
	class_addmethod(sigspec2_abs_class, (t_method)sigspec2_abs_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_abs_class, gensym("iemhelp2/help-spec2_abs~"));
}


/* ------------------------ sigspec2_sqrt~ ------------------------- */

static t_class *sigspec2_sqrt_class;

#define SPEC2DUMTAB1SIZE 256
#define SPEC2DUMTAB2SIZE 1024

static t_float spec2_rsqrt_exptab[SPEC2DUMTAB1SIZE], spec2_rsqrt_mantissatab[SPEC2DUMTAB2SIZE];

static void init_spec2_rsqrt(void)
{
	int i;
	
	for (i=0; i<SPEC2DUMTAB1SIZE; i++)
	{
		t_float f;
		long l = (i ? (i == SPEC2DUMTAB1SIZE-1 ? SPEC2DUMTAB1SIZE-2 : i) : 1)<< 23;

		*(long *)(&f) = l;
		spec2_rsqrt_exptab[i] = 1.0f/sqrt(f);	
	}

	for (i=0; i<SPEC2DUMTAB2SIZE; i++)
	{
		t_float f = 1.0f + (1.0f / (t_float)SPEC2DUMTAB2SIZE) * (t_float)i;

		spec2_rsqrt_mantissatab[i] = 1.0f / sqrt(f);	
	}
}

typedef struct _sigspec2_sqrt
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_sqrt;

static t_int *sigspec2_sqrt_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = w[3]+1;

	while(n--)
	{	
		t_float f = *in;
		long l = *(long *)(in++);

		if(f < 0.0f)
			*out++ = 0.0f;
		else
		{
			t_float g = spec2_rsqrt_exptab[(l >> 23) & 0xff] * spec2_rsqrt_mantissatab[(l >> 13) & 0x3ff];

			*out++ = g*(1.5f - 0.5f * g * g * f);
		}
	}
	return(w+4);
}

static void sigspec2_sqrt_dsp(t_sigspec2_sqrt *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	dsp_add(sigspec2_sqrt_perform, 3, sp[0]->s_vec, sp[0]->s_vec, n);
}

static void *sigspec2_sqrt_new(void)
{
	t_sigspec2_sqrt *x = (t_sigspec2_sqrt *)pd_new(sigspec2_sqrt_class);
	
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

void sigspec2_sqrt_setup(void)
{
	init_spec2_rsqrt();
	sigspec2_sqrt_class = class_new(gensym("spec2_sqrt~"), (t_newmethod)sigspec2_sqrt_new,
		0, sizeof(t_sigspec2_sqrt), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_sqrt_class, t_sigspec2_sqrt, x_msi);
	class_addmethod(sigspec2_sqrt_class, (t_method)sigspec2_sqrt_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_sqrt_class, gensym("iemhelp2/help-spec2_sqrt~"));
}
