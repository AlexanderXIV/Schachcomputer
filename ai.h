#pragma once
#include "Figur.h"
#include "Board.h"

typedef pair<pair<int, int>, pair<int, int>> piiii;

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

int Board::getScore(bool _white_turn) const
{
  // Both sides have no queens or every side which has a queen has additionally no other pieces or one minorpiece maximum.
  constexpr int INTINF = 1e7;
  // int special = 0;
  int score = 0;
  // node.getScore(node.is_white_turn()) - node.getScore(!node.is_white_turn())
  // for (int y = 0; y < 8; ++y)
  //   for (int x = 0; x < 8; ++x)
  //     if (None != board[y][x].type && board[y][x].is_white == _white_turn && (board[y][x].type == Turm || board[y][x].type == Springer || board[y][x].type == Laeufer || board[y][x].type == Dame))
  //       ++special;
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
          temp += INTINF;
          // if (special <= 1)
          //   temp += king_endgame[y1][x];
          // else
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

#pragma region MiniMax
//   if (depth == 0) // || node is a terminal node) -> matt / patt
// path[depth - 1] = result;

// pair<pair<int, long long>, piiii> minimax_maxi(Board node, int depth);

// pair<pair<int, long long>, piiii> minimax_mini(Board node, int depth) // CALL BY REFERENCE !!!!!
// {
//   long long value = 1e9;
//   int searched = 0;
//   piiii result;
//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
//       {
//         node.move_unsafe(x, y, child.first, child.second);
//         if (1 == depth)
//         {
//           ++searched;
//           const long long val = node.getScore(node.is_white_turn());
//           if (val < value)
//             value = val, result = {{0, 0}, {0, 0}};
//         }
//         else
//         {
//           auto k = minimax_maxi(node, depth - 1);
//           searched += k.first.first;
//           if (k.first.second < value)
//             value = k.first.second, result = {{x, y}, child};
//         }
//         node.undoMove();
//       }
//   return {{searched, value}, result};
// }

// pair<pair<int, long long>, piiii> minimax_maxi(Board node, int depth) // CALL BY REFERENCE !!!!!
// {
//   long long value = -1e9;
//   int searched = 0;
//   piiii result;
//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
//       {
//         node.move_unsafe(x, y, child.first, child.second);
//         if (1 == depth)
//         {
//           ++searched;
//           const long long val = node.getScore(node.is_white_turn());
//           if (val > value)
//             value = val, result = {{0, 0}, {0, 0}};
//         }
//         else
//         {
//           auto k = minimax_mini(node, depth - 1);
//           searched += k.first.first;
//           if (k.first.second > value)
//             value = k.first.second, result = {{x, y}, child};
//         }
//         node.undoMove();
//       }
//   return {{searched, value}, result};
// }

// pair<pair<int, long long>, piiii> minimax(Board node, int depth) // CALL BY REFERENCE !!!!!
// {
//   stack<pair<piiii, future<pair<pair<int, long long>, piiii>>>> stck;

//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
//       {
//         node.move_unsafe(x, y, child.first, child.second);
//         stck.push(make_pair(make_pair(make_pair(x, y), make_pair(child.first, child.second)), async(minimax_mini, node, depth - 1)));
//         node.undoMove();
//       }

//   long long value = -1e9;
//   int searched = 0;
//   piiii result;
//   while (!stck.empty())
//   {
//     auto k = stck.top().second.get().first;
//     searched += k.first;
//     if (k.second >= value)
//       value = k.second, result = stck.top().first;
//     stck.pop();
//   }
//   return {{searched, value}, result};
// }

pair<long long, long long> minimax_maxi(Board node, int depth);

pair<long long, long long> minimax_mini(Board node, int depth) // CALL BY REFERENCE !!!!!
{
  long long value = 1e9, searched = 0;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
      {
        node.move_unsafe(x, y, child.first, child.second);
        if (1 == depth)
        {
          ++searched;
          const long long val = node.getScore(node.is_white_turn());
          if (val < value)
            value = val;
        }
        else // if (node.getScore(node.is_white_turn()) > break_val)
        {
          auto k = minimax_maxi(node, depth - 1);
          searched += k.first;
          if (k.second < value)
            value = k.second;
        }
        node.undoMove();
      }
  return {searched, value};
}

pair<long long, long long> minimax_maxi(Board node, int depth) // CALL BY REFERENCE !!!!!
{
  long long value = -1e9, searched = 0;
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
      {
        node.move_unsafe(x, y, child.first, child.second);
        if (1 == depth)
        {
          ++searched;
          const long long val = node.getScore(node.is_white_turn());
          if (val > value)
            value = val;
        }
        else // if (node.getScore(node.is_white_turn()) < break_val)
        {
          auto k = minimax_mini(node, depth - 1);
          searched += k.first;
          if (k.second > value)
            value = k.second;
        }
        node.undoMove();
      }
  return {searched, value};
}

pair<pair<long long, long long>, piiii> minimax(Board node, int depth) // CALL BY REFERENCE !!!!!
{
  queue<pair<piiii, future<pair<long long, long long>>>> stck;

  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto child : node.get_all_moves_speed(x, y, node.is_white_turn()))
      {
        node.move_unsafe(x, y, child.first, child.second);
        stck.push(make_pair(make_pair(make_pair(x, y), make_pair(child.first, child.second)), async(minimax_mini, node, depth - 1)));
        node.undoMove();
      }

  long long value = -1e9, searched = 0;
  piiii result;
  while (!stck.empty())
  {
    auto k = stck.front().second.get();
    searched += k.first;
    if (k.second > value)
      value = k.second, result = stck.front().first;
    stck.pop();
  }
  return {{searched, value}, result};
}
#pragma endregion

