#include<bits/stdc++.h>
using namespace std;

class User {
public:
    string username;
    string password;
    int highestScore;

    User() {}

    User(const string& uname, const string& pwd, int score = 0)
        : username(uname), password(pwd), highestScore(score) {}
};
map<string, User> readUsers() {
    map<string, User> users;
    ifstream file("users.txt");
    if (file.is_open()) {
        string uname, pwd;
        int score;
        while (file >> uname >> pwd >> score) {
            users[uname] = User(uname, pwd, score);
        }
        file.close();
    }
    return users;
}
void saveUsers(const map<string, User>& users) {
    ofstream file("users.txt");
    if (file.is_open()) {
        for (const auto& pair : users) {
            file << pair.second.username << " " << pair.second.password << " " << pair.second.highestScore << endl;
        }
        file.close();
    }
}
bool registerUser(map<string, User>& users) {
    string uname, pwd;

    cout << "Enter username: ";
    cin >> uname;
    cout << "Enter password: ";
    cin >> pwd;

    if (users.find(uname) != users.end()) {
        cout << "Username already exists. Try a different one." << endl;
        return false;
    }

    users[uname] = User(uname, pwd);
    saveUsers(users);
    cout << "Registration successful!" << endl;
    return true;
}
bool loginUser(map<string, User>& users, User& currentUser) {
    string uname, pwd;

    cout << "Enter username: ";
    cin >> uname;
    cout << "Enter password: ";
    cin >> pwd;

    if (users.find(uname) != users.end() && users[uname].password == pwd) {
        currentUser = users[uname];
        cout << "Login successful!" << endl;
        return true;
    }

    cout << "Invalid username or password." << endl;
    return false;
}
void updateHighScore(map<string, User>& users, User& currentUser, int score) {
    if (score > currentUser.highestScore) {
        currentUser.highestScore = score;
        users[currentUser.username] = currentUser;
        saveUsers(users);
        cout << "New high score: " << score << endl;
    } else {
        cout << "Your high score remains: " << currentUser.highestScore << endl;
    }
}

class Hangman {
private:
    string secretWord;
    string guessedWord;
    int remainingGuesses;
    vector<char> wrongGuesses;

public:
    Hangman(const string& word, int maxGuesses)
        : secretWord(word), guessedWord(word.size(), '_'), remainingGuesses(maxGuesses) {}

    void displayStatus() const {
        cout << "Secret Word: " << guessedWord << "\n";
        cout << "Remaining Guesses: " << remainingGuesses << "\n";
        cout << "Wrong Guesses: ";
        for (char ch : wrongGuesses) {
            cout << ch << ' ';
        }
        cout << "\n\n";
    }

    bool makeGuess(char guess) {
        bool correct = false;
        for (size_t i = 0; i < secretWord.size(); ++i) {
            if (secretWord[i] == guess) {
                guessedWord[i] = guess;
                correct = true;
            }
        }
        if (!correct) {
            wrongGuesses.push_back(guess);
            --remainingGuesses;
        }
        return correct;
    }

    bool isGameWon() const {
        return secretWord == guessedWord;
    }

    bool isGameOver() const {
        return remainingGuesses <= 0;
    }

    int getRemainingGuesses() const {
        return remainingGuesses;
    }
};

class HangmanFigure {
public:
    static void displayFigure(int wrongGuesses) {
        switch (wrongGuesses) {
            case 0:
                cout << "  +---+\n      |\n      |\n      |\n     ===\n";
                break;
            case 1:
                cout << "  +---+\n  O   |\n      |\n      |\n     ===\n";
                break;
            case 2:
                cout << "  +---+\n  O   |\n  |   |\n      |\n     ===\n";
                break;
            case 3:
                cout << "  +---+\n  O   |\n /|   |\n      |\n     ===\n";
                break;
            case 4:
                cout << "  +---+\n  O   |\n /|\\  |\n      |\n     ===\n";
                break;
            case 5:
                cout << "  +---+\n  O   |\n /|\\  |\n /    |\n     ===\n";
                break;
            case 6:
                cout << "  +---+\n  O   |\n /|\\  |\n / \\  |\n     ===\n";
                break;
            default:
                cout << "Invalid number of wrong guesses.\n";
                break;
        }
    }
};

string getRandomWord(const vector<string>& words) {
    srand(static_cast<unsigned int>(time(0)));
    int randomIndex = rand() % words.size();
    return words[randomIndex];
}

int main() {
    map<string, User> users = readUsers();
    User currentUser;

    int choice;
    do {
        cout << "1. Register\n2. Login\nEnter your choice: ";
        cin >> choice;
        if (choice == 1) {
            registerUser(users);
        } else if (choice == 2) {
            if (loginUser(users, currentUser)) {
                break;
            }
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 2);

    vector<string> words = {"hangman", "programming", "computer", "science", "keyboard"};
    string secretWord = getRandomWord(words);
    int maxGuesses = 6;
    Hangman game(secretWord, maxGuesses);

    while (!game.isGameOver() && !game.isGameWon()) {
        game.displayStatus();
        HangmanFigure::displayFigure(6 - game.getRemainingGuesses());

        char guess;
        cout << "Enter your guess: ";
        cin >> guess;

        if (game.makeGuess(guess)) {
            cout << "Good guess!\n\n";
        } else {
            cout << "Wrong guess!\n\n";
        }
    }

    if (game.isGameWon()) {
        cout << "Congratulations! You've guessed the word: " << secretWord << "\n";
        updateHighScore(users, currentUser, game.getRemainingGuesses());
    } else {
        cout << "Game over! The correct word was: " << secretWord << "\n";
    }

    return 0;
}
