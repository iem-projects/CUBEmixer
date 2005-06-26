/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iemgui written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2005 */

#ifdef NT
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
#endif


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "m_pd.h"
#include "g_canvas.h"
#include "t_tk.h"
#include "g_all_guis.h"

#ifdef NT
#include <io.h>
#else
#include <unistd.h>
#endif

#include "iemlib.h"
#include "iemgui.h"

/* ---------- cube_sphere	my gui-canvas for a window ---------------- */

t_widgetbehavior cube_sphere_widgetbehavior;
static t_class *cube_sphere_class;

typedef struct _cube_sphere
{
	t_iemgui	x_gui;
	int				x_fontsize;
	int				x_n_src;
	int				x_pix_src_x[300];
	int				x_pix_src_y[300];
	int				x_col_src[300];
	int				x_pos_x;
	int				x_pos_y;
	float			x_pos_dx;
	float			x_pos_dy;
	float			x_pos_dr;
	int				x_sel_index;
	int       x_radius;
	float			x_90overradius;
	void			*x_out_para;
	t_atom		x_at[3];
} t_cube_sphere;

static void cube_sphere_out_all(t_cube_sphere *x)
{
	float delta, phi, diffr, diffx, diffy, fff=180.0f/3.14159265f;
	int i, n = x->x_n_src;
	int rad=x->x_radius;
	
	for(i=0; i<n; i++)
	{
		SETFLOAT(x->x_at, (float)(i+1));
		diffx = (float)(x->x_pix_src_x[i] - rad);
		diffy = (float)(x->x_pix_src_y[i] - rad);
		diffr = sqrt(diffx * diffx + diffy * diffy);
		delta = 90.0f - x->x_90overradius*diffr*1.01f;
		if(delta < 0.0f)
			delta = 0.0f;
		if(delta > 90.0f)
			delta = 90.0f;
		if(diffx == 0.0f)
		{
			if(diffy > 0.0f)
				phi = -180.0f;
			else
				phi = 0.0f;
		}
		else if(diffx > 0.0f)
		{
			if(diffy >= 0.0f)
				phi = fff*atan(diffx/diffy) - 180.0f;
			else
				phi = fff*atan(diffx/diffy);
		}
		else /* diffx < 0 */
		{
			if(diffy >= 0.0f)
				phi = fff*atan(diffx/diffy) + 180.0f;
			else
				phi = fff*atan(diffx/diffy);
		}
		SETFLOAT(x->x_at+1, delta);
		SETFLOAT(x->x_at+2, phi);
		outlet_list(x->x_out_para, &s_list, 3, x->x_at);
	}
}

static void cube_sphere_out_sel(t_cube_sphere *x)
{
	float delta, phi, fff=180.0f/3.14159265f;
	float diffr=x->x_pos_dr;
	float diffx=x->x_pos_dx;
	float diffy=x->x_pos_dy;
	int sel=x->x_sel_index;
	int n = x->x_n_src;
	int rad=x->x_radius;
	
	SETFLOAT(x->x_at, (float)(sel+1));
	delta = 90.0f - x->x_90overradius*diffr*1.01f;
	if(delta < 0.0f)
		delta = 0.0f;
	if(delta > 90.0f)
		delta = 90.0f;
	if(diffx == 0.0f)
	{
		if(diffy > 0.0f)
			phi = -180.0f;
		else
			phi = 0.0f;
	}
	else if(diffx > 0.0f)
	{
		if(diffy >= 0.0f)
			phi = fff*atan(diffx/diffy) - 180.0f;
		else
			phi = fff*atan(diffx/diffy);
	}
	else /* diffx < 0 */
	{
		if(diffy >= 0.0f)
			phi = fff*atan(diffx/diffy) + 180.0f;
		else
			phi = fff*atan(diffx/diffy);
	}
	SETFLOAT(x->x_at+1, delta);
	SETFLOAT(x->x_at+2, phi);
	outlet_list(x->x_out_para, &s_list, 3, x->x_at);
}

