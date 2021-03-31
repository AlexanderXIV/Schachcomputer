// #include <bits/stdc++.h>
#include "header.hpp"
#include <chrono>
#include <array>
using namespace std;

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
  bool already_moved;

  Figur() = default;
  explicit Figur(ChessFigure _type, bool _is_white = true, bool _already_moved = false) : type(_type), is_white(_is_white), already_moved(_already_moved){};

  Figur clone() const
  {
    return Figur(type, is_white, already_moved);
  }

  friend ostream &operator<<(ostream &os, const Figur &figur);
};

ostream &operator<<(ostream &os, const Figur &figur)
{
  switch (figur.type)
  {
  case None:
    os << "-";
    break;
  case Turm:
    os << ((figur.is_white) ? "♖" : "♜");
    break;
  case Springer:
    os << ((figur.is_white) ? "♘" : "♞");
    break;
  case Laeufer:
    os << ((figur.is_white) ? "♗" : "♝");
    break;
  case Koenig:
    os << ((figur.is_white) ? "♔" : "♚");
    break;
  case Dame:
    os << ((figur.is_white) ? "♕" : "♛");
    break;
  case Bauer:
    os << ((figur.is_white) ? "♙" : "♟");
    break;
  }
  return os;
}

bool operator==(const Figur &a, const Figur &b)
{
  if (a.type == b.type)
  {
    if (None == b.type)
      return true;
    else
      return a.is_white == b.is_white;
  }
  return false;
}

bool operator!=(const Figur &a, const Figur &b)
{
  return !(a == b);
}

// Rochade, Sonderregel
class Board
{
private:
public:
  bool bool_white_turn;
  array<array<Figur, 8>, 8> board; // board[y][x]
  int pos_x_king1 = 4, pos_y_king1 = 0;
  int pos_x_king2 = 4, pos_y_king2 = 7;
  int from_x = -1, from_y = -1, to_x = -1, to_y = -1;

  explicit Board(bool starting_white_turn);

  int getScore(bool _white_turn) const;
  pair<int, int> get_pos(Figur figur) const;
  vector<pair<int, int>> get_all_moves(int x1, int y1, bool _white_turn) const;
  bool is_possible(int x1, int y1, int x2, int y2, bool _white_turn) const;
  bool is_possible_without_chess(int x1, int y1, int x2, int y2, bool _white_turn) const;
  Board clone() const;

  bool move(int x1, int y1, int x2, int y2);
  void move_unsafe(int x1, int y1, int x2, int y2);

  void undoMove() = delete;
  bool is_white_turn() const = delete;
  bool is_checkmate(bool _white_turn) const = delete;
  bool is_chess(int y, int x, bool _white_turn) const = delete;
  bool is_chess(bool _white_turn) const = delete;
  bool is_valid(bool _white_turn) const = delete;

  // void operator&() = delete;
  // void operator&&() = delete;

  friend ostream &operator<<(ostream &os, const Board &board);
};

