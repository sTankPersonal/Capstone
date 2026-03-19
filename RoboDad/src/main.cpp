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

int main() {
    std::setlocale(LC_NUMERIC, "C");

    std::string model_path = "C:/Users/spide/source/repos/Capstone/RoboDad/includes/llama/refact-1_6b-fim-q4_k_m.gguf";
    std::string prompt = "Is it okay if my code is terrible? Yes or no?";
    int n_predict = 64;

    // load backend
    ggml_backend_load_all();

    std::cout << "load model\n";

    // load model
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = 0; // CPU only

    llama_model* model = llama_model_load_from_file(model_path.c_str(), model_params);
    if (!model) {
        std::cerr << "Failed to load model\n";
        return 1;
    }

    const llama_vocab* vocab = llama_model_get_vocab(model);

    std::cout << "tokenize prompt\n";
    // tokenize prompt
    int n_prompt = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), NULL, 0, true, true);
    std::vector<llama_token> tokens(n_prompt);

    if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), tokens.data(), tokens.size(), true, true) < 0) {
        std::cerr << "Tokenization failed\n";
        return 1;
    }

    std::cout << "context\n";
    // context
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = n_prompt + n_predict;
    ctx_params.n_batch = n_prompt;

    llama_context* ctx = llama_init_from_model(model, ctx_params);
    if (!ctx) {
        std::cerr << "Failed to create context\n";
        return 1;
    }

    // sampler (greedy)
    llama_sampler* smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_greedy());


    std::cout << "print prompt\n";
    // print prompt
    std::cout << prompt << std::endl;

    // first batch
    llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size());

    std::cout << "generation loop\n";
    // generation loop
    for (int i = 0; i < n_predict; i++) {
        if (llama_decode(ctx, batch)) {
            std::cerr << "Decode failed\n";
            return 1;
        }

        llama_token new_token = llama_sampler_sample(smpl, ctx, -1);

        if (llama_vocab_is_eog(vocab, new_token)) break;

        char buf[128];
        int n = llama_token_to_piece(vocab, new_token, buf, sizeof(buf), 0, true);
        std::cout << std::string(buf, n);
        std::cout.flush();

        batch = llama_batch_get_one(&new_token, 1);
    }

    std::cout << "\n";
    std::cout << "Starting cleanup\n";

    // cleanup
    llama_sampler_free(smpl);
    llama_free(ctx);
    llama_model_free(model);

    return 0;
}