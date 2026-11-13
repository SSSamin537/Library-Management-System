
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX 100

// Global arrays
int studentIDs[MAX], bookIDs[MAX];
int hours[MAX], minutes[MAX], ampm[MAX];
int count = 0;

// Function declarations
int is_validTime(int h, int m, int a);
int findBook(int *bids, int id);
int readInt(const char *message);

void pauseScreen();
void Welcome_message();
void menu();
void header(const char *title);

void Register_Borrow();
void Return_Book();
void Display_All_Books();
void Search_Book_ByID();
void Update_Borrow_Time();
void Exit_Program();

// Read integer input safely
int readInt(const char *message) {
    char line[100];
    char *end;
    long value;

    while (1) {
        printf("%s", message);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            return 0;
        }

        errno = 0;
        value = strtol(line, &end, 10);

        while (*end == ' ' || *end == '\t' ||
               *end == '\n' || *end == '\r') {
            end++;
        }

        if (end == line || *end != '\0' ||
            errno == ERANGE || value < -2147483647L ||
            value > 2147483647L) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        return (int)value;
    }
}

// Pause screen
void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

// Welcome screen
void Welcome_message() {
    printf("\n\n");
    printf("\t====================================================\n");
    printf("\t|                                                  |\n");
    printf("\t|       LIBRARY MANAGEMENT SYSTEM (LMS)            |\n");
    printf("\t|                                                  |\n");
    printf("\t====================================================\n");
    printf("\t| Welcome to the Library Management System!        |\n");
    printf("\t| Developed by: Department of CSE                  |\n");
    printf("\t| Semester: Spring 2025                             |\n");
    printf("\t|                                                  |\n");
    printf("\t| 1. Register new book borrowings                  |\n");
    printf("\t| 2. Return borrowed books                         |\n");
    printf("\t| 3. Display all borrowed books                    |\n");
    printf("\t| 4. Search and update borrow times                |\n");
    printf("\t====================================================\n");

    pauseScreen();
}

// Header
void header(const char *title) {
    printf("\n\n========== %s ==========\n\n", title);
}

// Find book using pointer arithmetic
int findBook(int *bids, int id) {
    for (int i = 0; i < count; i++) {
        if (*(bids + i) == id) {
            return i;
        }
    }

    return -1;
}

// Validate time
int is_validTime(int h, int m, int a) {
    return h >= 1 && h <= 12 &&
           m >= 0 && m <= 59 &&
           (a == 0 || a == 1);
}

// Register new borrowing
void Register_Borrow() {
    header("Register New Borrow");

    if (count >= MAX) {
        printf("Library record limit reached!\n");
        pauseScreen();
        return;
    }

    int studentID, bookID;
    int h, m, a;

    studentID = readInt("Student ID: ");
    bookID = readInt("Book ID: ");

    if (findBook(bookIDs, bookID) != -1) {
        printf("\nBook ID already exists!\n");
        pauseScreen();
        return;
    }

    h = readInt("Time (hour 1-12): ");
    m = readInt("Minute (0-59): ");
    a = readInt("AM/PM (0-AM, 1-PM): ");

    if (!is_validTime(h, m, a)) {
        printf("\nInvalid time!\n");
        pauseScreen();
        return;
    }

    *(studentIDs + count) = studentID;
    *(bookIDs + count) = bookID;
    *(hours + count) = h;
    *(minutes + count) = m;
    *(ampm + count) = a;

    count++;

    printf("\nBorrow registered successfully!\n");
    pauseScreen();
}

// Return book
void Return_Book() {
    header("Return Book");

    int id, idx;

    if (count == 0) {
        printf("No books borrowed.\n");
        pauseScreen();
        return;
    }

    id = readInt("Enter Book ID: ");

    idx = findBook(bookIDs, id);

    if (idx == -1) {
        printf("\nBook not found!\n");
        pauseScreen();
        return;
    }

    // Shift remaining records
    for (int i = idx; i < count - 1; i++) {
        *(studentIDs + i) = *(studentIDs + i + 1);
        *(bookIDs + i) = *(bookIDs + i + 1);
        *(hours + i) = *(hours + i + 1);
        *(minutes + i) = *(minutes + i + 1);
        *(ampm + i) = *(ampm + i + 1);
    }

    count--;

    printf("\nBook returned successfully!\n");
    pauseScreen();
}

// Display all borrowed books
void Display_All_Books() {
    header("All Borrowed Books");

    if (count == 0) {
        printf("No records available.\n");
        pauseScreen();
        return;
    }

    printf("Slot | StudentID | BookID | Time\n");
    printf("-----------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%4d | %9d | %6d | %02d:%02d %s\n",
               i + 1,
               *(studentIDs + i),
               *(bookIDs + i),
               *(hours + i),
               *(minutes + i),
               *(ampm + i) ? "PM" : "AM");
    }

    pauseScreen();
}

// Search book by ID
void Search_Book_ByID() {
    header("Search by Book ID");

    if (count == 0) {
        printf("No books available.\n");
        pauseScreen();
        return;
    }

    int id, idx;

    id = readInt("Enter Book ID: ");

    idx = findBook(bookIDs, id);

    if (idx == -1) {
        printf("\nBook not found!\n");
        pauseScreen();
        return;
    }

    printf("\nStudent ID: %d\n", *(studentIDs + idx));
    printf("Book ID: %d\n", *(bookIDs + idx));

    printf("Borrow Time: %02d:%02d %s\n",
           *(hours + idx),
           *(minutes + idx),
           *(ampm + idx) ? "PM" : "AM");

    pauseScreen();
}

// Update borrowing time
void Update_Borrow_Time() {
    header("Update Borrow Time");

    if (count == 0) {
        printf("No records available.\n");
        pauseScreen();
        return;
    }

    int id, idx, h, m, a;

    id = readInt("Enter Book ID: ");

    idx = findBook(bookIDs, id);

    if (idx == -1) {
        printf("\nBook not found!\n");
        pauseScreen();
        return;
    }

    h = readInt("New Hour (1-12): ");
    m = readInt("New Minute (0-59): ");
    a = readInt("AM/PM (0-AM, 1-PM): ");

    if (!is_validTime(h, m, a)) {
        printf("\nInvalid time!\n");
        pauseScreen();
        return;
    }

    *(hours + idx) = h;
    *(minutes + idx) = m;
    *(ampm + idx) = a;

    printf("\nBorrow time updated successfully!\n");
    pauseScreen();
}

// Exit program
void Exit_Program() {
    printf("\n");
    printf("============================================================\n");
    printf("     Thank You for Using Library Management System\n");
    printf("============================================================\n");

    exit(0);
}

// Main menu
void menu() {
    int ch;

    do {
        header("LIBRARY MANAGEMENT SYSTEM");

        printf("1. Register New Borrow\n");
        printf("2. Return Book\n");
        printf("3. Display All Borrowed Books\n");
        printf("4. Search by Book ID\n");
        printf("5. Update Borrow Time\n");
        printf("6. Exit\n");

        printf("\n");

        ch = readInt("Enter your choice: ");

        switch (ch) {
            case 1:
                Register_Borrow();
                break;

            case 2:
                Return_Book();
                break;

            case 3:
                Display_All_Books();
                break;

            case 4:
                Search_Book_ByID();
                break;

            case 5:
                Update_Borrow_Time();
                break;

            case 6:
                Exit_Program();
                break;

            default:
                printf("\nInvalid choice!\n");
                pauseScreen();
        }

    } while (ch != 6);
}

// Main function
int main() {
    Welcome_message();
    menu();

    return 0;
}
