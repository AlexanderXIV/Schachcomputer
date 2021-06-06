// still missing: Promotions
#include <chrono>
#include <stack>
#include <vector>
#include <iostream>
#include <array>
#include <utility>
#include <tuple>
#include <future>
using namespace std;

#include <cassert>

#define ASCII
#define DEBUG
#ifdef DEBUG
#define assertm(exp, msg) assert(((void)msg, exp))
#else
#define assertm(exp, msg)
#endif

#pragma region Chess
enum ChessFigure
{
  None,
  Turm,
  Springer,
  Laeufer,
  Koenig,
  Dame,
  Bauer
};

struct Figur
{
  ChessFigure type;
  bool is_white;

  explicit Figur(ChessFigure _type, bool _is_white = true) : type(_type), is_white(_is_white){};

  Figur clone() const { return Figur(type, is_white); }

  friend ostream &operator<<(ostream &os, const Figur &figur);
};

ostream &operator<<(ostream &os, const Figur &figur)
{
#ifndef ASCII
  switch (figur.type)
  {
  case None:
    os << "-";
    break;
  case Turm:
    os << ((figur.is_white) ? "♜" : "♖");
    break;
  case Springer:
    os << ((figur.is_white) ? "♞" : "♘");
    break;
  case Laeufer:
    os << ((figur.is_white) ? "♝" : "♗");
    break;
  case Koenig:
    os << ((figur.is_white) ? "♚" : "♔");
    break;
  case Dame:
    os << ((figur.is_white) ? "♛" : "♕");
    break;
  case Bauer:
    os << ((figur.is_white) ? "♟" : "♙");
    break;
  }
#else
  switch (figur.type)
  {
  case None:
    os << "--";
    break;
  case Turm:
    os << ((figur.is_white) ? "wT" : "bT");
    break;
  case Springer:
    os << ((figur.is_white) ? "wS" : "bS");
    break;
  case Laeufer:
    os << ((figur.is_white) ? "wL" : "bL");
    break;
  case Koenig:
    os << ((figur.is_white) ? "wK" : "bK");
    break;
  case Dame:
    os << ((figur.is_white) ? "wD" : "bD");
    break;
  case Bauer:
    os << ((figur.is_white) ? "wB" : "bB");
    break;
  }
#endif
  return os;
}

inline bool operator==(const Figur &a, const Figur &b) { return a.type == b.type && (None == a.type || a.is_white == b.is_white); }

inline bool operator!=(const Figur &a, const Figur &b) { return !(a == b); }

