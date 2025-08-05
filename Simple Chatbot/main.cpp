#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// Convert string to lowercase to make keyword matching case-insensitive
string toLower(const string &input) {
    string result = input;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void chatbotResponse(const string& input) {
    string userInput = toLower(input);

    if (userInput.find("hello") != string::npos ||
        userInput.find("hi") != string::npos) {
        cout << "Chatbot: Hello! How can I help you today?\n";
    } else if (userInput.find("how are you") != string::npos) {
        cout << "Chatbot: I'm a bot, but I'm doing great! How about you?\n";
    } else if (userInput.find("bye") != string::npos ||
               userInput.find("exit") != string::npos) {
        cout << "Chatbot: Goodbye! Have a nice day!\n";
    } else if (userInput.find("your name") != string::npos) {
        cout << "Chatbot: I'm ChatBot 1.0, your friendly assistant.\n";
    } else if (userInput.find("help") != string::npos) {
        cout << "Chatbot: I can answer simple questions like greetings, " 
             << "'how are you', 'your name', or say 'bye' to exit.\n";
    } else {
        cout << "Chatbot: Sorry, I didn't quite understand that. Try typing 'help'.\n";
    }
}

int main() {
    cout << "Welcome to Simple Chatbot! (type 'bye' to exit)\n";
    string userInput;

    while (true) {
        cout << "You: ";
        getline(cin, userInput);

        if (userInput.empty()) {
            cout << "Please enter something.\n";
            continue;
        }

        chatbotResponse(userInput);

        // If user says bye or exit, stop chatting
        string lowerInput = toLower(userInput);
        if (lowerInput.find("bye") != string::npos ||
            lowerInput.find("exit") != string::npos) {
            break;
        }
    }

    return 0;
}
