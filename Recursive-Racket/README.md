#  Recursive Racket

A few basic Racket functions operating recursively on lists and sets.

## Functions:
- `(set-equal? set1 set2)` returns true if the two sets contain the same members. e.g. (set-equal? '(1 2 3 4) '(4 2 1 3)) returns true.
- `(nested-set-equal? set1 set2)` returns true if the two sets contain the same members, and sets can contain other sets. e.g. (nested-set-equal? '(1 2 (3 4 5)) '(2 (4 3 5) 1)) returns true
- `(union set1 set2)` returns a set containing every element from either set, with no repetitions. e.g. (union '(1 2 3 4) '(2 3 4 5)) returns (1 2 3 4 5).
- `(intersection set1 set2)` returns a set containing every element present in both sets. e.g. (intersection '(1 2 3 4) '(2 3 4 5)) returns (2 3 4).
- `(mergesort list)` returns a sorted list using merge sort algorithm. e.g. (mergesort '(3 1 2 7 9)) returns (1 2 3 7 9).
- `(powerset list)` returns a list of list representing the powerset of the the input set. e.g. (powerset '(1 3 5)) returns (() (1) (3) (5) (1 3) (1 5) (3 5) (13 5)).
- `(nested-reduce list)` returns a list with no duplicate elements or sublists. e.g. (reduce '(1 3 (2 5) (2 5) (2 5 (2 5) (25)) 3 7 1)) returns (1 3 (2 5) (2 5 (2 5)) 7).

## Run in GitPod
You can also run Recursive Racket in Gitpod, a free online dev environment for GitHub:
[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/DavBebawy/School-Project/RecursiveRacket)