void cube_sphere_draw_new(t_cube_sphere *x, t_glist *glist)
{
	int xpos=text_xpix(&x->x_gui.x_obj, glist);
	int ypos=text_ypix(&x->x_gui.x_obj, glist);
	int x2=xpos+x->x_gui.x_w/2;
	int y2=ypos+x->x_gui.x_h/2;
	int r3=x->x_radius/3;
	int i, n=x->x_n_src;
	t_canvas *canvas=glist_getcanvas(glist);

	sys_vgui(".x%x.c create rectangle %d %d %d %d -outline #%6.6x -tags %xBASE\n",
	     canvas, xpos, ypos, xpos + x->x_gui.x_w, ypos + x->x_gui.x_h,
			 x->x_gui.x_fsf.x_selected?IEM_GUI_COLOR_SELECTED:x->x_gui.x_fcol, x);
	sys_vgui(".x%x.c create oval %d %d %d %d -fill #%6.6x -tags %xCIRC_AQ\n",
	     canvas, xpos, ypos, xpos + x->x_gui.x_w, ypos + x->x_gui.x_h, x->x_gui.x_bcol, x);
	sys_vgui(".x%x.c create oval %d %d %d %d -tags %xCIRC_WK\n",
	     canvas, x2-2*r3, y2-2*r3, x2+2*r3, y2+2*r3, x);
	sys_vgui(".x%x.c create oval %d %d %d %d -tags %xCIRC_PK\n",
	     canvas, x2-r3, y2-r3, x2+r3, y2+r3, x);
	sys_vgui(".x%x.c create oval %d %d %d %d -tags %xCIRC_NP\n",
	     canvas, x2-2, y2-2, x2+2, y2+2, x);
	for(i=0; i<n; i++)
	{
		sys_vgui(".x%x.c create text %d %d -text {%d} -anchor c \
			-font {times %d bold} -fill #%6.6x -tags %xSRC%d\n",
			canvas, xpos+x->x_pix_src_x[i], ypos+x->x_pix_src_y[i], i+1, x->x_fontsize,
			x->x_col_src[i], x, i);
	}
}

void cube_sphere_draw_move(t_cube_sphere *x, t_glist *glist)
{
	int xpos=text_xpix(&x->x_gui.x_obj, glist);
	int ypos=text_ypix(&x->x_gui.x_obj, glist);
	int x2=xpos+x->x_gui.x_w/2;
	int y2=ypos+x->x_gui.x_h/2;
	int r3=x->x_radius/3;
	int i, n=x->x_n_src;
	t_canvas *canvas=glist_getcanvas(glist);

	sys_vgui(".x%x.c coords %xBASE %d %d %d %d\n",
		canvas, x, xpos, ypos, xpos + x->x_gui.x_w, ypos + x->x_gui.x_h);
	sys_vgui(".x%x.c coords %xCIRC_AQ %d %d %d %d\n",
		canvas, x, xpos, ypos, xpos + x->x_gui.x_w, ypos + x->x_gui.x_h);
	sys_vgui(".x%x.c coords %xCIRC_WK %d %d %d %d\n",
		canvas, x, x2-2*r3, y2-2*r3, x2+2*r3, y2+2*r3);
	sys_vgui(".x%x.c coords %xCIRC_PK %d %d %d %d\n",
		canvas, x, x2-r3, y2-r3, x2+r3, y2+r3);
	sys_vgui(".x%x.c coords %xCIRC_NP %d %d %d %d\n",
		canvas, x, x2-2, y2-2, x2+2, y2+2);
	for(i=0; i<n; i++)
	{
		sys_vgui(".x%x.c coords %xSRC%d %d %d\n",
			canvas, x, i, xpos+x->x_pix_src_x[i], ypos+x->x_pix_src_y[i]);
	}
}

