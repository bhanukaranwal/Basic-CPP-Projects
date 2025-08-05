#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>

using namespace std;

// Callback for libcurl: appends downloaded data into string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    string url;
    cout << "Enter a URL (e.g., https://www.example.com): ";
    getline(cin, url);

    CURL* curl = curl_easy_init();
    string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "Failed to fetch page: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return 1;
        }
        curl_easy_cleanup(curl);
    } else {
        cerr << "CURL init failed.\n";
        return 1;
    }

    // Extract and print <title>
    regex titleRGX("<title.*?>(.*?)</title>", regex_constants::icase | regex_constants::dotall);
    smatch match;
    if (regex_search(readBuffer, match, titleRGX)) {
        cout << "Page Title: " << match[1] << endl;
    } else {
        cout << "No <title> tag found.\n";
    }

    // Extract and print all href links
    cout << "\nAll HTTP Links found on the page:\n";
    regex linkRGX(R"(href\s*=\s*["'](https?://[^\"'>]+)["'])", regex_constants::icase);
    auto links_begin = sregex_iterator(readBuffer.begin(), readBuffer.end(), linkRGX);
    auto links_end = sregex_iterator();
    int count = 0;
    for (auto it = links_begin; it != links_end; ++it) {
        cout << ++count << ". " << (*it)[1] << endl;
    }
    if (count == 0)
        cout << "No HTTP links found.\n";

    return 0;
}
