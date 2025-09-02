/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_mlx.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:36 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 18:46:38 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VLC_MLX_H
# define VLC_MLX_H

# include <stdbool.h>
# include <stdlib.h>

void	init_flags(void);
void	play_video(char *file);
void	play_audio(char *file);
void	set_play_speed(int speed);
bool	played_audio(void);
bool	should_play_video(void);
bool	should_clean_vlc(void);
bool	new_frame(void);
void	copy_frame(void *data_img, size_t img_w, size_t img_h);
void	copy_resized_frame(void *dt, size_t img_w, size_t img_h);
void	copy_start_end_frame(void *dt);
void	clear_vlc(void);
size_t	video_h(void);
size_t	video_w(void);
void	exit_clear_vlc(void);
int		play_speed(void);
void	set_frame_start_end(int start_x, int start_y, int end_x, int end_y);
void	copy_start_end_frame(void *data);
void	sound_track(char **files);

#endif