#  BlackJack

A simple single-player game of BlackJack vs the computer. The game displays a welcome message and optional rules. The player and dealer are dealt two cards, but one of the dealer cards is hidden. The hand is evaluated for a BlackJack or if it's over 21. The player can choose to Hit or stay. The dealer reveals their hand and it's evaluated, the dealer hits if needed and a winner is determined. Player is then asked if they want to play another round.

## Functions:
- `make-deck` creates a new 52 card deck.
- `eval-hand` evaluates your hand, Ace either 1 or 11 points.
- `deal!` creates a two card hand by removing the top two card from the deck.
- `hit!` add the top card from the deck to a hand
- `show-hand` display either a full or part hand

## Run in GitPod
You can also run BlackJack in Gitpod, a free online dev environment for GitHub:

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/DavBebawy/School-Project/BlackJack)