/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_ambi2 written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2008 */

#ifndef __IEMAMBI2_H__
#define __IEMAMBI2_H__

#define AMBI2_LS_REAL 0
#define AMBI2_LS_IND 0
#define AMBI2_LS_MRG 1
#define AMBI2_LS_MIR 2
#define AMBI2_LS_PHT 3

void iem_ambi2_calc_co_si(double *array_cosinus, double *array_sinus, int n, double phi);
void iem_ambi2_calc_legendre(double *array_legendre, int n, double theta);
void iem_ambi2_init_semi_norm(double *array_semi_norm, int n);
void iem_ambi2_init_ortho_norm(double *array_ortho_norm, int n);

#endif
