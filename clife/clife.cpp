#include "clife.h"

#include <boost/range/irange.hpp>

namespace vstd {
    std::function<void ( std::function<void() > ) > get_call_later_handler() {
        return std::function<void ( std::function<void() > ) >();
    }
    std::function<void ( std::function<void() > ) > get_call_async_handler() {
        return [] ( std::function<void() > f ) {
            static std::shared_ptr<vstd::thread_pool<16>> pool = std::make_shared<vstd::thread_pool<16>>()->start();
            pool->execute ( f );
        };
    }
    std::function<void ( std::function<void() > ) > get_call_later_block_handler() {
        return std::function<void ( std::function<void() > ) >();
    }
    std::function<void ( std::function<bool() > ) > get_wait_until_handler() {
        return std::function<void ( std::function<bool() > ) >();
    }
}

static bool get_cell ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board, std::pair<long, long> coords )  {
    return board->find ( coords ) != board->end();
}

static bool set_cell ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board, std::pair<long, long> coords,
                       bool val ) {
    const std::unordered_set<std::pair<long, long>>::iterator it = board->find ( coords );
    if ( val && ( it == board->end() ) ) {
        board->insert ( coords );
        return true;
    } else if ( it != board->end() ) {
        board->erase ( coords );
        return true;
    }
    return false;
}

static bool flip_cell ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board, std::pair<long, long> pair ) {
    return set_cell ( board, pair, !get_cell ( board, pair ) );
}

LifeBoard::LifeBoard ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board, int threads ) : _threads ( threads ),
    _prev_board ( board ),
    _prev_diff (
        build_diff (
            board ) ) {
}
int LifeBoard::adjacent_alive ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board,
                                std::pair<long, long> coords ) const {
    int alive = 0;
    const std::shared_ptr<std::set<std::pair<long, long>>> ptr = getNext ( coords );
    for ( std::pair<long, long> next:*ptr ) {
        if ( get_cell ( board, next ) ) {
            alive++;
        }
    }
    return alive;
}
bool LifeBoard::next_state ( std::shared_ptr<std::unordered_set<std::pair<long, long>>> board,
                             std::pair<long, long> param ) const {
    int nei = adjacent_alive ( board, param );
    bool cell = get_cell ( board, param );
    if ( cell ) {
        bool tmp = true;
        for ( int it:survive ) {
            tmp = tmp && ( nei != it );
        }
        if ( tmp ) {
            return false;
        }
    } else {
        bool tmp = false;
        for ( int it:born ) {
            tmp = tmp || ( nei == it );
        }
        if ( tmp ) {
            return true;
        }
    }
    return cell;
}
std::shared_ptr<std::list<std::pair<long, long>>> LifeBoard::iterate() {
    if(_iteration==0){
        return std::make_shared<std::list<std::pair<long, long>>>(_prev_board->begin(),_prev_board->end());
    }

    auto changed = std::make_shared<std::list<std::pair<long, long>>>();

    auto next_diff = std::make_shared<std::set<std::pair<long, long>>>();

    auto next_board=vstd::async ( [this]() {
        auto tmp = std::make_shared<std::unordered_set<std::pair<long, long>>>();
        tmp->insert ( _next_board->begin(),_next_board->end() );
        return tmp;
    } );

    auto chain = std::make_shared<vstd::chain<std::pair<long, long>>> ( [this,next_diff,next_board,changed] ( std::pair<long, long> crd ) {
        changed->push_back ( crd );
        flip_cell ( next_board->get(), crd );
        next_diff->insert ( crd );
        std::shared_ptr<std::set<std::pair<long, long>>> nxt = getNext ( crd );
        next_diff->insert ( nxt->begin(), nxt->end() );
    } );

    auto cb = [this,chain] ( int i ) {
        return vstd::async ( [this, chain,i]() {
            size_t step = _prev_diff->size() / _threads + 1;
            std::set<std::pair<long, long>>::iterator start = _prev_diff->begin();
            std::set<std::pair<long, long>>::iterator end = _prev_diff->begin();
            std::advance ( start, step * i );
            std::advance ( end, step * ( i + 1 ) );
            for ( ; start != end; start++ ) {
                if ( get_cell ( _next_board, *start ) != next_state ( _prev_board, *start ) ) {
                    chain->invoke_async ( *start );
                }
            }
        } );
    };

    vstd::join ( boost::irange ( 0, _threads ) | boost::adaptors::transformed ( cb ) )->thenAsync (
    [this,next_board,chain] ( std::set<void*> ) {
        chain->terminate()->thenAsync ( [this,next_board]() {
            _next_board=next_board->get();
        } );
    } )->get();

    _prev_board.swap ( _next_board );

    _prev_diff=next_diff;

    _iteration++;
    return changed;
}

LifeBoard::~LifeBoard() {

}

std::shared_ptr<std::set<std::pair<long, long>>> LifeBoard::getNext ( std::pair<long, long> i ) const {
    std::shared_ptr<std::set<std::pair<long, long>>> tmp = std::make_shared<std::set<std::pair<long, long>>>();
    long x = i.first;
    long y = i.second;
    tmp->insert ( std::pair<long, long> ( x - 1, y - 1 ) );
    tmp->insert ( std::pair<long, long> ( x - 1, y ) );
    tmp->insert ( std::pair<long, long> ( x - 1, y + 1 ) );
    tmp->insert ( std::pair<long, long> ( x, y - 1 ) );
    tmp->insert ( std::pair<long, long> ( x, y + 1 ) );
    tmp->insert ( std::pair<long, long> ( x + 1, y - 1 ) );
    tmp->insert ( std::pair<long, long> ( x + 1, y ) );
    tmp->insert ( std::pair<long, long> ( x + 1, y + 1 ) );
    return tmp;
}

std::shared_ptr<std::set<std::pair<long, long>>> LifeBoard::build_diff (
        std::shared_ptr<std::unordered_set<std::pair<long, long>>> board ) {
    std::shared_ptr<std::set<std::pair<long, long>>> tmp = std::make_shared<std::set<std::pair<long, long>>>();
    for ( std::pair<long, long> coords:*board ) {
        tmp->insert ( coords );
        std::shared_ptr<std::set<std::pair<long, long>>> ptr = getNext ( coords );
        tmp->insert ( ptr->begin(), ptr->end() );
    }
    return tmp;
}

int main ( int argc, char **args ) {
    srand(time(0));
    long SIZE = 10;
    float factor = 0.8;
    long seeds = ( long ) ( SIZE * SIZE * factor );
    std::shared_ptr<std::unordered_set<std::pair<long, long>>> tmp = std::make_shared<std::unordered_set<std::pair<long, long>>> ( seeds );
    for ( int i = 0; i < seeds; i++ ) {
        flip_cell ( tmp, std::pair<long, long> ( rand() % SIZE - SIZE / 2, rand() % SIZE - SIZE / 2 ) );
    }
    LifeBoard board ( tmp, 5 );
    while ( board._prev_diff->size() ) {
        std::cout << board.iterate()->size() << std::endl;
    }
}


