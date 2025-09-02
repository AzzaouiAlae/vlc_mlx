/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_mlx_init.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:30 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/09/02 14:31:27 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VLC_MLX_INIT_H
# define VLC_MLX_INIT_H

# include "vlc_mlx.h"
# include <fcntl.h>
# include <pthread.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <sys/mman.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>
# include <vlc/vlc.h>

# define SHM_NAME_LEN 16

typedef struct s_shared_int_t
{
	pthread_mutex_t			mutex;
	int						value;
}							t_shared_int;

typedef struct s_shared_flags_t
{
	t_shared_int			should_clean;
	t_shared_int			should_play;
	t_shared_int			new_frame_flag;
	t_shared_int			buff_size;
	t_shared_int			video_width;
	t_shared_int			video_height;
	t_shared_int			img_start_x;
	t_shared_int			img_start_y;
	t_shared_int			img_end_x;
	t_shared_int			img_end_y;
	t_shared_int			played_audio;
	t_shared_int			play_speed;
}							t_shared_flags;

typedef struct s_dt2
{
	void					*img;
	char					*addr;
	int						bits_per_pixel;
	int						line_length;
	int						endian;
	int						img_width;
	int						img_height;
}							t_dt2;

typedef struct s_resized_frame_dt
{
	int						x;
	int						y;
	int						img_y;
	int						img_x;
	int						video_w;
	int						video_h;
	int						step_x;
	int						step_y;
	int						sy;
	int						sx;
	unsigned int			c;
	unsigned char			r;
	unsigned char			g;
	unsigned char			b;
	unsigned int			*src;
	unsigned int			*dst;
	t_dt2					*data;
}							t_resized_frame_dt;

typedef struct s_copy_buff
{
	uint8_t					*dst;
	int						dst_stride;
	int						src_stride;
	int						rows;
	t_dt2					*win_img;
	uint8_t					*src;
	int						y;
}							t_copy_buff;

typedef struct s_video_ctx
{
	uint8_t					*pixels;
	unsigned int			width;
	unsigned int			height;
	unsigned int			pitch;
}							t_video_ctx;

typedef struct s_globle_vars
{
	t_shared_flags			*shared_flags;
	void					*shared_buffer;
	char					*flags_name;
	char					*buffer_name;
	pid_t					audio_pid;
	pid_t					video_pid;
	libvlc_media_t			*m;
	libvlc_media_player_t	*mp;
	libvlc_event_manager_t	*em;
	libvlc_instance_t		*inst;
}							t_globle_vars;

int							shared_int_access(t_shared_int *shared, int set,
								int value);
size_t						buff_size(void);
char						*generate_random_shm_name(void);
void						init_mutex(t_shared_int *sint);
void						init_audio(char *file);
void						init_video(char *file);
void						init_shared_buffer_child(void);
void						video_cleanup_cb(void *opaque);
void						*video_lock_cb(void *opaque, void **planes);
void						video_unlock_cb(void *opaque, void *picture,
								void *const *planes);
void						video_display_cb(void *opaque, void *picture);
void						init_shared_flags_child(void);
void						set_speed(void);
int							img_start_x(void);
int							img_start_y(void);
int							img_end_x(void);
int							img_end_y(void);
bool						file_exists(char *filename);
bool						is_video_file(char *path);
int							is_audio_file(char *path);
void						init_shared_buffer_parent(void);
void						clear_vlc_objects(bool is_video);
char						**make_execv_args(const char *filename,
								bool video_rendering);
void						play_sound_track(void);
bool						exec_sound_track(char *file);
char						**sound_name(int set, char **files);
void						sound_track(char **files);
pid_t						sound_track_pid(int set, pid_t value);
t_globle_vars				*vars(void);

#endif