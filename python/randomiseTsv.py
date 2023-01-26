import csv
import random


def randomiseList():
  with open("input.tsv") as file:
    tsv_file = csv.reader(file, delimiter="\t")
        
    tsvList = [line for line in tsv_file]

    outputList = []
    
    for count in range(len(tsvList) - 1, -1, -1):
      randomInt = random.randint(0, count)
      outputList.append(tsvList.pop(randomInt))

    with open("randomised.tsv", "w+") as outputFile:
      tsvOutput = csv.writer(outputFile, delimiter='\t')
      for row in outputList:
        tsvOutput.writerow(row)

if __name__ == "__main__":
    randomiseList() 