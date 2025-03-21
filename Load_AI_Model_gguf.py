from llama_cpp import Llama

# Load the model from the path where it is stored
llm = Llama(model_path="../../AI_models/downloaded/Tifa-Deepsex-14b-CoT-Q4_K_M.gguf")

# Start a conversation loop
print("AI: Hello! How can I assist you today? (Type 'exit' to end the conversation)")
while True:
    # Get user input
    user_input = input("You: ")

    # Exit the conversation if the user types 'exit'
    if user_input.lower() == "exit":
        print("AI: Goodbye!")
        break

    # Generate a response from the model
    response = llm(user_input, max_tokens=50)

    # Print the AI's response
    print(f"AI: {response['choices'][0]['text']}")