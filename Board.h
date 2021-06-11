#pragma once
#include "Figur.h"

class Board
{
private:
  stack<tuple<int, int, int, int, int, int, bool, Figur, bool, bool, bool, bool>> history;
  array<array<Figur, 8>, 8> board; // board[y][x]
  bool bool_white_turn, doubleMove;
  pair<int, int> pos_king_white, pos_king_black, epts; // en_passant_target_square_x
  bool lca[2], sca[2];                                 // large castle_allowed {black, white}

  bool check(int x1, int y1, int x2, int y2, bool _white_turn);
  bool is_chess(int x1, int y1, bool _white_turn) const;
  bool is_possible_without_chess(int x1, int y1, int x2, int y2, bool _white_turn) const;

public:
  explicit Board(bool starting_white_turn);
  explicit Board(string fen);

  vector<pair<int, int>> get_all_moves_speed(int x1, int y1, bool _white_turn);
  void move(int x1, int y1, int x2, int y2);
  void move_unsafe(int x1, int y1, int x2, int y2);
  void undoMove();

  int getScore(bool _white_turn) const;
  bool is_white_turn() { return bool_white_turn; };

  friend ostream &operator<<(ostream &os, const Board &chess);
};

ostream &operator<<(ostream &os, const Board &chess)
{
  for (int y = 0; y < 8; ++y)
  {
    bool first = true;
    for (Figur f : chess.board[y])
    {
      if (!first)
        os << " ";
      else
        first = false;
      os << f << " ";
    }
    os << "\n";
  }
  os << "\n";
  return os;
}

Board::Board(bool white_starting) : board{{{Figur(Turm, false), Figur(Springer, false), Figur(Laeufer, false), Figur(Dame, false), Figur(Koenig, false), Figur(Laeufer, false), Figur(Springer, false), Figur(Turm, false)},
                                           {Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false)},
                                           {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                           {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                           {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                           {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                           {Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer)},
                                           {Figur(Turm), Figur(Springer), Figur(Laeufer), Figur(Dame), Figur(Koenig), Figur(Laeufer), Figur(Springer), Figur(Turm)}}},
                                    bool_white_turn(white_starting),
                                    doubleMove(false),
                                    pos_king_white(make_pair(4, 7)),
                                    pos_king_black(make_pair(4, 0)),
                                    epts(make_pair(-1, -1)),
                                    lca{true, true},
                                    sca{true, true}
{
}

Board::Board(string fen) : board{{{Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                  {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                  {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                  {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                  {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                  {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                  {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                  {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)}}},
                           bool_white_turn(true),
                           doubleMove(false),
                           pos_king_white(make_pair(4, 7)),
                           pos_king_black(make_pair(4, 0)),
                           epts(make_pair(-1, -1)),
                           lca{false, false},
                           sca{false, false}
{
  size_t pos = 0;
  {
    int py = 0, px = 0;
    for (; pos < fen.size(); ++pos)
    {
      switch (fen[pos])
      {
      case 'p':
        board[py][px++] = Figur(Bauer, false);
        break;
      case 'n':
        board[py][px++] = Figur(Springer, false);
        break;
      case 'b':
        board[py][px++] = Figur(Laeufer, false);
        break;
      case 'r':
        board[py][px++] = Figur(Turm, false);
        break;
      case 'q':
        board[py][px++] = Figur(Dame, false);
        break;
      case 'k':
        pos_king_black = make_pair(px, py);
        board[py][px++] = Figur(Koenig, false);
        break;
      case 'P':
        board[py][px++] = Figur(Bauer);
        break;
      case 'N':
        board[py][px++] = Figur(Springer);
        break;
      case 'B':
        board[py][px++] = Figur(Laeufer);
        break;
      case 'R':
        board[py][px++] = Figur(Turm);
        break;
      case 'Q':
        board[py][px++] = Figur(Dame);
        break;
      case 'K':
        pos_king_white = make_pair(px, py);
        board[py][px++] = Figur(Koenig);
        break;
      case '/':
        assertm(8 == px, "error");
        px = 0;
        ++py;
        break;
      case ' ':
        break;
      default:
        int num = fen[pos] - '0';
        assertm(1 <= num && num <= 8, "error");
        px += num;
        break;
      }

      if (' ' == fen[pos])
        break;
    }
    assertm(' ' == fen[pos], "err");
    ++pos;
  }

  {
    if ('w' == fen[pos])
      bool_white_turn = true;
    else
    {
      assertm('b' == fen[pos], "error");
      bool_white_turn = false;
    }
    pos += 2;
  }

  {
    for (; pos < fen.size(); ++pos)
    {
      switch (fen[pos])
      {
      case 'k':
        sca[1] = true;
        break;
      case 'q':
        lca[1] = true;
        break;
      case 'K':
        sca[0] = true;
        break;
      case 'Q':
        lca[0] = true;
        break;
      case '-':
        break;
      case ' ':
        break;
      }

      if (' ' == fen[pos])
        break;
    }
    assertm(' ' == fen[pos], "err");
    ++pos;
  }

  if ('-' == fen[pos])
    pos += 2;
  else
  {
    int x1, y1;
    x1 = fen[pos++] - 'a';
    y1 = 8 - (fen[pos++] - '0');
    doubleMove = true;
    assertm(0 <= x1 && x1 <= 7, "error");
    assertm(0 <= y1 && y1 <= 7, "error");
    epts = {x1, y1};
  }
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
  case Koenig:
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
      if (0 <= x && 0 <= y && x < 8 && y < 8 && ((None != board[y][x].type && board[y][x].is_white != _white_turn) || (None == board[y][x].type && doubleMove && board[y1][x] == Figur(Bauer, !_white_turn) && epts.first == x && epts.second == y)) && check(x1, y1, x, y, _white_turn))
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
        return None != board[y2][x2].type || (doubleMove && board[y1][x1 + dif_x] == Figur(Bauer, !_white_turn) && epts.first == x2 && epts.second == y2);
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
  history.push(make_tuple(x1, y1, x2, y2, epts.first, epts.second, doubleMove, board[y2][x2], lca[0], lca[1], sca[0], sca[1]));

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
      pos_king_white = make_pair(x2, y2);
    else
      pos_king_black = make_pair(x2, y2);
  }
  epts = {x2, (y1 < y2) ? y1 + 1 : y1 - 1};
}

void Board::undoMove()
{
  int _x1, _y1, _x2, _y2, _epts_x, _epts_y;
  Figur f(None);
  bool b1, b2, b3, b4;
  tie(_x1, _y1, _x2, _y2, _epts_x, _epts_y, doubleMove, f, b1, b2, b3, b4) = history.top();
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
      pos_king_white = make_pair(_x1, _y1);
    else
      pos_king_black = make_pair(_x1, _y1);
  }

  board[_y1][_x1] = board[_y2][_x2];
  board[_y2][_x2] = f;

  epts = {_epts_x, _epts_y};
}

bool Board::check(int x1, int y1, int x2, int y2, bool _white_turn)
{
  move_unsafe(x1, y1, x2, y2);

  const pair<int, int> king = (_white_turn) ? pos_king_white : pos_king_black;
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

void Board::move(const int x1, const int y1, const int x2, const int y2)
{
  cout << "move " << char('a' + x1) << char('1' + 7 - y1) << char('a' + x2) << char('1' + 7 - y2);
  if (Bauer == board[y1][x1].type && ((y2 == 0) || y2 == 7))
  {
    board[y2][x2] = Figur(Dame, bool_white_turn);
    cout << 'q';
  }
  cout << '\n';
  move_unsafe(x1, y1, x2, y2);
}