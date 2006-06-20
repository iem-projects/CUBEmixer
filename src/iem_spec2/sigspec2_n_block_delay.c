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

/* -------------------------- sigspec2_n_block_delay~ ------------------------------ */
static t_class *sigspec2_n_block_delay_class;

typedef struct _sigspec2_n_block_delay
{
	t_object	x_obj;
	t_float		*x_begmem;
	int				x_n_delays;
	int				x_writeindex;
	int				x_blocksize;
	t_float		**x_io;
	t_float		x_msi;
} t_sigspec2_n_block_delay;

static t_int *sigspec2_n_block_delay_perform(t_int *w)
{
	t_sigspec2_n_block_delay *x = (t_sigspec2_n_block_delay *)(w[1]);
	int n=(int)(w[2]);
	int num_dels=x->x_n_delays;
	int readindex;
	int writeindex=x->x_writeindex;
	t_float *in;
	t_float *out;
	int i, j;
	t_float *writevec;
	t_float *readvec;

	writevec = x->x_begmem + writeindex*n;
	in = x->x_io[0];
	for(i=0; i<n; i++)
		writevec[i] = in[i];

	readindex = writeindex - 1;
	for(j=0; j<num_dels; j++)
	{
		out = x->x_io[j+1];
		if(readindex < 1)
			readindex += num_dels;
		readvec = writevec - readindex*n;
		for(i=0; i<n; i++)
			out[i] = readvec[i];
		readindex--;
	}

	writeindex++;
	if(writeindex > num_dels)
		writeindex -= num_dels + 1;
	x->x_writeindex = writeindex;

	return(w+3);
}

static t_int *sigspec2_n_block_delay_perf16(t_int *w)
{
	t_sigspec2_n_block_delay *x = (t_sigspec2_n_block_delay *)(w[1]);
	int n=(int)(w[2]);
	int num_dels=x->x_n_delays;
	int readindex;
	int writeindex=x->x_writeindex;
	t_float *in;
	t_float *out;
	int i, j;
	t_float *writevec;
	t_float *readvec;

	writevec = x->x_begmem + writeindex*n;
	in = x->x_io[0];
	i = n;
	while(i)
	{
		writevec[0] = in[0];
		writevec[1] = in[1];
		writevec[2] = in[2];
		writevec[3] = in[3];
		writevec[4] = in[4];
		writevec[5] = in[5];
		writevec[6] = in[6];
		writevec[7] = in[7];
		writevec[8] = in[8];
		writevec[9] = in[9];
		writevec[10] = in[10];
		writevec[11] = in[11];
		writevec[12] = in[12];
		writevec[13] = in[13];
		writevec[14] = in[14];
		writevec[15] = in[15];

		writevec += 16;
		in += 16;
		i -= 16;
	}

	readindex = writeindex - 1;
	for(j=0; j<num_dels; j++)
	{
		out = x->x_io[j+1];
		if(readindex < 1)
			readindex += num_dels;
		readvec = writevec - readindex*n;
		i = n;
		while(i)
		{
			out[0] = readvec[0];
			out[1] = readvec[1];
			out[2] = readvec[2];
			out[3] = readvec[3];
			out[4] = readvec[4];
			out[5] = readvec[5];
			out[6] = readvec[6];
			out[7] = readvec[7];
			out[8] = readvec[8];
			out[9] = readvec[9];
			out[10] = readvec[10];
			out[11] = readvec[11];
			out[12] = readvec[12];
			out[13] = readvec[13];
			out[14] = readvec[14];
			out[15] = readvec[15];
			out += 16;
			readvec += 16;
			i -= 16;
		}
		readindex--;
	}

	writeindex++;
	if(writeindex > num_dels)
		writeindex -= num_dels + 1;
	x->x_writeindex = writeindex;

	return(w+3);
}

static void sigspec2_n_block_delay_dsp(t_sigspec2_n_block_delay *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;
	int i, num_dels = x->x_n_delays;

	if(!x->x_blocksize)/*first time*/
	{
		x->x_begmem = (t_float *)getbytes(n*(num_dels+1)* sizeof(t_float));
		x->x_writeindex = 1;
	}
	else if(x->x_blocksize != n)
	{
		x->x_begmem = (t_float *)resizebytes(x->x_begmem, x->x_blocksize*(num_dels+1)*sizeof(t_float), n*(num_dels+1)*sizeof(t_float));
		x->x_writeindex = 1;
	}
	x->x_blocksize = n;
	for(i=0; i<=num_dels; i++)
		x->x_io[i] = sp[i]->s_vec;
	if(n&15)
		dsp_add(sigspec2_n_block_delay_perform, 2, x, n);
	else
		dsp_add(sigspec2_n_block_delay_perf16, 2, x, n);
}

static void *sigspec2_n_block_delay_new(t_floatarg n_delays)
{
	t_sigspec2_n_block_delay *x = (t_sigspec2_n_block_delay *)pd_new(sigspec2_n_block_delay_class);
	int i, n_out = (int)n_delays;

	n_out--;
	if(n_out < 1)
		n_out = 1;
	x->x_n_delays = n_out;
	x->x_begmem = (t_float *)0;
	x->x_writeindex = 1;
	x->x_blocksize = 0;
	x->x_io = (t_float **)getbytes((x->x_n_delays + 1) * sizeof(t_float *));
	for(i=0; i<n_out; i++)
		outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_n_block_delay_free(t_sigspec2_n_block_delay *x)
{
	freebytes(x->x_io, (x->x_n_delays+1) * sizeof(t_float *));
	if(x->x_begmem)
		freebytes(x->x_begmem, x->x_blocksize*(x->x_n_delays+1)*sizeof(t_float));
}

void sigspec2_n_block_delay_setup(void)
{
	sigspec2_n_block_delay_class = class_new(gensym("spec2_n_block_delay~"), (t_newmethod)sigspec2_n_block_delay_new, (t_method)sigspec2_n_block_delay_free,
		sizeof(t_sigspec2_n_block_delay), 0, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(sigspec2_n_block_delay_class, t_sigspec2_n_block_delay, x_msi);
	class_addmethod(sigspec2_n_block_delay_class, (t_method)sigspec2_n_block_delay_dsp, gensym("dsp"), 0);
}
