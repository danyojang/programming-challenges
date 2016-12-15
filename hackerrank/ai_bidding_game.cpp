// AI bidding game for https://www.hackerrank.com/contests/twitch/challenges/the-bidding-game
// Ranked 38/456 as of December 2016

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
bool win_if_tie(int player);
int updateBalance(int* my_moves, int * opponent_moves, int player);
int calculate_bid(int player, int pos,int* first_moves, int* second_moves);
int calculate(int player, int spots_remaining, int* my_moves, int* opponent_moves);
int size_array(int*);

const int board_size = 10;
const int initial_money = 100;
const int first_bid = 13;
const int max_bid = 20;

int my_money = initial_money;
int opponent_money = initial_money;
bool player_one_tie_win = true;

int calculate_bid(int player, int pos,int* first_moves, int* second_moves) {
    if (player == 1) {
        return calculate(player, pos, first_moves, second_moves);
    } else {
        return calculate(player, board_size-pos, second_moves, first_moves);
    }
}


int calculate(int player, int spots_remaining, int* my_moves, int * opponent_moves) {
    int last_move_index = size_array(my_moves);
    // AI to bid 13 on first bid, chose this to be my seed
    if (last_move_index < 1) {
        return first_bid;
    }
    int my_last_move = my_moves[last_move_index-1];
    int opponent_last_move = opponent_moves[last_move_index-1];
    int last_opponent_winner = updateBalance(my_moves, opponent_moves, player);
    int opponent_spots_remaining = board_size - spots_remaining;
    int bid = 0;
    if (opponent_spots_remaining == 0) {
        opponent_spots_remaining = 1;
    }
    // strategically calculate a base_bid
    int base_bid = my_money / opponent_spots_remaining;
    if (my_last_move >= 13) {
        my_last_move = 13;
    }
    // the scotch is in the middle, tied
    // I won or lost my last to tie it
    // never go above his last previous bid winner
    if (spots_remaining == opponent_spots_remaining) {
        // won my last
        if (my_last_move > opponent_last_move) {
            // take smaller of base_bid or my last move -1
            bid = (base_bid < (my_last_move - 1)) ? base_bid : my_last_move - 1;

        // lost my last
        } else if (my_last_move < opponent_last_move) {
            bid = opponent_last_move - 1;
        } else {
            // won my last as result of tie
            if (!win_if_tie(player)) {
                bid = my_last_move - 1;
            // lost my last as result of tie
            } else {
                // treat as a draw, bid the same as last time
                bid = opponent_last_move;
            }
        }

    // scotch closer to me, winning
    } else if (spots_remaining < opponent_spots_remaining) {
        // lets take the lesser of a calculated bid
         // take smaller of base_bid or my last move - 1
        bid = (base_bid < (my_last_move - 1)) ? base_bid : my_last_move - 1;

    // scotch closer to opponent, lets copy his last winning bid
    } else {
        bid = last_opponent_winner;
    }
    // if bid is too high set to max_bid, given we aren't in danger
    if (bid > max_bid) {
        bid = max_bid;
    }

    // opponent needs 1 more bid win to win
    if (opponent_spots_remaining <= 1) {
        if (my_money > opponent_money) {
            bid = opponent_money + 1;
        }  else if (my_money == opponent_money) {
            bid = opponent_money;
        } else {
            bid = my_money;
        }
    // 1 more spot to win, go all out or if bid is greater than my_money, set it equal to my money
    } else if (spots_remaining <= 1 || bid > my_money) {
        bid = my_money;
    }

    // if bid is 0 or negative and I have money left, set bid to 1
    if (bid < 1 && my_money) {
        bid = 1;
    }

    // if i have no money, set bid to 0
    if (my_money == 0) {
        bid = 0;
    }
    return bid;
}



// true if I win if tie, false if opponent win if tie
bool win_if_tie(int player){
    return  ((player_one_tie_win && player == 1) || (!player_one_tie_win && player == 2));
}


// updates balance and returns the latest value that the opponent won with
int updateBalance(int* my_moves, int * opponent_moves, int player) {
    my_money = initial_money;
    opponent_money = initial_money;
    int last_winner = 0;
    for (int i = 0; i < size_array(my_moves); ++i) {
        if (my_moves[i] > opponent_moves[i]) {
            my_money = my_money-my_moves[i];

        } else if (my_moves[i] < opponent_moves[i]){
            opponent_money = opponent_money - opponent_moves[i];

            last_winner = opponent_moves[i];
        } else {
            if (win_if_tie(player)) {
                my_money = my_money-my_moves[i];

            } else {
                opponent_money = opponent_money - opponent_moves[i];

                last_winner = opponent_moves[i];
            }
            player_one_tie_win = !player_one_tie_win;
        }
    }
    return last_winner;
}

int size_array(int * array) {
    int size = 0;

    while (array && *array) {
        ++array;
        ++size;
    }
    return size;
}


int main(void) {
    int player;                                 //1 if first player 2 if second
    int scotch_pos;                             //position of the scotch
    int bid,iter=0;                                    //Amount bid by the player
    size_t buf_limit = 500;
    char *first_move = (char *) malloc(buf_limit);      //previous bids of the first player
    char *second_move = (char *) malloc(buf_limit);     //prevous bids of the second player
    char remove_new_line[2];
    int first_moves[200] = {0};
    int second_moves[200] = {0};
    char *tok_1,*tok_2;
    cin>>player;
    cin>>scotch_pos;
    cin.getline(remove_new_line,2);           //removes a new line from the buffer
    cin.getline(first_move,200);
    cin.getline(second_move,200);
    tok_1 = strtok(first_move," ");
    for(int i=0; tok_1; i++) {
        first_moves[i] = atoi(tok_1);
        tok_1 = strtok(NULL," ");
    }
    tok_2 = strtok(second_move," ");
    for(int i=0;tok_2;i++) {
        second_moves[i] = atoi(tok_2);
        tok_2 = strtok(NULL," ");
    }
    bid = calculate_bid(player,scotch_pos,first_moves,second_moves);
    cout<<bid;
    return 0;
}