void cube_sphere_draw_erase(t_cube_sphere* x, t_glist* glist)
{
	int i, n;
	t_canvas *canvas=glist_getcanvas(glist);

	sys_vgui(".x%x.c delete %xBASE\n", canvas, x);
	sys_vgui(".x%x.c delete %xCIRC_AQ\n", canvas, x);
	sys_vgui(".x%x.c delete %xCIRC_WK\n", canvas, x);
	sys_vgui(".x%x.c delete %xCIRC_PK\n", canvas, x);
	sys_vgui(".x%x.c delete %xCIRC_NP\n", canvas, x);
	n = x->x_n_src;
	for(i=0; i<n; i++)
	{
		sys_vgui(".x%x.c delete %xSRC%d\n", canvas, x, i);
	}
}

void cube_sphere_draw_select(t_cube_sphere* x, t_glist* glist)
{
	t_canvas *canvas=glist_getcanvas(glist);

	sys_vgui(".x%x.c itemconfigure %xBASE -outline #%6.6x\n",
		canvas, x, x->x_gui.x_fsf.x_selected?IEM_GUI_COLOR_SELECTED:x->x_gui.x_fcol);
}

void cube_sphere_draw(t_cube_sphere *x, t_glist *glist, int mode)
{
	if(mode == IEM_GUI_DRAW_MODE_MOVE)
		cube_sphere_draw_move(x, glist);
	else if(mode == IEM_GUI_DRAW_MODE_NEW)
		cube_sphere_draw_new(x, glist);
	else if(mode == IEM_GUI_DRAW_MODE_SELECT)
		cube_sphere_draw_select(x, glist);
	else if(mode == IEM_GUI_DRAW_MODE_ERASE)
		cube_sphere_draw_erase(x, glist);
}

/* ------------------------ cnv widgetbehaviour----------------------------- */

static void cube_sphere_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
	t_cube_sphere *x = (t_cube_sphere *)z;

	*xp1 = text_xpix(&x->x_gui.x_obj, glist);
	*yp1 = text_ypix(&x->x_gui.x_obj, glist);
	*xp2 = *xp1 + x->x_gui.x_w;
	*yp2 = *yp1 + x->x_gui.x_h;
}

static void cube_sphere_save(t_gobj *z, t_binbuf *b)
{
	t_cube_sphere *x = (t_cube_sphere *)z;
	int i, j, c, n=x->x_n_src;
	
	binbuf_addv(b, "ssiis", gensym("#X"),gensym("obj"),
		(t_int)x->x_gui.x_obj.te_xpix, (t_int)x->x_gui.x_obj.te_ypix, gensym("cube_sphere"));
	binbuf_addv(b, "iii", x->x_n_src, x->x_radius, x->x_fontsize);
	c = x->x_gui.x_bcol;
	j = (((0xfc0000 & c) >> 6)|((0xfc00 & c) >> 4)|((0xfc & c) >> 2));
	binbuf_addv(b, "i", j);
	c = x->x_gui.x_fcol;
	j = (((0xfc0000 & c) >> 6)|((0xfc00 & c) >> 4)|((0xfc & c) >> 2));
	binbuf_addv(b, "i", j);
	for(i=0; i<n; i++)
	{
		c = x->x_col_src[i];
		j = (((0xfc0000 & c) >> 6)|((0xfc00 & c) >> 4)|((0xfc & c) >> 2));
		binbuf_addv(b, "iii", j, x->x_pix_src_x[i], x->x_pix_src_y[i]);
	}
	binbuf_addv(b, ";");
}

