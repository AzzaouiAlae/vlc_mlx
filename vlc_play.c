#include "vlc_mlx_init.h"
void clear_vlc_objects(bool is_video);

void init_inst(int n, const char **vlc_args)
{
	inst = libvlc_new(n, (const char *const *)vlc_args);
	if (!inst)
	{
		fprintf(stderr, "Failed to create VLC instance\n");
		exit(1);
	}
}

void init_media(char *file)
{
	m = libvlc_media_new_path(inst, file);
	if (!m)
	{
		fprintf(stderr, "Failed to load media: %s\n", file);
		libvlc_release(inst);
		exit(1);
	}
}

void	handle_end_reached(const libvlc_event_t *event, void *user_data)
{
	(void)event;
	(void)user_data;
	shared_int_access(&shared_flags->should_play, 1, 0);
	shared_int_access(&shared_flags->should_clean, 1, 1);
}

void	init_audio(char *file)
{
	const char	*vlc_args[] = {"--no-video", "--no-xlib", "--quiet"};

	init_shared_flags_child();
	shared_int_access(&shared_flags->played_audio, 1, 1);
	init_inst(3, vlc_args);
	init_media(file);
	mp = libvlc_media_player_new_from_media(m);
	if (libvlc_media_player_play(mp) != 0)
	{
		fprintf(stderr, "Failed to play media\n");
		libvlc_media_player_release(mp);
		libvlc_release(inst);
		exit(1);
	}
	set_speed();
	usleep(50000);
	while (libvlc_media_player_is_playing(mp))
		usleep(5000);
	shared_int_access(&shared_flags->should_clean, 1, 1);
	clear_vlc_objects(false);
}

void init_video(char *file)
{
	const char	*vlc_args[] = {"--no-video-title-show", "--aout=pipewire",
			"--vout=x11"};
	init_shared_flags_child();
	inst = libvlc_new(3, vlc_args);
	m = libvlc_media_new_path(inst, file);
	mp = libvlc_media_player_new_from_media(m);
	em = libvlc_media_player_event_manager(mp);
	libvlc_event_attach(em, libvlc_MediaPlayerEndReached,
		handle_end_reached, NULL);
	shared_int_access(&shared_flags->should_play, 1, 1);
	libvlc_video_set_format_callbacks(mp, video_format_cb,
		video_cleanup_cb);
	libvlc_video_set_callbacks(mp, video_lock_cb, video_unlock_cb,
		video_display_cb, NULL);
	if (libvlc_media_player_play(mp) != 0)
	{
		fprintf(stderr, "play failed\n");
	}
	set_speed();
	while (should_play_video())
		usleep(1000000);
	clear_vlc_objects(true);
}
void clear_vlc_objects(bool is_video)
{
    if (mp) 
	{
        libvlc_media_player_stop(mp);
        libvlc_media_player_release(mp);
    }
    if (m)
        libvlc_media_release(m);
    if (inst)
	{
        libvlc_release(inst);
	}
	if (is_video)
	{
		munmap(shared_buffer, buff_size());
		shm_unlink(buffer_name);
	}
}