ostream &operator<<(ostream &os, const Board &chess)
{
  os << "Schachbrett:\n";
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

Board::Board(bool starting_white_turn) : bool_white_turn(starting_white_turn),
                                         board{{{Figur(Turm, false), Figur(Springer, false), Figur(Laeufer, false), Figur(Dame, false), Figur(Koenig, false), Figur(Laeufer, false), Figur(Springer, false), Figur(Turm, false)},
                                                {Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false), Figur(Bauer, false)},
                                                {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                                {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                                {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                                {Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None), Figur(None)},
                                                {Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer), Figur(Bauer)},
                                                {Figur(Turm), Figur(Springer), Figur(Laeufer), Figur(Dame), Figur(Koenig), Figur(Laeufer), Figur(Springer), Figur(Turm)}}} {}

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

vector<pair<int, int>> Board::get_all_moves(int x1, int y1, bool _white_turn) const
{
  vector<pair<int, int>> result;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      if (is_possible(x1, y1, x, y, _white_turn))
        result.push_back({x, y});
  return result;
}

// als parameter board !!!!
bool Board::is_possible(int x1, int y1, int x2, int y2, bool _white_turn) const
{
  if (is_possible_without_chess(x1, y1, x2, y2, _white_turn))
  {
    Board my_clone = clone();
    my_clone.move_unsafe(x1, y1, x2, y2);

    int x_king, y_king;
    if (_white_turn)
      x_king = my_clone.pos_x_king2, y_king = my_clone.pos_y_king2;
    else
      x_king = my_clone.pos_x_king1, y_king = my_clone.pos_y_king1;

    for (int y = 0; y < 8; ++y)
      for (int x = 0; x < 8; ++x)
        if (my_clone.is_possible_without_chess(x, y, x_king, y_king, !_white_turn))
          return false;
    return true;
  }
  return false;
}

bool Board::is_possible_without_chess(int x1, int y1, int x2, int y2, bool _white_turn) const
{
  if ((0 == x2 - x1 && 0 == y2 - y1) || !((0 <= x1 && x1 < 8) || (0 <= y1 && y1 < 8) || (0 <= x2 && x2 < 8) || (0 <= y2 && y2 < 8)))
    return false;

  Figur src = board[y1][x1], dest = board[y2][x2];
  if (None == src.type || _white_turn != src.is_white || (None != dest.type && src.is_white == dest.is_white))
    return false;

  if (Turm == src.type)
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
  else if (Springer == src.type)
  {
    int dif_x = abs(x2 - x1);
    int dif_y = abs(y2 - y1);

    return (dif_x == 2 && dif_y == 1) || (dif_x == 1 && dif_y == 2);
  }
  else if (Laeufer == src.type)
  {
    if (x1 > x2)
    {
      swap(x1, x2);
      swap(y1, y2);
    }

    int dif_x = x2 - x1;
    int dif_y = y2 - y1;

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
  else if (Dame == src.type)
  {
    if (x1 > x2)
    {
      swap(x1, x2);
      swap(y1, y2);
    }

    int dif_x = x2 - x1;
    int dif_y = y2 - y1;

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
  else if (Bauer == src.type)
  {
    int dif_x = x2 - x1;
    int dif_y = y2 - y1;

    int first = 1;
    if (src.is_white)
    {
      dif_y = -dif_y;
      first = 7 - first;
    }

    if (dif_y == 1)
    {
      if (-1 == dif_x || 1 == dif_x)
        return None != dest.type || (board[y1][x1 + dif_x] == Figur(Bauer, !_white_turn) && to_x == from_x && from_x == x1 + dif_x && to_y == y1 && abs(from_y - to_y) == 2);
      else if (0 == dif_x && None == dest.type)
        return true;
    }

    if (dif_y == 2)
      if (0 == dif_x && y1 == first && None == dest.type)
        return None == board[y1 + (y2 - y1) / 2][x1].type;

    return false;
  }
  else if (Koenig == src.type)
    return !(abs(x2 - x1) > 1 || abs(y2 - y1) > 1);
  return false;
}

bool Board::move(int x1, int y1, int x2, int y2)
{
  if (y1 == y2 && ((x1 == 4 && x2 == 2) || (x1 == 4 && x2 == 6)) && (0 == y1 || 7 == y2))
  {
    move_unsafe(x1, y1, x2, y2);
    move_unsafe(7, y1, 5, y2);
    bool_white_turn = !bool_white_turn;
  }

  if (Bauer == board[y1][x1].type && ((y2 == 0) || y2 == 7))
  {
    board[y2][x2] = Figur(Dame, bool_white_turn);
    // "q" fehlt
  }

  bool is_pos = is_possible(x1, y1, x2, y2, bool_white_turn);
  if (is_pos)
    move_unsafe(x1, y1, x2, y2);

  return is_pos;
}

void Board::move_unsafe(int x1, int y1, int x2, int y2)
{
  bool_white_turn = !bool_white_turn;

  board[y2][x2] = board[y1][x1];
  board[y2][x2].already_moved = true;
  board[y1][x1] = Figur(None);

  if (Koenig == board[y2][x2].type)
  {
    if (board[y2][x2].is_white)
      pos_x_king2 = x2, pos_y_king2 = y2;
    else
      pos_x_king1 = x2, pos_y_king1 = y2;
  }
  from_x = x1, from_y = y1, to_x = x2, to_y = y2;
}

Board Board::clone() const
{
  Board new_board(bool_white_turn);
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      new_board.board[y][x] = board[y][x].clone();
  new_board.pos_x_king1 = pos_x_king1;
  new_board.from_x = from_x;
  new_board.from_y = from_y;
  new_board.to_x = to_x;
  new_board.to_y = to_y;
  new_board.pos_x_king2 = pos_x_king2;
  new_board.pos_y_king1 = pos_y_king1;
  new_board.pos_y_king2 = pos_y_king2;
  return new_board;
}
#pragma endregion

#pragma region Test
static constexpr int LIMIT = 4;
static int k1[LIMIT];
void steptest(const Board &b, int n = 0)
{
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto item : b.get_all_moves(x, y, b.bool_white_turn))
      {
        Board f = b.clone();
        f.move_unsafe(x, y, item.first, item.second);
        if (n < LIMIT - 1)
          steptest(f, n + 1);
        ++k1[n];
      }
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

// int main()
// {
//   Board b(true);
//   bool state = false;
//   while (true)
//   {
//     string s;
//     if (getline(cin, s))
//     {
//       cout << "#" << s << endl;
//       if (s == "xboard")
//         cout << "\n";
//       else if (s == "protover 2")
//         cout << "feature usermove=1 done=1\n";
//       else if (s == "new")
//         b = Board(true);
//       else if (s == "quit")
//         return 0;
//       else if (s.rfind("usermove", 0) == 0)
//       {
//         bool success = b.move((int)(s[9] - 'a'), 7 - (int)(s[10] - '1'), (int)(s[11] - 'a'), 7 - (int)(s[12] - '1'));

//         // auto result = ki(b);
//         // pair<pair<int, int>, pair<int, int>> result;
//         // ki2(b, result);
//         if (state)
//         {
//           auto res1 = minimax(b, 4);
//           piiii result = res1.second;
//           // cout << "# " << success << ", score: " << res1.first << "\n";
//           cout << "# score: " << res1.first << ", counter: " << counter << "\n";
//           b.move(result.first.first, result.first.second, result.second.first, result.second.second);
//           int x1, y1, x2, y2;
//           tie(x1, y1) = result.first;
//           tie(x2, y2) = result.second;

//           cout << "move " << (char)('a' + (x1)) << (char)('1' + 7 - y1) << (char)('a' + (x2)) << char('1' + 7 - y2) << "\n";
//           cout << "info depth 5 seldepth 5 multipv 1 score cp 54 nodes 1664 nps 151272 tbhits 0 time 11 pv e2e4 d7d5 e4e5 e7e6 d2d4" << endl;
//         }
//         else
//         {
//           cout << "# NIX" << endl;
//         }
//       }
//       else if (s == "force")
//       {
//         cout << "# SET FALSE " << endl;
//         state = false;
//       }
//       else if (s == "go")
//       {
//         cout << "# SET TRUE " << endl;
//         state = true;
//         // int x1, y1, x2, y2;
//         // x1 = 4;
//         // y1 = 6;
//         // x2 = 4;
//         // y2 = 4;
//         // b.move(x1, y1, x2, y2);
//         // cout << "move " << (char)('a' + (x1)) << (char)('1' + 7 - y1) << (char)('a' + (x2)) << char('1' + 7 - y2) << "\n";
//         {
//           auto res1 = minimax(b, 5);
//           piiii result = res1.second;
//           cout << "# "
//                << "1"
//                << ", score: " << res1.first << "\n";
//           b.move(result.first.first, result.first.second, result.second.first, result.second.second);
//           int x1, y1, x2, y2;
//           tie(x1, y1) = result.first;
//           tie(x2, y2) = result.second;

//           cout << "move " << (char)('a' + (x1)) << (char)('1' + 7 - y1) << (char)('a' + (x2)) << char('1' + 7 - y2) << "\n";
//         }
//       }

//       cout << flush;
//     }
//   }
// }

int main()
{
  // cout << "start" << endl;
  Board b(false);
  // cout << b.getScore(true) << endl;
  // cout << b.getScore(false) << endl;

  auto start = std::chrono::system_clock::now();
  steptest(b);
  auto end = std::chrono::system_clock::now();
  cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

  for (int i = 0; i < LIMIT; ++i)
    cout << (i + 1) << ": " << k1[i] << endl;

  // cout << "end" << endl;
}
