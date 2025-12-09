# 🧠 THE BRAIN MATRIX Quiz Game

This is a console-based C++ quiz game designed to test knowledge across different subjects and difficulty levels. The game utilizes file I/O for question storage and incorporates features like dynamic score tracking, streak bonuses, lifelines, and a persistent leaderboard.

## ✨ Key Features

* **Dynamic Question Selection:** Questions are randomly selected from external text files (`.txt`) based on the user's subject and difficulty choices.
* **File Handling (I/O):** All questions, options, correct answers, logs, and scores are managed using external files (`ifstream` and `ofstream`).
* **Time Tracking:** The duration of the quiz session is logged, and a time limit is imposed on answering each individual question.
* **Lifelines:** Players can use lifelines during the quiz, including **50/50**, **Skip Question**, **Swap Question**, and **Time Extension**.
* **Leaderboard:** Scores are saved to a persistent `Leaderboard.txt` file, which is loaded and sorted to display the top 5 scores.

---

## ⚙️ Core Logic Explained

The program is structured around several functions responsible for specific tasks.

### 1. File Handling & Randomization (`getQuestionCount` & `StartQuiz`)

The quiz operates entirely without storing all questions in memory, relying on direct file access and pointer manipulation.

* **File Format:** Each question in the external text files (e.g., `Science_easy.txt`) must occupy exactly **6 consecutive lines**:
    1.  Question Text
    2.  Option A Text
    3.  Option B Text
    4.  Option C Text
    5.  Option D Text
    6.  Correct Answer (e.g., `A`, `B`, `C`, or `D`)
* **Question Count:** The `getQuestionCount` function determines the total number of questions by dividing the total number of lines in the file by **6**.
* **Randomization:** In `StartQuiz`, a random number between 1 and `total_questions` is generated (`randomquestion`).
* **File Pointer Jump:** The program opens the file and uses a loop with `getline(input, trash)` to skip exactly `(randomquestion - 1) * 6` lines, positioning the file pointer directly at the start of the randomly chosen question.

### 2. Time Management

The game uses standard C-style time functions for tracking.

* **Per-Question Limit:** Start time is recorded with `time_t timestart = time(0);`. The elapsed time is calculated using the `difftime` function:
    ```cpp
    double time_taken = difftime(timefinish, timestart);
    ```
* **Session Logging:** The `saveLog` function uses `ctime(&now)` to convert the recorded `time_t` value (`now`) into a human-readable timestamp string before saving it to `quiz_logs.txt`.

### 3. Scoring and Streaks

The scoring system encourages consecutive correct answers.

| Action | Points (Easy/Medium/Hard) | Effect |
| :--- | :--- | :--- |
| **Correct Answer** | +1 point (base) | Streak counter increases. |
| **Incorrect Answer** | -2 / -3 / -5 points | Streak counter resets to 0. |
| **Time Out** | -2 / -3 / -5 points | Streak counter resets to 0. |
| **3-Question Streak** | **+5 points bonus** | |
| **5-Question Streak** | **+15 points bonus** | |

### 4. Lifeline Logic (`processLifelineMenu`)

The lifelines are managed using boolean flags (e.g., `used_5050`) to ensure each is used only once per quiz.

* **Swap (Choice '3'):** This lifeline sets `trigger_swap = true`. This flag forces the `StartQuiz` loop to execute `goto ResetQuestion;`, which immediately generates a new random question and closes/reopens the file stream.
* **Skip (Choice '2'):** This sets `is_skipped = true`, allowing the main question loop to `continue` to the next iteration without checking the time or answer.
* **Time Extension (Choice '4'):** This simply increases the `time_limit` variable by `10.0` seconds.

---

## ▶️ How to Run

1.  **File Placement:** Ensure **all** the question data files (`Science_easy.txt`, `General_hard.txt`, etc.) are placed in the **same directory** as the compiled executable file.
2.  **Compile:** Compile `main.cpp` using a standard C++ compiler (like g++ or within an IDE like Visual Studio).
3.  **Execute:** Run the compiled program and follow the on-screen menus to start the quiz.
