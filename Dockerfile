FROM debian:bookworm-slim

ARG DEBIAN_FRONTEND=noninteractive
ARG PKGS="build-essential binutils gcc-multilib make gdb valgrind file less vim neovim tmux ca-certificates procps bsdextrautils"

RUN apt-get update \
  && apt-get install -y --no-install-recommends $PKGS \
  && rm -rf /var/lib/apt/lists/*

# RUN printf '%s\n' \
#   'export HISTFILE=/work/.bash_history' \
#   'export HISTSIZE=10000' \
#   'export HISTFILESIZE=20000' \
#   'shopt -s histappend' \
#   'PROMPT_COMMAND="history -a; history -n"' \
#   > /root/.bashrc

USER 65532:65532

WORKDIR /work
