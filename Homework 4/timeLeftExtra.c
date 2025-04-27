#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    time_t now, target_time;
    struct tm future_tm = {0};
    char current_time_str[100];
    int month, day, year;

    // Get current time
    time(&now);
    struct tm *current_tm = localtime(&now);

    // Format and display current time
    strftime(current_time_str, sizeof(current_time_str), "%Y-%m-%d %A %I:%M:%S %p, %Z", current_tm);
    printf("Current date and time: %s\n", current_time_str);

    // Get future date input from user
    printf("Please enter a target date in the future(mm/dd/yyyy): ");
    scanf("%d/%d/%d", &month, &day, &year);

    // Setup future date struct
    future_tm.tm_year = year - 1900;
    future_tm.tm_mon = month - 1;
    future_tm.tm_mday = day;
    future_tm.tm_hour = current_tm->tm_hour;
    future_tm.tm_min = current_tm->tm_min;
    future_tm.tm_sec = current_tm->tm_sec;
    future_tm.tm_isdst = current_tm->tm_isdst;

    // Convert to time_t
    target_time = mktime(&future_tm);

    // Validate future date
    if (target_time == -1 || difftime(target_time, now) < 0) {
        printf("Invalid or past date entered. Please run the program again.\n");
        return 1;
    }

    // Calculate difference
    double diff_seconds = difftime(target_time, now);
    int total_seconds = (int)diff_seconds;
    int days = total_seconds / 86400;
    int hours = (total_seconds % 86400) / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    // Output result
    printf("Until the target date: %d Days, %d Hours, %d Minutes, %d Seconds\n",
           days, hours, minutes, seconds);
    printf("Total number of seconds of the target date: %d seconds\n", total_seconds);

    // Extra Credit: Verification
    int verify_seconds = days * 86400 + hours * 3600 + minutes * 60 + seconds;
    if (verify_seconds == total_seconds) {
        printf("Verification Passed :) : Breakdown matches total seconds.\n");
    } else {
        printf("Verification Failed :( : Breakdown does not match total seconds.\n");
    }

    return 0;
}
