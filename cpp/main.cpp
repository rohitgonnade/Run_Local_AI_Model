
#include "llama.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

/*
mkdir -p build && cd build
cmake ..
make
./chatbot_server

*/

llama_model * model = nullptr;
llama_context * ctx = nullptr;
const llama_vocab * vocab = nullptr;
llama_sampler * smpl = nullptr;

std::vector<llama_chat_message> messages;

const std::string model_path = "/Rohit/Local_model/Qwen2.5-14B-Instruct-Q4_K_M.gguf"; 
const std::string log_path = "/Rohit/chat_log.txt";

void initializeModel()
{

    // load dynamic backends
    ggml_backend_load_all();

    // initialize the model
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = 20;

    model = llama_model_load_from_file(model_path.c_str(), model_params);
    if (!model) {
        fprintf(stderr , "%s: error: unable to load model\n" , __func__);
        return ;
    }

    vocab = llama_model_get_vocab(model);

    // initialize the context
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 2048;
    ctx_params.n_batch = 512; //n_batch controls how many tokens are processed at once during inference (not training)

    ctx = llama_init_from_model(model, ctx_params);
    if (!ctx) {
        fprintf(stderr , "%s: error: failed to create the llama_context\n" , __func__);
        return ;
    }

    // initialize the sampler
    smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_min_p(0.05f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.8f));
    llama_sampler_chain_add(smpl, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));
}

void generateResponse(const std::string& userMessage)
{
    
    // helper function to evaluate a prompt and generate a response
    auto generate = [&](const std::string & prompt) {
        std::string response;

        const bool is_first = llama_kv_self_used_cells(ctx) == 0;

        // tokenize the prompt
        const int n_prompt_tokens = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), NULL, 0, is_first, true);
        std::vector<llama_token> prompt_tokens(n_prompt_tokens);
        if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), is_first, true) < 0) {
            GGML_ABORT("failed to tokenize the prompt\n");
        }

        // prepare a batch for the prompt
        llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
        llama_token new_token_id;
        while (true) {
            // check if we have enough space in the context to evaluate this batch
            int n_ctx = llama_n_ctx(ctx);
            int n_ctx_used = llama_kv_self_used_cells(ctx);
            if (n_ctx_used + batch.n_tokens > n_ctx) {
                printf("\033[0m\n");
                fprintf(stderr, "context size exceeded\n");
                exit(0);
            }

            if (llama_decode(ctx, batch)) {
                GGML_ABORT("failed to decode\n");
            }

            // sample the next token
            new_token_id = llama_sampler_sample(smpl, ctx, -1);

            // is it an end of generation?
            if (llama_vocab_is_eog(vocab, new_token_id)) {
                break;
            }

            // convert the token to a string, print it and add it to the response
            char buf[256];
            int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
            if (n < 0) {
                GGML_ABORT("failed to convert token to piece\n");
            }
            std::string piece(buf, n);
            printf("%s", piece.c_str());
            fflush(stdout);
            response += piece;

            // prepare the next batch with the sampled token
            batch = llama_batch_get_one(&new_token_id, 1);
        }

        return response;
    };

    std::vector<char> formatted(llama_n_ctx(ctx));
    int prev_len = 0;

    const char * tmpl = llama_model_chat_template(model, /* name */ nullptr);

    // add the user input to the message list and format it
    messages.emplace_back(llama_chat_message{"user",  strdup(userMessage.c_str())});

    int new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    if (new_len > (int)formatted.size()) {
        formatted.resize(new_len);
        new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    }
    if (new_len < 0) {
        fprintf(stderr, "failed to apply the chat template\n");
        return ;
    }

    // remove previous messages to obtain the prompt to generate the response
    std::string prompt(formatted.begin() + prev_len, formatted.begin() + new_len);

    // generate a response
    printf("\033[33m");
    std::string response = generate(prompt);
    printf("\n\033[0m");

    // add the response to the messages
    messages.emplace_back(llama_chat_message{"assistant", strdup(response.c_str())});
    prev_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), false, nullptr, 0);
    if (prev_len < 0) {
        fprintf(stderr, "failed to apply the chat template\n");
        return ;
    }
    
}

int main() {
   
    initializeModel();

    std::string user;
    while (true) {
        // get user input
        printf("\033[32m> \033[0m");
        std::getline(std::cin, user);

        generateResponse(user);
        if (user == "exit") {
            break;
        }
    }

    // Save messages to a file
    std::ofstream outFile(log_path);
    if (outFile.is_open()) {
        for (auto &msg : messages) {
            outFile << "[" << msg.role << "] " << msg.content << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Failed to open chat_log.txt for writing.\n";
    }

    // free resources
    for (auto & msg : messages) {
        free(const_cast<char *>(msg.content));
    }

    llama_sampler_free(smpl);
    llama_free(ctx);
    llama_model_free(model);
}
