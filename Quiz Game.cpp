#define _CRT_SECURE_NO_WARNINGS     //Ignores the unsafe warmings when using ctime function
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include <cstdlib> 
#include <ctime>
using namespace std;

int displaymenu();                              //Displays the mainmenu of the Quiz game
int subjects();                                 //Displays the subject menu of the Quiz game
int getdifficulty();                            //Displays the difficulty menu of the quiz game

int getQuestionCount(const string& filename);  // Counts the total questions present in the text files
int StartQuiz(int subject, int difficulty);    // Actual function to start the quiz

void processLifelineMenu(bool& u_50, bool& u_skip, bool& u_swap, bool& u_time, string correct_ans,
    double& t_limit, bool& is_skipped, bool& trigger_swap);              //Lifeline menu shows all options  

string getValidName();                                                //gets the name of the player after the quiz
void saveLog(string name, int score, int subject, int difficulty);   //saves the session on a txt file everytime a quiz is attempted 
void saveScore(string name, int score);                              //Saves the name and score in the leaderboard.txt file
void showLeaderboard();
int main() {
    /*
    25F-0644 Ali Hassan
    25F-0527 Abdul Naffey
    25F-0623 Kashifa Mansoor
    */
    srand((unsigned int)time(0));      // Seeds the random number generator with current time to ensure unique results each run.
    int score;
    int choice = displaymenu();
    if (choice == 1) {
        cout << "\n--- Starting Quiz Setup ---\n";
        int subject_choice = subjects();
        cout << "\nYou selected subject option: " << subject_choice << "\n";
        int difficulty_choice = getdifficulty();
        cout << "\nQuiz Ready!\n";
        cout << "Subject ID: " << subject_choice << "\n";
        cout << "Difficulty ID: " << difficulty_choice << "\n";
        score = StartQuiz(subject_choice, difficulty_choice);
        string playerName = getValidName();
        saveLog(playerName, score, subject_choice, difficulty_choice);
        saveScore(playerName, score);
    }
    else if (choice == 2) {
        cout << "\n--- Displaying Leaderboard ---\n";
        showLeaderboard();
    }
    else if (choice == 3) {
        cout << "\n--- Exiting THE BRAIN MATRIX ---\n";
    }
    system("pause>0");
    return 0;
}

int displaymenu() {
    cout << "**************************************\n";
    cout << "**         THE BRAIN MATRIX         **\n";
    cout << "**************************************\n";
    cout << "* (1) Start Quiz                     *\n";
    cout << "* (2) Check Leaderboard              *\n";
    cout << "* (3) Exit                           *\n";
    cout << "**************************************\n";
    cout << "Enter you option (1, 2 or 3)\n";
    int choice;
    do {
        cin >> choice;
        if (cin.fail()) {                                 //It checks if input is of same datatype as entered or not
            cin.clear();                                 //Clears the input typed
            cin.ignore(100, '\n');                      // Ignores if charaters typed upto 100 characters
            cout << "Invalid input. Please enter a number (1, 2, or 3).\n\n";
            continue;
        }
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n\n";
        }
    } while (choice < 1 || choice > 3);
    return choice;
}

int subjects() {
    int choice;
    do {
        cout << "\n*****************************\n";
        cout << "* SELECT A SUBJECT          *\n";
        cout << "*****************************\n";
        cout << "* 1. Science                *\n";
        cout << "* 2. General Knowledge      *\n";
        cout << "* 3. World History          *\n";
        cout << "*****************************\n";
        cout << "Enter Choice (1-3): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input. Please enter a number (1, 2, or 3).\n";
            continue;
        }
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    } while (choice < 1 || choice > 3);
    return choice;
}

