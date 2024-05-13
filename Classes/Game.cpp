#include "Game.h"

void Game::Draw() const {
    for (int i = 0; i < 3; ++i) {
        std::cout << " — — — " << std::endl;
        std::cout << "|";
        for (int j = 0; j < 3; ++j) {
            std::cout << 3 * i + j + 1 << "|";
        }
        std::cout << std::endl;
    }
    std::cout << " — — — " << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << " — — — " << std::endl;
        std::cout << "|";
        for (int j = 0; j < 3; ++j) {
            std::cout << VField(_batlefield[i][j]) << "|";
        }
        std::cout << std::endl;
    }
    std::cout << " — — — " << std::endl;
}

FieldType Game::CheckEnd() const noexcept {
    int count_all = 0;
    for (int i = 0; i < 3; ++i) {
        int count_cross = 0, count_zero = 0;
        for (int j = 0; j < 3; ++j) {
            count_cross += (_batlefield[i][j] == CROSS);
            count_zero += (_batlefield[i][j] == ZERO);
        }
        if (count_cross == 3) {
            return CROSS;
        }
        if (count_zero == 3) {
            return ZERO;
        }
        count_all += count_zero + count_cross;
    }

    if (count_all == 9) {
        return DRAW;
    }

    for (int j = 0; j < 3; ++j) {
        int count_cross = 0, count_zero = 0;
        for (int i = 0; i < 3; ++i) {
            count_cross += (_batlefield[i][j] == CROSS);
            count_zero += (_batlefield[i][j] == ZERO);
        }
        if (count_cross == 3) {
            return CROSS;
        }
        if (count_zero == 3) {
            return ZERO;
        }
    }

    int count_cross = 0, count_zero = 0;
    for (int i = 0; i < 3; ++i) {
        count_cross += (_batlefield[i][i] == CROSS);
        count_zero += (_batlefield[i][i] == ZERO);
    }
    if (count_cross == 3) {
        return CROSS;
    }
    if (count_zero == 3) {
        return ZERO;
    }

    count_cross = 0, count_zero = 0;
    for (int i = 0; i < 3; ++i) {
        count_cross += (_batlefield[2 - i][i] == CROSS);
        count_zero += (_batlefield[2 - i][i] == ZERO);
    }
    if (count_cross == 3) {
        return CROSS;
    }
    if (count_zero == 3) {
        return ZERO;
    }

    return NONE;
}

bool Game::Step(int step) noexcept {
    if (!_player_turn) {
        step = (_difficulty == RANDOM ? GetRandomStep() : GetStep());
    }
    --step;
    const int row = step / 3, col = step % 3;
    if (_batlefield[row][col] != NONE) {
        return false;
    }
    _player_turn = !_player_turn;
    _batlefield[row][col] = _turn;
    _turn = FieldType(_turn % 2 + 1);
    return true;
}

FieldType Game::BestResult(FieldType (&batlefield)[3][3], int m, FieldType type) {
    FieldType checkend = ::CheckEnd(batlefield);
    if (checkend != NONE) return checkend;
    if (m == 0) return NONE;

    std::pair<int*, int> p = ::GetChanges(batlefield);
    FieldType* arr = new FieldType[p.second];
    for (int i = 0; i < p.second; ++i) {
        FieldType new_batlefield[3][3];
        for (int k = 0; k < 3; ++k) {
            for (int j = 0; j < 3; ++j) {
                new_batlefield[k][j] = batlefield[k][j];
            }
        }
        new_batlefield[(p.first[i] - 1) / 3][(p.first[i] - 1) % 3] = type;
        arr[i] = BestResult(new_batlefield, m - 1, FieldType(type % 2 + 1));
    }

    if ((m - 1) % 2 == 0) {
        FieldType ans = _player_type_field;
        bool flag_draw = false;
        for (int i = 0; i < p.second; ++i) {
            if (arr[i] == FieldType(_player_type_field % 2 + 1)) {
                ans = arr[i];
                break;
            }
            if (arr[i] == DRAW) {
                ans = arr[i];
                flag_draw = true;
            }
            if (arr[i] == NONE && !flag_draw) {
                ans = arr[i];
            }
        }
        delete[] arr;
        return ans;
    } else {
        FieldType ans = FieldType(_player_type_field % 2 + 1);
        bool flag_none = false;
        for (int i = 0; i < p.second; ++i) {
            if (arr[i] == _player_type_field) {
                ans = arr[i];
                break;
            }
            if (arr[i] == NONE) {
                ans = arr[i];
                flag_none = true;
            }
            if (arr[i] == DRAW && !flag_none) {
                ans = arr[i];
            }
        }
        delete[] arr;
        return ans;
    }
}

