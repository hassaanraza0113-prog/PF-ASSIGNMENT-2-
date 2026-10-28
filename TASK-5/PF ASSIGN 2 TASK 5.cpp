#include <stdio.h>
#include <string.h>

#define BOOKS 4
#define MAX_BORROWED 3

void displayMenu(void)
{
    printf("\n===== LIBRARY MENU =====\n");
    printf("1. Display available books\n");
    printf("2. Borrow a book\n");
    printf("3. Return a book\n");
    printf("4. Display member record\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
}

void registerMember(char name[], int *studentID, int *memberType)
{
    int type;

    printf("Enter member name: ");
    scanf(" %49[^\n]", name);
    printf("Enter student ID: ");
    scanf("%d", studentID);

    do {
        printf("Membership type (1 = Student, 2 = Faculty): ");
        scanf("%d", &type);
        if (type != 1 && type != 2)
            printf("Please enter 1 or 2.\n");
    } while (type != 1 && type != 2);

    *memberType = type;
}

void displayBooks(int bookIDs[], char bookTitles[][50],
                  int availableCopies[], int totalBooks)
{
    int i;

    printf("\nID\\tTitle\\t\\t\\tAvailable copies\n");
    for (i = 0; i < totalBooks; i++)
        printf("%d\\t%-22s\\t%d\n",
               bookIDs[i], bookTitles[i], availableCopies[i]);
}

void borrowBook(int bookIDs[], char bookTitles[][50],
                int availableCopies[], int totalBooks,
                int borrowedBookIDs[], int *borrowedCount, int memberType)
{
    int id, days, i, bookIndex = -1, j;

    printf("Enter book ID: ");
    scanf("%d", &id);
    printf("How many days will you keep it? ");
    scanf("%d", &days);

    if (days <= 0) {
        printf("Days must be greater than zero.\n");
        return;
    }

    for (i = 0; i < totalBooks; i++) {
        if (bookIDs[i] == id) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Invalid book ID.\n");
        return;
    }
    if (availableCopies[bookIndex] <= 0) {
        printf("No copies of this book are available.\n");
        return;
    }
    if (*borrowedCount >= MAX_BORROWED) {
        printf("You cannot borrow more than three books at a time.\n");
        return;
    }

    for (j = 0; j < *borrowedCount; j++) {
        if (borrowedBookIDs[j] == id) {
            printf("You have already borrowed this book.\n");
            return;
        }
    }

    borrowedBookIDs[*borrowedCount] = id;
    (*borrowedCount)++;
    availableCopies[bookIndex]--;

    printf("You borrowed: %s for %d days.\n", bookTitles[bookIndex], days);
    if (memberType == 1)
        printf("Student borrowing limit: 14 days.\n");
    else
        printf("Faculty borrowing limit: 30 days.\n");
}

float returnBook(int bookIDs[], int availableCopies[], int totalBooks,
                 int borrowedBookIDs[], int *borrowedCount,
                 int memberType, float *totalFine)
{
    int id, actualDays, i, bookIndex = -1, borrowedIndex = -1;
    int allowedDays, extraDays;
    float fine = 0;
    char code[30];

    printf("Enter book ID to return: ");
    scanf("%d", &id);
    printf("Enter actual number of days kept: ");
    scanf("%d", &actualDays);

    if (actualDays <= 0) {
        printf("Days must be greater than zero.\n");
        return 0;
    }

    for (i = 0; i < totalBooks; i++)
        if (bookIDs[i] == id) bookIndex = i;

    for (i = 0; i < *borrowedCount; i++)
        if (borrowedBookIDs[i] == id) borrowedIndex = i;

    if (bookIndex == -1 || borrowedIndex == -1) {
        printf("You have not borrowed this book, or the ID is invalid.\n");
        return 0;
    }

    allowedDays = (memberType == 1) ? 14 : 30;
    if (actualDays > allowedDays) {
        extraDays = actualDays - allowedDays;
        fine = (memberType == 1) ? extraDays * 20.0f : extraDays * 10.0f;

        printf("Fine before waiver: Rs. %.2f\n", fine);
        printf("Enter membership code for 50%% waiver (or NONE): ");
        scanf("%29s", code);

        if (strcmp(code, "LIB2026") == 0) {
            fine = fine * 0.5f;
            printf("Correct code. 50%% waiver applied.\n");
        }
    }

    *totalFine += fine;
    availableCopies[bookIndex]++;

    for (i = borrowedIndex; i < *borrowedCount - 1; i++)
        borrowedBookIDs[i] = borrowedBookIDs[i + 1];
    (*borrowedCount)--;

    printf("Book returned successfully. Fine payable: Rs. %.2f\n", fine);
    return fine;
}

void displayMemberRecord(char name[], int studentID, int memberType,
                         int borrowedBookIDs[], int borrowedCount,
                         float totalFine, float fineAfterWaiver)
{
    int i;

    printf("\n===== MEMBER RECORD =====\n");
    printf("Name: %s\n", name);
    printf("Student ID: %d\n", studentID);
    printf("Membership: %s\n", memberType == 1 ? "Student" : "Faculty");

    printf("Currently borrowed book IDs: ");
    if (borrowedCount == 0)
        printf("None");
    for (i = 0; i < borrowedCount; i++)
        printf("%d ", borrowedBookIDs[i]);
    printf("\n");

    printf("Total fine before waivers: Rs. %.2f\n", totalFine);
    printf("Final fine payable after waivers: Rs. %.2f\n", fineAfterWaiver);
}

int main(void)
{
    int bookIDs[BOOKS] = {101, 102, 103, 104};
    char bookTitles[BOOKS][50] = {
        "Programming in C", "Data Structures",
        "Database Systems", "Computer Networks"
    };
    int availableCopies[BOOKS] = {5, 3, 0, 4};
    int borrowedBookIDs[MAX_BORROWED];
    int borrowedCount = 0, studentID = 0, memberType = 1, choice;
    char name[50];
    float totalFine = 0, fineAfterWaiver = 0, paidFine;

    registerMember(name, &studentID, &memberType);

    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBooks(bookIDs, bookTitles, availableCopies, BOOKS);
                break;
            case 2:
                borrowBook(bookIDs, bookTitles, availableCopies, BOOKS,
                           borrowedBookIDs, &borrowedCount, memberType);
                break;
            case 3:
                paidFine = returnBook(bookIDs, availableCopies, BOOKS,
                                      borrowedBookIDs, &borrowedCount,
                                      memberType, &totalFine);
                fineAfterWaiver += paidFine;
                break;
            case 4:
                displayMemberRecord(name, studentID, memberType,
                                    borrowedBookIDs, borrowedCount,
                                    totalFine, fineAfterWaiver);
                break;
            case 5:
                printf("Exiting library system. Goodbye!\n");
                break;
            default:
                printf("Invalid menu choice. Please select 1 to 5.\n");
        }
    } while (choice != 5);

    return 0;
}

