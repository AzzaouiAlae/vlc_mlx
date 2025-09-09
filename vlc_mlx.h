/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_mlx.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:36 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/09/09 15:38:09 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VLC_MLX_H
# define VLC_MLX_H

/**
 * @file vlc_mlx.h
 * @brief Minimal VLC wrapper for rendering video/audio in MiniLibX.
 *
 * Provides a flag-driven API to play video/audio and copy video frames
 * into mlx images.
 */

# include <stdbool.h>
# include <stdlib.h>

/**
 * @brief Initialize internal flags for vlc_mlx.
 *
 * Must be called once before using any other vlc_mlx functions.
 */
void	init_flags(void);

/**
 * @brief Start playback of a video file with audio.
 *
 * The video frames are made available for copying into an mlx image.
 * Use new_frame() and copy_frame()/copy_resized_frame() to render frames.
 *
 * @param file Path to the video file.
 */
void	play_video(char *file);

/**
 * @brief Play an audio file without video.
 *
 * @param file Path to the audio file.
 */
void	play_audio(char *file);

/**
 * @brief Set playback speed.
 *
 * @param speed Playback speed percentage (100 = normal, 200 = 2×, etc.).
 */
void	set_play_speed(int speed);

/**
 * @brief Check if audio is currently playing.
 *
 * @return true if audio is playing, false otherwise.
 */
bool	played_audio(void);

/**
 * @brief Check if a video is currently playing.
 *
 * @return true if a video is being played.
 */
bool	should_play_video(void);

/**
 * @brief Check if VLC resources should be cleaned up.
 *
 * Call clear_vlc() when this returns true.
 *
 * @return true if cleanup is required.
 */
bool	should_clean_vlc(void);

/**
 * @brief Check if a new video frame is available.
 *
 * @return true if a new frame is ready.
 */
bool	new_frame(void);

/**
 * @brief Copy the current video frame into an mlx image.
 *
 * @param data_img Pointer to mlx image data buffer.
 * @param img_w    Width of the mlx image.
 * @param img_h    Height of the mlx image.
 */
void	copy_frame(void *data_img, size_t img_w, size_t img_h);

/**
 * @brief Copy and resize the current frame into an mlx image.
 *
 * @param dt    Pointer to mlx image data buffer.
 * @param img_w Destination image width.
 * @param img_h Destination image height.
 */
void	copy_resized_frame(void *dt, size_t img_w, size_t img_h);

/**
 * @brief Copy part of the current frame (cropped to start/end).
 *
 * Requires set_frame_start_end() to define the region.
 *
 * @param data Pointer to mlx image data buffer.
 */
void	copy_start_end_frame(void *data);

/**
 * @brief Set start and end coordinates of a frame region for cropping.
 *
 * Used with copy_start_end_frame().
 *
 * @param start_x X-coordinate of crop start.
 * @param start_y Y-coordinate of crop start.
 * @param end_x   X-coordinate of crop end.
 * @param end_y   Y-coordinate of crop end.
 */
void	set_frame_start_end(int start_x, int start_y, int end_x, int end_y);

/**
 * @brief Clean up VLC resources after playback.
 *
 * Should be called when should_clean_vlc() is true.
 */
void	clear_vlc(void);

/**
 * @brief Get the height of the current video.
 *
 * @return Video height in pixels.
 */
size_t	video_h(void);

/**
 * @brief Get the width of the current video.
 *
 * @return Video width in pixels.
 */
size_t	video_w(void);

/**
 * @brief Clean up all VLC resources before program exit.
 *
 * Should be called at the end of your program.
 */
void	exit_clear_vlc(void);

/**
 * @brief Get the current playback speed.
 *
 * @return Playback speed (100 = normal).
 */
int		play_speed(void);

/**
 * @brief Play multiple audio tracks sequentially.
 *
 * @param files NULL-terminated array of file paths.
 */
void	sound_track(char **files);

#endif