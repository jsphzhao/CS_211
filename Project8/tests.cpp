/*tests.c*/

//
// Google test cases for our set class.
//
// <<< Joseph Zhao >>>
//
// Initial template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <set>  // for comparing answers

using std::string;
using std::vector;
using std::pair;

#include "set.h"
#include "gtest/gtest.h"


TEST(myset, empty_set)
{
  set<int> S;

  ASSERT_EQ(S.size(), 0);
}

TEST(myset, set_with_one)
{
  set<int> S;

  ASSERT_EQ(S.size(), 0);

  S.insert(123);

  ASSERT_EQ(S.size(), 1);

  ASSERT_TRUE(S.contains(123));
  ASSERT_TRUE(S[123]);

  ASSERT_FALSE(S.contains(100));
  ASSERT_FALSE(S[100]);
  ASSERT_FALSE(S.contains(200));
  ASSERT_FALSE(S[200]);
}

TEST(myset, set_with_four_strings)
{
  set<string> S;

  ASSERT_EQ(S.size(), 0);

  S.insert("banana");
  S.insert("apple");
  S.insert("chocolate");
  S.insert("pear");

  ASSERT_EQ(S.size(), 4);

  ASSERT_TRUE(S.contains("pear"));
  ASSERT_TRUE(S["banana"]);
  ASSERT_TRUE(S.contains("chocolate"));
  ASSERT_TRUE(S["apple"]);

  ASSERT_FALSE(S.contains("Apple"));
  ASSERT_FALSE(S["carmel"]);
  ASSERT_FALSE(S.contains("appl"));
  ASSERT_FALSE(S["chocolatee"]);
}

class Movie
{
public:
  string Title;
  int    ID;
  double Revenue;

  Movie(string title, int id, double revenue)
    : Title(title), ID(id), Revenue(revenue)
  { }

  bool operator<(const Movie& other)
  {
    if (this->Title < other.Title)
      return true;
    else
      return false;
  }

  bool operator>(const Movie& other)
  {
    if (this->Title > other.Title)
      return true;
    else
      return false;
  }

  bool operator==(const Movie& other) const {
    return this->Title == other.Title;
  }
};

TEST(myset, set_with_movies)
{
  set<Movie> S;

  ASSERT_EQ(S.size(), 0);

  Movie Sleepless("Sleepless in Seattle", 123, 123456789.00);
  S.insert(Sleepless);

  Movie Matrix("The Matrix", 456, 400000000.00);
  S.insert(Matrix);

  Movie AnimalHouse("Animal House", 789, 1000000000.00);
  S.insert(AnimalHouse);

  ASSERT_EQ(S.size(), 3);

  vector<Movie> V = S.toVector();

  ASSERT_EQ(V[0].Title, "Animal House");
  ASSERT_EQ(V[1].Title, "Sleepless in Seattle");
  ASSERT_EQ(V[2].Title, "The Matrix");
}

TEST(myset, set_from_class_with_nine)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
    ASSERT_TRUE(S[x]);
  }

  ASSERT_FALSE(S.contains(0));
  ASSERT_FALSE(S[0]);
  ASSERT_FALSE(S.contains(2));
  ASSERT_FALSE(S[2]);
  ASSERT_FALSE(S.contains(4));
  ASSERT_FALSE(S[4]);
  ASSERT_FALSE(S.contains(29));
  ASSERT_FALSE(S[31]);
  ASSERT_FALSE(S.contains(40));
  ASSERT_FALSE(S[42]);
}

TEST(myset, set_no_duplicates)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  // try to insert them all again:
  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
    ASSERT_TRUE(S[x]);
  }
}

TEST(myset, toVector)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  vector<int> V2 = S.toVector();

  ASSERT_EQ(V2.size(), V.size());

  std::sort(V.begin(), V.end());

  //
  // V and V2 should have the same elements in 
  // the same order:
  //
  auto iterV = V.begin();
  auto iterV2 = V2.begin();

  while (iterV != V.end()) {
    ASSERT_EQ(*iterV, *iterV2);

    iterV++;
    iterV2++;
  }
}

