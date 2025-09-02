/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 19:26:14 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/09/02 14:31:20 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

void	clear_sound_track(void)
{
	pid_t	pid;

	pid = sound_track_pid(0, 0);
	if (!pid)
		return ;
	if (pid)
		kill(pid, SIGTERM);
	waitpid(pid, 0, 0);
}

void	clear_vlc(void)
{
	if (should_play_video())
	{
		munmap(vars()->shared_buffer, buff_size());
		shm_unlink(vars()->buffer_name);
		free(vars()->buffer_name);
		vars()->buffer_name = NULL;
	}
	shared_int_access(&(vars()->shared_flags)->should_clean, 1, 0);
	shared_int_access(&(vars()->shared_flags)->should_play, 1, 0);
	shared_int_access(&(vars()->shared_flags)->video_height, 1, 0);
	shared_int_access(&(vars()->shared_flags)->video_width, 1, 0);
	if (vars()->audio_pid)
	{
		waitpid(vars()->audio_pid, 0, 0);
		vars()->audio_pid = 0;
	}
	if (vars()->video_pid)
	{
		waitpid(vars()->video_pid, 0, 0);
		vars()->video_pid = 0;
	}
}

void	exit_clear_vlc(void)
{
	if (vars()->audio_pid)
		kill(vars()->audio_pid, SIGTERM);
	if (vars()->video_pid)
		kill(vars()->video_pid, SIGTERM);
	if (vars()->buffer_name)
		free(vars()->buffer_name);
	if (vars()->shared_flags)
	{
		munmap(vars()->shared_flags, sizeof(*(vars()->shared_flags)));
		vars()->shared_flags = NULL;
	}
	if (vars()->flags_name)
	{
		shm_unlink(vars()->flags_name);
		free(vars()->flags_name);
		vars()->flags_name = NULL;
	}
	clear_sound_track();
	waitpid(vars()->audio_pid, 0, 0);
	waitpid(vars()->video_pid, 0, 0);
}
