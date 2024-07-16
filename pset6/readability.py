from cs50 import get_string

text = get_string("Text: ")

letters = 0
words = 1
sentence = 1

for i in text:
    if i.isalpha():
        letters += 1
    elif i == " ":
        words += 2
    elif i == "." or i == "?" or i == "!":
        sentence += 1

index = 0.0588 * (letters/words * 100)- 0.296 * (sentence/words*100) - 15.8

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade ", round(index))