static void cube_sphere_motion(t_cube_sphere *x, t_floatarg dx, t_floatarg dy)
{
	int i, diffx, diffy, diffr, xx, yy;
	int sel=x->x_sel_index;
	int rad = x->x_radius;
	int xpos=text_xpix(&x->x_gui.x_obj, x->x_gui.x_glist);
	int ypos=text_ypix(&x->x_gui.x_obj, x->x_gui.x_glist);
	t_canvas *canvas=glist_getcanvas(x->x_gui.x_glist);

	x->x_pos_x += (int)dx;
	x->x_pos_y += (int)dy;
	x->x_pix_src_x[sel] = x->x_pos_x;
	x->x_pix_src_y[sel] = x->x_pos_y;
	diffx = x->x_pix_src_x[sel] - rad;
	diffy = x->x_pix_src_y[sel] - rad;
	x->x_pos_dx=(float)diffx;
	x->x_pos_dy=(float)diffy;
	x->x_pos_dr = sqrt(diffx * diffx + diffy * diffy);
	diffr = (int)(x->x_pos_dr+0.49999f);
	if(diffr > rad)
	{
		xx = rad * diffx;
		xx /= diffr;
		yy = rad * diffy;
		yy /= diffr;
		x->x_pix_src_y[sel] = rad + yy;
		x->x_pix_src_x[sel] = rad + xx;
	}
	cube_sphere_out_sel(x);
	sys_vgui(".x%x.c coords %xSRC%d %d %d\n",
		canvas, x, sel, xpos+x->x_pix_src_x[sel], ypos+x->x_pix_src_y[sel]);
}

static void cube_sphere_click(t_cube_sphere *x, t_floatarg xpos, t_floatarg ypos,
			    t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
	int xpix=text_xpix(&x->x_gui.x_obj, x->x_gui.x_glist);
	int ypix=text_ypix(&x->x_gui.x_obj, x->x_gui.x_glist);
	int cxpos=xpix+x->x_radius;
	int cypos=ypix+x->x_radius;
	int w = (int)xpos - xpix;
	int h = (int)ypos - ypix;
	int i, n=x->x_n_src;
	int diff, maxdiff=10000, sel=-1, diffx, diffy, diffr;
	int fs=x->x_fontsize/2+2;

	diffx = xpos - cxpos;
	diffy = ypos - cypos;
	diffr = (int)(sqrt(diffx * diffx + diffy * diffy)+0.49999f);
	if(diffr <= x->x_radius)
	{
		for(i=0; i<n; i++)
		{
			if((w >= (x->x_pix_src_x[i]-fs)) && (w <= (x->x_pix_src_x[i]+fs)) && (h >= (x->x_pix_src_y[i]-fs)) && (h <= (x->x_pix_src_y[i]+fs)))
			{
				diff = w - x->x_pix_src_x[i];
				if(diff < 0)
					diff *= -1;
				if(diff < maxdiff)
				{
					maxdiff = diff;
					sel = i;
				}
				diff = h - x->x_pix_src_y[i];
				if(diff < 0)
					diff *= -1;
				if(diff < maxdiff)
				{
					maxdiff = diff;
					sel = i;
				}
			}
		}
		if(sel >= 0)
		{
			x->x_sel_index = sel;
			x->x_pos_x = x->x_pix_src_x[sel];
			x->x_pos_y = x->x_pix_src_y[sel];
			glist_grab(x->x_gui.x_glist, &x->x_gui.x_obj.te_g, (t_glistmotionfn)cube_sphere_motion, 0, xpos, ypos);
		}
	}
}

static int cube_sphere_newclick(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
	t_cube_sphere* x = (t_cube_sphere *)z;

	if(doit)
	{
		cube_sphere_click( x, (t_floatarg)xpix, (t_floatarg)ypix, (t_floatarg)shift, 0, (t_floatarg)alt);
	}
	return (1);
}

static void cube_sphere_bang(t_cube_sphere *x)
{
	cube_sphere_out_all(x);
}

