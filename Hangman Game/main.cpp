#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

const vector<string> WORDS = {
    "computer", "hangman", "challenge", "programming", "giraffe",
    "banana", "universe", "umbrella", "library", "project"
};

void printHangman(int wrong) {
    static const vector<string> stages = {
        "  +---+\n      |\n      |\n      |\n     ===\n",
        "  +---+\n  O   |\n      |\n      |\n     ===\n",
        "  +---+\n  O   |\n  |   |\n      |\n     ===\n",
        "  +---+\n  O   |\n /|   |\n      |\n     ===\n",
        "  +---+\n  O   |\n /|\\  |\n      |\n     ===\n",
        "  +---+\n  O   |\n /|\\  |\n /    |\n     ===\n",
        "  +---+\n  O   |\n /|\\  |\n / \\  |\n     ===\n"
    };
    cout << stages[wrong] << endl;
}

int main() {
    srand(time(0));
    string word = WORDS[rand() % WORDS.size()];
    string guessed(word.size(), '_');
    vector<char> wrongGuesses;
    int maxWrong = 6, wrong = 0;

    cout << "--- Hangman Game ---\n";
    while (wrong < maxWrong && guessed != word) {
        printHangman(wrong);
        cout << "\nWord: ";
        for (char c : guessed) cout << c << ' ';
        cout << "\nWrong guesses: ";
        for (char c : wrongGuesses) cout << c << ' ';
        cout << "\nGuess a letter: ";
        char letter; cin >> letter;
        letter = tolower(letter);
        bool good = false;

        for (size_t i = 0; i < word.size(); ++i) {
            if (word[i] == letter && guessed[i] == '_') {
                guessed[i] = letter;
                good = true;
            }
        }
        if (!good) {
            if (find(wrongGuesses.begin(), wrongGuesses.end(), letter) == wrongGuesses.end()) {
                wrong++;
                wrongGuesses.push_back(letter);
            } else {
                cout << "You already guessed that letter!\n";
            }
        }
    }

    printHangman(wrong);
    if (guessed == word) {
        cout << "Congratulations, you guessed the word: " << word << "!\n";
    } else {
        cout << "Sorry, you lost. The word was: " << word << "\n";
    }
    return 0;
}
