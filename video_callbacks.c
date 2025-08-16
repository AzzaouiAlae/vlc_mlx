#include "vlc_mlx_init.h"

unsigned	video_format_cb(void **opaque, char *chroma, unsigned *width,
		unsigned *height, unsigned *pitches, unsigned *lines)
{
	video_ctx_t	*ctx;

	memcpy(chroma, "RV32", 4);
	if (!video_h() && !video_w())
	{
		shared_int_access(&shared_flags->video_height, 1, *height);
		shared_int_access(&shared_flags->video_width, 1, *width);
	}
	if (pitches[0] == 0)
    	pitches[0] = video_w() * 4;
	lines[0] = video_h();
	ctx = malloc(sizeof(*ctx));
	ctx->width = video_w();
	ctx->height = video_h();
	if (!shared_buffer)
	{
		shared_int_access(&shared_flags->buff_size, 1, pitches[0] * lines[0]);
		init_shared_buffer_child();
	}
	ctx->pitch = pitches[0];
	ctx->pixels = shared_buffer;
	*opaque = ctx;
	return (1);
}

void	video_cleanup_cb(void *opaque)
{
	free(opaque);
}

void	*video_lock_cb(void *opaque, void **planes)
{
	video_ctx_t	*ctx;

	ctx = (video_ctx_t *)opaque;
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
	shared_int_access(&shared_flags->new_frame_flag, 1, 1);
}
