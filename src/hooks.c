#include "../includes/fractol.h"

int 			deal_key(int key, t_fcl *f)
{
	if (key == 123) // left
		f->offset1 += f->tmp1;
	if (key == 124) // right
		f->offset1 -= f->tmp1;
	if (key == 126) // up
		f->offset2 -= f->tmp1;
	if (key == 125) // down
		f->offset2 += f->tmp1;
	if (key == 6)
		f->stop_move = (f->stop_move == 1) ? 0 : 1;
	if (key == 53)
		exit(0);
	if (key == 13) // w
		f->scale += 1.1f;
	if (key == 1) // d
		f->scale -= 1.1f;
	printf("offset1 %f offset2 %f tmp %f \n", f->offset1, f->offset2, f->tmp1);
	showing(f);
	return (0);
}

static int 			choose_zoom(t_fcl *f, int x, int y, int key)
{
	t_complex	mouse;

	if (key == 4)
	{
		mouse = init_complex((float)x / (WTH / (f->max.re - f->min.re)) + f->min.re,
							 (float)y / (HGT / (f->max.im - f->min.im)) + f->min.im);
		apply_zoom(f, mouse, 1.25f);
	}
	if (key == 5)
	{
		if (f->min.im == -2.0f && f->min.re == -2.0f && f->max.im == 2.0f && f->max.re == 2.0f)
			return (0);
		mouse = init_complex((float)x / (WTH / (f->max.re - f->min.re)) + f->min.re,
							 (float)y / (HGT / (f->max.im - f->min.im)) + f->min.im);
		apply_zoom(f, mouse, 0.8f);
	}
	return (1);
}

int 			deal_mouse(int key, int x, int y, t_fcl *f)
{
	y = (int)HGT - y;
	if (key == 1)
	{
		f->max_i += 40;
	}
	if (key == 2)
	{
		f->max_i -= 40;
	}
	if (key == 4 || key == 5)
		if (choose_zoom(f, x, y, key) == 0)
			return (0);
	showing(f);
	printf("scroll down %d c_re %f  c_im %f \n", key, f->c.re, f->c.im);
	return (0);
}

int 			mouse_move(int x_, int y_, t_fcl *f)
{
	float		x;
	float 		y;

	x = (float)x_;
	y = (float)y_;
	if (f->stop_move == 0)
		return (0);
	if (x < 0 || x > WTH)
		x = (x > WTH) ? WTH : 0;
	if (y < 0 || y > HGT)
		y = (y > HGT) ? HGT : 0;
	f->c = init_complex((x - WTH * 0.5f) / WTH,
						4.0f * ((HGT - y) - HGT * 0.5f) / HGT);
	showing(f);
	return (0);
}

void			hooks(t_fcl *f)
{
	mlx_hook(f->mlx.win, 2, 5, deal_key, f);
	mlx_hook(f->mlx.win, 6, 0, mouse_move, f);
	mlx_hook(f->mlx.win, 4, 5, deal_mouse, f);
}
