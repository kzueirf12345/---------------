#include <iostream>

#include "Classes/Game.h"

int main() {
    srand(time(nullptr));

    std::cout << "0. Random" << std::endl;
    std::cout << "1. Easy" << std::endl;
    std::cout << "2. Medium" << std::endl;
    std::cout << "3. Hard" << std::endl;
    std::cout << "Choose difficulty: ";
    int difficulty_num;
    std::cin >> difficulty_num;
    Difficulty difficulty = Difficulty(difficulty_num);


    std::cout << "Will you be the first to play? 'Y/y' or 'N/n': ";
    char ans_player_first;
    std::cin >> ans_player_first;
    bool player_first = (ans_player_first == 'Y' || ans_player_first == 'y');

    std::cout << "Will you play for 'X/x' or for 'O/o'?: ";
    char ans_player_type_field;
    std::cin >> ans_player_type_field;
    FieldType player_type_field =
        (ans_player_type_field == 'X' || ans_player_type_field == 'x' ? CROSS : ZERO);

    Game game(player_first, player_type_field, difficulty);

    // FieldType temp[3][3] =
    // { {NONE, ZERO, NONE},
    //   {NONE, CROSS, NONE},
    //   {NONE, NONE, NONE} };

    // FieldType bestResult = game.BestResult(temp, 10, FieldType(player_type_field % 2 + 1));



    FieldType result;
    do {
      game.Draw();
        if (game.PlayerTurn()) {
            std::cout << "Input Your Choise: ";
            int choice;
            std::cin >> choice;
            game.Step(choice);
        } else {
            game.Step();
        }
    } while (!(result = game.CheckEnd()));
    game.Draw();

    if (result == DRAW) {
        std::cout << "\nDraw!" << std::endl;
    } else if (result == player_type_field) {
        std::cout << "You've won!" << std::endl;
    } else {
        std::cout << "You've lost!" << std::endl;
    }
    return 0;
}