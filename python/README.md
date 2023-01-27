**Summary**
This is a very crude way to randomise and generate pairings.

**randomiseTSV.py**
Will take in a `"input.tsv"` in the current directory and randomise it, saving it to `"randomised.tsv"`.

**generateJson.py**
Will take in a `"randomised.tsv"` in the current directory and jsonify it, saving it to `"participants.json"`.

**main.py**
Does the above 2 for you.

**TSV Format**
Note the tsvs have to be in the following format:

{Telegram Username : string} {Telegram UID : int} {Full Name : string} {isDevil : boolean} {prankLevel : int} {roomNumber : int} {interests : string} {notes : string}
