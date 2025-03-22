from llama_cpp import Llama

try:
    # Load the model
    llm = Llama(
        model_path="/Rohit/AI_models/downloaded/Tifa-Deepsex-14b-CoT-Q4_K_M.gguf",
        n_gpu_layers=20,  # Adjust based on GPU memory
        n_ctx=2048, #Controls the maximum number of tokens the model can consider at once (the "context window")
        n_threads=4, # cpu threads
        verbose=True  # Enable detailed logs
    )

    print("AI: Hello! How can I assist you today? (Type 'exit' to end the conversation)")

    while True:
        user_input = input("You: ")
        
        if user_input.lower() == "exit":
            print("AI: Goodbye!")
            break

        # Generate a response
        response = llm(user_input, max_tokens=50)
        
        # Safely extract response text
        ai_response = response.get("choices", [{}])[0].get("text", "").strip()
        
        print(f"AI: {ai_response if ai_response else 'I am not sure how to respond.'}")

except Exception as e:
    print(f"Error: {e}")
