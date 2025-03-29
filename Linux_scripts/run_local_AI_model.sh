#!/bin/bash

# Path to llama-cli and model
LLAMA_CLI_PATH="/Rohit/llama.cpp/buildLlama/build/bin/llama-cli"
MODEL_PATH="/Rohit/Local_model/Qwen2.5-14B-Instruct-Q4_K_M.gguf"

# Run llama model in interactive mode
"$LLAMA_CLI_PATH" -m "$MODEL_PATH" --n-gpu-layers 20 --interactive
