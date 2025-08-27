/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   video_callbacks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:20 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/27 09:44:15 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

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
