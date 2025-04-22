#include "deck.h"
#include <algorithm>
#include <random>

std::vector<Card> mockDeck;

void initMockDeck() {
    mockDeck.clear();
    for (int s = 0; s < 4; ++s) {
        for (int r = 2; r <= 14; ++r) {
            mockDeck.push_back(Card{static_cast<Rank>(r), static_cast<Suit>(s)});
        }
    }

    // Shuffle using C++11 random engine
    int seedSource = analogRead(1);
    unsigned long seed = millis() + seedSource;
    std::mt19937 g(seed);
    std::shuffle(mockDeck.begin(), mockDeck.end(), g);
}

Card drawCard() {
    if (mockDeck.empty()) {
        return Card{RANK_2, CLUBS};  // Fallback
    }
    Card card = mockDeck.back();
    mockDeck.pop_back();
    return card;
}
