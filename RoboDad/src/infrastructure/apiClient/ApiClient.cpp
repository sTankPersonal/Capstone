#include "infrastructure/apiClient/ApiClient.h"
#include <curl/curl.h>
#include <stdexcept>

ApiClient::ApiClient(std::string apiKey, std::string baseUrl)
    : apiKey_(std::move(apiKey)), baseUrl_(std::move(baseUrl)) {}

static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t total = size * nmemb;
    static_cast<std::string*>(userdata)->append(ptr, total);
    return total;
}

static std::string performRequest(CURL* curl) {
    std::string buffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        throw std::runtime_error(
            std::string("ApiClient: curl error: ") + curl_easy_strerror(res)
        );
    }

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    if (httpCode >= 400) {
        throw std::runtime_error(
            "ApiClient: HTTP " + std::to_string(httpCode) + " — " + buffer
        );
    }

    return buffer;
}

std::string ApiClient::httpPost(const std::string& url, const std::string& body,
                                const std::vector<std::string>& headers) const {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("ApiClient: failed to initialize curl");

    struct curl_slist* hdrs = nullptr;
    for (const auto& h : headers)
        hdrs = curl_slist_append(hdrs, h.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    std::string result;
    try {
        result = performRequest(curl);
    } catch (...) {
        curl_slist_free_all(hdrs);
        curl_easy_cleanup(curl);
        throw;
    }

    curl_slist_free_all(hdrs);
    curl_easy_cleanup(curl);
    return result;
}

std::string ApiClient::httpGet(const std::string& url,
                               const std::vector<std::string>& headers) const {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("ApiClient: failed to initialize curl");

    struct curl_slist* hdrs = nullptr;
    for (const auto& h : headers)
        hdrs = curl_slist_append(hdrs, h.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    std::string result;
    try {
        result = performRequest(curl);
    } catch (...) {
        curl_slist_free_all(hdrs);
        curl_easy_cleanup(curl);
        throw;
    }

    curl_slist_free_all(hdrs);
    curl_easy_cleanup(curl);
    return result;
}
