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


/* ---------- spec2_blockfilter_ladder~ - signal matrix multiplication object with message matrix-coeff. ----------- */

typedef struct sigspec2_blockfilter_ladder
{
	t_object	x_obj;
	int				*x_matbuf;
	t_float		**x_io;
	t_float		*x_outsumbuf;
	int				x_outsumbufsize;
	int				x_n_in;	/* columns */
	int				x_n_out;   /* rows	*/
	float			x_msi;
} t_sigspec2_blockfilter_ladder;

t_class *sigspec2_blockfilter_ladder_class;

static void sigspec2_blockfilter_ladder_element(t_sigspec2_blockfilter_ladder *x, t_symbol *s, int argc, t_atom *argv)
{
	int inindex, outindex;
	int *matrix = x->x_matbuf;

	if(argc < 2)
	{
		post("spec2_blockfilter_ladder~ : bad list: <int> output_row_index <int> input_col_index !");
		return;
	}

	outindex = (int)atom_getint(argv);
	argv++;
	inindex = (int)atom_getint(argv) - 1;

	if(inindex >= x->x_n_in)
		inindex = x->x_n_in - 1;
	if(inindex < 0)
		inindex = 0;
	if(outindex >= x->x_n_out)
		outindex = x->x_n_out;
	if(outindex < 0)
		outindex = 0;

	matrix[inindex] = outindex;
}

static void sigspec2_blockfilter_ladder_list(t_sigspec2_blockfilter_ladder *x, t_symbol *s, int argc, t_atom *argv)
{
	int outindex, i, n=x->x_n_in;
	int *matrix = x->x_matbuf;

	if(argc < n)
	{
		post("spec2_blockfilter_ladder~ : bad list: (number_of_input_cols = %d) * <int> output_row_index !", n);
		return;
	}

	for(i=0; i<n; i++)
	{
		outindex = (int)atom_getint(argv);
		argv++;
		if(outindex >= x->x_n_out)
			outindex = x->x_n_out;
		if(outindex < 0)
			outindex = 0;
		matrix[i] = outindex;
	}
}

static void sigspec2_blockfilter_ladder_bundle(t_sigspec2_blockfilter_ladder *x, t_symbol *s, int argc, t_atom *argv)
{
	sigspec2_blockfilter_ladder_list(x, &s_list, argc, argv);
}

/* the dsp thing */

static t_int *sigspec2_blockfilter_ladder_perform(t_int *w)
{
	t_sigspec2_blockfilter_ladder *x = (t_sigspec2_blockfilter_ladder *)(w[1]);
	int n = (int)(w[2]);

	t_float **io = x->x_io;
	t_float *outsum;
	int *mat  = x->x_matbuf;
	int n_in = x->x_n_in;   /* columns */
	int n_out = x->x_n_out; /* rows	*/
	t_float *in, *out;
	int i, j, thrw;

	outsum = x->x_outsumbuf;
	for(j=0; j<n_out; j++)/* reset out-buffer */
	{
		for(i=0; i<n; i++)
			*outsum++ = 0.0f;
	}

	for(j=0; j<n_in; j++)/* each in */
	{
		in = io[j];
		thrw = mat[j];
		if(thrw)
		{
			thrw--;
			outsum = x->x_outsumbuf + n*thrw;
			for(i=0; i<n; i++)
				*outsum++ += *in++;
		}
	}

	outsum = x->x_outsumbuf;
	for(j=0; j<n_out; j++)/* copy out-buffer to out */
	{
		out = io[n_in+j];
		for(i=0; i<n; i++)
			*out++ = *outsum++;
	}
	return (w+3);
}

static t_int *sigspec2_blockfilter_ladder_perf8(t_int *w)
{
	t_sigspec2_blockfilter_ladder *x = (t_sigspec2_blockfilter_ladder *)(w[1]);
	int n = (int)(w[2]);

	t_float **io = x->x_io;
	t_float *outsum;
	int *mat  = x->x_matbuf;
	int n_in = x->x_n_in;   /* columns */
	int n_out = x->x_n_out; /* rows	*/
	t_float *in, *out;
	int i, j, thrw;

	outsum = x->x_outsumbuf;
	for(j=0; j<n_out; j++)/* reset out-buffer */
	{
		for(i=n; i; i -= 8, outsum += 8)
		{
			outsum[0] = 0.0f;
			outsum[1] = 0.0f;
			outsum[2] = 0.0f;
			outsum[3] = 0.0f;
			outsum[4] = 0.0f;
			outsum[5] = 0.0f;
			outsum[6] = 0.0f;
			outsum[7] = 0.0f;
		}
	}

	for(j=0; j<n_in; j++)/* each in */
	{
		in = io[j];
		thrw = mat[j];
		if(thrw)
		{
			thrw--;
			outsum = x->x_outsumbuf + n*thrw;
			for(i=n; i; i -= 8, outsum += 8, in += 8)
			{
				outsum[0] += in[0];
				outsum[1] += in[1];
				outsum[2] += in[2];
				outsum[3] += in[3];
				outsum[4] += in[4];
				outsum[5] += in[5];
				outsum[6] += in[6];
				outsum[7] += in[7];
			}
		}
	}

	outsum = x->x_outsumbuf;
	for(j=0; j<n_out; j++)/* copy out-buffer to out */
	{
		out = io[n_in+j];
		for (i=n; i; i -= 8, out += 8, outsum += 8)
		{
			out[0] = outsum[0];
			out[1] = outsum[1];
			out[2] = outsum[2];
			out[3] = outsum[3];
			out[4] = outsum[4];
			out[5] = outsum[5];
			out[6] = outsum[6];
			out[7] = outsum[7];
		}
	}
	return (w+3);
}