std::pair<int*, int> Game::GetChanges() const {
    int count_none = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            count_none += (_batlefield[i][j] == NONE);
        }
    }
    int* changes = new int[count_none];
    for (int i = 0, k = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (_batlefield[i][j] == NONE) {
                changes[k++] = i * 3 + j + 1;
            }
        }
    }
    return std::make_pair(changes, count_none);
}

int Game::GetRandomStep() const {
    std::pair<int*, int> p = GetChanges();
    const int step = p.first[rand() % p.second];
    delete[] p.first;
    return step;
}

int Game::GetStep() {
    std::pair<int*, int> p = GetChanges();

    FieldType* arr = new FieldType[p.second];
    for (int i = 0; i < p.second; ++i) {
        FieldType new_batlefield[3][3];
        for (int k = 0; k < 3; ++k) {
            for (int j = 0; j < 3; ++j) {
                new_batlefield[k][j] = _batlefield[k][j];
            }
        }
        new_batlefield[(p.first[i] - 1) / 3][(p.first[i] - 1) % 3] =
            FieldType(_player_type_field % 2 + 1);
        arr[i] = BestResult(new_batlefield, _difficulty*2 + 2, _player_type_field);
    }
    for (int i = 0; i < p.second; ++i) {
        if (arr[i] == FieldType(_player_type_field % 2 + 1)) {
            return p.first[i];
        }
    }
    for (int i = 0; i < p.second; ++i) {
        if (arr[i] == DRAW) {
            return p.first[i];
        }
    }
    for (int i = 0; i < p.second; ++i) {
        if (arr[i] == NONE) {
            return p.first[i];
        }
    }
    return p.first[0];
}

FieldType CheckEnd(FieldType (&_batlefield)[3][3]) {
    int count_all = 0;
    for (int i = 0; i < 3; ++i) {
        int count_cross = 0, count_zero = 0;
        for (int j = 0; j < 3; ++j) {
            count_cross += (_batlefield[i][j] == CROSS);
            count_zero += (_batlefield[i][j] == ZERO);
        }
        if (count_cross == 3) {
            return CROSS;
        }
        if (count_zero == 3) {
            return ZERO;
        }
        count_all += count_zero + count_cross;
    }

    if (count_all == 9) {
        return DRAW;
    }

    for (int j = 0; j < 3; ++j) {
        int count_cross = 0, count_zero = 0;
        for (int i = 0; i < 3; ++i) {
            count_cross += (_batlefield[i][j] == CROSS);
            count_zero += (_batlefield[i][j] == ZERO);
        }
        if (count_cross == 3) {
            return CROSS;
        }
        if (count_zero == 3) {
            return ZERO;
        }
    }

    int count_cross = 0, count_zero = 0;
    for (int i = 0; i < 3; ++i) {
        count_cross += (_batlefield[i][i] == CROSS);
        count_zero += (_batlefield[i][i] == ZERO);
    }
    if (count_cross == 3) {
        return CROSS;
    }
    if (count_zero == 3) {
        return ZERO;
    }

    count_cross = 0, count_zero = 0;
    for (int i = 0; i < 3; ++i) {
        count_cross += (_batlefield[2 - i][i] == CROSS);
        count_zero += (_batlefield[2 - i][i] == ZERO);
    }
    if (count_cross == 3) {
        return CROSS;
    }
    if (count_zero == 3) {
        return ZERO;
    }

    return NONE;
}

std::pair<int*, int> GetChanges(FieldType (&_batlefield)[3][3]) {
    int count_none = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            count_none += (_batlefield[i][j] == NONE);
        }
    }
    int* changes = new int[count_none];
    for (int i = 0, k = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (_batlefield[i][j] == NONE) {
                changes[k++] = i * 3 + j + 1;
            }
        }
    }
    return std::make_pair(changes, count_none);
}