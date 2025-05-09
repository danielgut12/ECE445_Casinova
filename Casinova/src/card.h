#ifndef CARD_H
#define CARD_H

#include <Arduino.h>  // ← Required for String

enum Suit { CLUBS, SPADES, HEARTS, DIAMONDS };
enum Rank {
  RANK_2 = 2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7,
  RANK_8, RANK_9, RANK_T, RANK_J, RANK_Q, RANK_K, RANK_A
};

struct Card {
  Rank rank;
  Suit suit;
};

Rank charToRank(char c);
Suit charToSuit(char c);
Card stringToCard(const String& str);
String cardToString(const Card& card);

#endif
