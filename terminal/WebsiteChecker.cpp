#include "WebsiteChecker.hpp"
#include <curl/curl.h>
#include <iostream>

bool WebsiteChecker::check_website(const std::string& url) {
    CURL* curl;
    CURLcode res;
    bool is_up = false;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // we don't need body content
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // set timeout to 10 seconds

        res = curl_easy_perform(curl);

        if(res == CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            if(response_code == 200) {
                is_up = true;
            } else {
                std::cout << "Received HTTP response code: " << response_code << std::endl;
            }
        } else {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return is_up;
}