TEST(myset, copy_empty)
{
  set<int> S1;

  {
    //
    // create a new scope, which will trigger destructor:
    //
    set<int> S2 = S1;  // this will call copy constructor:

    S1.insert(123);  // this should have no impact on S2:
    S1.insert(100);
    S1.insert(150);

    ASSERT_EQ(S2.size(), 0);

    vector<int> V2 = S2.toVector();

    ASSERT_EQ((int) V2.size(), 0);
  }
}

TEST(myset, copy_constructor)
{
  set<int> S1;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S1.insert(x);

  ASSERT_EQ(S1.size(), (int) V.size());

  {
    //
    // create a new scope, which will trigger destructor:
    //
    set<int> S2 = S1;  // this will call copy constructor:

    S1.insert(123);  // this should have no impact on S2:
    S1.insert(100);
    S1.insert(150);

    ASSERT_EQ(S2.size(), (int) V.size());

    vector<int> V2 = S2.toVector();

    ASSERT_EQ(V2.size(), V.size());

    std::sort(V.begin(), V.end());

    //
    // V and V2 should have the same elements in 
    // the same order:
    //
    auto iterV = V.begin();
    auto iterV2 = V2.begin();

    while (iterV != V.end()) {
      ASSERT_EQ(*iterV, *iterV2);

      iterV++;
      iterV2++;
    }

    S2.insert(1000);  // this should have no impact on S1:
    S2.insert(2000);
    S2.insert(3000);
    S2.insert(4000);
    S2.insert(5000);

    V.push_back(123);
    V.push_back(100);
    V.push_back(150);
  }

  //
  // the copy was just destroyed, the original set
  // should still be the same as it was earlier:
  //
  ASSERT_EQ(S1.size(), (int) V.size());

  vector<int> V2 = S1.toVector();

  ASSERT_EQ(V2.size(), V.size());

  std::sort(V.begin(), V.end());

  //
  // V and V2 should have the same elements in 
  // the same order:
  //
  auto iterV = V.begin();
  auto iterV2 = V2.begin();

  while (iterV != V.end()) {
    ASSERT_EQ(*iterV, *iterV2);

    iterV++;
    iterV2++;
  }
}

TEST(myset, find_empty)
{
  set<int> S;

  auto iter = S.find(22);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, find_one)
{
  set<int> S;

  S.insert(1234);

  auto iter = S.find(123);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(1234);
  if (iter == S.end()) {  // this should not happen:
    ASSERT_TRUE(false); // fail:
  }

  ASSERT_EQ(*iter, 1234);

  iter = S.find(1235);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, find_with_set_from_class)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  //
  // make sure we can find each of the values we inserted:
  //
  for (auto x : V) {
    auto iter = S.find(x);

    if (iter == S.end()) {  // this should not happen:
      ASSERT_TRUE(false); // fail:
    }

    ASSERT_EQ(*iter, x);
  }

  //
  // these searches should all fail:
  //
  auto iter = S.find(0);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(-1);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(1);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(4);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(34);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(36);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, stress_test)
{
  set<long long> S;
  std::set<long long> C;

  long long N = 1000000;

  //
  // setup random number generator so tree will
  // be relatively balanced given insertion of
  // random numbers:
  //
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> distrib(1, N * 100);  // inclusive

  vector<long long> V;  // collect a few values for searching:
  int count = 0;

  while (S.size() != N) {

    long long x = distrib(gen);

    S.insert(x);
    C.insert(x);

    count++;
    if (count == 1000) { // save every 1,000th value:

      V.push_back(x);
      count = 0;
    }
  }

  ASSERT_EQ(S.size(), N);

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
  }

  ASSERT_FALSE(S.contains(0));
  ASSERT_FALSE(S.contains(-1));

  //
  // now let's compare our set to C++ set:
  //
  V.clear();
  V = S.toVector();

  ASSERT_EQ(V.size(), C.size());
  ASSERT_EQ(S.size(), (int) C.size());

  int i = 0;

  for (auto x : C) {
    ASSERT_EQ(V[i], x);
    i++;
  }
}

