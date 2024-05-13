#pragma once

#include <iostream>

enum FieldType { NONE = 0, CROSS = 1, ZERO = 2, DRAW = 3 };

enum Difficulty {RANDOM=0, EASY=1, MEDIUM=2, HARD=3};

FieldType CheckEnd(FieldType (&_batlefield)[3][3]);
std::pair<int*, int> GetChanges(FieldType (&_batlefield)[3][3]);

class Game {
   public:
    Game(bool player_turn, FieldType player_type_field, Difficulty difficulty)
        : _player_turn(player_turn), _player_type_field(player_type_field), _difficulty(difficulty) {
        _turn = (_player_turn ? player_type_field : FieldType(player_type_field % 2 + 1));
    }

    void Draw() const;

    FieldType CheckEnd() const noexcept;

    bool Step(int step = -1) noexcept;

    bool PlayerTurn() const noexcept { return _player_turn; }

    FieldType BestResult(FieldType (&batlefield)[3][3], int m, FieldType type);

   private:
    char VField(FieldType type) const noexcept {
        return (type == NONE ? ' ' : (type == CROSS ? 'X' : 'O'));
    }
    FieldType _batlefield[3][3] = {{NONE, NONE, NONE}, {NONE, NONE, NONE}, {NONE, NONE, NONE}};
    FieldType _turn;
    FieldType _player_type_field;
    bool _player_turn;

    Difficulty _difficulty;

    std::pair<int*, int> GetChanges() const;
    int GetRandomStep() const;
    int GetStep();
};