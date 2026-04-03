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


#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <cmath>

struct Stats {
    int min;
    int max;
    double mean;
    double stddev;
    double median;

    // Common percentiles
    double p25;
    double p50; // same as median
    double p75;
    double p90;
    double p95;
    double p99;

    std::unordered_map<int, int> frequency;
};

// Helper to compute percentile
double compute_percentile(const std::vector<int>& sorted, double percentile) {
    double index = percentile * (sorted.size() - 1);
    int lower = static_cast<int>(index);
    int upper = lower + 1;

    if (upper >= sorted.size())
        return sorted[lower];

    double fraction = index - lower;
    return sorted[lower] + fraction * (sorted[upper] - sorted[lower]);
}

Stats analyze_random_numbers(int count = 500, int min_val = 0, int max_val = 100) {
    std::vector<int> numbers;
    numbers.reserve(count);

    // RNG setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min_val, max_val);

    // Generate numbers
    for (int i = 0; i < count; i++) {
        numbers.push_back(dist(gen));
    }

    // Min / Max
    auto [min_it, max_it] = std::minmax_element(numbers.begin(), numbers.end());

    // Mean
    double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
    double mean = sum / count;

    // Std Dev
    double variance = 0.0;
    for (int num : numbers) {
        variance += (num - mean) * (num - mean);
    }
    variance /= count;
    double stddev = std::sqrt(variance);

    // Frequency
    std::unordered_map<int, int> freq;
    for (int num : numbers) {
        freq[num]++;
    }

    // Sort for median + percentiles
    std::sort(numbers.begin(), numbers.end());

    // Median
    double median;
    if (count % 2 == 0) {
        median = (numbers[count / 2 - 1] + numbers[count / 2]) / 2.0;
    }
    else {
        median = numbers[count / 2];
    }

    // Percentiles
    double p25 = compute_percentile(numbers, 0.25);
    double p50 = compute_percentile(numbers, 0.50);
    double p75 = compute_percentile(numbers, 0.75);
    double p90 = compute_percentile(numbers, 0.90);
    double p95 = compute_percentile(numbers, 0.95);
    double p99 = compute_percentile(numbers, 0.99);

    return Stats{
        *min_it,
        *max_it,
        mean,
        stddev,
        median,
        p25,
        p50,
        p75,
        p90,
        p95,
        p99,
        freq
    };
}

void print_stats(const Stats& s) {
    std::cout << "Min: " << s.min << "\n";
    std::cout << "Max: " << s.max << "\n";
    std::cout << "Mean: " << s.mean << "\n";
    std::cout << "Std Dev: " << s.stddev << "\n";
    std::cout << "Median: " << s.median << "\n";

    std::cout << "\nPercentiles:\n";
    std::cout << "25%: " << s.p25 << "\n";
    std::cout << "50%: " << s.p50 << "\n";
    std::cout << "75%: " << s.p75 << "\n";
    std::cout << "90%: " << s.p90 << "\n";
    std::cout << "95%: " << s.p95 << "\n";
    std::cout << "99%: " << s.p99 << "\n";

    std::cout << "\nFrequencies:\n";
    for (const auto& [value, count] : s.frequency) {
        std::cout << value << ": " << count << "\n";
    }
}

std::string stats_to_prompt(const Stats& s) {
    std::string prompt;
    prompt += "Here are some summary statistics of a set of numbers:\n";
    prompt += "Min: " + std::to_string(s.min) + "\n";
    prompt += "Max: " + std::to_string(s.max) + "\n";
    prompt += "Mean: " + std::to_string(s.mean) + "\n";
    prompt += "Median: " + std::to_string(s.median) + "\n";
    prompt += "Std Dev: " + std::to_string(s.stddev) + "\n";
    prompt += "25th percentile: " + std::to_string(s.p25) + "\n";
    prompt += "50th percentile: " + std::to_string(s.p50) + "\n";
    prompt += "75th percentile: " + std::to_string(s.p75) + "\n";
    prompt += "90th percentile: " + std::to_string(s.p90) + "\n";
    prompt += "95th percentile: " + std::to_string(s.p95) + "\n";
    prompt += "99th percentile: " + std::to_string(s.p99) + "\n";
    return prompt;
}


int main() {
    std::setlocale(LC_NUMERIC, "C");

    std::string model_path = "C:/models/llama-3-8b-instruct-q4.gguf";
    int n_predict = 128;

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


    Stats stats = analyze_random_numbers();
    //print_stats(stats);
    std::string num_insights = stats_to_prompt(stats);


    while (true) {
        std::string user_input;
        std::cout << "\n> ";
        std::getline(std::cin, user_input);

        // Skip empty input
        if (user_input.empty()) continue;
        if (user_input == "exit" || user_input == "quit") break;

        // Preprompt for "dad persona" without explicit BOS
        std::string preprompt =
            "You are a dad. Answer the user's questions "
            "Have a conversation with the user and provide guidance.\n";
            //"Here are some summary statistics of a set of numbers that you know about:\n" +
            //num_insights;


        // Combine preprompt with current user question
        std::string prompt =
            "<|start_header_id|>user<|end_header_id|>\n" +
            preprompt +
            "Please give insights about these numbers in a supportive, dad-style manner.\n" +
            "<|eot_id|><|start_header_id|>assistant<|end_header_id|>\n";

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