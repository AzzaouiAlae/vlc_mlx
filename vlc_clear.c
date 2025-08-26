/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 19:26:14 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 19:26:17 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

void	clear_vlc(void)
{
	if (should_play_video())
	{
		munmap(g_shared_buffer, buff_size());
		shm_unlink(g_buffer_name);
		free(g_buffer_name);
		g_buffer_name = NULL;
	}
	shared_int_access(&g_shared_flags->should_clean, 1, 0);
	shared_int_access(&g_shared_flags->should_play, 1, 0);
	shared_int_access(&g_shared_flags->video_height, 1, 0);
	shared_int_access(&g_shared_flags->video_width, 1, 0);
	if (g_pid)
	{
		waitpid(g_pid, 0, 0);
		g_pid = 0;
	}
	if (g_video_pid)
	{
		waitpid(g_video_pid, 0, 0);
		g_video_pid = 0;
	}
}

void	exit_clear_vlc(void)
{
	if (g_pid)
		kill(g_pid, SIGTERM);
	if (g_video_pid)
		kill(g_video_pid, SIGTERM);
	if (g_buffer_name)
		free(g_buffer_name);
	if (g_shared_flags)
	{
		munmap(g_shared_flags, sizeof(*g_shared_flags));
		g_shared_flags = NULL;
	}
	if (g_flags_name)
	{
		shm_unlink(g_flags_name);
		free(g_flags_name);
		g_flags_name = NULL;
	}
	waitpid(g_pid, 0, 0);
	waitpid(g_video_pid, 0, 0);
}
