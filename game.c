#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int value;
int round_number = 1;
int game_on = 1;
char* draw1 = "";
char* draw2 = "";
char* outcome1 = "";
int player1_wins = 0;
char* outcome2 = "";
int player2_wins = 0;
int who_won_last_round = -1;

pthread_t thread1, thread2;
int err;

pthread_mutex_t lock;

char* draw(void)
{
    value = rand() % 100;
    char* outcome = "";

    if (value >= 0 && value < 33)
        outcome = "rock";
    if (value >= 33 && value < 66)
        outcome = "paper";
    if (value >= 66 && value < 100)
        outcome = "scissors";

    return outcome;
}

void who_first(char* p)
{
    printf("%s draws first.\n", p);
}

void *player1(void *arg)
{
    while (game_on != 0) {
    pthread_mutex_lock(&lock);

    draw1 = draw();

    pthread_mutex_unlock(&lock);
    }
    return NULL;
} 

void *player2(void *arg)
{

    while (game_on != 0) {
    pthread_mutex_lock(&lock);

    draw2 = draw();

    pthread_mutex_unlock(&lock);
    }
    return NULL;
} 

int main(void)
{
    srand(time(NULL));

    err = pthread_create(&thread1, NULL, &player1, NULL);
        if (err)
            return 1;
    err = pthread_create(&thread2, NULL, &player2, NULL);
        if (err)
            return 1;

    do {
    sleep(2);
    printf("Round number: %d\n", round_number);
    if (who_won_last_round == -1) {
            value = rand() % 100;
            if (value <= 50) {
                who_first("Player 1");
                outcome1 = draw1;
                printf("Player 1: %s.\n", draw1);
                outcome2 = draw2;
                printf("Player 2: %s.\n", draw2);
            } else if (value > 50) {
                who_first("Player 2");
                outcome2 = draw2;
                printf("Player 2: %s.\n", draw2);
                outcome1 = draw1;
                printf("Player 1: %s.\n", draw1);
            }
        }

    if (who_won_last_round == 1) {
        who_first("Player 2");
        outcome2 = draw2;
        printf("Player 2: %s.\n", draw2);
        outcome1 = draw1;
        printf("Player 1: %s.\n", draw1);
    }

    if (who_won_last_round == 2) {
        who_first("Player 1");
        outcome1 = draw1;
        printf("Player 1: %s.\n", draw1);
        outcome2 = draw2;
        printf("Player 2: %s.\n", draw2);
    }

    /* rozstrzygnięcie tury */
    if ((strcmp(outcome1, "rock") == 0) && (strcmp(outcome2, "scissors") == 0)) {
        printf("Player 1 wins.\n\n");
        player1_wins++;
        who_won_last_round = 1;
    } else if ((strcmp(outcome1, "paper") == 0) && (strcmp(outcome2, "rock") == 0)) {
        printf("Player 1 wins.\n\n");
        player1_wins++;
        who_won_last_round = 1;
    } else if ((strcmp(outcome1, "scissors") == 0) && (strcmp(outcome2, "paper") == 0)) {
        printf("Player 1 wins.\n\n");
        player1_wins++;
        who_won_last_round = 1;
    } else if ((strcmp(outcome1, "scissors") == 0) && (strcmp(outcome2, "rock") == 0)) {
        printf("Player 2 wins.\n\n");
        player2_wins++;
        who_won_last_round = 2;
    } else if ((strcmp(outcome1, "rock") == 0) && (strcmp(outcome2, "paper") == 0)) {
        printf("Player 2 wins.\n\n");
        player2_wins++;
        who_won_last_round = 2;
    } else if ((strcmp(outcome1, "paper") == 0) && (strcmp(outcome2, "scissors") == 0)) {
        printf("Player 2 wins.\n\n");
        player2_wins++;
        who_won_last_round = 2;
    } else if (strcmp(outcome1, outcome2) == 0) {
        printf("It's a tie.\n\n");
        who_won_last_round = -1;
    }

    if (player1_wins == 10) {
        printf("\nPlayer 1 won 10 times!\n");
        game_on = 0;
        break;
    }
    if (player2_wins == 10) {
        printf("\nPlayer 2 won 10 times!\n");
        game_on = 0;
        break;
    }

    round_number++; 

} while (1);

    err = pthread_join(thread1, NULL);
    err = pthread_join(thread2, NULL);
    if (err)
        fprintf(stderr, "pthread_join() error");

    pthread_mutex_destroy(&lock);

    return 0;
}