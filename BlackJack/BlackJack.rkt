#lang Racket
(define faces '(2 3 4 5 6 7 8 9 10 J Q K A))
(define suits '(♣ ♦ ♥ ♠️))

;make-deck -- Creates a new (unshuffled) 52 card deck

;Parameters: none
;Returns: The deck (a list of cards)
(define make-deck
  (letrec [(helper (lambda (lst)
                     (if (null? lst)
                         null
                         (append (map (curryr cons (car lst)) faces) (helper (cdr lst))))))]
    (append (map (curryr cons (car suits)) faces) (helper (cdr suits))))) ;build deck by appending the first suit to the face and recursively call helper to do the rest

;eval-hand -- Determine the best value of a hand, given that an Ace can be worth either 1 or 11 points.

;Parameter: hand -- A list of cards to evaluate
;Returns: The best possible value of the hand (fixnum)
(define eval-hand
  (lambda (hand)
    (letrec [(fixnum 0)
             ;eval-helper accepts list of cards and count of aces
             (eval-helper (lambda (lst aces)
                            (if (null? lst) ;base case, return fixnum or call aces-helper
                                (if (= aces 0)
                                    fixnum
                                    (aces-helper aces))
                                (begin (cond [(number? (caar lst)) ;face cards
                                              (set! fixnum (+ fixnum (caar lst)))]
                                             [(eqv? (caar lst) 'A) ;increment aces
                                              (set! aces (+ aces 1))]
                                             [else
                                              (set! fixnum (+ fixnum 10))]) ;K, Q, J are worth 10
                                       (eval-helper (cdr lst) aces)))))
             ;aces-helper returns the best value for aces in hand
             (aces-helper (lambda (aces)
                            (if (= aces 0)
                                fixnum ;base case, return fixnum
                                (begin (if (<= (+ fixnum 11 (* (- aces 1) 1)) 21) ;check if A can be 11 with rest of Acess atleast as 1 <= 21
                                           (set! fixnum (+ fixnum 11))
                                           (set! fixnum (+ fixnum 1)))
                                       (aces-helper (- aces 1))))))]
      (eval-helper hand 0))))

;deal! -- Create a two card hand, by removing the top two cards from the deck.

;Parameter: deck -- The deck to deal from
;Returns: A new two-card hand, consisting of the first two cards in the deck.
;Side effects: The first two cards are removed from the deck.
(define-syntax-rule
  (deal! thedeck)
  (let ((delt-hand '()))
    (set! delt-hand (list (car thedeck) (cadr thedeck))) ;deals the first 2 cards to the hand
    (set! thedeck (cddr thedeck)) ;removes the first 2 cards from the deck
    delt-hand)) ;return the delt hand

;hit! -- Take the top card from the deck and add it to a hand.

;Parameters: deck -- The deck to deal from, hand -- The hand to deal into
;Returns: Nothing
;Side effects: The first card in the deck is removed from the deck and added to the hand.
(define-syntax-rule
  (hit! thedeck hand)
  (begin 
    (set! hand (append hand (list (car thedeck)))) ;deals the first card to the hand
    (set! thedeck (cdr thedeck)) ;removes the first card from the deck
    ))

;show-hand -- Display a hand (or most of it) to the screen.

;Parameter 1: hand -- The hand to display
;Parameter 2: how -- Should be either 'Full or 'Part. If how is 'Full, then show the entire hand. If how is 'Part, then show only ***** for the first card, and then show the rest of the hand normally.
;Parameter 3: description -- A string to be displayed to the screen before the actual hand.
;Returns: Nothing 
(define show-hand
  (lambda (hand how description)
    (letrec
        ;recursevily show the rest of the cards
        [(show-helper (lambda (lst)
                        (if (null? lst)
                            (displayln ".")
                            (begin (display (car lst))
                                   (show-helper (cdr lst))))))]
      (display description)
      (if (eqv? how 'Part)
          (display "***** ") ;sensor 1st card of the dealer 
          (display (car hand)))
      (show-helper (cdr hand)))))

;main function to run code
(define main
  (lambda ()
    (letrec [(thedeck make-deck)
             (player-hand '())
             (dealer-hand '())
             (user-input '())
             (dealer-hand-value 0)
             (player-hand-value 0)
             ;main helper function
             (main-helper (lambda ()
                            ;deal the dealer and player hands and display them
                            (set! player-hand (deal! thedeck))
                            (set! dealer-hand (deal! thedeck))
                            (show-hand dealer-hand 'Part "The dealer has: ")
                            (show-hand player-hand 'Full "You have: ")

                            ;call player-helper
                            (unless (player-helper)
                              (begin
                                (show-hand dealer-hand 'Full "The dealer has: ") ;Reveal dealer full hand
                                (unless (dealer-helper)
                                  (cond [(= dealer-hand-value player-hand-value ) ;Ties
                                         (begin
                                           (display "You and the dealer have ")
                                           (display player-hand-value)
                                           (displayln ". It's a tie!"))]
                                        [(> dealer-hand-value player-hand-value) ;Dealerwins
                                         (begin 
                                           (display "You have ")
                                           (display player-hand-value)
                                           (display " and the dealer has ")
                                           (display dealer-hand-value)
                                           (displayln ". Dealer wins!"))]
                                        [else ;Player wins
                                         (begin 
                                           (display "You have ")
                                           (display player-hand-value)
                                           (display " and the dealer has ")
                                           (display dealer-hand-value)
                                           (displayln ". You win!"))]))))))
             ;asks the player if they want to play again
             (gameloop (lambda ()
                         (displayln " ")
                         ;If the deck is 3/4 used, shuffle all the cards
                         (when (< (length thedeck) 13)
                           (begin
                             (displayln "Shuffling the deck....")
                             (set! thedeck (shuffle make-deck))))
                         (display "Would you like to play again? Y or N ")
                         (set! user-input (read))
                         (displayln " ")
                         (if (eqv? user-input 'n)
                             (displayln "Thanks for playing! Racket Blackjack was created by David Bebawy. (:")
                             (begin
                               (if (eqv? user-input 'y)
                                   (main-helper) ;start a new game
                                   (display "Please type Y or N. ")) ;check user input
                               (gameloop)))))
             ;displays the game rules to the user
             (game-rules (lambda ()
                           (set! user-input (read))
                           (unless (eqv? user-input 'n)
                             (begin
                               (if (eqv? user-input 'y)
                                   (begin
                                     (displayln " ")
                                     (display "The player is dealt two cards face up. The dealer is also dealt two cards, one down (hidden) and one up (exposed). ")
                                     (display "The value of cards two through ten is their value (2 through 10). Face cards (Jack, Queen, and King) are all worth 10. ")
                                     (display "Aces can be worth 1 or 11. A hand's value is the sum of the card values. Players are allowed to draw additional cards to improve their hands. ")
                                     (display "A hand with an ace valued as 11 is called 'soft', meaning that the hand will not bust by taking an additional card. ")
                                     (display "The value of the ace will become one to prevent the hand from exceeding 21. Otherwise, the hand is called 'hard'. ")
                                     (display "Once the player has completed their hand, it is the dealer's turn. The dealer hand will not be completed if the player have either busted or received blackjacks. ")
                                     (display "The dealer then reveals the hidden card and must hit until the cards total up to 17 points. At 17 points or higher the dealer must stay. ")
                                     (display "You are betting that you have a better hand than the dealer. The better hand is the hand where the sum of the card values is closer to 21 without exceeding 21. ")
                                     (displayln "The detailed outcome of the hand follows:")
                                     (displayln "   * If the player is dealt an Ace and a ten-value card (called a 'blackjack' or 'natural'), and the dealer does not, the player wins.")
                                     (displayln "   * If the player exceeds a sum of 21 ('busts'), the player loses, even if the dealer also exceeds 21.")
                                     (displayln "   * If the dealer exceeds 21 ('busts') and the player does not, the player wins.")
                                     (displayln "   * If the player attains a final sum higher than the dealer and does not bust, the player wins.")
                                     (displayln "   * If both dealer and player receive a blackjack or any other hands with the same sum called a 'push', no one wins.")
                                     (displayln " "))
                                   (begin
                                     (display "Please type Y or N. ") ;check user input
                                     (game-rules)))))))
             ;Check for Blackjack, asks the player to hit or stay and checks if his hand is over 21
             (player-helper (lambda ()
                              (set! player-hand-value (eval-hand player-hand))
                              (when (= player-hand-value 21) ;Blackjack, check if dealer also has blackjack for a tie or the player wins!
                                (display "Blackjack! ")
                                (show-hand dealer-hand 'Full "The dealer has: ")
                                (if (= (eval-hand dealer-hand) 21)
                                    (displayln "It's a tie!")
                                    (displayln "You win!")))
                              (if (> player-hand-value 21) ;Hand went over 21, player loses
                                  (begin
                                    (display "You have ")
                                    (display player-hand-value)
                                    (displayln ". House wins."))
                                  (begin
                                    (display "Would you like to hit or stay? ")
                                    (when (hitorstay) ;If the player hits, recursively call player-helper
                                        (player-helper))))))
             ;Ask the player to hit or stay
             (hitorstay (lambda ()
                          (set! user-input (read))
                          (cond [(eqv? user-input 'hit)  ;Hit, add card and display hand
                                 (begin
                                   (hit! thedeck player-hand)
                                   (display "You hit! ")
                                   (show-hand player-hand 'Full "You have: "))]
                                [(eqv? user-input 'stay) ;Stay, return false
                                 #t]
                                [else
                                 (begin
                                   (display "Please type hit or stay ") ;check user input
                                   (hitorstay))])))
             ;Evaluate dealer hand, hit till at least 17
             (dealer-helper (lambda ()
                              (set! dealer-hand-value (eval-hand dealer-hand)) ;evaluate hand
                              (cond [(> dealer-hand-value 21) ;Hand went over 21, dealer loses
                                     (begin
                                       (display "The dealer has ")
                                       (display dealer-hand-value)
                                       (displayln ". You win!"))]
                                    [(< dealer-hand-value 17) ;Hand under 17, must hit and display dealer hand
                                     (begin
                                       (hit! thedeck dealer-hand)
                                       (display "Dealer hit! ")
                                       (show-hand dealer-hand 'Full "The dealer has: ")
                                       (dealer-helper))]
                                    [else ;If hand between 17 and 21, return
                                     #f])))]
      ;Ask if player needs the rules
      (display "Welcome to Racket Blackjack! Would you like the rule of the game? Y or N ")
      (game-rules)

      ;Shuffle the deck to start
      (set! thedeck (shuffle thedeck))

      ;Play the game the first time
      (main-helper)

      ;Ask player if they want to play again
      (gameloop))))

;Ignore case-sensitivy on read, allows the user to enter Y or y, HIT or hit, etc..
(read-case-sensitive #f)
;Start the game
(main)