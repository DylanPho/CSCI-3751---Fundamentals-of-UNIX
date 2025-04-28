#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

volatile sig_atomic_t waiting_for_input = 0;
volatile sig_atomic_t alarm_count = 0;
volatile sig_atomic_t sigint_count = 0;

enum TargetDate { TARGET_CHRISTMAS, TARGET_JULY4 };

void daysLeft(enum TargetDate target) {
    static int sequence = 0;
    time_t now = time(NULL);
    struct tm *current_tm = localtime(&now);
    struct tm target_tm = *current_tm; // start with current date/time

    char current_str[100];
    char target_date_str[20];

    // Set target month/day
    if (target == TARGET_CHRISTMAS) {
        target_tm.tm_mon = 11;  // December
        target_tm.tm_mday = 25;
    } else if (target == TARGET_JULY4) {
        target_tm.tm_mon = 6;   // July
        target_tm.tm_mday = 4;
    }

    // Target time at 00:00:00 midnight
    target_tm.tm_hour = 0;
    target_tm.tm_min = 0;
    target_tm.tm_sec = 0;

    // Adjust year if target already passed
    time_t target_time = mktime(&target_tm);
    if (difftime(target_time, now) < 0) {
        target_tm.tm_year += 1;
        target_time = mktime(&target_tm);
    }

    // Correct: use 'now' directly, don't remangle current_tm
    double diff = difftime(target_time, now);
    if (diff < 0) diff = 0;
    long total_seconds = (long) diff;

    long days = total_seconds / 86400;
    long rem = total_seconds % 86400;
    long hours = rem / 3600;
    rem %= 3600;
    long minutes = rem / 60;
    long seconds = rem % 60;

    // Print output
    if (target == TARGET_CHRISTMAS) {
        printf("%d By SIGALRM:\n", sequence++);
    } else {
        printf("%d By SIGINT:\n", sequence++);
    }
    strftime(current_str, sizeof(current_str), "%Y-%m-%d %A %I:%M:%S %p, %Z", current_tm);
    strftime(target_date_str, sizeof(target_date_str), "%Y-%m-%d", &target_tm);

    printf("\tCurrent date and time: %s\n", current_str);
    printf("\tUntil %s : %ld Days, %ld Hours, %ld Minutes, %ld Seconds\n",
           target_date_str, days, hours, minutes, seconds);
    printf("\tTotal number of seconds: %ld seconds\n", total_seconds);
    fflush(stdout);
}

void handle_sigalrm(int signum) {
    if (waiting_for_input) return; // Ignore alarms while waiting for quit input

    daysLeft(TARGET_CHRISTMAS);
    alarm_count++;

    if (alarm_count % 3 == 0) {
        alarm(8);
    } else {
        alarm(3);
    }
}

void handle_sigint(int signum) {
    sigint_count++;

    if (sigint_count > 3) {
        // After 3 interrupts, restore default behavior and re-raise
        signal(SIGINT, SIG_DFL);
        raise(SIGINT);
        return;
    }

    // Pause alarm while prompting
    alarm(0);
    waiting_for_input = 1;
    daysLeft(TARGET_JULY4);

    printf("Quit (y/n)? ");
    fflush(stdout);
}

int main() {
    struct sigaction sa_int, sa_alrm;
    memset(&sa_int, 0, sizeof(sa_int));
    memset(&sa_alrm, 0, sizeof(sa_alrm));

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sigaddset(&sa_int.sa_mask, SIGALRM);
    sa_int.sa_flags = SA_RESTART;

    sa_alrm.sa_handler = handle_sigalrm;
    sigemptyset(&sa_alrm.sa_mask);
    sigaddset(&sa_alrm.sa_mask, SIGINT);
    sa_alrm.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa_int, NULL);
    sigaction(SIGALRM, &sa_alrm, NULL);

    alarm(3); // Initial alarm after 3 seconds

    sigset_t blockmask, oldmask;
    sigemptyset(&blockmask);
    sigaddset(&blockmask, SIGINT);
    sigaddset(&blockmask, SIGALRM);

    while (1) {
        if (!waiting_for_input) {
            pause();
        }

        if (waiting_for_input) {
            // Block signals while reading
            sigprocmask(SIG_BLOCK, &blockmask, &oldmask);

            char input[16];
            if (fgets(input, sizeof(input), stdin) == NULL) {
                sigprocmask(SIG_SETMASK, &oldmask, NULL);
                continue;
            }
            char response = input[0];
            if (response == 'y' || response == 'Y') {
                signal(SIGINT, SIG_DFL);
                signal(SIGALRM, SIG_DFL);
                exit(0);
            } else if (response == 'n' || response == 'N') {
                waiting_for_input = 0;
                // Resume alarm based on current count
                if (alarm_count % 3 == 0) {
                    alarm(8);
                } else {
                    alarm(3);
                }
            } else {
                printf("Invalid input. Please type y or n.\n");
                printf("Quit (y/n)? ");
                fflush(stdout);
            }

            sigprocmask(SIG_SETMASK, &oldmask, NULL);
        }
    }
}
