/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_spec2 written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2005 */

#ifdef NT
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
#endif


#include "m_pd.h"
#include "iemlib.h"

static t_class *iem_spec2_class;

static void *iem_spec2_new(void)
{
	t_object *x = (t_object *)pd_new(iem_spec2_class);
    
	return (x);
}

void sigspec2_block_delay_setup(void);
void sigspec2_add_setup(void);
void sigspec2_sub_setup(void);
void sigspec2_mul_setup(void);
void sigspec2_add_scalar_setup(void);
void sigspec2_mul_scalar_setup(void);
void sigspec2_tabreceive_setup(void);
void sigspec2_tabreceive_enable_setup(void);
void sigspec2_tab_conv_setup(void);
void sigspec2_clip_max_setup(void);
void sigspec2_clip_min_setup(void);
void sigspec2_1p1z_time_setup(void);
void sigspec2_1p1z_freq_setup(void);
void sigspec2_sqrt_setup(void);
void sigspec2_abs_setup(void);
void sigspec2_dbtorms_setup(void);
void sigspec2_dbtopow_setup(void);
void sigspec2_rmstodb_setup(void);
void sigspec2_powtodb_setup(void);
void sigspec2_stretch_setup(void);
void sigspec2_shift_setup(void);
void sigspec2_matrix_bundle_stat_setup(void);

/* ------------------------ setup routine ------------------------- */

void iem_spec2_setup(void)
{
	iem_spec2_class = class_new(gensym("iem_spec2"), iem_spec2_new, 0,
    	sizeof(t_object), CLASS_NOINLET, 0);

	sigspec2_block_delay_setup();
	sigspec2_add_setup();
	sigspec2_sub_setup();
	sigspec2_mul_setup();
	sigspec2_add_scalar_setup();
	sigspec2_mul_scalar_setup();
	sigspec2_tabreceive_setup();
	sigspec2_tabreceive_enable_setup();
	sigspec2_tab_conv_setup();
	sigspec2_clip_max_setup();
	sigspec2_clip_min_setup();
	sigspec2_1p1z_time_setup();
	sigspec2_1p1z_freq_setup();
	sigspec2_sqrt_setup();
	sigspec2_abs_setup();
	sigspec2_dbtorms_setup();
	sigspec2_dbtopow_setup();
	sigspec2_rmstodb_setup();
	sigspec2_powtodb_setup();
	sigspec2_stretch_setup();
	sigspec2_shift_setup();
	sigspec2_matrix_bundle_stat_setup();

	post("iem_spec2 (R-1.16) library loaded!   (c) Thomas Musil 05.2005");
	post("   musil%ciem.at iem KUG Graz Austria", '@');
}
