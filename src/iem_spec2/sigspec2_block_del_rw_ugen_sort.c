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

extern int ugen_getsortno(void);

/* -------------------------- spec2_block_delwrite~ ------------------------------ */
static t_class *sigspec2_block_delwrite_class;

typedef struct _sigspec2_block_delwrite
{
	t_object	x_obj;
	t_float		*x_begmem;
	int				x_blocksize;
	t_symbol	*x_sym;
	int				x_max_blocks;
	int				x_sortno;   /* DSP sort number at which this was last put on chain */
	int				x_rsortno;  /* DSP sort # for first delread or write in chain */
	int				x_write_index;
	t_float		x_msi;
} t_sigspec2_block_delwrite;

static t_int *sigspec2_block_delwrite_perform(t_int *w)
{
	t_float *in = (float *)(w[1]);
	t_sigspec2_block_delwrite *x = (t_sigspec2_block_delwrite *)(w[2]);
	int i, n = (t_int)(w[3]);
	t_float *w_vec;

	w_vec = x->x_begmem + x->x_blocksize*x->x_write_index;
	for(i=0; i<n; i++)
	{
		w_vec[i] = in[i];
	}
	x->x_write_index++;
	if(x->x_write_index > x->x_max_blocks)
		x->x_write_index -= (x->x_max_blocks+1);
	return(w+4);
}

static t_int *sigspec2_block_delwrite_perf16(t_int *w)
{
	t_float *in = (float *)(w[1]);
	t_sigspec2_block_delwrite *x = (t_sigspec2_block_delwrite *)(w[2]);
	int i, n = (t_int)(w[3]);
	t_float *w_vec;

	w_vec = x->x_begmem + x->x_blocksize*x->x_write_index;
	while(n)
	{
		w_vec[0] = in[0];
		w_vec[1] = in[1];
		w_vec[2] = in[2];
		w_vec[3] = in[3];
		w_vec[4] = in[4];
		w_vec[5] = in[5];
		w_vec[6] = in[6];
		w_vec[7] = in[7];
		w_vec[8] = in[8];
		w_vec[9] = in[9];
		w_vec[10] = in[10];
		w_vec[11] = in[11];
		w_vec[12] = in[12];
		w_vec[13] = in[13];
		w_vec[14] = in[14];
		w_vec[15] = in[15];

		w_vec += 16;
		in += 16;
		n -= 16;
	}
	x->x_write_index++;
	if(x->x_write_index > x->x_max_blocks)
		x->x_write_index -= (x->x_max_blocks+1);
	return(w+4);
}

static void sigspec2_block_delwrite_dsp(t_sigspec2_block_delwrite *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	if(!x->x_blocksize)/*first time*/
	{
		x->x_begmem = (t_float *)getbytes(n*(x->x_max_blocks+1)*sizeof(t_float));
		x->x_blocksize = n;
	}
	else if(x->x_blocksize != n)
	{
		x->x_begmem = (t_float *)resizebytes(x->x_begmem, x->x_blocksize*(x->x_max_blocks+1)*sizeof(t_float), n*(x->x_max_blocks+1)*sizeof(t_float));
		x->x_blocksize = n;
	}
	post("spec2_block_delwrite~ sort_nr = %d", ugen_getsortno());
	if(n&15)
		dsp_add(sigspec2_block_delwrite_perform, 3, sp[0]->s_vec, x, n);
	else
		dsp_add(sigspec2_block_delwrite_perf16, 3, sp[0]->s_vec, x, n);
}

