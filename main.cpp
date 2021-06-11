// partly missing: Promotions
#include <chrono>
#include <queue>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <utility>
#include <tuple>
#include <future>
#include <sstream>
#include <string>
#include <cassert>
using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))

#include "ai.h"

#pragma region Test
// void steptest_optimized(Board b, unsigned long *result, const int n = 0) // call-by-reference
// {
//   stack<pair<unsigned long *, future<void>>> stck;
//   for (int y = 0; y < 8; ++y)
//     for (int x = 0; x < 8; ++x)
//       for (auto item : b.get_all_moves_speed(x, y, b.is_white_turn()))
//       {
//         if (n < LIMIT - 1)
//         {
//           b.move_unsafe(x, y, item.first, item.second);
//           if (n == 0)
//           {
//             unsigned long *test = new unsigned long[LIMIT];
//             for (int i = 0; i < LIMIT; ++i)
//               test[i] = 0;
//             stck.push({test, async(steptest_optimized, b, test, n + 1)});
//           }
//           else
//             steptest_optimized(b, result, n + 1);
//           b.undoMove();
//         }
//         ++result[n];
//       }
//   while (!stck.empty())
//   {
//     pair<unsigned long *, future<void>> &item = stck.top();
//     item.second.get();
//     for (int i = 0; i < LIMIT; ++i)
//       result[i] += item.first[i];
//     delete[] item.first;
//     stck.pop();
//   }
// }

vector<unsigned long> steptest2(Board b, const int LIMIT, const int n = 0)
{
  stack<future<vector<unsigned long>>> stck;
  vector<unsigned long> result(LIMIT, 0);
  for (int y = 0; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      for (auto item : b.get_all_moves_speed(x, y, b.is_white_turn()))
      {
        // cout << x << ", " << y << " | " << item.first << ", " << item.second << endl;
        if (n < LIMIT - 1)
        {
          b.move_unsafe(x, y, item.first, item.second);
          if (n == 1)
            stck.push(async(steptest2, b, LIMIT, n + 1));
          else
          {
            int c1 = 0;
            for (auto item : steptest2(b, LIMIT, n + 1))
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

// int LIMIT = 5;
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

void test()
{
  ifstream infile("validation.txt");

  string line, p1, p2, p3, p4;
  while (getline(infile, line))
  {
    istringstream iss(line);

    getline(iss, p1, ',');
    getline(iss, p2, ',');
    // getline(iss, p3, ',');
    // getline(iss, p4, ',');

    int i1 = stoi(p2);
    // int i2 = stoi(p2);
    // int i3 = stoi(p3);

    Board b(p1);
    auto res = steptest2(b, 2);
    if (res[0] != i1 && i1 < 10)
    {
      cout << "ERROR: " << res[0] << " | " << i1 << ", " << p1 << endl;
      exit(0);
    }
  }
}

// {
//   int limit = 7;
//   Board bb(false);
//   auto start = chrono::system_clock::now();
//   auto res = steptest2(bb, limit);
//   auto end = chrono::system_clock::now();
//   for (int i = 0; i < limit; ++i)
//     cout << res[i] << "\n";
//   cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
// }
// exit(0);

// int main(int argc, char **argv)
// {
//   if (2 > argc)
//   {
//     cout << "please enter Limit" << endl;
//     exit(0);
//   }
//   else
//   {
//     int val = (int)(argv[1][0] - '0');
//     if (0 < val && val <= 10)
//       LIMIT = val;
//     else
//     {
//       cout << "not a valid number ([1..10])";
//       exit(0);
//     }
//   }

//   cout << "depth " << LIMIT << ":" << endl;
//   Board b(false);
//   if (3 == argc)
//   {
//     string str(argv[2]);
//     b = Board(str);
//   }

//   {
//       // unsigned long *result = new unsigned long[LIMIT];
//       // for (int i = 0; i < LIMIT; ++i)
//       //   result[i] = 0;
//       // auto start = std::chrono::system_clock::now();
//       // steptest_optimized(b, result);
//       // auto end = std::chrono::system_clock::now();
//       // for (int i = 0; i < LIMIT; ++i)
//       //   cout << (i + 1) << ": " << result[i] << endl;
//       // delete[] result;
//       // cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
//   }

//   {
//     auto start = std::chrono::system_clock::now();
//     // steptest(b);
//     auto res = steptest2(b);
//     auto end = std::chrono::system_clock::now();
//     for (int i = 0; i < LIMIT; ++i)
//       cout << (i + 1) << ": " << res[i] << endl;
//     cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
//   }

//   // test();

//   if (4 == argc)
//   {
//     int i;
//     cin >> i;
//   }
// }
#pragma endregion

int main()
{
  ai();
}

// Initial Position; perft(7) = 3195901860, correct; 473055 ms = 7 min, 53 sec
// Position 6; perft(5) = 164075551, correct
// Position 3; perft(6) = 11024419, correct (without promo)
// Position 2; perft(4) = 4074339, false (promo?)
// Position 4; perft(2) = 222, false (promo?)
// Position 5; perft(2) = 222, false (promo?)