class Board
{
private:
  stack<tuple<int, int, int, int, int, int, bool, Figur, bool, bool, bool, bool>> history;
  bool bool_white_turn;
  array<array<Figur, 8>, 8> board{{{Figur(Turm, false), Figur(Springer, false), Figur(Laeufer, false), Figur(Dame, false), Figur(Koenig, false), Figur(Laeufer, false), Figur(Springer, false), Figur(Turm, false)},
                                   {Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false)},
                                   {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                   {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                   {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                   {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                   {Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer)},
                                   {Figur(Turm), Figur(Springer), Figur(Laeufer), Figur(Dame), Figur(Koenig), Figur(Laeufer), Figur(Springer), Figur(Turm)}}}; // board[y][x]

  bool doubleMove = false;
  pair<int, int> pos_king_white = make_pair(4, 0);
  pair<int, int> pos_king_black = make_pair(4, 7);
  int to_x = -1, to_y = -1;
  bool lca[2]{true, true}, sca[2]{true, true}; // large castle_allowed {black, white}

  // explicit Board(string boardLayout, bool starting_white_turn) = delete;

  // pair<int, int> get_pos(Figur figur) const = delete;
  // bool is_white_turn() const = delete;
  // bool is_checkmate(bool _white_turn) const = delete;
  // bool is_chess(bool _white_turn) = delete;
  // bool is_valid(bool _white_turn) const = delete;
  // Board clone() const = delete;

  // void operator&() = delete;
  // void operator&&() = delete;

  // vector<pair<int, int>> get_all_moves_fast(int x1, int y1, bool _white_turn) const;
  // vector<pair<int, int>> get_all_moves(int x1, int y1, bool _white_turn) = delete;
  // bool is_possible(int x1, int y1, int x2, int y2, bool _white_turn);
  bool check(int x1, int y1, int x2, int y2, bool _white_turn);
  bool is_chess(int x1, int y1, bool _white_turn) const;
  bool is_possible_without_chess(int x1, int y1, int x2, int y2, bool _white_turn) const;

public:
  explicit Board(bool starting_white_turn);

  vector<pair<int, int>> get_all_moves_speed(int x1, int y1, bool _white_turn);

  // bool move(int x1, int y1, int x2, int y2);
  void move_unsafe(int x1, int y1, int x2, int y2);
  void undoMove();

  int getScore(bool _white_turn) const;
  bool is_white_turn() { return bool_white_turn; };

  friend ostream &operator<<(ostream &os, const Board &board);
};

ostream &operator<<(ostream &os, const Board &chess)
{
  for (int y = 0; y < 8; ++y)
  {
    bool first = true;
    for (Figur f : chess.board[y])
    {
      if (!first)
        cout << " ";
      else
        first = false;
      os << f << " ";
    }
    os << "\n";
  }
  os << "\n";
  return os;
}

Board::Board(bool starting_white_turn) : bool_white_turn(starting_white_turn)
{
  //   bool_white_turn = true;
  //   bool b = false;
  // board = array<array<Figur, 8>, 8>{{{Figur(Turm, false), Figur(None), Figur(None), Figur(None), Figur(Koenig, false), Figur(None), Figur(None), Figur(Turm, false)},
  //                                    {Figur(Bauer, false), Figur(None), Figur(Bauer, false), Figur(Bauer, false), Figur(Dame, false), Figur(Bauer, false), Figur(Laeufer, false), Figur(None)},
  //                                    {Figur(Laeufer, false), Figur(Springer, false), Figur(None), Figur(None), Figur(Bauer, false), Figur(Springer, false), Figur(Bauer, false), Figur(None)},
  //                                    {Figur(None), Figur(None), Figur(None), Figur(Bauer), Figur(Springer), Figur(None), Figur(None), Figur(None)},
  //                                    {Figur(None), Figur(Bauer, false), Figur(None), Figur(None), Figur(Bauer), Figur(None), Figur(None), Figur(None)},
  //                                    {Figur(None), Figur(None), Figur(Springer), Figur(None), Figur(None), Figur(Dame), Figur(None), Figur(Bauer, false)},
  //                                    {Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Laeufer), Figur(Laeufer), Figur(Bauer), Figur(Bauer), Figur(Bauer)},
  //                                    {Figur(Turm), Figur(None), Figur(None), Figur(None), Figur(Koenig), Figur(None), Figur(None), Figur(Turm)}}};

  // board = array<array<Figur, 8>, 8>{{{Figur(Turm, false), Figur(Springer, false), Figur(Laeufer, false), Figur(Dame, false), Figur(Koenig, false), Figur(Laeufer, false), Figur(Springer, false), Figur(Turm, false)},
  //                                    {Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false)},
  //                                    {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
  //                                    {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
  //                                    {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
  //                                    {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
  //                                    {Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer)},
  //                                    {Figur(Turm), Figur(None), Figur(None), Figur(None), Figur(Koenig), Figur(None), Figur(None), Figur(Turm)}}};

  cout << *this << endl;

  //   for (int y = 0; y < 8; ++y)
  //     for (int x = 0; x < 8; ++x)
  //       if (Koenig == board[y][x].type)
  //       {
  //         if (!board[y][x].is_white)
  //           pos_king_white = make_pair(x, y);
  //         else
  //           pos_king_black = make_pair(x, y);
  //       }
}

vector<pair<int, int>> Board::get_all_moves_speed(int x1, int y1, bool _white_turn)
{
  vector<pair<int, int>> result;
  if (_white_turn != board[y1][x1].is_white)
    return result;

  auto lambda = [&](int x, int y)
  {
    const bool success = None == board[y][x].type;
    if ((success || board[y][x].is_white != _white_turn) && check(x1, y1, x, y, _white_turn))
      result.push_back({x, y});
    return success;
  };

  auto singleLambda = [&](int x, int y)
  {
    if (0 <= x && 0 <= y && x < 8 && y < 8 && (None == board[y][x].type || board[y][x].is_white != _white_turn) && check(x1, y1, x, y, _white_turn))
      result.push_back({x, y});
  };

  switch (board[y1][x1].type)
  {
  case None:
  {
    break;
  }
  case Turm:
  {
    for (int x = x1 + 1; x < 8; ++x)
      if (!lambda(x, y1))
        break;

    for (int x = x1 - 1; x >= 0; --x)
      if (!lambda(x, y1))
        break;

    for (int y = y1 + 1; y < 8; ++y)
      if (!lambda(x1, y))
        break;

    for (int y = y1 - 1; y >= 0; --y)
      if (!lambda(x1, y))
        break;
    break;
  }
  case Dame:
  {
    for (int x = x1 + 1; x < 8; ++x)
      if (!lambda(x, y1))
        break;

    for (int x = x1 - 1; x >= 0; --x)
      if (!lambda(x, y1))
        break;

    for (int y = y1 + 1; y < 8; ++y)
      if (!lambda(x1, y))
        break;

    for (int y = y1 - 1; y >= 0; --y)
      if (!lambda(x1, y))
        break;

    for (int i = 1; x1 + i < 8 && y1 + i < 8; ++i)
      if (!lambda(x1 + i, y1 + i))
        break;

    for (int i = 1; x1 + i < 8 && y1 - i >= 0; ++i)
      if (!lambda(x1 + i, y1 - i))
        break;

    for (int i = 1; x1 - i >= 0 && y1 + i < 8; ++i)
      if (!lambda(x1 - i, y1 + i))
        break;

    for (int i = 1; x1 - i >= 0 && y1 - i >= 0; ++i)
      if (!lambda(x1 - i, y1 - i))
        break;
    break;
  }
  case Laeufer:
  {
    for (int i = 1; x1 + i < 8 && y1 + i < 8; ++i)
      if (!lambda(x1 + i, y1 + i))
        break;

    for (int i = 1; x1 + i < 8 && y1 - i >= 0; ++i)
      if (!lambda(x1 + i, y1 - i))
        break;

    for (int i = 1; x1 - i >= 0 && y1 + i < 8; ++i)
      if (!lambda(x1 - i, y1 + i))
        break;

    for (int i = 1; x1 - i >= 0 && y1 - i >= 0; ++i)
      if (!lambda(x1 - i, y1 - i))
        break;
    break;
  }
  case Koenig: // kleine / große Rochade
  {
    singleLambda(x1 - 1, y1 - 1);
    singleLambda(x1 - 1, y1 + 0);
    singleLambda(x1 - 1, y1 + 1);
    singleLambda(x1 + 0, y1 - 1);
    singleLambda(x1 + 0, y1 + 1);
    singleLambda(x1 + 1, y1 + 1);
    singleLambda(x1 + 1, y1 + 0);
    singleLambda(x1 + 1, y1 - 1);

    if (0 <= x1 + 2 && x1 + 2 < 8 && sca[board[y1][x1].is_white] &&
        !is_chess(x1, y1, board[y1][x1].is_white) && !is_chess(x1 + 1, y1, board[y1][x1].is_white) && !is_chess(x1 + 2, y1, board[y1][x1].is_white) &&
        None == board[y1][x1 + 1].type && None == board[y1][x1 + 2].type)
      result.push_back({(x1 + 2), y1});

    if (0 <= x1 - 2 && x1 - 2 < 8 && lca[board[y1][x1].is_white] &&
        !is_chess(x1, y1, board[y1][x1].is_white) && !is_chess(x1 - 1, y1, board[y1][x1].is_white) && !is_chess(x1 - 2, y1, board[y1][x1].is_white) &&
        None == board[y1][x1 - 1].type && None == board[y1][x1 - 2].type && None == board[y1][x1 - 3].type)
      result.push_back({(x1 - 2), y1});

    break;
  }
  case Springer:
  {
    singleLambda(x1 + 2, y1 + 1);
    singleLambda(x1 + 1, y1 + 2);
    singleLambda(x1 + 2, y1 - 1);
    singleLambda(x1 + 1, y1 - 2);
    singleLambda(x1 - 2, y1 + 1);
    singleLambda(x1 - 1, y1 + 2);
    singleLambda(x1 - 2, y1 - 1);
    singleLambda(x1 - 1, y1 - 2);

    break;
  }
  case Bauer:
  {
    auto pawn1 = [&](int x, int y)
    {
      const bool success = (0 <= x && 0 <= y && x < 8 && y < 8 && None == board[y][x].type);
      if (success && check(x1, y1, x, y, _white_turn))
        result.push_back({x, y});
      return success;
    };

    auto pawn2 = [&](int x, int y)
    {
      if (0 <= x && 0 <= y && x < 8 && y < 8 && ((None != board[y][x].type && board[y][x].is_white != _white_turn) || (None == board[y][x].type && doubleMove && board[y1][x] == Figur(Bauer, !_white_turn) && to_x == x && to_y == y1)) && check(x1, y1, x, y, _white_turn))
        result.push_back({x, y});
    };

    if (_white_turn)
    {
      if (pawn1(x1, y1 - 1) && y1 == 6)
        pawn1(x1, y1 - 2);
      pawn2(x1 - 1, y1 - 1);
      pawn2(x1 + 1, y1 - 1);
    }
    else
    {
      if (pawn1(x1, y1 + 1) && y1 == 1)
        pawn1(x1, y1 + 2);
      pawn2(x1 - 1, y1 + 1);
      pawn2(x1 + 1, y1 + 1);
    }
    break;
  }
  }
  return result;
}

bool Board::check(int x1, int y1, int x2, int y2, bool _white_turn)
{
  move_unsafe(x1, y1, x2, y2);

  const pair<int, int> king = (_white_turn) ? pos_king_black : pos_king_white;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      if (is_possible_without_chess(x, y, king.first, king.second, !_white_turn))
      {
        undoMove();
        return false;
      }
  undoMove();
  return true;
}

bool Board::is_chess(int x1, int y1, bool _white_turn) const
{
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      if (is_possible_without_chess(x, y, x1, y1, !_white_turn))
        return true;
  return false;
}

bool Board::is_possible_without_chess(int x1, int y1, int x2, int y2, bool _white_turn) const
{
  assertm((0 <= x1 && x1 < 8) || (0 <= y1 && y1 < 8) || (0 <= x2 && x2 < 8) || (0 <= y2 && y2 < 8), "OutOfBounds");

  if ((x1 == x2 && y1 == y2) || _white_turn != board[y1][x1].is_white || (None != board[y2][x2].type && board[y1][x1].is_white == board[y2][x2].is_white))
    return false;

  switch (board[y1][x1].type)
  {
  case None:
    return false;
  case Turm:
  {
    if (x1 == x2)
    {
      if (y2 < y1)
        swap(y1, y2); // y1 <= y2

      for (int y = y1 + 1; y < y2; ++y)
        if (None != board[y][x1].type)
          return false;
      return true;
    }
    else if (y1 == y2)
    {
      if (x2 < x1)
        swap(x1, x2); // x1 <= x2

      for (int x = x1 + 1; x < x2; ++x)
        if (None != board[y1][x].type)
          return false;
      return true;
    }
    return false;
  }
  case Springer:
  {
    const int dif_x = abs(x2 - x1);
    const int dif_y = abs(y2 - y1);

    return (dif_x == 2 && dif_y == 1) || (dif_x == 1 && dif_y == 2);
  }
  case Laeufer:
  {
    if (x1 > x2)
    {
      swap(x1, x2);
      swap(y1, y2);
    }

    const int dif_x = x2 - x1;
    const int dif_y = y2 - y1;

    if (dif_x == dif_y)
    {
      for (int i = 1; i < dif_x; ++i)
        if (None != board[y1 + i][x1 + i].type)
          return false;
      return true;
    }
    else if (dif_x == -dif_y)
    {
      for (int i = 1; i < dif_x; ++i)
        if (None != board[y1 - i][x1 + i].type)
          return false;
      return true;
    }
    return false;
  }
  case Dame:
  {
    if (x1 > x2)
    {
      swap(x1, x2);
      swap(y1, y2);
    }

    const int dif_x = x2 - x1;
    const int dif_y = y2 - y1;

    if (x1 == x2)
    {
      if (y2 < y1)
        swap(y1, y2); // y1 <= y2

      for (int y = y1 + 1; y < y2; ++y)
        if (None != board[y][x1].type)
          return false;
      return true;
    }
    else if (y1 == y2)
    {
      if (x2 < x1)
        swap(x1, x2); // x1 <= x2

      for (int x = x1 + 1; x < x2; ++x)
        if (None != board[y1][x].type)
          return false;
      return true;
    }
    else if (dif_x == dif_y)
    {
      for (int i = 1; i < dif_x; ++i)
        if (None != board[y1 + i][x1 + i].type)
          return false;
      return true;
    }
    else if (dif_x == -dif_y)
    {
      for (int i = 1; i < dif_x; ++i)
        if (None != board[y1 - i][x1 + i].type)
          return false;
      return true;
    }
    return false;
  }
  case Bauer:
  {
    const int dif_x = x2 - x1;
    const int dif_y = (board[y1][x1].is_white) ? y1 - y2 : y2 - y1;

    if (1 == dif_y)
    {
      if (-1 == dif_x || 1 == dif_x)
        return None != board[y2][x2].type || (doubleMove && board[y1][x1 + dif_x] == Figur(Bauer, !_white_turn) && to_x == x1 + dif_x && to_y == y1);
      return 0 == dif_x && None == board[y2][x2].type;
    }
    else if (2 == dif_y && 0 == dif_x && y1 == ((board[y1][x1].is_white) ? 6 : 1) && None == board[y2][x2].type)
      return None == board[y1 + ((board[y1][x1].is_white) ? -1 : 1)][x1].type;
    return false;
  }
  case Koenig:
    return abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1;
  }
  return false;
}

void Board::move_unsafe(int x1, int y1, int x2, int y2)
{
  history.push(make_tuple(x1, y1, x2, y2, to_x, to_y, doubleMove, board[y2][x2], lca[0], lca[1], sca[0], sca[1]));

  // en passant
  if (1 == abs(y2 - y1) && 1 == abs(x2 - x1) && Bauer == board[y1][x1].type && None == board[y2][x2].type)
    board[y1][x2] = Figur(None);

  // Rochade
  if (Koenig == board[y1][x1].type)
  {
    if (2 == abs(x2 - x1))
    {
      if (x1 < x2)
        swap(board[y1][5], board[y1][7]);
      else
        swap(board[y1][3], board[y1][0]);
    }
    lca[board[y1][x1].is_white] = false, sca[board[y1][x1].is_white] = false;
  }

  if (Turm == board[y1][x1].type)
  {
    if (x1 == 0)
      lca[board[y1][x1].is_white] = false;
    else if (x1 == 7)
      sca[board[y1][x1].is_white] = false;
  }

  doubleMove = (Bauer == board[y1][x1].type && 2 == abs(y1 - y2));
  bool_white_turn = !bool_white_turn;

  board[y2][x2] = board[y1][x1];
  board[y1][x1] = Figur(None);

  if (Koenig == board[y2][x2].type)
  {
    if (board[y2][x2].is_white)
      pos_king_black = make_pair(x2, y2);
    else
      pos_king_white = make_pair(x2, y2);
  }
  to_x = x2, to_y = y2;
}

void Board::undoMove()
{
  int _x1, _y1, _x2, _y2, _to_x, _to_y;
  Figur f(None);
  bool b1, b2, b3, b4;
  tie(_x1, _y1, _x2, _y2, _to_x, _to_y, doubleMove, f, b1, b2, b3, b4) = history.top();
  history.pop();

  lca[0] = b1, lca[1] = b2, sca[0] = b3, sca[1] = b4;

  // en passant
  if (1 == abs(_y2 - _y1) && 1 == abs(_x2 - _x1) && Bauer == board[_y2][_x2].type && None == f.type)
    board[_y1][_x2] = Figur(Bauer, !board[_y2][_x2].is_white);

  // Rochade
  if (Koenig == board[_y2][_x2].type && 2 == abs(_x2 - _x1))
  {
    if (_x1 < _x2)
      swap(board[_y1][5], board[_y1][7]);
    else
      swap(board[_y1][3], board[_y1][0]);
  }

  bool_white_turn = !bool_white_turn;

  if (Koenig == board[_y2][_x2].type)
  {
    if (board[_y2][_x2].is_white)
      pos_king_black = make_pair(_x1, _y1);
    else
      pos_king_white = make_pair(_x1, _y1);
  }

  board[_y1][_x1] = board[_y2][_x2];
  board[_y2][_x2] = f;

  to_x = _to_x, to_y = _to_y;
}
#pragma endregion

#ifdef DEBUG
int LIMIT = 0;

vector<unsigned long> steptest2(Board b, const int n = 0)
{
  stack<future<vector<unsigned long>>> stck;
  vector<unsigned long> result(LIMIT, 0);
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto item : b.get_all_moves_speed(x, y, b.is_white_turn()))
      {
        if (n < LIMIT - 1)
        {
          b.move_unsafe(x, y, item.first, item.second);
          if (n == 0)
            stck.push(async(steptest2, b, n + 1));
          else
          {
            int c1 = 0;
            for (auto item : steptest2(b, n + 1))
              result[c1++] += item;
          }
          b.undoMove();
        }
        ++result[n];
      }
  while (!stck.empty())
  {
    int c1 = 0;
    for (auto item : stck.top().get())
      result[c1++] += item;
    stck.pop();
  }
  return result;
}

int main(int argc, char **argv)
{
  if (2 != argc)
  {
    cout << "please enter Limit" << endl;
    exit(0);
  }

  int val = (int)(argv[1][0] - '0');
  if (0 < val && val <= 10)
    LIMIT = val;
  else
  {
    cout << "not a valid number ([1..10])";
    exit(0);
  }

  cout << "depth " << LIMIT << ":" << endl;
  Board b(true);

  auto start = std::chrono::system_clock::now();
  auto res = steptest2(b);
  auto end = std::chrono::system_clock::now();
  for (int i = 0; i < LIMIT; ++i)
    cout << (i + 1) << ": " << res[i] << endl;
  cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

  if (3 == argc)
  {
    int i;
    cin >> i;
  }
}
#else
// bool Board::is_possible(int x1, int y1, int x2, int y2, bool _white_turn)
// {
//   if (is_possible_without_chess(x1, y1, x2, y2, _white_turn))
//   {
//     move_unsafe(x1, y1, x2, y2);

//     const pair<int, int> king = (_white_turn) ? pos_king_black : pos_king_white;

//     for (int y = 0; y < 8; ++y)
//       for (int x = 0; x < 8; ++x)
//         if (is_possible_without_chess(x, y, king.first, king.second, !_white_turn))
//         {
//           undoMove();
//           return false;
//         }
//     undoMove();
//     return true;
//   }
//   return false;
// }

// bool Board::move(int x1, int y1, int x2, int y2)
// {
//   if (y1 == y2 && ((x1 == 4 && x2 == 2) || (x1 == 4 && x2 == 6)) && (0 == y1 || 7 == y2))
//   {
//     move_unsafe(x1, y1, x2, y2); // ???
//     move_unsafe(7, y1, 5, y2);
//     bool_white_turn = !bool_white_turn;
//     // return true ???
//   }

//   if (Bauer == board[y1][x1].type && ((y2 == 0) || y2 == 7))
//   {
//     board[y2][x2] = Figur(Dame, bool_white_turn);
//     // "q" fehlt
//   }

//   bool is_pos = is_possible(x1, y1, x2, y2, bool_white_turn);
//   if (is_pos)
//     move_unsafe(x1, y1, x2, y2);

//   return is_pos;
// }

#pragma region getScore
typedef array<array<int, 8>, 8> arr2d;
arr2d pawn{{{0, 0, 0, 0, 0, 0, 0, 0},
            {50, 50, 50, 50, 50, 50, 50, 50},
            {10, 10, 20, 30, 30, 20, 10, 10},
            {5, 5, 10, 25, 25, 10, 5, 5},
            {0, 0, 0, 20, 20, 0, 0, 0},
            {5, -5, -10, 0, 0, -10, -5, 5},
            {5, 10, 10, -20, -20, 10, 10, 5},
            {0, 0, 0, 0, 0, 0, 0, 0}}};

arr2d knight{
    {{-50, -40, -30, -30, -30, -30, -40, -50},
     {-40, -20, 0, 0, 0, 0, -20, -40},
     {-30, 0, 10, 15, 15, 10, 0, -30},
     {-30, 5, 15, 20, 20, 15, 5, -30},
     {-30, 0, 15, 20, 20, 15, 0, -30},
     {-30, 5, 10, 15, 15, 10, 5, -30},
     {-40, -20, 0, 5, 5, 0, -20, -40},
     {-50, -40, -30, -30, -30, -30, -40, -50}}};

arr2d bishop{{{-20, -10, -10, -10, -10, -10, -10, -20},
              {-10, 0, 0, 0, 0, 0, 0, -10},
              {-10, 0, 5, 10, 10, 5, 0, -10},
              {-10, 5, 5, 10, 10, 5, 5, -10},
              {-10, 0, 10, 10, 10, 10, 0, -10},
              {-10, 10, 10, 10, 10, 10, 10, -10},
              {-10, 5, 0, 0, 0, 0, 5, -10},
              {-20, -10, -10, -10, -10, -10, -10, -20}}};

arr2d rook{{{0, 0, 0, 0, 0, 0, 0, 0},
            {5, 10, 10, 10, 10, 10, 10, 5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 5, 5, 0, 0, -5}}};
// {0, 0, 0, 5, 5, 0, 0, 0}}};

arr2d queen{{{-20, -10, -10, -5, -5, -10, -10, -20},
             {-10, 0, 0, 0, 0, 0, 0, -10},
             {-10, 0, 5, 5, 5, 5, 0, -10},
             {-5, 0, 5, 5, 5, 5, 0, -5},
             {0, 0, 5, 5, 5, 5, 0, -5},
             {-10, 5, 5, 5, 5, 5, 0, -10},
             {-10, 0, 5, 0, 0, 0, 0, -10},
             {-20, -10, -10, -5, -5, -10, -10, -20}}};

arr2d king_game{{{-30, -40, -40, -50, -50, -40, -40, -30},
                 {-30, -40, -40, -50, -50, -40, -40, -30},
                 {-30, -40, -40, -50, -50, -40, -40, -30},
                 {-30, -40, -40, -50, -50, -40, -40, -30},
                 {-20, -30, -30, -40, -40, -30, -30, -20},
                 {-10, -20, -20, -20, -20, -20, -20, -10},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 //  {20, 20, 0, 0, 0, 0, 20, 20},
                 {0, 0, 0, 0, 0, 0, 0, 0}}};
//  {20, 30, 10, 0, 0, 10, 30, 20}}};

arr2d king_endgame{{{-50, -40, -30, -20, -20, -30, -40, -50},
                    {-30, -20, -10, 0, 0, -10, -20, -30},
                    {-30, -10, 20, 30, 30, 20, -10, -30},
                    {-30, -10, 30, 40, 40, 30, -10, -30},
                    {-30, -10, 30, 40, 40, 30, -10, -30},
                    {-30, -10, 20, 30, 30, 20, -10, -30},
                    {-30, -30, 0, 0, 0, 0, -30, -30},
                    {-50, -30, -30, -30, -30, -30, -30, -50}}};

int Board::getScore(bool _white_turn) const
{
  // Both sides have no queens or every side which has a queen has additionally no other pieces or one minorpiece maximum.
  constexpr int INTINF = 1e7;
  int special = 0;
  int score = 0;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      if (None != board[y][x].type && board[y][x].is_white == _white_turn && (board[y][x].type == Turm || board[y][x].type == Springer || board[y][x].type == Laeufer || board[y][x].type == Dame))
        ++special;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      if (None != board[y][x].type && board[y][x].is_white == _white_turn)
      {
        switch (board[y][x].type)
        {
        case None:
          break;
        case Turm:
          score += 500;
          score += rook[(_white_turn) ? y : 7 - y][x];
          break;
        case Springer:
          score += 300;
          score += knight[(_white_turn) ? y : 7 - y][x];
          break;
        case Laeufer:
          score += 300;
          score += bishop[(_white_turn) ? y : 7 - y][x];
          break;
        case Koenig:
          score += INTINF;
          if (special <= 1)
            score += king_endgame[(_white_turn) ? y : 7 - y][x];
          else
            score += king_game[(_white_turn) ? y : 7 - y][x];
          break;
        case Dame:
          score += 900;
          score += queen[(_white_turn) ? y : 7 - y][x];
          break;
        case Bauer:
          score += 100;
          score += pawn[(_white_turn) ? y : 7 - y][x];
          break;
        }
      }
  return score;
}
#pragma endregion

#pragma region KI
static int counter = 0;
typedef pair<pair<int, int>, pair<int, int>> piiii;
pair<long long, piiii> minimax(const Board &node, int depth, bool maximizingPlayer = true)
{
  ++counter;
  if (depth == 0) // || node is a terminal node) -> matt / patt
  {
    bool zug = node.bool_white_turn; //
    return {node.getScore(zug) - node.getScore(!zug), {{0, 0}, {0, 0}}};
  }
  if (maximizingPlayer)
  {
    long long value = -1e9;
    piiii result;
    for (int y = 0; y < 8; ++y)
      for (int x = 0; x < 8; ++x)
        for (auto child : node.get_all_moves(x, y, node.bool_white_turn))
        {
          Board f = node.clone();
          f.move(x, y, child.first, child.second);
          auto k = minimax(f, depth - 1, false);
          if (k.first > value)
          {
            value = k.first;
            result = {{x, y}, child};
          }
        }
    return {value, result};
  }
  else
  {
    long long value = 1e9;
    piiii result;
    for (int y = 0; y < 8; ++y)
      for (int x = 0; x < 8; ++x)
        for (auto child : node.get_all_moves(x, y, node.bool_white_turn))
        {
          Board f = node.clone();
          f.move(x, y, child.first, child.second);
          auto k = minimax(f, depth - 1, true);
          if (k.first < value)
          {
            value = k.first;
            result = {{x, y}, child};
          }
        }
    return {value, result};
  }
}
#pragma endregion

int main()
{
  Board b(true);
  bool state = false;
  while (true)
  {
    string s;
    if (getline(cin, s))
    {
      cout << "#" << s << endl;
      if (s == "xboard")
        cout << "\n";
      else if (s == "protover 2")
        cout << "feature usermove=1 done=1\n";
      else if (s == "new")
        b = Board(true);
      else if (s == "quit")
        return 0;
      else if (s.rfind("usermove", 0) == 0)
      {
        bool success = b.move((int)(s[9] - 'a'), 7 - (int)(s[10] - '1'), (int)(s[11] - 'a'), 7 - (int)(s[12] - '1'));

        // auto result = ki(b);
        // pair<pair<int, int>, pair<int, int>> result;
        // ki2(b, result);
        if (state)
        {
          auto res1 = minimax(b, 4);
          piiii result = res1.second;
          // cout << "# " << success << ", score: " << res1.first << "\n";
          cout << "# score: " << res1.first << ", counter: " << counter << "\n";
          b.move(result.first.first, result.first.second, result.second.first, result.second.second);
          int x1, y1, x2, y2;
          tie(x1, y1) = result.first;
          tie(x2, y2) = result.second;

          cout << "move " << (char)('a' + (x1)) << (char)('1' + 7 - y1) << (char)('a' + (x2)) << char('1' + 7 - y2) << "\n";
          cout << "info depth 5 seldepth 5 multipv 1 score cp 54 nodes 1664 nps 151272 tbhits 0 time 11 pv e2e4 d7d5 e4e5 e7e6 d2d4" << endl;
        }
        else
        {
          cout << "# NIX" << endl;
        }
      }
      else if (s == "force")
      {
        cout << "# SET FALSE " << endl;
        state = false;
      }
      else if (s == "go")
      {
        cout << "# SET TRUE " << endl;
        state = true;
        // int x1, y1, x2, y2;
        // x1 = 4;
        // y1 = 6;
        // x2 = 4;
        // y2 = 4;
        // b.move(x1, y1, x2, y2);
        // cout << "move " << (char)('a' + (x1)) << (char)('1' + 7 - y1) << (char)('a' + (x2)) << char('1' + 7 - y2) << "\n";
        {
          auto res1 = minimax(b, 5);
          piiii result = res1.second;
          cout << "# "
               << "1"
               << ", score: " << res1.first << "\n";
          b.move(result.first.first, result.first.second, result.second.first, result.second.second);
          int x1, y1, x2, y2;
          tie(x1, y1) = result.first;
          tie(x2, y2) = result.second;

          cout << "move " << (char)('a' + (x1)) << (char)('1' + 7 - y1) << (char)('a' + (x2)) << char('1' + 7 - y2) << "\n";
        }
      }

      cout << flush;
    }
  }
}
#endif

#pragma region garbage
// vector<unsigned long> k1(LIMIT);
// void steptest(Board &b, int n = 0)
// {
//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       for (auto item : b.get_all_moves_speed(x, y, b.is_white_turn()))
//       {
//         b.move_unsafe(x, y, item.first, item.second);
//         if (n < LIMIT - 1)
//           steptest(b, n + 1);
//         b.undoMove();
//         ++k1[n];
//       }
// }

// // #define SINGLE
// #ifdef SINGLE
//   auto start = std::chrono::system_clock::now();
//   steptest(b);
//   auto end = std::chrono::system_clock::now();
//   for (int i = 0; i < LIMIT; ++i)
//     cout << (i + 1) << ": " << k1[i] << endl;
//   cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
// #else
//   auto start = std::chrono::system_clock::now();
//   auto res = steptest2(b);
//   auto end = std::chrono::system_clock::now();
//   for (int i = 0; i < LIMIT; ++i)
//     cout << (i + 1) << ": " << res[i] << endl;
//   cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
// #endif

// vector<pair<int, int>> Board::get_all_moves_fast(int x1, int y1, bool _white_turn) const
// {
//   bool needDetail = false, needDetailKNOW = false;
//   vector<pair<int, int>> result;
//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       if (is_possible_without_chess(x1, y1, x, y, _white_turn))
//       {
//         if (!needDetailKNOW)
//         {
//           Board my_clone = clone();
//           if (Koenig != my_clone.board[y1][x1].type)
//           {
//             my_clone.board[y1][x1] = Figur(None);

//             int x_king, y_king;
//             if (_white_turn)
//               x_king = my_clone.pos_x_king2, y_king = my_clone.pos_y_king2;
//             else
//               x_king = my_clone.pos_x_king1, y_king = my_clone.pos_y_king1;

//             for (int ys = 0; ys < 8 && !needDetail; ++ys)
//               for (int xs = 0; xs < 8; ++xs)
//                 if (my_clone.is_possible_without_chess(xs, ys, x_king, y_king, !_white_turn))
//                 {
//                   needDetail = true;
//                   break;
//                 }
//           }
//           else
//             needDetail = true;
//           needDetailKNOW = true;
//         }

//         bool success = true;
//         if (needDetail)
//         {
//           Board my_clone = clone();
//           my_clone.move_unsafe(x1, y1, x, y);

//           int x_king, y_king;
//           if (_white_turn)
//             x_king = my_clone.pos_x_king2, y_king = my_clone.pos_y_king2;
//           else
//             x_king = my_clone.pos_x_king1, y_king = my_clone.pos_y_king1;

//           for (int ys = 0; ys < 8 && success; ++ys)
//             for (int xs = 0; xs < 8; ++xs)
//               if (my_clone.is_possible_without_chess(xs, ys, x_king, y_king, !_white_turn))
//               {
//                 success = false;
//                 break;
//               }
//         }
//         if (success)
//           result.push_back({x, y});
//       }
//   return result;
// }

// Board Board::clone() const
// {
//   Board new_board(bool_white_turn);
//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       new_board.board[y][x] = board[y][x].clone();
//   new_board.pos_x_king1 = pos_x_king1;
//   new_board.from_x = from_x;
//   new_board.from_y = from_y;
//   new_board.to_x = to_x;
//   new_board.to_y = to_y;
//   new_board.pos_x_king2 = pos_x_king2;
//   new_board.pos_y_king1 = pos_y_king1;
//   new_board.pos_y_king2 = pos_y_king2;
//   return new_board;
// }

// Board::Board(string boardLayout, bool starting_white_turn) : bool_white_turn(starting_white_turn) {}

// vector<pair<int, int>> Board::get_all_moves(int x1, int y1, bool _white_turn)
// {
//   vector<pair<int, int>> result;
//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       if (is_possible(x1, y1, x, y, _white_turn))
//         result.push_back({x, y});
//   return result;
// }
#pragma endregion

// g++ -Wall -Wextra -Wconversion -Wno-unknown-pragmas -Wmaybe-uninitialized -fsanitize=undefined,address -D_GLIBCXX_DEBUG -O2 -g

// 1: 20
// 2: 400
// 3: 8902
// 4: 197281
// 5: 4865609
// 6: 119060324
// 11616 ms ~ 12s

// 1: 20
// 2: 400
// 3: 8902
// 4: 197281
// 5: 4865609
// 6: 119060324
// 7: 3195901860
// 408347 ms ~ 6min 48s
