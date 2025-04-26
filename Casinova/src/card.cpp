#include "card.h"

Rank charToRank(char c) {
  switch (c) {
    case '2': return RANK_2;
    case '3': return RANK_3;
    case '4': return RANK_4;
    case '5': return RANK_5;
    case '6': return RANK_6;
    case '7': return RANK_7;
    case '8': return RANK_8;
    case '9': return RANK_9;
    case 'T': return RANK_T;
    case 'J': return RANK_J;
    case 'Q': return RANK_Q;
    case 'K': return RANK_K;
    case 'A': return RANK_A;
    default:  return RANK_2;
  }
}

Suit charToSuit(char c) {
  switch (c) {
    case 'C': return CLUBS;
    case 'S': return SPADES;
    case 'H': return HEARTS;
    case 'D': return DIAMONDS;
    default:  return CLUBS;
  }
}

Card stringToCard(const String& str) {
  if (str.length() < 2) {
    return Card{RANK_2, CLUBS}; // default fallback
  }

  char rankChar = str.charAt(0);
  char suitChar = str.charAt(1);
  return Card{charToRank(rankChar), charToSuit(suitChar)};
}



String rankToChar(Rank rank) {
  switch (rank) {
    case RANK_T: return "T";
    case RANK_J: return "J";
    case RANK_Q: return "Q";
    case RANK_K: return "K";
    case RANK_A: return "A";
    default: return String((int)rank); // 2–9
  }
}

String suitToChar(Suit suit) {
  switch (suit) {
    case CLUBS: return "♣";
    case SPADES: return "♠";
    case HEARTS: return "♥";
    case DIAMONDS: return "♦";
    default: return "?";
  }
}

String cardToString(const Card& card) {
  return rankToChar(card.rank) + suitToChar(card.suit);
}