static void sigspec2_blockfilter_ladder_dsp(t_sigspec2_blockfilter_ladder *x, t_signal **sp)
{
	int i, n=sp[0]->s_n*x->x_n_out/2;
	
	if(!x->x_outsumbuf)
	{
		x->x_outsumbufsize = n;
		x->x_outsumbuf = (t_float *)getbytes(x->x_outsumbufsize * sizeof(t_float));
	}
	else if(x->x_outsumbufsize != n)
	{
		x->x_outsumbuf = (t_float *)resizebytes(x->x_outsumbuf, x->x_outsumbufsize*sizeof(t_float), n*sizeof(t_float));
		x->x_outsumbufsize = n;
	}

	n = x->x_n_in + x->x_n_out;
	for(i=0; i<n; i++)
	{
		x->x_io[i] = sp[i]->s_vec;
		/*post("iovec_addr = %d", (unsigned int)x->x_io[i]);*/
	}

	n = sp[0]->s_n/2;
	if(n&7)
		dsp_add(sigspec2_blockfilter_ladder_perform, 2, x, n);
	else
		dsp_add(sigspec2_blockfilter_ladder_perf8, 2, x, n);
}


/* setup/setdown things */

static void sigspec2_blockfilter_ladder_free(t_sigspec2_blockfilter_ladder *x)
{
	freebytes(x->x_matbuf, x->x_n_in * sizeof(int));
	freebytes(x->x_io, (x->x_n_in + x->x_n_out) * sizeof(t_float *));
	if(x->x_outsumbuf)
		freebytes(x->x_outsumbuf, x->x_outsumbufsize * sizeof(t_float));
}

static void *sigspec2_blockfilter_ladder_new(t_symbol *s, int argc, t_atom *argv)
{
	t_sigspec2_blockfilter_ladder *x = (t_sigspec2_blockfilter_ladder *)pd_new(sigspec2_blockfilter_ladder_class);
	int i;

	switch (argc)
	{
		case 0:
			x->x_n_in = x->x_n_out = 1;
			break;
		case 1:
			x->x_n_in = x->x_n_out = (int)atom_getint(argv);
			break;
		default:
			x->x_n_in = (int)atom_getint(argv);
			x->x_n_out = (int)atom_getint(argv+1);
			break;
	}

	if(x->x_n_in < 1)
		x->x_n_in = 1;
	if(x->x_n_out < 1)
		x->x_n_out = 1;
	i = x->x_n_in - 1;
	while(i--)
		inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	i = x->x_n_out;
	while(i--)
		outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0;
	x->x_outsumbuf = (t_float *)0;
	x->x_outsumbufsize = 0;
	x->x_matbuf = (int *)getbytes(x->x_n_in * sizeof(int));
	x->x_io = (t_float **)getbytes((x->x_n_in + x->x_n_out) * sizeof(t_float *));
	return (x);
}

void sigspec2_blockfilter_ladder_setup(void)
{
	sigspec2_blockfilter_ladder_class = class_new(gensym("spec2_blockfilter_ladder~"), (t_newmethod)sigspec2_blockfilter_ladder_new, (t_method)sigspec2_blockfilter_ladder_free,
				   sizeof(t_sigspec2_blockfilter_ladder), 0, A_GIMME, 0);
	CLASS_MAINSIGNALIN(sigspec2_blockfilter_ladder_class, t_sigspec2_blockfilter_ladder, x_msi);
	class_addmethod(sigspec2_blockfilter_ladder_class, (t_method)sigspec2_blockfilter_ladder_dsp, gensym("dsp"), 0);
	class_addlist(sigspec2_blockfilter_ladder_class, (t_method)sigspec2_blockfilter_ladder_list);
	class_addmethod(sigspec2_blockfilter_ladder_class, (t_method)sigspec2_blockfilter_ladder_element, gensym("element"), A_GIMME, 0);
	class_addmethod(sigspec2_blockfilter_ladder_class, (t_method)sigspec2_blockfilter_ladder_bundle, gensym("bundle"), A_GIMME, 0);
	class_sethelpsymbol(sigspec2_blockfilter_ladder_class, gensym("iemhelp2/help-spec2_blockfilter_ladder~"));
}
