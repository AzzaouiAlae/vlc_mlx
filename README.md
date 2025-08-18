# vlc\_mlx

A small, easy-to-use wrapper that integrates VLC media playback with `mlx`.
Use `vlc_mlx` to render video frames and play audio inside an `mlx` application with a minimal, flag-driven API.

---

## Features

* Play video with audio while rendering frames into an `mlx` image.
* Play standalone audio files and control playback speed.
* Simple flag-based API to integrate video/audio into your game loop.
* Explicit init / cleanup functions to avoid leaking resources.

---

## Requirements

* `mlx` (MiniLibX)
* A working VLC/libVLC build accessible to the library (linking handled when building).

---

## Quick start

Include the header:

```c
#include "vlc_mlx/vlc_mlx.h"
```

Initialize flags before using the library:

```c
init_flags();
```

Start playback:

```c
play_video("media/intro1.mp4");   // plays video + audio, provides frames to render
play_audio("sfx/footstep.wav");   // plays an audio file
set_play_speed(150);              // 100 == normal speed, 150 == 1.5x
```

When your program is about to exit, free all library resources:

```c
exit_clear_vlc();
```

---

## Public API

The header exposes the following functions:

```c
void    init_flags(void);
void    play_video(char *file);
void    play_audio(char *file);
void    set_play_speed(int speed);
bool    played_audio(void);
bool    should_play_video(void);
bool    should_clean_vlc(void);
bool    new_frame(void);
void    copy_frame(void *data_img, size_t img_w, size_t img_h);
void    copy_resized_frame(void *dt, size_t img_w, size_t img_h);
void    clear_vlc(void);
size_t  video_h(void);
size_t  video_w(void);
void    exit_clear_vlc(void);
int     play_speed(void);
```

**Notes**

* `play_video` plays both the video and its audio track. It also makes new frames available through `new_frame()` and `copy_frame()`.
* `copy_frame()` copies the current frame into your `mlx` image buffer. Use `video_w()`/`video_h()` for frame dimensions.
* `copy_resized_frame()` is provided if you need to copy into a buffer with different dimensions (implementation-specific).
* `should_clean_vlc()` and `clear_vlc()` let you free temporary playback resources when playback finishes.
* `set_play_speed(int speed)` takes an integer where `100` is normal speed (i.e., `150` == 1.5×).

---

## Example

A concise example integrating `vlc_mlx` into an `mlx` render loop:

```c
int render_game(void *param)
{
    (void)param;

    if (new_frame())
    {
        // copy the latest video frame into your mlx image and present it
        copy_frame(&g_win_img, 1920, 1080);
        mlx_put_image_to_window(g_mlx, g_win, g_win_img.img, 0, 0);
    }
    else if (!should_play_video())
    {
        // normal game rendering & logic when video is not playing
        // e.g. player movement, HUD, etc.
        set_play_speed(150); // example: 1.5x audio speed
        play_audio(get_audio_file_name());
    }

    if (should_clean_vlc())
        clear_vlc();

    return 0;
}

int main(int argc, char **argv)
{
    // initialization...
    init_flags();

    // set up mlx, windows, images, etc.
    mlx_loop_hook(g_mlx, render_game, NULL);

    // play a video (this will provide frames/audio to the loop)
    play_video("media/intro1.mp4");

    mlx_loop(g_mlx);

    // on exit, clean library resources (if you reach here)
    exit_clear_vlc();
    return 0;
}

int close_window(void *param)
{
    (void)param;
    exit_clear_vlc();
    exit(0);
    return (0);
}
```

---

## Tips & integration notes

* Call `init_flags()` once before starting playback.
* Use `new_frame()` to detect when `copy_frame()` has fresh frame data to render.
* Always call `exit_clear_vlc()` (for final cleanup) before terminating your program.
* `should_clean_vlc()` becomes true when playback has finished and resources should be released with `clear_vlc()`.

---

## Contributing

Contributions, bug reports, and feature requests are welcome. Please open an issue or submit a pull request with a clear description and a minimal repro when possible.

---

## License

Specify your preferred license here (e.g. MIT, Apache-2.0).
