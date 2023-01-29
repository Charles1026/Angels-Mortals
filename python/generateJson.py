import csv
import json

def generateJson():
    with open("randomised.tsv") as file:
        tsv_file = csv.reader(file, delimiter="\t")
        
        tsvList = [line for line in tsv_file]
        
        if len(tsvList) <= 1:
            print("Need more than 2 participants")
            return -1

        jsonList = []
        
        # Generate Id
        for line in tsvList:
            if len(line) != 8:
                print("Error row must be of size 8, see README for columns")
                return -1

            jsonList.append({
                "username": line[0],
                "id": int(line[1]),
                "name": line[2],
                "isDevil": bool(line[3]),
                "prankLevel": int(line[4]),
                "roomNumber": int(line[5]),
                "interests": line[6],
                "notes": line[7],
                "angelChatId": -1,
                "angelId": -1,
                "mortalChatId": -1,
                "mortalId": -1,
                "mortalUsername": "",
            })

        # Generate Angel Id
        angelId : int = 0 
        for participant in jsonList:
            participant["angelId"] = angelId
            angelId = participant["id"]
        jsonList[0]["angelId"] = angelId

        # Generate Mortal Id
        mortalId : int = 0 
        for participant in reversed(jsonList):
            participant["mortalId"] = mortalId
            mortalId = participant["id"]
        jsonList[-1]["mortalId"] = mortalId

        with open("participants.json", "w+") as jsonFile:
            jsonFile.write(json.dumps(jsonList, indent=2))


if __name__ == "__main__":
    generateJson()  
