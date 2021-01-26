#lang Racket

;Function: in? check if item is in a list
;Parameters: item - to be check, lst - list to be compared
;Returns: #t if item is in the list
;Example: (in? 1 '(2 3 4 1))
(define in?
  (lambda (item lst)

    ;Base case, return #f if lst is null
    (if (null? lst)
        #f

        ;Check if the car = item and return #t, else recursively call in? with the cdr
        (if (eqv? item (car lst))
            #t
            (in? item (cdr lst))))))

;Function: set-equal? check if two sets are equal
;Parameters: set1 set2 - the 2 sets being compared
;Returns: #t if sets are equal
; Example: (set-equal? '(1 2 3 4) '(4 2 1 3)) should return #t.
(define set-equal?
  (lambda (set1 set2)
    (letrec

        ;Helper function: equal-helper
        [(equal-helper (lambda (set1 set2)
                         ;Base case, return #f if set1 is null
                         (if (null? set1)
                             #f
                             ;checks if the car of set1 is in set2
                             (if (in? (car set1) set2)
                                 ;if reached the end of set1, return #t
                                 (if (null? (cdr set1))
                                     #t
                                     ;else recursively call equal-helper with the cdr
                                     (equal-helper (cdr set1) set2))
                                 #f
                                 ))))]
      ;checks if the two sets have an equal number of items, calls helper function if true
      (if (= (length set1) (length set2))
          (equal-helper set1 set2)
          #f))))

;Function: nested-set-equal? check if two sets the can contain other sets are equal
;Parameters: set1 set2 - the 2 sets being compared
;Returns: #t if sets are equal
;Example: (nested-set-equal? '(1 2 (3 4 5)) '(2 (4 3 5) 1)) should return #t.
(define nested-set-equal?
  (lambda (set1 set2)
    (letrec
        
        ;Helper function: equal-helper
        [(equal-helper (lambda (set1 set2)
                         ;Base case, return #t if reached the end of set1
                         (cond [(null? set1)
                                #t]
                               ;checks if the car is a list
                               [(list? (car set1)) ;list
                                ;checks if the car of set1 is in set2
                                (if (in? (car set1) set2 #t)
                                    ;recursively call equal-helper the cdr set1 and set2
                                    (equal-helper (cdr set1) set2)
                                    #f)]
                               ;else, the car is an item
                               [else
                                ;checks if the car of set1 is in set2
                                (if (in? (car set1) set2 #f)
                                    ;recursively call equal-helper the cdr set1 and set2
                                    (equal-helper (cdr set1) set2)
                                    #f)])))
         ;Helper function: in?
         ;Parameters: set1 set2 lst - the 2 sets being compared, isLst is boolean corresponding to set1
         ;Returns: #t if set1 is in set2
         (in? (lambda (set1 set2 isList)
                ;Base case, return #f if reached the end of set2
                (if (null? set2)
                    #f
                    ;checks if set1 is a list
                    (if isList
                        ;checks if the car set2 is a list
                        (if (list? (car set2))
                            ;recursively call nested-set-equal? with set1 (car set2) 
                            (if (nested-set-equal? set1 (car set2))
                                #t
                                ;recursively call in? with set1 (cdr set2) isList 
                                (in? set1 (cdr set2) #t))
                            ;recursively call in? with set1 (cdr set2) isList
                            (in? set1 (cdr set2) #t))
                        ;checks if (car set2) is a list
                        (if (list? (car set2))
                            ;recursively call in? with set1 (cdr set2) notList
                            (in? set1 (cdr set2) #f)
                            ;Check if the set1 = (car set2) and return #t, else recursively call in? with set1 (cdr set2) notList
                            (if (eqv? set1 (car set2))
                                #t
                                (in? set1 (cdr set2) #f)))))))]
      ;checks if the two sets have an equal number of items, calls equal-helper function if true
      (if (= (length set1) (length set2))
          (equal-helper set1 set2)
          #f))))

;Function: union returns the union of the two sets
;Parameters: set1 set2 - the 2 sets being compared
;Returns: a list containing every element from either of the two sets, in any order
;Example: (union '(1 2 3 4) '(2 3 4 5)) should return (1 2 3 4 5)
(define union
  (lambda (set1 set2)
    ;Base case, return set1 if reached the end of set2
    (if (null? set2)
        set1
        ;else check if the car set2 is in set1
        (if (in? (car set2) set1)
            ;recursively call union on set1 (cdr set2)
            (union set1 (cdr set2))
            ;if (car set2) is not in set1, append it to set1 and recursively call union with updated list
            (union (append set1 (list (car set2))) (cdr set2))))))

;Function: intersection - returns the intersection of the two sets
;Parameters: set1 set2 - the 2 sets being compared
;Returns: a list containing elements present in the two sets, in any order
;Example: (intersection '(1 2 3 4) '(2 3 4 5)) should return (2 3 4)
(define intersection
  (lambda (set1 set2)

    ;Create intersectionList as an empty list that will hold items present in both sets
    (letrec [ (intersectionList '())
              
              ;Helper function: equal-helper
              (intersectionHelper (lambda (set1 set2)
                                    ;Base case, return null if reached the end of set1
                                    (if (null? set2)
                                        null
                                        (begin
                                          ;else check if the car set2 is in set1
                                          (if (in? (car set2) set1)
                                              ;append intersectionList with item present in both lists
                                              (set! intersectionList (append intersectionList (list (car set2))))
                                              null)
                                          ;recursively call intersectionHelper with set1 (cdr set2)
                                          (intersectionHelper set1 (cdr set2))))))]
      ;call helper function
      (intersectionHelper set1 set2)
      ;return the list containing elements present in the two sets
      intersectionList)))

;Function: mergesort - Sorts integers using merge sort algorithm
;Parameters: lst - a flat list of integers
;Returns: a sorted list
;Example: (mergesort '(3 1 2 7 9)) should return (1 2 3 7 9)
(define mergesort
  (lambda (lst)

    ;helper function: merge - recursively merges two lists from smallest to largest
    ;Parameters: set1 set2 - the 2 sets being compared
    ;Returns: one merged sorted list
    (letrec [(merge (lambda (set1 set2)
                      ;Base case, return null if set1 and set2 are null
                      (cond [(and (null? set1) (null? set2)) null]
                            ;If set1 is null, append (car set2) with recursive merge with set1 (cdr set2)
                            [(null? set1) (append (list (car set2)) (merge set1 (cdr set2)))]
                            ;If set1\2 is null, append (car set1) with recursive merge with (cdr set1) set2
                            [(null? set2) (append (list (car set1)) (merge (cdr set1) set2))]
                            ;else if (car set1) < (car set2)
                            [else (if (< (car set1) ( car set2))
                                      ;append (car set1) with recursive merge with (cdr set1) set2
                                      (append (list (car set1)) (merge (cdr set1) set2))
                                      ;append (car set2) with recursive merge with set1 (cdr set2)
                                      (append (list (car set2)) (merge set1 (cdr set2)))
                                      )])))
             ;helper function: list-split - splits one list into one list containg two separate lists
             ;Parameters: lst - list being split 
             ;Returns: one list containg two separate lists
             (list-split (lambda (lst)
                           (let-values (((set1 set2) (split-at lst (exact-floor (/ (length lst) 2)))))
                             (if (= (length lst) 1)
                                 lst
                                 (list set1 set2)))))]
      ;Base case, return null if reached the end of list
      (cond [(null? lst) null]
            ;if lst contains only one item, return item
            [(= (length lst) 1) lst]
            ;else, call merge on two halves of a list, each recursively passed to mergesort first
            [else (merge (mergesort (car (list-split lst)))
                         (mergesort (cadr (list-split lst))))]))))


;(powerset lst) -- Expects a flat (no sub-lists) list representing a set of of integers. Returns a list of lists representing the powerset of the input set.
;(Recall that the the powerset of a set is the set of all subsets of the original set). Example: (powerset '(1 3 5)) should return (() (1) (3) (5) (1 3) (1 5) (3 5) (1 3 5))(.

;Function: powerset - returns a powerset of the input set
;Parameters: lst - a flat list of integers
;Returns: a list containing all subsets 
;Example: (powerset '(1 3 5)) should return (() (1) (3) (5) (1 3) (1 5) (3 5) (1 3 5))
(define powerset
  (lambda (lst)
    ;list to hold powersetofCdr
    (letrec [(powersetOfCdr '())
             
             ;Function: cons-all - appends an item to the sublists of setsSoFar
             ;Parameters: item setsSoFar - item to be appended, list of sublists
             ;Returns: a list containing all appended subsets
             (cons-all
              (lambda (item setsSoFar)
                (if (empty? setsSoFar)
                    empty
                    (cons (cons item (car setsSoFar)) (cons-all item (cdr setsSoFar))))))]
      ;Base case, if lst is empty return an empty list
      (if (empty? lst)
          (list empty)
          (begin
            ;Recursive call powerset with (cdr lst) and store it in powerserOfCdr
            (set! powersetOfCdr (powerset (cdr lst)))
            ;append powersetOfCdr and cons-all with (car list) and powersetOfCdr
            (append powersetOfCdr (cons-all (car lst) powersetOfCdr)))))))


;Function: nested-reduce removes any duplicates in the list or in its sublists
;Parameters: lst - list being examined
;Returns: list with no duplicates
;Example: (reduce '(1 3 (2 5) (2 5) (2 5 (2 5) (2 5)) 3 7 1)) should return (1 3 (2 5) (2 5 (2 5)) 7)
(define nested-reduce
  (lambda (lst)
    (letrec
        ;Create an empty set to hold unique items and lists
        [(reducedList '())
         ;helper function: reduceHelper
         (reduceHelper (lambda (lst)
                         ;base case, return null when lst is null
                         (if (null? lst)
                             null
                             ;Check if (car lst) is a list
                             (begin (if (list? (car lst))
                                        ;Call listReduce on the recursive call nested-reduce of (car lst) and reducedList
                                        (listReduce (nested-reduce (car lst)) reducedList)
                                        ;else call itemReduce with (car lst) and reducedList
                                        (itemReduce (car lst) reducedList))
                                    ;recursively call reduceHelper on (cdr lst)
                                    (reduceHelper (cdr lst))))))
         ;helper function: itemReduce adds unique items to reducedList
         ;Parameters: item lst2 - item being examined, lst2 is the reduced list so far
         ;Returns: null, appends unique items to reducedList
         (itemReduce (lambda (item lst2)
                       ;Base case, if reached the end of lst2, append item to the end of the list
                       (if (null? lst2)
                           (set! reducedList (append reducedList (list item)))
                           ;Checks if (car lst2) is a list
                           (if (list? (car lst2))
                               ;Recursively calls itemReduce with item (cdr lst2)
                               (itemReduce item (cdr lst2))
                               ;else, if it's an item, check if it's eqv? to (car lst2)
                               (if (eqv? item (car lst2))
                                   ;item is not unique, return null
                                   null
                                   ;Recursively calls itemReduce with item (cdr lst2)
                                   (itemReduce item (cdr lst2)))))))
         ;helper function: listReduce adds unique lists to reducedList
         ;Parameters: lst lst2 - list being examined, lst2 is the reduced list so far
         ;Returns: null, appends unique lists to reducedList
         (listReduce (lambda (lst lst2)
                       ;Base case, if reached the end of lst2, append list to the end of the list
                       (if (null? lst2)
                           (set! reducedList (append reducedList (list lst)))
                           ;Checks if (car lst2) is a list
                           (if (list? (car lst2))
                               ;Checks if lst and (car lst2) are equal using nested-set-equal?
                               (if (nested-set-equal? lst (car lst2))
                                   ;list is not unique, return null
                                   null
                                   ;Recursively calls itemReduce with lst (cdr lst2)
                                   (itemReduce lst (cdr lst2)))
                               ;Recursively calls itemReduce with lst (cdr lst2)
                               (itemReduce lst (cdr lst2))))))]
      ;Call helper function
      (reduceHelper lst)
      ;return reduced list
      reducedList)))