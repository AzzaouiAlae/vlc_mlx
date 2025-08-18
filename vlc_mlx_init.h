#ifndef VLC_MLX_INIT_H
#define VLC_MLX_INIT_H

#include "vlc_mlx.h"
#include <vlc/vlc.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h> 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <strings.h>

#define SHM_NAME_LEN 16

typedef struct {
    pthread_mutex_t mutex;
    int value;
} shared_int_t;

typedef struct {
    shared_int_t should_clean;
    shared_int_t should_play;
    shared_int_t new_frame_flag;
    shared_int_t buff_size;
	shared_int_t video_width;
	shared_int_t video_height;
	shared_int_t played_audio;
	shared_int_t play_speed;
} shared_flags_t;

typedef struct {
	int x;
	int y;
	int img_y;
	int img_x;
	int videoW;
	int videoH;
	int step_x;
	int step_y;
	int sy;
	int sx;
	unsigned int c;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned int *src;
	unsigned int *dst;
	t_dt2 *data;
} t_resized_frame_dt;

typedef struct s_dt2
{
	void				*img;
	char				*addr;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
	int					img_width;
	int					img_height;
}						t_dt2;

extern shared_flags_t	*shared_flags;
extern void			*shared_buffer;
extern char			*flags_name;
extern char			*buffer_name;
extern pid_t			pid;
extern libvlc_media_t			*m;
extern libvlc_media_player_t	*mp;
extern libvlc_event_manager_t	*em;
extern libvlc_instance_t		*inst;
extern pid_t			video_pid;

typedef struct
{
	uint8_t				*pixels;
	unsigned			width;
	unsigned			height;
	unsigned			pitch;
}						video_ctx_t;

int	shared_int_access(shared_int_t *shared, int set, int value);
size_t	buff_size(void);
char	*generate_random_shm_name(void);
void	init_mutex(shared_int_t *sint);
void	init_audio(char *file);
void init_video(char *file);
void	init_shared_buffer_child(void);
unsigned	video_format_cb(void **opaque, char *chroma, unsigned *width,
		unsigned *height, unsigned *pitches, unsigned *lines);
void	video_cleanup_cb(void *opaque);
void	*video_lock_cb(void *opaque, void **planes);
void	video_unlock_cb(void *opaque, void *picture, void *const *planes);
void	video_display_cb(void *opaque, void *picture);
void init_shared_flags_child(void);
void set_speed();

#endif