static void cube_sphere_src_font(t_cube_sphere *x, t_floatarg ff)
{
	int fs=(int)(ff + 0.49999f);
	int i, n=x->x_n_src;
	t_canvas *canvas=glist_getcanvas(x->x_gui.x_glist);

	if(fs < 8)
		fs = 8;
	if(fs > 50)
		fs = 50;
	x->x_fontsize = fs;

	for(i=0; i<n; i++)
	{
		sys_vgui(".x%x.c itemconfigure %xSRC%d -font {times %d bold}\n", canvas, x, i, fs);
	}
}

static void cube_sphere_src_dp(t_cube_sphere *x, t_symbol *s, int argc, t_atom *argv)
{
	float delta, phi;
	int i, n=x->x_n_src;
	int xpos=text_xpix(&x->x_gui.x_obj, x->x_gui.x_glist);
	int ypos=text_ypix(&x->x_gui.x_obj, x->x_gui.x_glist);
	t_canvas *canvas=glist_getcanvas(x->x_gui.x_glist);

	if(argc < 3)
	{
		post("cube_sphere ERROR: src_dp-input needs 1 index + 2 float-angles: src_index, delta [degree], phi [degree]");
		return;
	}
	i = (int)atom_getint(argv++)-1;
	if((i >= 0)&&(i < n))
	{
		delta = 90.0f - atom_getfloat(argv++);
		phi = atom_getfloat(argv);
		phi /= 180.0f;
		phi *= 3.14159265f;
		delta /= x->x_90overradius*0.99;
		if(delta < 0.0f)
			delta = 0.0f;
		if(delta > (float)x->x_radius)
			delta = (float)x->x_radius;

		x->x_pix_src_x[i] = x->x_radius - (int)(delta*sin(phi) + 0.49999f);
		x->x_pix_src_y[i] = x->x_radius - (int)(delta*cos(phi) + 0.49999f);
		sys_vgui(".x%x.c coords %xSRC%d %d %d\n",
			canvas, x, i, xpos+x->x_pix_src_x[i], ypos+x->x_pix_src_y[i]);
	}
}

static void cube_sphere_size(t_cube_sphere *x, t_floatarg size)
{
	float rad=x->x_radius;
	float xr, yr;
	int i, diffr, diffx, diffy, xx, yy, sz, n=x->x_n_src;
	int xpos=text_xpix(&x->x_gui.x_obj, x->x_gui.x_glist);
	int ypos=text_ypix(&x->x_gui.x_obj, x->x_gui.x_glist);

	size /= 6.0f;
	sz = (int)(size + 0.4999f);
	sz *= 3;
	if(sz < 30)
		sz = 30;
	if(sz > 1800)
		sz = 1800;

	for(i=0; i<n; i++)
	{
		diffx = x->x_pix_src_x[i] - rad;
		diffy = x->x_pix_src_y[i] - rad;
		diffr = (int)(sqrt(diffx * diffx + diffy * diffy)+0.49999f);
		if(diffr > sz)
		{
			if(diffr)
			{
				xx = sz * diffx;
				xx /= diffr;
				yy = sz * diffy;
				yy /= diffr;
			}
			else
			{
				xx = 0;
				yy = 0;
			}
			x->x_pix_src_x[i] = sz + xx;
			x->x_pix_src_y[i] = sz + yy;
		}
	}

	x->x_90overradius = 90.0f / (float)sz;
	x->x_radius = sz;
	x->x_gui.x_h = 2*sz;
	x->x_gui.x_w = 2*sz;

	cube_sphere_out_all(x);
	(*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_MOVE);
	canvas_fixlinesfor(glist_getcanvas(x->x_gui.x_glist), (t_text*)x);
}