static void *sigspec2_block_delwrite_new(t_symbol *s, t_floatarg f)
{
	t_sigspec2_block_delwrite *x = (t_sigspec2_block_delwrite *)pd_new(sigspec2_block_delwrite_class);
	int i=(int)f;

	if(!*s->s_name)
		s = gensym("spec2_block_delwrite~");
	pd_bind(&x->x_obj.ob_pd, s);
	x->x_sym = s;
	if(i < 1)
		i = 1;
	x->x_blocksize = 0;
	x->x_max_blocks = i;
	x->x_sortno = 0;
	x->x_rsortno = 0;
	x->x_write_index = 0;
	x->x_begmem = (t_float *)0;
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_block_delwrite_free(t_sigspec2_block_delwrite *x)
{
	pd_unbind(&x->x_obj.ob_pd, x->x_sym);
	if(x->x_begmem)
		freebytes(x->x_begmem, x->x_blocksize * (x->x_max_blocks+1) * sizeof(t_float));
}

void sigspec2_block_delwrite_setup(void)
{
	sigspec2_block_delwrite_class = class_new(gensym("spec2_block_delwrite~"), (t_newmethod)sigspec2_block_delwrite_new, (t_method)sigspec2_block_delwrite_free,
		sizeof(t_sigspec2_block_delwrite), 0, A_DEFSYMBOL, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(sigspec2_block_delwrite_class, t_sigspec2_block_delwrite, x_msi);
	class_addmethod(sigspec2_block_delwrite_class, (t_method)sigspec2_block_delwrite_dsp, gensym("dsp"), 0);
}

/* -------------------------- spec2_block_delread~ ------------------------------ */
static t_class *sigspec2_block_delread_class;

typedef struct _sigspec2_block_delread
{
	t_object	x_obj;
	t_float		*x_begmem;
	int				x_blocksize;
	t_symbol	*x_sym;
	int				x_max_blocks;
	int				x_extra_block;
	int				x_del_blocks;
	int				x_read_index;
} t_sigspec2_block_delread;

static t_int *sigspec2_block_delread_perf_zero(t_int *w)
{
	t_float *out = (float *)(w[1]);
	t_sigspec2_block_delread *x = (t_sigspec2_block_delread *)(w[2]);
	int i, n = (t_int)(w[3]);
	
	for(i=0; i<n; i++)
		out[i] = 0.0f;
	return(w+4);
}

static t_int *sigspec2_block_delread_perform(t_int *w)
{
	t_float *out = (float *)(w[1]);
	t_sigspec2_block_delread *x = (t_sigspec2_block_delread *)(w[2]);
	int i, n = (t_int)(w[3]);
	t_float *r_vec;

/*	if(x->x_first)
	{
		t_sigspec2_block_delwrite *delwriter=(t_sigspec2_block_delwrite *)pd_findbyclass(x->x_sym, sigspec2_block_delwrite_class);

		x->x_first = 0;
		delwriter->x_r_schedule_counter++;
		if(delwriter->x_w_schedule_counter)
			x->x_extra_block = 0;
		else
			x->x_extra_block = 1;
		x->x_read_index = delwriter->x_write_index - x->x_del_blocks;
		if(x->x_read_index < 0)
			x->x_read_index += x->x_max_blocks;
		post("spec2_block_delread~ mit delay von %d : extra = %d", x->x_del_blocks, x->x_extra_block);
	}*/
	if(x->x_read_index > x->x_max_blocks)
		x->x_read_index = 0;
	r_vec = x->x_begmem + x->x_blocksize*x->x_read_index;
	for(i=0; i<n; i++)
	{
		out[i] = r_vec[i];
	}
	x->x_read_index++;
	return(w+4);
}

static t_int *sigspec2_block_delread_perf16(t_int *w)
{
	t_float *out = (float *)(w[1]);
	t_sigspec2_block_delread *x = (t_sigspec2_block_delread *)(w[2]);
	int n = (t_int)(w[3]);
	t_float *r_vec;

/*	if(x->x_first)
	{
		t_sigspec2_block_delwrite *delwriter=(t_sigspec2_block_delwrite *)pd_findbyclass(x->x_sym, sigspec2_block_delwrite_class);

		x->x_first = 0;
		delwriter->x_r_schedule_counter++;
		if(delwriter->x_w_schedule_counter)
			x->x_extra_block = 0;
		else
			x->x_extra_block = 1;
		x->x_read_index = delwriter->x_write_index - x->x_del_blocks;
		if(x->x_read_index < 0)
			x->x_read_index += x->x_max_blocks;
		post("spec2_block_delread~ mit delay von %d : extra = %d", x->x_del_blocks, x->x_extra_block);
	}*/
	if(x->x_read_index > x->x_max_blocks)
		x->x_read_index = 0;
	r_vec = x->x_begmem + x->x_blocksize*x->x_read_index;
	while(n)
	{
		out[0] = r_vec[0];
		out[1] = r_vec[1];
		out[2] = r_vec[2];
		out[3] = r_vec[3];
		out[4] = r_vec[4];
		out[5] = r_vec[5];
		out[6] = r_vec[6];
		out[7] = r_vec[7];
		out[8] = r_vec[8];
		out[9] = r_vec[9];
		out[10] = r_vec[10];
		out[11] = r_vec[11];
		out[12] = r_vec[12];
		out[13] = r_vec[13];
		out[14] = r_vec[14];
		out[15] = r_vec[15];

		r_vec += 16;
		out += 16;
		n -= 16;
	}
	x->x_read_index++;
	return(w+4);
}

static void sigspec2_block_delread_dsp(t_sigspec2_block_delread *x, t_signal **sp)
{
	t_sigspec2_block_delwrite *delwriter=(t_sigspec2_block_delwrite *)pd_findbyclass(x->x_sym, sigspec2_block_delwrite_class);
	int n = (sp[0]->s_n)/2;

	if(delwriter)
	{
		if(n == delwriter->x_blocksize)
		{
			x->x_begmem = delwriter->x_begmem;
			x->x_blocksize = n;
			x->x_max_blocks = delwriter->x_max_blocks;
			x->x_extra_block = 0;
			if(x->x_del_blocks > x->x_max_blocks)
				x->x_del_blocks = x->x_max_blocks;
			post("spec2_block_delread~ sort_nr = %d", ugen_getsortno());


/*			sigdelwrite_checkvecsize(delwriter, n);
			if(delwriter->x_rsortno != ugen_getsortno())
			{
				x->x_vecsize = vecsize;
	x->x_rsortno = ugen_getsortno();
    }
    else if (vecsize != x->x_vecsize)
    	pd_error(x, "delread/delwrite/vd vector size mismatch");

			x->x_zerodel = (delwriter->x_sortno == ugen_getsortno() ? 0 : delwriter->x_vecsize);

			sigdelread_float(x, x->x_deltime);
			int samps;
    t_sigdelwrite *delwriter =
    	(t_sigdelwrite *)pd_findbyclass(x->x_sym, sigdelwrite_class);
    x->x_deltime = f;
    if (delwriter)
    {
    	int delsize = delwriter->x_cspace.c_n;
    	x->x_delsamps = (int)(0.5 + x->x_sr * x->x_deltime)
    	    + x->x_n - x->x_zerodel;
    	if (x->x_delsamps < x->x_n) x->x_delsamps = x->x_n;
    	else if (x->x_delsamps > delwriter->x_cspace.c_n - DEFDELVS)
    	    x->x_delsamps = delwriter->x_cspace.c_n - DEFDELVS;
    }*/


			x->x_read_index = delwriter->x_write_index - x->x_del_blocks;
			if(x->x_read_index < 0)
				x->x_read_index += x->x_max_blocks;

			if(n&15)
				dsp_add(sigspec2_block_delread_perform, 3, sp[0]->s_vec, x, n);
			else
				dsp_add(sigspec2_block_delread_perf16, 3, sp[0]->s_vec, x, n);
		}
		else
		{
			pd_error(x, "spec2_block_delread~/spec2_block_delwrite~ vector size mismatch");
			dsp_add(sigspec2_block_delread_perf_zero, 3, sp[0]->s_vec, x, n);
		}
	}
	else if(*x->x_sym->s_name)
	{
		error("spec2_block_delread~: %s: no such spec2_block_delwrite~",x->x_sym->s_name);
		dsp_add(sigspec2_block_delread_perf_zero, 3, sp[0]->s_vec, x, n);
	}
}

static void *sigspec2_block_delread_new(t_symbol *s, t_floatarg f)
{
	t_sigspec2_block_delread *x = (t_sigspec2_block_delread *)pd_new(sigspec2_block_delread_class);
	int i=(int)f;

	x->x_sym = s;
	if(i < 0)
		i = 0;
	x->x_del_blocks = i;
	x->x_blocksize = 0;
	x->x_max_blocks = 0;
	x->x_extra_block = 0;
	x->x_read_index = 0;
	x->x_begmem = (t_float *)0;
	outlet_new(&x->x_obj, &s_signal);
	return (x);
}

static void sigspec2_block_delread_free(t_sigspec2_block_delread *x)
{
}

void sigspec2_block_delread_setup(void)
{
	sigspec2_block_delread_class = class_new(gensym("spec2_block_delread~"), (t_newmethod)sigspec2_block_delread_new, (t_method)sigspec2_block_delread_free,
		sizeof(t_sigspec2_block_delread), CLASS_NOINLET, A_DEFSYMBOL, A_DEFFLOAT, 0);
	class_addmethod(sigspec2_block_delread_class, (t_method)sigspec2_block_delread_dsp, gensym("dsp"), 0);
}
