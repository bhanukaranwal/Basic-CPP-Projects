#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>
using namespace std;

// libcurl callback: store downloaded data in a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    string url;
    cout << "Enter RSS feed URL (e.g., https://feeds.bbci.co.uk/news/rss.xml): ";
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
            cerr << "Failed to fetch feed: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return 1;
        }
        curl_easy_cleanup(curl);
    } else {
        cerr << "CURL init failed.\n";
        return 1;
    }

    // Parse <item><title>...</title></item>
    vector<string> titles;
    regex itemRGX(R"(<item>(.*?)</item>)", regex::icase | regex::dotall);
    regex titleRGX(R"(<title>(.*?)</title>)", regex::icase | regex::dotall);
    auto item_begin = sregex_iterator(readBuffer.begin(), readBuffer.end(), itemRGX);
    auto item_end = sregex_iterator();
    for (auto it = item_begin; it != item_end; ++it) {
        smatch m;
        string item = (*it)[1];
        if (regex_search(item, m, titleRGX)) {
            string headline = regex_replace(m[1].str(), regex(R"(\s+)"), " ");
            titles.push_back(headline);
        }
    }

    // Output
    cout << "\nLatest RSS Headlines:\n";
    int n = 1;
    for (const auto &t : titles)
        cout << n++ << ". " << t << endl;
    if (titles.empty())
        cout << "No headlines found or feed format not supported.\n";

    return 0;
}