int getdifficulty() {
    int choice;
    do {
        cout << "\n*****************************\n";
        cout << "* SELECT DIFFICULTY         *\n";
        cout << "*****************************\n";
        cout << "* 1. Easy                   *\n";
        cout << "* 2. Medium                 *\n";
        cout << "* 3. Hard                   *\n";
        cout << "*****************************\n";
        cout << "Enter Choice (1-3): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input. Please enter a number (1, 2, or 3).\n";
            continue;
        }
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    } while (choice < 1 || choice > 3);
    return choice;
}

string getValidName() {
    string inputName;
    cin.ignore();
    while (true) {
        cout << "Enter your player name (min 3 letters): ";
        getline(cin, inputName);

        if (inputName.length() >= 3) {
            return inputName;
        }
        else {
            cout << " Error: Name is too short. Try again.\n" << endl;
        }
    }
}

int getQuestionCount(const string& filename) {
    ifstream file(filename);
    int lines = 0;
    string temp;
    while (getline(file, temp)) {
        lines++;
    }
    return lines / 6;
} //divides the questions in 6 lines


int StartQuiz(int subject, int difficulty) {
    ifstream input;
    string question, option_A, option_B, option_C, option_D, correct_answer;
    string filename;

    string wrongAnswers[10][6];  //2D array to store wrong answers
    int wrongCount = 0;

    if (subject == 1) {
        if (difficulty == 1) filename = "Science_easy.txt";
        if (difficulty == 2) filename = "Science_medium.txt";
        if (difficulty == 3) filename = "Science_hard.txt";
    }
    else if (subject == 2) {
        if (difficulty == 1) filename = "General_easy.txt";
        if (difficulty == 2) filename = "General_medium.txt";
        if (difficulty == 3) filename = "General_hard.txt";
    }
    else if (subject == 3) {
        if (difficulty == 1) filename = "History_easy.txt";
        if (difficulty == 2) filename = "History_medium.txt";
        if (difficulty == 3) filename = "History_hard.txt";
    }

    if (filename.empty()) {
        cout << "\nError: Quiz files for this selection are not configured yet." << endl;
        return 0;
    }

    int total_questions = getQuestionCount(filename);
    if (total_questions == 0) {
        cout << "\nError:File not found " << endl;
        return 0;
    }



    int score = 0;
    int streak = 0;
    bool used_5050 = false;
    bool used_skip = false;
    bool used_swap = false;
    bool used_time = false;

    for (int q = 0; q < 10; q++) {

    ResetQuestion:
        int randomquestion = (rand() % total_questions) + 1;

        bool is_skipped = false;
        input.open(filename);
        if (!input.is_open()) {
            cout << "Error: File not found." << endl;
            return 0;
        }
        string trash;
        for (int i = 0; i < (randomquestion - 1) * 6; ++i) {
            getline(input, trash);
        }

        if (getline(input, question) &&
            getline(input, option_A) &&
            getline(input, option_B) &&
            getline(input, option_C) &&
            getline(input, option_D) &&
            getline(input, correct_answer))
        {
            cout << "\nQuestion " << (q + 1) << ": " << question << endl;
            cout << option_A << endl;
            cout << option_B << endl;
            cout << option_C << endl;
            cout << option_D << endl;
            cout << "------------------------------------------------\n";
            cout << " (You have 10 seconds! Press 'L' for Lifelines)" << endl;

            string user_answer;
            char clean_answer;
            bool valid_input = false;
            double time_limit = 10.0;

            time_t timestart = time(0);  //sets the current time in timestart
            bool trigger_swap = false;
            do {
                cout << "Enter your answer (A, B, C, D) or 'L': ";
                cin >> user_answer;

                if (user_answer.length() == 1) {
                    clean_answer = toupper(user_answer[0]); //converts answer to uppercase

                    if (clean_answer == 'L') {
                        processLifelineMenu(used_5050, used_skip, used_swap, used_time,
                            correct_answer, time_limit, is_skipped, trigger_swap);

                        if (trigger_swap == true) {
                            input.close();
                            goto ResetQuestion;
                        }
                        if (is_skipped == true) {
                            valid_input = true;
                        }
                        continue;
                    }
                    if (clean_answer >= 'A' && clean_answer <= 'D') {
                        valid_input = true;
                    }
                }
            } while (!valid_input);

            if (is_skipped == true) {
                cout << "Question Skipped. No points changed.\n";
                continue;
            }

            time_t timefinish = time(0);
            double time_taken = difftime(timefinish, timestart);
            cout << " -> Time taken: " << time_taken << "s (Limit: " << time_limit << "s)" << endl;

            if (time_taken > time_limit) {
                cout << "\n [!] TIME LIMIT EXCEEDED! [!]" << endl;
                cout << " Correct answer was: " << correct_answer << endl;
                streak = 0;

                if (difficulty == 1) { score -= 2; cout << " -2 points\n"; }
                else if (difficulty == 2) { score -= 3; cout << " -3 points\n"; }
                else { score -= 5; cout << " -5 points\n"; }

                if (wrongCount < 10) {
                    wrongAnswers[wrongCount][0] = question;
                    wrongAnswers[wrongCount][1] = option_A;
                    wrongAnswers[wrongCount][2] = option_B;
                    wrongAnswers[wrongCount][3] = option_C;
                    wrongAnswers[wrongCount][4] = option_D;
                    wrongAnswers[wrongCount][5] = correct_answer;
                    wrongCount++;
                }
            }
            else {

                string answer(1, clean_answer);
                if (answer == correct_answer) {
                    cout << " Correct! +1 point\n";
                    score++;
                    streak++;

                    if (streak == 3) { score += 5; cout << " Streak Bonus! +5 points\n"; }
                    else if (streak == 5) { score += 15; cout << " Mega Streak Bonus! +15 points\n"; }
                }
                else {
                    cout << " Incorrect. Correct answer is " << correct_answer << "\n";
                    streak = 0;

                    if (difficulty == 1) { score -= 2; cout << " -2 points\n"; }
                    else if (difficulty == 2) { score -= 3; cout << " -3 points\n"; }
                    else { score -= 5; cout << " -5 points\n"; }


                    if (wrongCount < 10) {
                        wrongAnswers[wrongCount][0] = question;
                        wrongAnswers[wrongCount][1] = option_A;
                        wrongAnswers[wrongCount][2] = option_B;
                        wrongAnswers[wrongCount][3] = option_C;
                        wrongAnswers[wrongCount][4] = option_D;
                        wrongAnswers[wrongCount][5] = correct_answer;
                        wrongCount++;
                    }
                }
            }    input.close();
        }
        else {
            input.close();
        }
    }
    cout << "\nQuiz finished! Your final score is: " << score << endl;

    if (wrongCount > 0) {
        char reviewChoice;
        cout << "\nYou got " << wrongCount << " questions wrong.";
        cout << "\nWould you like to review them? (y/n): ";
        cin >> reviewChoice;

        if (reviewChoice == 'y' || reviewChoice == 'Y') {
            cout << "\n--- REVIEW MODE ---\n";
            for (int i = 0; i < wrongCount; i++) {
                cout << "\nQ: " << wrongAnswers[i][0] << endl;
                cout << "A) " << wrongAnswers[i][1] << endl;
                cout << "B) " << wrongAnswers[i][2] << endl;
                cout << "C) " << wrongAnswers[i][3] << endl;
                cout << "D) " << wrongAnswers[i][4] << endl;
                cout << ">> Correct Answer: " << wrongAnswers[i][5] << endl;
                cout << "-----------------------------------";
            }
        }
    }
    return score;
}

