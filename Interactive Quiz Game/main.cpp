#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Question {
    string text;
    vector<string> choices;
    int correct; // zero-based index of correct answer
};

void printQuestion(const Question& q, int qnum) {
    cout << "\nQ" << qnum << ". " << q.text << "\n";
    for (size_t i = 0; i < q.choices.size(); ++i)
        cout << "  " << char('A'+i) << ") " << q.choices[i] << "\n";
    cout << "Your answer (A-" << char('A'+q.choices.size()-1) << "): ";
}

int main() {
    vector<Question> quiz = {
        {"What is the capital of France?", {"London","Berlin","Paris","Madrid"}, 2},
        {"Which planet is known as the Red Planet?", {"Earth","Mars","Jupiter","Venus"}, 1},
        {"How many continents are there?", {"5","6","7","8"}, 2},
        {"Who wrote 'Romeo and Juliet'?", {"Shakespeare","Tagore","Tolstoy","Dickens"}, 0},
        {"What is 3 x 3?", {"6","9","12","27"}, 1}
    };

    int score = 0;
    char ans;
    cout << "--- Interactive Quiz Game ---\n";
    for (size_t i = 0; i < quiz.size(); ++i) {
        printQuestion(quiz[i], i+1);
        cin >> ans;
        ans = toupper(ans);
        if (ans - 'A' == quiz[i].correct) {
            cout << "Correct!\n";
            score++;
        } else {
            cout << "Incorrect. The correct answer is " 
                 << char('A'+quiz[i].correct) << ") "
                 << quiz[i].choices[quiz[i].correct] << "\n";
        }
    }
    cout << "\nQuiz Over! Your Score: " << score << "/" << quiz.size() << endl;
    if (score == (int)quiz.size()) cout << "Excellent!!\n";
    else if (score >= 3) cout << "Good job!\n";
    else cout << "Better luck next time.\n";
    return 0;
}
