# vlc\_mlx

A small, easy-to-use wrapper that integrates VLC media playback with `mlx`.
`vlc_mlx` lets you render video frames and play audio inside an `mlx` application via a minimal, flag-driven API.

---

## Features

* Play video with audio while rendering frames into an `mlx` image.
* Play standalone audio files and control playback speed.
* Simple flag-based API for easy integration into a game loop.
* Explicit init / cleanup functions to avoid leaking resources.

---

## Requirements

* `mlx` (MiniLibX)

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

* `play_video()` plays the video and its audio track and makes frames available via `new_frame()`/`copy_frame()`.
* `copy_frame()` copies the current frame into your `mlx` image buffer. Use `video_w()` / `video_h()` to query frame size.
* `copy_resized_frame()` can be used when destination buffer dimensions differ from the source.
* `should_clean_vlc()` becomes true when playback finishes; call `clear_vlc()` to free playback resources.
* `set_play_speed(int speed)` expects an integer where `100` is normal speed (e.g. `150` == 1.5×).

---

## Example integration

```c
int render_game(void *param)
{
    (void)param;

    if (new_frame())
    {
        // copy latest video frame into your mlx image and present it
        copy_frame(&g_win_img, 1920, 1080);
        mlx_put_image_to_window(g_mlx, g_win, g_win_img.img, 0, 0);
    }
    else if (!should_play_video())
    {
        // normal game rendering & logic
        set_play_speed(150); // example: 1.5x audio speed
        play_audio(get_audio_file_name());
    }

    if (should_clean_vlc())
        clear_vlc();

    return 0;
}

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    // initialize library flags and mlx
    init_flags();
    // ... create mlx context, window, and images ...

    mlx_loop_hook(g_mlx, render_game, NULL);

    // start playback (provides frames to the render loop)
    play_video("media/intro1.mp4");

    mlx_loop(g_mlx);

    // final cleanup in case the loop exits
    exit_clear_vlc();
    return 0;
}

int close_window(void *param)
{
    (void)param;
    exit_clear_vlc();
    exit(0);
    return 0;
}
```

---

## How to compile

The repository builds `vlc_mlx` as a sub-target. Example `Makefile` snippet to compile your program and build the `vlc_mlx` static library first:

```makefile
NAME := my_app
CC   := gcc
CFLAGS := -Wall -Wextra -Werror `pkg-config --cflags libvlc` 

SRCS := main.c game.c ...
OBJ  := $(SRCS:.c=.o)

all: mlx_vlc $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) vlc_mlx/libvlcmlx.a -o $(NAME)

# build the vlc_mlx library (runs the Makefile inside the subdirectory)
mlx_vlc:
	@$(MAKE) -C vlc_mlx
```

**Notes & tips**

* Ensure `vlc_mlx/libvlcmlx.a` is built before linking your binary (the `mlx_vlc` target handles this).

---

## Tips & integration notes

* Call `init_flags()` once before starting playback.
* Use `new_frame()` to detect when `copy_frame()` has fresh frame data to render.
* Always call `exit_clear_vlc()` for final cleanup before terminating the program.
* Use `should_clean_vlc()` + `clear_vlc()` to free playback resources immediately after a playback finishes.

---

## Contributing

Contributions, bug reports, and feature requests are welcome. Please open an issue or submit a pull request with a clear description and a minimal reproduction case when possible.

---

## License

Choose and include a license (e.g., MIT, Apache-2.0). If you want, I can add an MIT or Apache license file for you.

---

If you want, I can:

* add a short `Makefile` tailored to your project layout,
* include a small sample program that compiles and runs with `vlc_mlx`, or
* produce a ready-to-use `LICENSE` file. Which would you like next?