static void cube_sphere_sphere_col(t_cube_sphere *x, t_floatarg fcol)
{
	int col=(int)fcol;
	int i;
	t_canvas *canvas=glist_getcanvas(x->x_gui.x_glist);

	if(col < 0)
	{
		i = -1 - col;
		x->x_gui.x_bcol = ((i & 0x3f000) << 6)|((i & 0xfc0) << 4)|((i & 0x3f) << 2);
	}
	else
	{
		if(col > 29)
			col = 29;
		x->x_gui.x_bcol = my_iemgui_color_hex[col];
	}
	sys_vgui(".x%x.c itemconfigure %xCIRC_AQ -fill #%6.6x\n", canvas, x, x->x_gui.x_bcol);
}

static void cube_sphere_frame_col(t_cube_sphere *x, t_floatarg fcol)
{
	int col=(int)fcol;
	int i;
	t_canvas *canvas=glist_getcanvas(x->x_gui.x_glist);

	if(col < 0)
	{
		i = -1 - col;
		x->x_gui.x_fcol = ((i & 0x3f000) << 6)|((i & 0xfc0) << 4)|((i & 0x3f) << 2);
	}
	else
	{
		if(col > 29)
			col = 29;
		x->x_gui.x_fcol = my_iemgui_color_hex[col];
	}
	sys_vgui(".x%x.c itemconfigure %xBASE -outline #%6.6x\n",
		canvas, x, x->x_gui.x_fsf.x_selected?IEM_GUI_COLOR_SELECTED:x->x_gui.x_fcol);
}

static void cube_sphere_src_col(t_cube_sphere *x, t_symbol *s, int argc, t_atom *argv)
{
	int col;
	int i, j, n=x->x_n_src;
	t_canvas *canvas=glist_getcanvas(x->x_gui.x_glist);

	if((argc >= 2)&&IS_A_FLOAT(argv,0)&&IS_A_FLOAT(argv,1))
	{
		j = (int)atom_getintarg(0, argc, argv)-1;
		if((j >= 0)&&(j < n))
		{
			col = (int)atom_getintarg(1, argc, argv);
			if(col < 0)
			{
				i = -1 - col;
				x->x_col_src[j] = ((i & 0x3f000) << 6)|((i & 0xfc0) << 4)|((i & 0x3f) << 2);
			}
			else
			{
				if(col > 29)
					col = 29;
				x->x_col_src[j] = my_iemgui_color_hex[col];
			}
			sys_vgui(".x%x.c itemconfigure %xSRC%d -fill #%6.6x\n", canvas, x, j, x->x_col_src[j]);
		}
	}
}

static void *cube_sphere_new(t_symbol *s, int argc, t_atom *argv)
{
	t_cube_sphere *x = (t_cube_sphere *)pd_new(cube_sphere_class);
	int i, j, n=1, c;

	if((argc >= 1)&&IS_A_FLOAT(argv,0))
	{
		n = (int)atom_getintarg(0, argc, argv);
		if(n < 1)
			n = 1;
		if(n > 300)
			n = 300;
		x->x_n_src = n;
	}
	if(argc == (3*n + 5))
	{
		x->x_radius = atom_getintarg(1, argc, argv);
		x->x_fontsize = (int)atom_getintarg(2, argc, argv);
		c = (int)atom_getintarg(3, argc, argv);
		x->x_gui.x_bcol = ((c & 0x3f000) << 6)|((c & 0xfc0) << 4)|((c & 0x3f) << 2);
		c = (int)atom_getintarg(4, argc, argv);
		x->x_gui.x_fcol = ((c & 0x3f000) << 6)|((c & 0xfc0) << 4)|((c & 0x3f) << 2);
		for(i=0; i<n; i++)
		{
			c = (int)atom_getintarg(5+3*i, argc, argv);
			x->x_col_src[i] = ((c & 0x3f000) << 6)|((c & 0xfc0) << 4)|((c & 0x3f) << 2);
			x->x_pix_src_x[i] = (int)atom_getintarg(6+3*i, argc, argv);
			x->x_pix_src_y[i] = (int)atom_getintarg(7+3*i, argc, argv);
		}
	}
	else
	{
		x->x_radius = 180;
		x->x_fontsize = 12;
		x->x_gui.x_bcol = my_iemgui_color_hex[IEM_GUI_COLNR_GREEN];
		x->x_gui.x_fcol = my_iemgui_color_hex[IEM_GUI_COLNR_L_GREY];
		j = 0;
		for(i=0; i<n; i++)
		{
			x->x_col_src[i] = simularca_color_hex[j];
			x->x_pix_src_x[i] = x->x_radius;
			x->x_pix_src_y[i] = x->x_radius;
			j++;
			j %= 7;
		}
	}

	x->x_90overradius = 90.0f / (float)x->x_radius;
	x->x_gui.x_w = 2*x->x_radius;
	x->x_gui.x_h = 2*x->x_radius;
	

	x->x_gui.x_draw = (t_iemfunptr)cube_sphere_draw;
	x->x_gui.x_glist = (t_glist *)canvas_getcurrent();
	
	x->x_out_para = outlet_new(&x->x_gui.x_obj, &s_list);
	return (x);
}

