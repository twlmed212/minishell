# Use Ubuntu as base image for better compatibility with 42 projects
FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install necessary packages
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    gcc \
    g++ \
    make \
    libc6-dev \
    libc6-dbg \
    libreadline-dev \
    readline-common \
    valgrind \
    gdb \
    vim \
    git \
    curl \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Verify valgrind installation
RUN valgrind --version

# Set working directory
WORKDIR /minishell

# Copy the project files
COPY . .

# Clean any existing build artifacts
RUN make fclean || true

# Build the project
RUN make

# Set the entrypoint to run minishell
ENTRYPOINT ["./minishell"]

# Default command (can be overridden)
CMD []