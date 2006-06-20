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

/* -------------------------- spec2_clip_min~ ------------------------------ */
static t_class *sigspec2_clip_min_class;

typedef struct _sigspec2_clip_min
{
	t_object	x_obj;
	t_float		x_msi;
} t_sigspec2_clip_min;

static t_int *sigspec2_clip_min_perform(t_int *w)
{
	t_float *in = (float *)(w[1]);
	t_float *min = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int i, n = (t_int)(w[4]);

	for(i=0; i<=n; i++)
	{
		if(in[i] < min[i])
			out[i] = min[i];
		else
			out[i] = in[i];
	}
	return(w+5);
}

static t_int *sigspec2_clip_min_perf16(t_int *w)
{
	t_float *in = (float *)(w[1]);
	t_float *min = (float *)(w[2]);
	t_float *out = (float *)(w[3]);
	int n = (t_int)(w[4]);

	while(n)
	{
		if(in[0] < min[0])
			out[0] = min[0];
		else
			out[0] = in[0];

		if(in[1] < min[1])
			out[1] = min[1];
		else
			out[1] = in[1];

		if(in[2] < min[2])
			out[2] = min[2];
		else
			out[2] = in[2];

		if(in[3] < min[3])
			out[3] = min[3];
		else
			out[3] = in[3];

		if(in[4] < min[4])
			out[4] = min[4];
		else
			out[4] = in[4];

		if(in[5] < min[5])
			out[5] = min[5];
		else
			out[5] = in[5];

		if(in[6] < min[6])
			out[6] = min[6];
		else
			out[6] = in[6];

		if(in[7] < min[7])
			out[7] = min[7];
		else
			out[7] = in[7];

		if(in[8] < min[8])
			out[8] = min[8];
		else
			out[8] = in[8];

		if(in[9] < min[9])
			out[9] = min[9];
		else
			out[9] = in[9];

		if(in[10] < min[10])
			out[10] = min[10];
		else
			out[10] = in[10];

		if(in[11] < min[11])
			out[11] = min[11];
		else
			out[11] = in[11];

		if(in[12] < min[12])
			out[12] = min[12];
		else
			out[12] = in[12];

		if(in[13] < min[13])
			out[13] = min[13];
		else
			out[13] = in[13];

		if(in[14] < min[14])
			out[14] = min[14];
		else
			out[14] = in[14];

		if(in[15] < min[15])
			out[15] = min[15];
		else
			out[15] = in[15];

		in += 16;
		min += 16;
		out += 16;
		n -= 16;
	}
	if(in[0] < min[0])
		out[0] = min[0];
	else
		out[0] = in[0];
	return(w+5);
}

static void sigspec2_clip_min_dsp(t_sigspec2_clip_min *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(n&15)
		dsp_add(sigspec2_clip_min_perform, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
	else
		dsp_add(sigspec2_clip_min_perf16, 4, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, n);
}

static void *sigspec2_clip_min_new(void)
{
	t_sigspec2_clip_min *x = (t_sigspec2_clip_min *)pd_new(sigspec2_clip_min_class);

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_clip_min_free(t_sigspec2_clip_min *x)
{
}

void sigspec2_clip_min_setup(void)
{
	sigspec2_clip_min_class = class_new(gensym("spec2_clip_min~"), (t_newmethod)sigspec2_clip_min_new, (t_method)sigspec2_clip_min_free,
		sizeof(t_sigspec2_clip_min), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_clip_min_class, t_sigspec2_clip_min, x_msi);
	class_addmethod(sigspec2_clip_min_class, (t_method)sigspec2_clip_min_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_clip_min_class, gensym("iemhelp2/help-spec2_clip_min~"));
}
