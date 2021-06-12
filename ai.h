#pragma once
#include "Figur.h"
#include "Board.h"

typedef pair<pair<int, int>, pair<int, int>> piiii;
typedef array<array<int, 8>, 8> arr2d;

constexpr int INF = 1e7;
#pragma region getScore
arr2d pawn{{{0, 0, 0, 0, 0, 0, 0, 0},
            {50, 50, 50, 50, 50, 50, 50, 50},
            {10, 10, 20, 30, 30, 20, 10, 10},
            {5, 5, 10, 25, 25, 10, 5, 5},
            {0, 0, 0, 20, 20, 0, 0, 0},
            {5, -5, -10, 0, 0, -10, -5, 5},
            {5, 10, 10, -20, -20, 10, 10, 5},
            {0, 0, 0, 0, 0, 0, 0, 0}}};

arr2d knight{{{-50, -40, -30, -30, -30, -30, -40, -50},
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
            {0, 0, 0, 5, 5, 0, 0, 0}}};

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
                 {20, 20, 0, 0, 0, 0, 20, 20},
                 {20, 30, 10, 0, 0, 10, 30, 20}}};

arr2d king_endgame{{{-50, -40, -30, -20, -20, -30, -40, -50},
                    {-30, -20, -10, 0, 0, -10, -20, -30},
                    {-30, -10, 20, 30, 30, 20, -10, -30},
                    {-30, -10, 30, 40, 40, 30, -10, -30},
                    {-30, -10, 30, 40, 40, 30, -10, -30},
                    {-30, -10, 20, 30, 30, 20, -10, -30},
                    {-30, -30, 0, 0, 0, 0, -30, -30},
                    {-50, -30, -30, -30, -30, -30, -30, -50}}};

// Both sides have no queens or every side which has a queen has additionally no other pieces or one minorpiece maximum.
// int special = 0;
// for (int y = 0; y < 8; ++y)
//   for (int x = 0; x < 8; ++x)
//     if (None != board[y][x].type && board[y][x].is_white == _white_turn && (board[y][x].type == Turm || board[y][x].type == Springer || board[y][x].type == Laeufer || board[y][x].type == Dame))
//       ++special;

int Board::getScore(bool _white_turn, bool is_endGame) const
{
  int score = 0;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      if (None != board[y][x].type)
      {
        const int y1 = (board[y][x].is_white) ? y : 7 - y;
        int temp = 0;
        switch (board[y][x].type)
        {
        case Turm:
          temp += 500;
          temp += rook[y1][x];
          break;
        case Springer:
          temp += 300;
          temp += knight[y1][x];
          break;
        case Laeufer:
          temp += 300;
          temp += bishop[y1][x];
          break;
        case Koenig:
          temp += INF;
          if (is_endGame)
            temp += king_endgame[y1][x];
          else
            temp += king_game[y1][x];
          break;
        case Dame:
          temp += 900;
          temp += queen[y1][x];
          break;
        case Bauer:
          temp += 100;
          temp += pawn[y1][x];
          break;
        }
        if (board[y][x].is_white == _white_turn)
          score += temp;
        else
          score -= temp;
      }
  return score;
}
#pragma endregion

#pragma region alpha_beta

pair<long long, int> minimax_alpha_beta_minimize(Board &node, vector<piiii> &path, const int max_depth, int depth, int alpha, int beta);

pair<long long, int> minimax_alpha_beta_maximize(Board &node, vector<piiii> &path, const int max_depth, int depth, int alpha, int beta)
{
  if (depth >= max_depth)
    return {1, node.getScore(node.is_white_turn(), false)};

  long long searched = 0;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
      {
        node.move_unsafe(x, y, child.first, child.second);
        auto k = minimax_alpha_beta_minimize(node, path, max_depth, depth + 1, alpha, beta);
        searched += k.first;
        if (k.second > alpha)
          alpha = k.second, path[depth] = {{x, y}, child};
        node.undoMove();
        if (alpha >= beta)
          return {searched, alpha};
      }

  if (0 == searched)
    return {1, -INF};
  // return {1, node.getScore(node.is_white_turn(), false)};
  return {searched, alpha};
}

