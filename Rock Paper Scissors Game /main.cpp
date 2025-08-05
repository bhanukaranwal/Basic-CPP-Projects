#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

string getMoveName(int move) {
    switch (move) {
        case 1: return "Rock";
        case 2: return "Paper";
        case 3: return "Scissors";
        default: return "Unknown";
    }
}

// Returns: 0-draw, 1-user wins, 2-computer wins
int decideWinner(int user, int comp) {
    if (user == comp) return 0;
    if ((user == 1 && comp == 3) || 
        (user == 2 && comp == 1) || 
        (user == 3 && comp == 2)) return 1;
    return 2;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int userScore = 0, compScore = 0, rounds = 0;
    cout << "--- Rock Paper Scissors Game ---\n";
    cout << "Choices: 1 = Rock, 2 = Paper, 3 = Scissors\n";

    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        int userChoice;
        cout << "\nEnter your choice (1-3): ";
        cin >> userChoice;
        if (userChoice < 1 || userChoice > 3) {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
            continue;
        }
        int compChoice = 1 + rand() % 3;
        cout << "You chose: " << getMoveName(userChoice) << "\n";
        cout << "Computer chose: " << getMoveName(compChoice) << "\n";

        int winner = decideWinner(userChoice, compChoice);
        if (winner == 0) {
            cout << "It's a draw!\n";
        } else if (winner == 1) {
            cout << "You WIN this round!\n";
            userScore++;
        } else {
            cout << "Computer WINS this round!\n";
            compScore++;
        }
        rounds++;
        cout << "Score: You " << userScore << " - " << compScore << " Computer (Rounds: " << rounds << ")\n";
        cout << "Play again? (y/n): ";
        cin >> playAgain;
    }
    cout << "\nFinal Score: You " << userScore << " - " << compScore << " Computer (Total rounds: " << rounds << ")\n";
    if (userScore == compScore) cout << "Overall Result: DRAW!\n";
    else if (userScore > compScore) cout << "You WIN overall, congrats!\n";
    else cout << "Computer wins overall, better luck next time.\n";
    cout << "Goodbye!\n";
    return 0;
}
