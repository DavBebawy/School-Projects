import random
import sys

textfile = "wordlist.txt"
userinput = True
wordcounts = dict()
words = dict()

#Print an error and exit gracefully if wordlist is not found
try:
    open(textfile)
except FileNotFoundError:
    print(textfile + " not find. Please make sure it exists in current directory.")
    sys.exit()

#Open the wordlist.txt and read each line into the dictionary wordcounts in a with the word length as the key    
with open(textfile) as infile:    
    for line in infile:
        for word in line.split():
            wordcounts.setdefault(len(word),[]).append(word)

print("Welcome to Hangman")

#Ask for user input, check if it's an int bigger than 2 and smaller than the maximum length
while userinput:
    wordlength = input("What length word would you like me to choose? ==> ")
    try:
        int(wordlength)
    except ValueError:
        print("Input is invalid, please try again")
        continue
    wordlength = int(wordlength)
    words = wordcounts.get(wordlength)
    if  not words or wordlength < 2:
        print("Input is invalid, please try again")
    else:
        userinput = False

#choose a random word with the specified length  
word = random.choice(words)

#create a list of *s with the same length as the chosen word
filled_word = ['*'] * len(word)
guesses_left = 7

#loop: ask for user input for a letter or a word, give and error and try again if not
while guesses_left > 0:
    print("\nWord: " + ''.join(filled_word))
    print("You have " + str(guesses_left) + " guesses remaining.")
    guess = input("Type a letter or a word guess: ")
    if not guess.isalpha():
        print("Wrong input.\n")
        continue
    elif len(guess) == 1: #letter guess, check the occurence of the letter, 'reveal' the letter in the word, or if it doesn't exist, decrement guesses_left and restart loop
        guess = guess.lower()
        index = 0
        found = []
        while index < len(word):
            index = word.find(guess, index)
            if index >= 0:
                found.append(index)
                index += 1
            elif len(found) > 0:
                print("There is " + str(len(found)) + " " + guess + "!")
                break
            else:
                print("Sorry, there are no " + guess + "'s.")
                guesses_left -= 1
                break
        for x in found:
            filled_word[x] = guess
        if ''.join(filled_word) == word:
            print("\nCongratulations, you guessed it!")
            break
    else: #word guess, check if the guess matches the word. End the game if guessed correctly, otherwise decrement guesses_left and restart loop
        guess = guess.lower()
        if guess == word:
            print("\nCongratulations, you guessed it!")
            break
        else:
            guesses_left -= 1
            print("Sorry, the word is not '" + guess + "'")

if guesses_left == 0: #if the user runs out of guesses, reveal the word
    print("\nYou are out of guesses. The word was: " + word)

print("Game Over")