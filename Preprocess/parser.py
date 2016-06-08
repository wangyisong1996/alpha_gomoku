#!/usr/bin/python

import re
import os
import csv

path = "./renju_csv/"
base = ord('a') - 1
if not os.path.exists(path):
    os.mkdir(path)

fieldnames = ["x", "y"]


def writecsv(num, moves):
    with open(path + "record" + str(num) + ".csv", "w") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for move in moves:
            writer.writerow({"x": str(ord(move[0]) - base), "y": move[1:]})
    with open(path + "record" + str(num + 1) + ".csv", "w") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for move in moves:
            writer.writerow(
                {"x": str(16 - (ord(move[0]) - base)), "y": move[1:]})
    with open(path + "record" + str(num + 2) + ".csv", "w") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for move in moves:
            writer.writerow(
                {"x": str(ord(move[0]) - base), "y": str(16 - int(move[1:]))})
    with open(path + "record" + str(num + 3) + ".csv", "w") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for move in moves:
            writer.writerow(
                {"x": str(16 - (ord(move[0]) - base)), "y": str(16 - int(move[1:]))})
    with open(path + "record" + str(num + 4) + ".csv", "w") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for move in moves:
            writer.writerow(
                {"x": move[1:], "y": str(16 - (ord(move[0]) - base))})
    with open(path + "record" + str(num + 5) + ".csv", "w") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for move in moves:
            writer.writerow(
                {"x": str(16 - (ord(move[0]) - base)), "y": str(16 - int(move[1:]))})
    with open(path + "record" + str(num + 6) + ".csv", "w") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for move in moves:
            writer.writerow(
                {"x": str(16 - int(move[1:])), "y": str(ord(move[0]) - base)})


def main():
    pattern = re.compile(r'<game id.*>\r\n<move>(.*)<\/move>')
    # pattern = re.compile(r'<game id.*rule=\"1\".*>\r\n.*<\/move>', re.M)
    datafile = open("renju.rif", "r")
    database = datafile.read()
    datafile.close()
    i = 0
    for match in re.findall(pattern, database):
        writecsv(i, match.split())
        i += 7


if __name__ == "__main__":
    main()
