LIBVLC_DIR := $(HOME)/libvlc
LIBVLC_URL := git@github.com:AzzaouiAlae/lib_vlc_for_mlx_vlc.git

.PHONY: all libvlc

all: libvlc

libvlc:
	@if pkg-config --exists libvlc; then \
		echo "✅ libvlc already installed system-wide." >/dev/null ; \
	elif [ -d "$(LIBVLC_DIR)" ]; then \
		echo "✅ libvlc already downloaded in $(LIBVLC_DIR)." >/dev/null; \
	else \
		echo "⚠️ libvlc not found. Downloading precompiled version..." >/dev/null; \
		mkdir -p $(LIBVLC_DIR); \
		git clone $(LIBVLC_URL) $(LIBVLC_DIR); \
	fi
	
