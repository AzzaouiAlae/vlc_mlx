/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_play.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:39 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 19:17:12 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

void	init_inst(int n, const char **vlc_args)
{
	g_inst = libvlc_new(n, (const char *const *)vlc_args);
	if (!g_inst)
	{
		fprintf(stderr, "Failed to create VLC instance\n");
		exit(1);
	}
}

void	init_media(char *file)
{
	g_m = libvlc_media_new_path(g_inst, file);
	if (!g_m)
	{
		fprintf(stderr, "Failed to load media: %s\n", file);
		libvlc_release(g_inst);
		exit(1);
	}
}

void	handle_end_reached(const libvlc_event_t *event, void *user_data)
{
	(void)event;
	(void)user_data;
	shared_int_access(&g_shared_flags->should_play, 1, 0);
	shared_int_access(&g_shared_flags->should_clean, 1, 1);
}

void	init_audio(char *file)
{
	const char	*vlc_args[] = {"--no-video", "--no-xlib", "--quiet"};

	init_shared_flags_child();
	shared_int_access(&g_shared_flags->played_audio, 1, 1);
	init_inst(3, vlc_args);
	init_media(file);
	g_mp = libvlc_media_player_new_from_media(g_m);
	if (libvlc_media_player_play(g_mp) != 0)
	{
		fprintf(stderr, "Failed to play media\n");
		libvlc_media_player_release(g_mp);
		libvlc_release(g_inst);
		exit(1);
	}
	set_speed();
	usleep(50000);
	while (libvlc_media_player_is_playing(g_mp))
		usleep(5000);
	shared_int_access(&g_shared_flags->should_clean, 1, 1);
	clear_vlc_objects(false);
}

void	init_video(char *file)
{
	int			i;
	const char	*vlc_args[] = {"--no-video-title-show", "--aout=pipewire",
		"--vout=x11"};

	i = 0;
	init_shared_flags_child();
	g_inst = libvlc_new(3, vlc_args);
	g_m = libvlc_media_new_path(g_inst, file);
	g_mp = libvlc_media_player_new_from_media(g_m);
	g_em = libvlc_media_player_event_manager(g_mp);
	libvlc_event_attach(g_em, libvlc_MediaPlayerEndReached, handle_end_reached,
		NULL);
	libvlc_video_set_format_callbacks(g_mp, video_format_cb, video_cleanup_cb);
	libvlc_video_set_callbacks(g_mp, video_lock_cb, video_unlock_cb,
		video_display_cb, NULL);
	libvlc_media_player_play(g_mp);
	set_speed();
	while (i++ < 500 && !should_play_video())
		usleep(2000);
	if (!should_play_video())
		shared_int_access(&g_shared_flags->should_clean, 1, 1);
	while (should_play_video())
		usleep(1000000);
	clear_vlc_objects(true);
}
