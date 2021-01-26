# Ruby Pair

Pair is a Racket class that mimics the cons box (a.k.a dotted pair) in Racket

## Functions:
- `car` returns the first element
- `cdr` returns the second element
- `to_s` returns a string representation of the pair similar to Racket. e.g. cons(1, cons(2, cons(3, cons(4, 5)))) puts a.to_s returns (1 2 3 4 . 5)
- `list?` returns true if the pair is valid list similar to Racket.
- `count` if the pair is a list returns the number of items in the top of the list, otherwise returns false.
- `append(other)` if the pair is a list returns a new different list consisting of other appended to the original list, otherwise returns false.
- `null?` returns true if the pair is an empty list, otherwise false.

## Run in GitPod
You can also run Ruby Pair in Gitpod, a free online dev environment for GitHub:
[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/DavBebawy/School-Project/Ruby-Pair)