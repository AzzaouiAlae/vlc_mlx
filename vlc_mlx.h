#ifndef VLC_MLX_H
# define VLC_MLX_H

# include <stdbool.h>
# include <stdlib.h>

bool	new_frame(void);
bool	should_play_video(void);
bool	should_clean_vlc(void);
bool	played_audio(void);
void	play_video(char *file);
void	play_audio(char *file);
void	copy_frame(void *data_img, size_t img_w, size_t img_h);
void	copy_resized_frame(void *dt, size_t img_w, size_t img_h);
void	clear_vlc(void);
size_t	video_h(void);
size_t	video_w(void);
void	init_shared_flags_parent(void);
void	exit_clear_vlc(void);
int		play_speed(void);
void	set_play_speed(int speed);

#endif