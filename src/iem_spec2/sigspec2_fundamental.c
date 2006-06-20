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

/* -------------------------- spec2_fundamental~ ------------------------------ */
static t_class *sigspec2_fundamental_class;

typedef struct _sigspec2_fundamental
{
	t_object	x_obj;
	int				x_max_num_of_peaks;
	int				x_min_main_lobe_width_bins;
	float			x_min_main_lobe_height_db;
	int				x_lowest_freq_bins;
	int				x_highest_freq_bins;
	t_float		x_msi;
} t_sigspec2_fundamental;

static t_int *sigspec2_fundamental_perform(t_int *w)
{
	t_float *in1_diff = (float *)(w[1]);
	t_float *in2_spec = (float *)(w[2]);
	t_sigspec2_fundamental *x = (t_sigspec2_fundamental *)(w[3]);
	int i, n = (t_int)(w[4]);
	int low_freq=x->x_lowest_freq_bins;
	int high_freq=x->x_highest_freq_bins;
	int lobe_width=x->x_min_main_lobe_width_bins;/*has to be odd*/
	float lobe_height=x->x_min_main_lobe_height_db;
	int max_peaks=x->x_max_num_of_peaks;
	int peak_index_array[1000];
	int num_peaks=0;
	int lw2=lobe_width/2;
	int ok;
	float exact_peak_freq_array[1000];
	float exact_peak_amp_array[1000];
	int amp_order_array[1000];
	int num_exact_peaks=0;
	float xmax;
	int not_first;
	float xmax;
	float diff;

	for(i=low_freq; i<=high_freq; i++)/* find releative maxima */
	{
		if((in1_diff[i] >= in1_diff[i-1]) && (in1_diff[i] >= in1_diff[i+1]))
		{
			peak_index_array[num_peaks] = i;
			num_peaks++;
		}
	}

	for(j=0; j<num_peaks; j++)/* eval if maxima are main-lobes */
	{
		i = peak_index_array[j];
		ok = 1;
		for(k=-lw2; k<0; k++)
		{
			if((in1_diff[i+k] + lobe_height) < in1_diff[i])
				ok = 0;
			if((in1_diff[i-k] + lobe_height) < in1_diff[i])
				ok = 0;
		}
		if(!ok)
			peak_index_array[j] = -1;
	}

	not_first = 0;
	for(j=0; j<num_peaks; j++)/* calc exactly amplitude and frequency */
	{
		if(peak_index_array[j] > 0)
		{
			i = peak_index_array[j];
			xmax = 0.5f * (in2_spec[i-1] - in2_spec[i+1]) / (in2_spec[i-1] + in2_spec[i+1] - 2.0f*in2_spec[i]);
			exact_peak_freq_array[num_exact_peaks] = i + xmax;
			exact_peak_amp_array[num_exact_peaks] = in2_spec[i] + 0.5f*(in2_spec[i+1] - in2_spec[i-1])*xmax + \
				(0.5f*(in2_spec[i-1] + in2_spec[i+1]) - in2_spec[i])*xmax*xmax;
			if(not_first && (not_first <= max_peaks))
			{
				
			}
			diff = (exact_peak_freq_array[num_exact_peaks])*44100.0f/(float)(2*n);
			post("%d.peak: %g Hz , %g dB", num_exact_peaks+1, diff, exact_peak_amp_array[num_exact_peaks]);
			not_first++;
			old_freq = exact_peak_freq_array[num_exact_peaks];
			num_exact_peaks++;
		}
	}
	
	return(w+5);
}

static void sigspec2_fundamental_dsp(t_sigspec2_fundamental *x, t_signal **sp)
{
	int n = (sp[0]->s_n)/2;

	dsp_add(sigspec2_fundamental_perform, 4, sp[0]->s_vec, sp[1]->s_vec, x, n);
}

static void *sigspec2_fundamental_new(void)
{
	t_sigspec2_fundamental *x = (t_sigspec2_fundamental *)pd_new(sigspec2_fundamental_class);

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	outlet_new(&x->x_obj, &s_signal);
	x->x_msi = 0.0f;
	return (x);
}

static void sigspec2_fundamental_free(t_sigspec2_fundamental *x)
{
}

void sigspec2_fundamental_setup(void)
{
	sigspec2_fundamental_class = class_new(gensym("spec2+~"), (t_newmethod)sigspec2_fundamental_new, (t_method)sigspec2_fundamental_free,
		sizeof(t_sigspec2_fundamental), 0, 0);
	CLASS_MAINSIGNALIN(sigspec2_fundamental_class, t_sigspec2_fundamental, x_msi);
	class_addmethod(sigspec2_fundamental_class, (t_method)sigspec2_fundamental_dsp, gensym("dsp"), 0);
	class_sethelpsymbol(sigspec2_fundamental_class, gensym("iemhelp/help-spec2_fundamental~"));
}
