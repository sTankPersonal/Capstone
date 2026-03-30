//#include <iostream>
//#define ASIO_STANDALONE
//#include "asio.hpp"
//#include "crow/crow_all.h"
//
//int main() {
//    crow::SimpleApp app;
//
//    CROW_ROUTE(app, "/")([]() {
//        return "<h1>Hello, Crow + ASIO is working!</h1>";
//        });
//
//    std::cout << "Server running on http://127.0.0.1:18080\n";
//
//    app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
//}
#include "llama.h"
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <chrono>   // <-- include for timing

int main() {
    std::setlocale(LC_NUMERIC, "C");

    std::string model_path = "C:/models/llama-3-8b-instruct-q4.gguf";
    int n_predict = 128;

    // Preprompt for "dad persona" without explicit BOS
    std::string preprompt =
        "You are a caring, wise dad. Answer the user’s questions "
        "like a supportive father would, giving advice and guidance. Limit responses to 50 words\n";

    // Load backend
    ggml_backend_load_all();

    std::cout << "Loading model...\n";
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = 35;
    llama_model* model = llama_model_load_from_file(model_path.c_str(), model_params);
    if (!model) {
        std::cerr << "Failed to load model\n";
        return 1;
    }

    const llama_vocab* vocab = llama_model_get_vocab(model);

    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 2048;
    ctx_params.n_batch = 2048;
    ctx_params.n_threads = 12;

    llama_context* ctx = llama_init_from_model(model, ctx_params);
    if (!ctx) {
        std::cerr << "Failed to create context\n";
        return 1;
    }

    llama_sampler* smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_greedy());

    std::cout << "Model loaded. You can now ask questions. Type 'exit' to quit.\n";

    while (true) {
        std::string user_input;
        std::cout << "\n> ";
        std::getline(std::cin, user_input);

        // Skip empty input
        if (user_input.empty()) continue;
        if (user_input == "exit" || user_input == "quit") break;

        // Combine preprompt with current user question
        std::string prompt =
            "<|start_header_id|>user<|end_header_id|>\n" +
            preprompt +
            "<|start_header_id|>user<|end_header_id|>\n" +
            user_input + "\n<|eot_id|>"
            "<|start_header_id|>assistant<|end_header_id|>\n";

        // Tokenize
        int n_prompt = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), nullptr, 0, true, true);
        std::vector<llama_token> tokens(n_prompt);
        if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), tokens.data(), tokens.size(), true, true) < 0) {
            std::cerr << "Tokenization failed\n";
            continue;
        }

        llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size());
        std::string out_buffer;

        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n_predict; i++) {
            if (llama_decode(ctx, batch)) {
                std::cerr << "Decode failed\n";
                break;
            }

            llama_token new_token = llama_sampler_sample(smpl, ctx, -1);
            if (llama_vocab_is_eog(vocab, new_token)) break;

            char buf[128];
            int n = llama_token_to_piece(vocab, new_token, buf, sizeof(buf), 0, true);

            // STREAM OUTPUT: print as we generate
            std::cout.write(buf, n);
            std::cout.flush();

            batch = llama_batch_get_one(&new_token, 1);
        }
        std::cout << "\n";

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;

        std::cout << out_buffer << "\n";
        std::cout << "(Response generated in " << elapsed.count() << " seconds)\n";
    }

    llama_sampler_free(smpl);
    llama_free(ctx);
    llama_model_free(model);

    return 0;
}