pair<long long, int> minimax_alpha_beta_minimize(Board &node, vector<piiii> &path, const int max_depth, int depth, int alpha, int beta)
{
  if (depth >= max_depth)
    return {1, node.getScore(node.is_white_turn(), false)};

  long long searched = 0;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
      {
        node.move_unsafe(x, y, child.first, child.second);
        auto k = minimax_alpha_beta_maximize(node, path, max_depth, depth + 1, alpha, beta);
        searched += k.first;
        if (k.second < beta)
          beta = k.second, path[depth] = {{x, y}, child};
        node.undoMove();
        if (alpha >= beta)
          return {searched, beta};
      }

  if (0 == searched)
    return {1, INF};
  // return {1, node.getScore(node.is_white_turn(), false)};
  return {searched, beta};
}

// pair<pair<long long, long long>, piiii> minimax_alpha_beta(Board &node, vector<piiii> &path, const int max_depth, int depth = 0, int alpha = -INF, int beta = INF)
// {
//   queue<pair<piiii, future<pair<long long, long long>>>> stck;

//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
//       {
//         node.move_unsafe(x, y, child.first, child.second);
//         stck.push(make_pair(make_pair(make_pair(x, y), make_pair(child.first, child.second)), async(minimax_alpha_beta_maximize, node, depth - 1)));
//         node.undoMove();
//       }

//   long long value = -1e9, searched = 0;
//   piiii result;
//   while (!stck.empty())
//   {
//     auto k = stck.front().second.get();
//     searched += k.first;
//     if (k.second > value)
//       value = k.second, result = stck.front().first;
//     stck.pop();
//   }
//   return {{searched, value}, result};
// }
#pragma endregion

ostream &operator<<(ostream &os, const piiii &p)
{
  os << char('a' + (p.first.first)) << char('1' + 7 - p.first.second) << char('a' + (p.second.first)) << char('1' + 7 - p.second.second);
  return os;
}

ostream &operator<<(ostream &os, const vector<piiii> &vp)
{
  os << '[';
  bool first = true;
  for (auto item : vp)
    if (first)
    {
      os << item;
      first = false;
    }
    else
      os << ", " << item;
  os << ']';
  return os;
}

void debug(const int current_depth, const int score, const int time, const long long searched, const vector<piiii> &path)
{
  cout << current_depth /* Integer giving current search depth */ << " "
       << score /*Integer giving current evaluation in centipawns.*/ << " "
       << (int)(time / 10) /*Current search time in centiseconds (ex:1028 = 10.28 seconds).*/ << " "
       << searched /*Nodes searched.*/ << " "
       << path /*planned moves.*/ << endl;
}

void ai()
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
        return;
      else if (s == "go" || s.rfind("usermove", 0) == 0)
      {
        if (s == "go")
          state = true;
        else
          b.move_unsafe((int)(s[9] - 'a'), 7 - (int)(s[10] - '1'), (int)(s[11] - 'a'), 7 - (int)(s[12] - '1'));

        if (state)
        {
          unsigned long long time = 0;
          long long searched;
          int current_depth = 6;
          long long score;
          vector<piiii> path;

          // while (time < 75)
          {
            path = vector<piiii>(current_depth);

            auto start = chrono::system_clock::now();
            tie(searched, score) = minimax_alpha_beta_maximize(b, path, current_depth, 0, -INF, INF);
            auto end = chrono::system_clock::now();
            time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            // current_depth += 2;

            debug(current_depth, score, time, searched, path);
          }

          b.move(path[0].first.first, path[0].first.second, path[0].second.first, path[0].second.second);
          debug(current_depth, score, time, searched, path);
        }
      }
      else if (s == "force")
        state = false;

      cout << flush;
    }
  }
}
