#pragma once
#include <unordered_set>
#include <vstd.h>

namespace std {
    template<>
    struct hash<std::pair<long, long>> {
        size_t operator() ( const std::pair<long, long> &val ) const {
            return ( size_t ) ( 31 * val.first + val.second );
        }
    };
}

class LifeBoard {
public:
    LifeBoard ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board, int threads );

    ~LifeBoard();

    std::shared_ptr<std::list<std::pair<long, long>>> iterate();

    std::shared_ptr<std::unordered_set<std::pair<long, long>>> _prev_board = std::make_shared<std::unordered_set<std::pair<long, long>>>();
    std::shared_ptr<std::unordered_set<std::pair<long, long>>> _next_board = std::make_shared<std::unordered_set<std::pair<long, long>>>();

    std::shared_ptr<std::set<std::pair<long, long>>> _prev_diff = std::make_shared<std::set<std::pair<long, long>>>();

    bool next_state ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board,
                      std::pair<long, long> param ) const;

    int _iteration = 0;

    const int _threads;

    int adjacent_alive ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board,
                         std::pair<long, long> coords ) const;

    std::set<int> survive = {2, 3};
    std::set<int> born = {3};

    std::shared_ptr<std::set<std::pair<long, long>>> getNext ( std::pair<long, long> i ) const;

    std::shared_ptr<std::set<std::pair<long, long>>> build_diff (
        std::shared_ptr<std::unordered_set<std::pair<long, long>>> shared_ptr );
};