void processLifelineMenu(bool& u_50, bool& u_skip, bool& u_swap, bool& u_time,
    string correct_ans, double& t_limit, bool& is_skipped, bool& trigger_swap) {

    cout << "\n--- LIFELINE MENU ---\n";

    cout << "1. 50/50 ";
    if (u_50) { cout << "(USED)"; }
    else { cout << "(Available)"; }
    cout << endl;
    cout << "2. Skip ";
    if (u_skip) { cout << "(USED)"; }
    else { cout << "(Available)"; }
    cout << endl;
    cout << "3. Swap ";
    if (u_swap) { cout << "(USED)"; }
    else { cout << "(Available)"; }
    cout << endl;
    cout << "4. Time Ext ";
    if (u_time) { cout << "(USED)"; }
    else { cout << "(Available)"; }
    cout << endl;
    cout << "5. Cancel\nChoice: ";
    int l_choice;
    cin >> l_choice;

    if (l_choice == 1) {
        if (u_50 == false) {
            u_50 = true;
            string fake_opt;
            if (correct_ans == "A") { fake_opt = "C"; }
            else { fake_opt = "A"; }
            cout << "\n[50/50 APPLIED]: The answer is either " << correct_ans << " or " << fake_opt << "!\n";
        }
        else {
            cout << "\nError: You already used this lifeline!\n";
        }
    }
    else if (l_choice == 2) {
        if (u_skip == false) {
            u_skip = true;
            is_skipped = true;
            cout << "\n[SKIP APPLIED]: Moving to next question...\n";
        }
        else {
            cout << "\nError: You already used this lifeline!\n";
        }
    }
    else if (l_choice == 3) {
        if (u_swap == false) {
            u_swap = true;
            trigger_swap = true;
            cout << "\n[SWAP APPLIED]: Getting a new question...\n";
        }
        else {
            cout << "\nError: You already used this lifeline!\n";
        }
    }
    else if (l_choice == 4) {
        if (u_time == false) {
            u_time = true;
            t_limit = t_limit + 10.0;
            cout << "\n[TIME EXTENDED]: You now have " << t_limit << " seconds!\n";
        }
        else {
            cout << "\nError: You already used this lifeline!\n";
        }
    }
    else if (l_choice == 5) {
        cout << "Cancelled.\n";
    }
}


