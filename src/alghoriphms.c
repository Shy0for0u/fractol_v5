#include "../includes/fractol.h"

int 			mandelbrot(t_complex c, void *f_)
{
	return (0);
}

int 			mandelbar(t_complex c, void *f_)
{
	return (0);
}

int				julia(t_complex c, void *f_)
{
	int			i;
	t_complex	z;
	t_fcl		*f;

	f = (t_fcl *)f_;
	z = c;
	i = 0;
	while (z.re * z.re + z.im * z.im <= 4 && i < f->max_i)
	{
		z = init_complex(z.re * z.re - z.im * z.im + f->c.re,
						 2 * z.re * z.im + f->c.im);
		i++;
	}
	return (i);
}

void			*fractals(void *thread)
{
	t_thr		*t;
	int 		x;
	t_color		black;
	int			iter;
	t_complex	c;

	t = (t_thr *)thread;
	while (++(t->in) < t->out)
	{
		c.im = (t->fcl.max.im + t->fcl.offset_y - (double)(t->in) * t->fcl.factor.im);
		x = -1;
		while (++x < (int)WTH)
		{
			c.re = (t->fcl.min.re + t->fcl.offset_x + (double)x * t->fcl.factor.re);
			if ((iter = t->fcl.f(c, &(t->fcl))) < t->fcl.max_i)
				set_color(iter, x, t->in, &(t->fcl));
			else
			{
				black.r = 0;
				black.g = 0;
				black.b = 0;
				pixel_to_img(&t->fcl, x, t->in, black);
			}
		}
	}
	return (NULL);
}
