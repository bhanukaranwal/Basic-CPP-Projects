#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>
using namespace std;

// Replace with your OpenWeatherMap API Key!
const string API_KEY = "YOUR_API_KEY_HERE";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Helper to extract value from json by regex
string extract(const string& text, const string& rgx) {
    smatch m;
    if (regex_search(text, m, regex(rgx))) 
        return m[1];
    return "N/A";
}

int main() {
    cout << "--- Weather Information Fetcher ---\n";
    cout << "Enter city name: ";
    string city;
    getline(cin, city);
    string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city +
                 "&appid=" + API_KEY + "&units=metric";
    CURL* curl = curl_easy_init();
    string buffer;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "Failed to fetch weather: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return 1;
        }
        curl_easy_cleanup(curl);
    } else {
        cerr << "CURL init failed.\n";
        return 1;
    }

    // Parse JSON with regex (simple, not robust)
    string temp = extract(buffer, R"("temp":([0-9.\-]+))");
    string desc = extract(buffer, R"("description":"([^"]+)")");
    string humidity = extract(buffer, R"("humidity":([0-9]+))");
    string wind = extract(buffer, R"("speed":([0-9.\-]+))");

    cout << "\n-- Weather for " << city << " --\n";
    cout << "Temperature: " << temp << " \xB0C\n";
    cout << "Description: " << desc << "\n";
    cout << "Humidity:    " << humidity << "%\n";
    cout << "Wind Speed:  " << wind << " m/s\n";
    cout << "\n(Data from OpenWeatherMap.org)\n";

    return 0;
}
