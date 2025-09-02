/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_copy_resize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:03 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 18:58:21 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

bool	init_copy_resized(t_resized_frame_dt *dt, void *data, size_t img_w,
		size_t img_h)
{
	bzero(dt, sizeof(t_resized_frame_dt));
	dt->data = data;
	if (!dt->data || !dt->data->addr)
	{
		return (false);
	}
	if (!(vars()->shared_buffer))
	{
		init_shared_buffer_parent();
	}
	dt->video_w = video_w();
	dt->video_h = video_h();
	dt->step_x = (dt->video_w << 16) / img_w;
	dt->step_y = (dt->video_h << 16) / img_h;
	dt->src = (unsigned int *)(vars()->shared_buffer);
	dt->img_y = 0;
	dt->sy = 0;
	dt->img_x = 0;
	dt->sx = 0;
	shared_int_access(&(vars()->shared_flags)->new_frame_flag, 1, 0);
	return (true);
}

void	copy_resized_frame(void *data, size_t img_w, size_t img_h)
{
	t_resized_frame_dt	dt;

	init_copy_resized(&dt, data, img_w, img_h);
	while (dt.img_y < (int)img_h)
	{
		dt.y = dt.sy >> 16;
		dt.dst = (unsigned int *)(dt.data->addr + dt.img_y
				* dt.data->line_length);
		dt.img_x = 0;
		dt.sx = 0;
		while (dt.img_x < (int)img_w)
		{
			dt.x = dt.sx >> 16;
			dt.c = dt.src[dt.y * dt.video_w + dt.x];
			dt.r = (dt.c >> 16) & 0xFF;
			dt.g = (dt.c >> 8) & 0xFF;
			dt.b = dt.c & 0xFF;
			dt.c = (dt.r << 16) | (dt.g << 8) | dt.b;
			dt.dst[dt.img_x] = dt.c;
			dt.img_x++;
			dt.sx += dt.step_x;
		}
		dt.img_y++;
		dt.sy += dt.step_y;
	}
}

bool	init_start_end_resized(t_resized_frame_dt *dt, void *data)
{
	bzero(dt, sizeof(t_resized_frame_dt));
	dt->data = data;
	if (!dt->data || !dt->data->addr)
	{
		return (false);
	}
	if (!(vars()->shared_buffer))
	{
		init_shared_buffer_parent();
	}
	dt->video_w = video_w();
	dt->video_h = video_h();
	dt->step_x = (dt->video_w << 16) / (img_end_x() - img_start_x());
	dt->step_y = (dt->video_h << 16) / (img_end_y() - img_start_y());
	dt->src = (unsigned int *)(vars()->shared_buffer);
	dt->img_x = img_start_x();
	dt->img_y = img_start_y();
	dt->sx = 0;
	dt->sy = 0;
	shared_int_access(&(vars()->shared_flags)->new_frame_flag, 1, 0);
	return (true);
}

void	copy_start_end_frame(void *data)
{
	t_resized_frame_dt	dt;

	init_start_end_resized(&dt, data);
	while (dt.img_y < img_end_y())
	{
		dt.y = dt.sy >> 16;
		dt.dst = (unsigned int *)(dt.data->addr + dt.img_y
				* dt.data->line_length);
		dt.sx = 0;
		dt.img_x = img_start_x();
		while (dt.img_x < img_end_x())
		{
			dt.x = dt.sx >> 16;
			dt.c = dt.src[dt.y * dt.video_w + dt.x];
			dt.r = (dt.c >> 16) & 0xFF;
			dt.g = (dt.c >> 8) & 0xFF;
			dt.b = dt.c & 0xFF;
			dt.c = (dt.r << 16) | (dt.g << 8) | dt.b;
			dt.dst[dt.img_x] = dt.c;
			dt.img_x++;
			dt.sx += dt.step_x;
		}
		dt.img_y++;
		dt.sy += dt.step_y;
	}
}
