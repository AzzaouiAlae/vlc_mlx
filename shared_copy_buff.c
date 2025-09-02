/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_copy_buff.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:00 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 19:21:25 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

void	copy_frame(void *data_img, size_t img_w, size_t img_h)
{
	t_copy_buff	dt;

	if (!data_img)
		return ;
	shared_int_access(&(vars()->shared_flags)->new_frame_flag, 1, 0);
	dt.win_img = data_img;
	if (!(vars()->shared_buffer))
		init_shared_buffer_parent();
	dt.src = vars()->shared_buffer;
	dt.dst = (uint8_t *)(dt.win_img->addr);
	dt.dst_stride = (dt.win_img->line_length);
	dt.src_stride = img_w * 4;
	if (img_w > video_w())
		dt.src_stride = img_w * 4;
	dt.rows = img_h;
	if (img_h > video_h())
		dt.rows = video_h();
	dt.y = 0;
	while (dt.y++ < dt.rows)
		memcpy(dt.dst + dt.y * dt.dst_stride, dt.src + dt.y * dt.src_stride,
			dt.src_stride);
}