TEST(myset, toPairsEmptySet) {
    set<int> S;
    auto pairs = S.toPairs(-1);
    EXPECT_TRUE(pairs.empty());
}

TEST(myset, toPairsSingleElement) {
    set<int> S;
    S.insert(10);
    auto pairs = S.toPairs(-1);
    EXPECT_EQ(static_cast<size_t>(1), pairs.size());
    EXPECT_EQ(std::make_pair(10, -1), pairs[0]);
}

TEST(myset, toPairsMultipleElements) {
    set<int> S;
    S.insert(30);
    S.insert(15);
    S.insert(50);
    S.insert(8);
    S.insert(25);
    S.insert(70);
    S.insert(20);
    S.insert(28);
    S.insert(60);
    auto pairs = S.toPairs(-1);
    EXPECT_EQ(std::make_pair(8, 15), pairs[0]);
    EXPECT_EQ(std::make_pair(15, -1), pairs[1]);
    EXPECT_EQ(std::make_pair(20, 25), pairs[2]);
    EXPECT_EQ(std::make_pair(25, -1), pairs[3]);
    EXPECT_EQ(std::make_pair(28, 30), pairs[4]);
    EXPECT_EQ(std::make_pair(30, -1), pairs[5]);
    EXPECT_EQ(std::make_pair(50, -1), pairs[6]);
    EXPECT_EQ(std::make_pair(60, 70), pairs[7]);
    EXPECT_EQ(std::make_pair(70, -1), pairs[8]);
}

TEST(myset, toPairsNonNumeric) {
    set<string> S;
    S.insert("d");
    S.insert("b");
    S.insert("a");
    S.insert("c");
    S.insert("e");  
    auto pairs = S.toPairs(std::string("none"));
    EXPECT_EQ(std::make_pair(std::string("a"), std::string("b")), pairs[0]); 
    EXPECT_EQ(std::make_pair(std::string("b"), std::string("none")), pairs[1]);
    EXPECT_EQ(std::make_pair(std::string("c"), std::string("d")), pairs[2]);
    EXPECT_EQ(std::make_pair(std::string("d"), std::string("none")), pairs[3]);
    EXPECT_EQ(std::make_pair(std::string("e"), std::string("none")), pairs[4]);
}

TEST(myset, foreachEmptySet) {
  set<int> s;
  for (auto var : s) {
    FAIL();
  }
}

TEST(myset, foreachSingleElement) {
  set<int> s;
  s.insert(10);
  int count = 0;
  for (auto var : s) {
    EXPECT_EQ(10, var);
    count++;
  }
  EXPECT_EQ(1, count);
}

TEST(myset, foreachMultipleElements) {
  set<int> s;
  s.insert(5);
  s.insert(10);
  s.insert(15);
  std::vector<int> expected = {5, 10, 15};
  std::vector<int> result;
  for (auto var : s) {
    result.push_back(var);
  }
  EXPECT_EQ(expected, result);
}

TEST(myset, foreachNonNumeric) {
  set<std::string> s;
  s.insert("apple");
  s.insert("banana");
  s.insert("cherry");
  std::vector<std::string> expected = {"apple", "banana", "cherry"};
  std::vector<std::string> result;
  for (auto var : s) {
    result.push_back(var);
  }
  EXPECT_EQ(expected, result);
}

TEST(myset, foreachStressTest) {
  set<int> s;
  for (int i = 0; i < 10000; ++i) {
    s.insert(i);
  }
  int count = 0;
  for (auto var : s) {
    EXPECT_EQ(count, var);
    count++;
  }
  EXPECT_EQ(10000, count);
}

TEST(myset, foreachAndtoPairs) {
  set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(15);

  std::vector<std::pair<int, int>> expected = {{10, -1}, {15, 20}, {20, -1}};
  auto result = s.toPairs(-1);

  EXPECT_EQ(result.size(), expected.size());

  for (size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(result[i].first, expected[i].first);
    EXPECT_EQ(result[i].second, expected[i].second);
  }
}