static void cube_sphere_ff(t_cube_sphere *x)
{
	gfxstub_deleteforkey(x);
}

void cube_sphere_setup(void)
{
	cube_sphere_class = class_new(gensym("cube_sphere"), (t_newmethod)cube_sphere_new,
				(t_method)cube_sphere_ff, sizeof(t_cube_sphere), 0, A_GIMME, 0);
	class_addcreator((t_newmethod)cube_sphere_new, gensym("cube_sphere"), A_GIMME, 0);
	class_addmethod(cube_sphere_class, (t_method)cube_sphere_click, gensym("click"),
		    A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(cube_sphere_class, (t_method)cube_sphere_motion, gensym("motion"),
		    A_FLOAT, A_FLOAT, 0);
	class_addbang(cube_sphere_class, (t_method)cube_sphere_bang);
	class_addmethod(cube_sphere_class, (t_method)cube_sphere_sphere_col, gensym("sphere_col"), A_DEFFLOAT, 0);
	class_addmethod(cube_sphere_class, (t_method)cube_sphere_frame_col, gensym("frame_col"), A_DEFFLOAT, 0);
	class_addmethod(cube_sphere_class, (t_method)cube_sphere_src_col, gensym("src_col"), A_GIMME, 0);
	class_addmethod(cube_sphere_class, (t_method)cube_sphere_src_dp, gensym("src_dp"), A_GIMME, 0);
	class_addmethod(cube_sphere_class, (t_method)cube_sphere_size, gensym("size"), A_DEFFLOAT, 0);
	class_addmethod(cube_sphere_class, (t_method)cube_sphere_src_font, gensym("src_font"), A_DEFFLOAT, 0);

/*	if(!iemgui_key_sym2)
			iemgui_key_sym2 = gensym("#keyname");*/
	cube_sphere_widgetbehavior.w_getrectfn = cube_sphere_getrect;
	cube_sphere_widgetbehavior.w_displacefn = iemgui_displace;
	cube_sphere_widgetbehavior.w_selectfn = iemgui_select;
	cube_sphere_widgetbehavior.w_activatefn = NULL;
	cube_sphere_widgetbehavior.w_deletefn = iemgui_delete;
	cube_sphere_widgetbehavior.w_visfn = iemgui_vis;
	cube_sphere_widgetbehavior.w_clickfn = cube_sphere_newclick;
	
#if defined(PD_MAJOR_VERSION) && (PD_MINOR_VERSION >= 37)
	class_setsavefn(cube_sphere_class, cube_sphere_save);
#else
	cube_sphere_widgetbehavior.w_propertiesfn = NULL;
	cube_sphere_widgetbehavior.w_savefn = cube_sphere_save;
#endif

	class_setwidget(cube_sphere_class, &cube_sphere_widgetbehavior);
	class_sethelpsymbol(cube_sphere_class, gensym("iemhelp2/help-cube_sphere"));
}