void saveLog(string name, int score, int subject, int difficulty) {
    ofstream logFile("quiz_logs.txt", ios::app);

    if (logFile.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        string genre;
        string level;
        if (subject == 1) genre = "Science";
        else if (subject == 2) genre = "General Knowledge";
        else genre = "History";
        if (difficulty == 1) level = "Easy";
        else if (difficulty == 2) level = "Medium";
        else level = "Hard";

        logFile << "User: " << name << " | Score: " << score << "|Subject: " << genre << "|Difficulty: " << level << "| Time: " << dt;
        logFile.close();
        cout << "Session saved to quiz_logs.txt successfully.\n";
    }
    else {
        cout << "Error opening log file.\n";
    }
}
void saveScore(string name, int score) {
    ofstream fout("Leaderboard.txt", ios::app);
    if (!fout) {
        cout << "Error opening file to save score!" << endl;
        return;
    }

    fout << name << " " << score << endl;
    fout.close();
    cout << "Score saved to Leaderboard!" << endl;
}
void showLeaderboard() {
    ifstream fin("Leaderboard.txt");
    if (!fin) {
        cout << "\nNo leaderboard data found yet. Play a game first!\n";
        return;
    }

    string names[100];
    int scores[100];
    int count = 0;

    while (fin >> names[count] >> scores[count]) {
        count++;
        if (count >= 100) break;
    }
    fin.close();

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {

            if (scores[j] > scores[i]) {
                int tempscore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempscore;
                string tempname = names[i];
                names[i] = names[j];
                names[j] = tempname;
            }
        }
    }
    cout << "\n*********************************\n";
    cout << "          LEADERBOARD             \n";
    cout << "***********************************\n";
    cout << left << setw(15) << "NAME" << "SCORE" << endl;
    cout << "***********************************\n";

    int limit;
    if (count < 5)
        limit = count;
    else limit = 5;
    for (int k = 0; k < limit; k++) {
        cout << left << setw(15) << names[k] << scores[k] << endl;
    }
    cout << "***********************************\n";
}