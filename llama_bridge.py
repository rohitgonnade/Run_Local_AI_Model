import pexpect
import sys

# Path to llama-cli and model
LLAMA_CLI_PATH="/Rohit/llama.cpp/buildLlama/build/bin/llama-cli"
MODEL_PATH="/Rohit/Local_model/Qwen2.5-14B-Instruct-Q4_K_M.gguf"

# Launch llama-cli in interactive mode
child = pexpect.spawn(f"{LLAMA_CLI_PATH} -m {MODEL_PATH} --interactive -sp", encoding='utf-8', timeout=None)

# Optional: Wait for the initial prompt (if your model outputs one)
child.expect(">", timeout=20)  # Adjust if your model uses a different prompt

# Send hardcoded input
prompt = "What is the capital of France?"
child.sendline(prompt)

# Wait for the model to respond (again, wait until the next prompt appears)
child.expect(">")

# Capture and print the response
response = child.before.strip()
print(response)
