/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   video_callbacks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:20 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 19:04:03 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

unsigned int	video_format_cb(void **opaque, char *chroma,
		unsigned int *width, unsigned int *height, unsigned int *pitches,
		unsigned int *lines)
{
	t_video_ctx	*ctx;

	memcpy(chroma, "RV32", 4);
	if (!video_h() && !video_w())
	{
		shared_int_access(&g_shared_flags->video_height, 1, *height);
		shared_int_access(&g_shared_flags->video_width, 1, *width);
	}
	if (pitches[0] == 0)
		pitches[0] = video_w() * 4;
	lines[0] = video_h();
	ctx = malloc(sizeof(*ctx));
	ctx->width = video_w();
	ctx->height = video_h();
	if (!g_shared_buffer)
	{
		shared_int_access(&g_shared_flags->buff_size, 1, pitches[0] * lines[0]);
		init_shared_buffer_child();
	}
	ctx->pitch = pitches[0];
	ctx->pixels = g_shared_buffer;
	*opaque = ctx;
	return (1);
}

void	video_cleanup_cb(void *opaque)
{
	free(opaque);
}

void	*video_lock_cb(void *opaque, void **planes)
{
	t_video_ctx	*ctx;

	shared_int_access(&g_shared_flags->should_play, 1, 1);
	ctx = (t_video_ctx *)opaque;
	*planes = ctx->pixels;
	return (ctx);
}

void	video_unlock_cb(void *opaque, void *picture, void *const *planes)
{
	(void)opaque;
	(void)picture;
	(void)planes;
}

void	video_display_cb(void *opaque, void *picture)
{
	(void)opaque;
	(void)picture;
	if (!should_play_video())
		return ;
	shared_int_access(&g_shared_flags->new_frame_flag, 1, 1);
}
