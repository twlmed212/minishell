#!/bin/bash

# Minishell Docker Helper Script
# This script provides convenient commands for Docker operations

set -e

DOCKER_IMAGE="minishell:latest"
CONTAINER_NAME="minishell_container"

function show_help() {
    echo "Minishell Docker Helper"
    echo ""
    echo "Usage: ./docker-helper.sh [command]"
    echo ""
    echo "Commands:"
    echo "  build       - Build the Docker image"
    echo "  run         - Run minishell in a container"
    echo "  dev         - Start development environment (interactive bash)"
    echo "  rebuild     - Rebuild minishell inside running container"
    echo "  clean       - Remove Docker image and containers"
    echo "  valgrind    - Run minishell with valgrind"
    echo "  logs        - Show container logs"
    echo "  shell       - Connect to running container"
    echo "  help        - Show this help message"
}

function build_image() {
    echo "Building Docker image..."
    docker build -t $DOCKER_IMAGE .
    echo "Image built successfully!"
}

function run_minishell() {
    echo "Running minishell..."
    docker run -it --rm $DOCKER_IMAGE
}

function dev_mode() {
    echo "Starting development environment..."
    docker run -it --rm -v "$(pwd):/minishell" --name $CONTAINER_NAME $DOCKER_IMAGE /bin/bash
}

function rebuild_project() {
    echo "Rebuilding minishell in container..."
    docker exec $CONTAINER_NAME make re
}

function clean_docker() {
    echo "Cleaning Docker resources..."
    docker rm -f $CONTAINER_NAME 2>/dev/null || true
    docker rmi $DOCKER_IMAGE 2>/dev/null || true
    echo "Cleaned!"
}

function run_valgrind() {
    echo "Running with valgrind..."
    docker run -it --rm -v "$(pwd):/minishell" $DOCKER_IMAGE \
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
}

function show_logs() {
    docker logs $CONTAINER_NAME
}

function connect_shell() {
    docker exec -it $CONTAINER_NAME /bin/bash
}

# Main script logic
case "${1:-help}" in
    build)
        build_image
        ;;
    run)
        run_minishell
        ;;
    dev)
        dev_mode
        ;;
    rebuild)
        rebuild_project
        ;;
    clean)
        clean_docker
        ;;
    valgrind)
        run_valgrind
        ;;
    logs)
        show_logs
        ;;
    shell)
        connect_shell
        ;;
    help|*)
        show_help
        ;;
esac