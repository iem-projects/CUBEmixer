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

/* -------------------------- spec2_block_delay~ ------------------------------ */
static t_class *sigspec2_block_delay_class;

typedef struct _sigspec2_block_delay
{
	t_object	x_obj;
	t_float		*x_begmem;
	int				x_blocksize;
	t_float		x_msi;
} t_sigspec2_block_delay;

static t_int *sigspec2_block_delay_perform(t_int *w)
{
	t_float *in = (float *)(w[1]);
	t_float *out = (float *)(w[2]);
	t_sigspec2_block_delay *x = (t_sigspec2_block_delay *)(w[3]);
	int i, n = (t_int)(w[4]);
	t_float *rw_vec;

	rw_vec = x->x_begmem;
	for(i=0; i<=n; i++)
	{
		t_float f = in[i];
		out[i] = rw_vec[i];
		rw_vec[i] = f;
	}
	return(w+5);
}

static t_int *sigspec2_block_delay_perf16(t_int *w)
{
	t_float *in = (float *)(w[1]);
	t_float *out = (float *)(w[2]);
	t_sigspec2_block_delay *x = (t_sigspec2_block_delay *)(w[3]);
	int i, n = (t_int)(w[4]);
	t_float *rw_vec, ff;

	rw_vec = x->x_begmem;
	while(n)
	{
		t_float f[16];

		f[0] = in[0];
		f[1] = in[1];
		f[2] = in[2];
		f[3] = in[3];
		f[4] = in[4];
		f[5] = in[5];
		f[6] = in[6];
		f[7] = in[7];
		f[8] = in[8];
		f[9] = in[9];
		f[10] = in[10];
		f[11] = in[11];
		f[12] = in[12];
		f[13] = in[13];
		f[14] = in[14];
		f[15] = in[15];

		out[0] = rw_vec[0];
		out[1] = rw_vec[1];
		out[2] = rw_vec[2];
		out[3] = rw_vec[3];
		out[4] = rw_vec[4];
		out[5] = rw_vec[5];
		out[6] = rw_vec[6];
		out[7] = rw_vec[7];
		out[8] = rw_vec[8];
		out[9] = rw_vec[9];
		out[10] = rw_vec[10];
		out[11] = rw_vec[11];
		out[12] = rw_vec[12];
		out[13] = rw_vec[13];
		out[14] = rw_vec[14];
		out[15] = rw_vec[15];

		rw_vec[0] = f[0];
		rw_vec[1] = f[1];
		rw_vec[2] = f[2];
		rw_vec[3] = f[3];
		rw_vec[4] = f[4];
		rw_vec[5] = f[5];
		rw_vec[6] = f[6];
		rw_vec[7] = f[7];
		rw_vec[8] = f[8];
		rw_vec[9] = f[9];
		rw_vec[10] = f[10];
		rw_vec[11] = f[11];
		rw_vec[12] = f[12];
		rw_vec[13] = f[13];
		rw_vec[14] = f[14];
		rw_vec[15] = f[15];

		rw_vec += 16;
		in += 16;
		out += 16;
		n -= 16;
	}
	ff = in[0];
	out[0] = rw_vec[0];
	rw_vec[0] = ff;
	return(w+5);
}

static void sigspec2_block_delay_dsp(t_sigspec2_block_delay *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(!x->x_blocksize)/*first time*/
	{
		x->x_begmem = (t_float *)getbytes((n+1) * sizeof(t_float));
		x->x_blocksize = n;
	}
	else if(x->x_blocksize != n)
	{
		x->x_begmem = (t_float *)resizebytes(x->x_begmem, (x->x_blocksize+1)*sizeof(t_float), (n+1)*sizeof(t_float));
		x->x_blocksize = n;
	}
	if(n&15)
		dsp_add(sigspec2_block_delay_perform, 4, sp[0]->s_vec, sp[1]->s_vec, x, n);
	else
		dsp_add(sigspec2_block_delay_perf16, 4, sp[0]->s_vec, sp[1]->s_vec, x, n);
}

static void *sigspec2_block_delay_new(void)
{
	t_sigspec2_block_delay *x = (t_sigspec2_block_delay *)pd_new(sigspec2_block_delay_class);

	x->x_blocksize = 0;
	x->x_begmem = (t_float *)0;
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_block_delay_free(t_sigspec2_block_delay *x)
{
	if(x->x_begmem)
		freebytes(x->x_begmem, (x->x_blocksize+1) * sizeof(t_float));
}

void sigspec2_block_delay_setup(void)
{
	sigspec2_block_delay_class = class_new(gensym("spec2_block_delay~"), (t_newmethod)sigspec2_block_delay_new, (t_method)sigspec2_block_delay_free,
		sizeof(t_sigspec2_block_delay), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_block_delay_class, t_sigspec2_block_delay, x_msi);
	class_addmethod(sigspec2_block_delay_class, (t_method)sigspec2_block_delay_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_block_delay_class, gensym("iemhelp2/help-spec2_block_delay~"));
}