#pragma region alpha_beta
// int alphabeta(Board node, int depth, int α = -INF, int β = INF, bool maximizingPlayer = true)
// {
//   if (0 == depth) // || "node is a terminal node"
//     return node.getScore(node.is_white_turn());
//   if (maximizingPlayer)
//   {
//     int value = -INF;
//     for (auto child : "each child of node do")
//     {
//       value = max(value, alphabeta(child, depth - 1, α, β, false));
//       if (value >= β)
//         break; // (* β cutoff *)
//       α = max(α, value);
//     }
//     return value;
//   }
//   else
//   {
//     int value = INF;
//     for (auto child : "each child of node do")
//     {
//       value = min(value, alphabeta(child, depth - 1, α, β, true));
//       if (value <= α)
//         break; // (* α cutoff *)
//       β = min(β, value);
//     }
//     return value;
//   }
// }

constexpr int INF = 1e9;

pair<long long, int> minimax_alpha_beta(Board &root, piiii &best, const int max_depth, int depth = 0, bool maximizingPlayer = true, int alpha = -INF, int beta = INF)
{
  if (depth >= max_depth)
    return {1, root.getScore(root.is_white_turn())};
  else if (maximizingPlayer)
  {
    long long searched = 0;
    for (int y = 0; y < 8; ++y)
      for (int x = 0; x < 8; ++x)
        for (auto child : root.get_all_moves_speed(x, y, root.is_white_turn()))
        {
          root.move_unsafe(x, y, child.first, child.second);
          auto k = minimax_alpha_beta(root, best, max_depth, depth + 1, false, alpha, beta); // max(value, ...) ???
          searched += k.first;
          if (k.second > alpha)
          {
            if (0 == depth)
              best = {{x, y}, child};
            alpha = k.second;
          }
          root.undoMove();
          if (alpha >= beta)
            return {searched, alpha};
        }
    if (0 == searched)
      return {1, root.getScore(root.is_white_turn())};
    return {searched, alpha};
  }
  else
  {
    long long searched = 0;
    for (int y = 0; y < 8; ++y)
      for (int x = 0; x < 8; ++x)
        for (auto child : root.get_all_moves_speed(x, y, root.is_white_turn()))
        {
          root.move_unsafe(x, y, child.first, child.second);
          auto k = minimax_alpha_beta(root, best, max_depth, depth + 1, true, alpha, beta);
          searched += k.first;
          if (k.second < beta)
          {
            if (0 == depth)
              best = {{x, y}, child};
            beta = k.second;
          }
          root.undoMove();
          if (alpha >= beta)
            return {searched, beta};
        }
    if (0 == searched)
      return {1, root.getScore(root.is_white_turn())};
    return {searched, beta};
  }
}
#pragma endregion

ostream &operator<<(ostream &os, const piiii &p)
{
  os << char('a' + (p.first.first)) << char('1' + 7 - p.first.second) << char('a' + (p.second.first)) << char('1' + 7 - p.second.second);
  return os;
}

void debug(const int current_depth, const int score, const int time, const long long searched, const vector<piiii> &path)
{
  cout << current_depth /* Integer giving current search depth */ << " "
       << score /*Integer giving current evaluation in centipawns.*/ << " "
       << (int)(time / 10) /*Current search time in centiseconds (ex:1028 = 10.28 seconds).*/ << " "
       << searched /*Nodes searched.*/ << " "
       << "[";
  for (int i = path.size() - 1; i >= 0; --i)
    cout << path[i] << ((i == 0) ? "" : ", ");
  cout << "]" << endl;
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
          pair<long long, long long> kk;
          int current_depth = 6;
          long long score;
          piiii result;
          vector<piiii> path;

          // while (time < 75)
          {
            path = vector<piiii>(current_depth);
            auto start = chrono::system_clock::now();

            {
                // tie(kk, result) = minimax(b, current_depth);
                // tie(searched, score) = kk;
            }

            {
              tie(searched, score) = minimax_alpha_beta(b, result, current_depth);
            }

            auto end = chrono::system_clock::now();
            time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            // current_depth += 2;

            debug(current_depth, score, time, searched, path);
          }

          b.move(result.first.first, result.first.second, result.second.first, result.second.second);
          debug(current_depth, score, time, searched, path);
        }
      }
      else if (s == "force")
        state = false;

      cout << flush;
    }
  }
}