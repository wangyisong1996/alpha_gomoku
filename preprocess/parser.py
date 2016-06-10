#!/usr/bin/python

import re
import os
import csv
import numpy as np

path = "./renju_csv/"
base = ord('a') - 1
if not os.path.exists(path):
    os.mkdir(path)

fieldnames = ["x", "y"]


def writecsv(num, moves):
    N = len(moves)
    act = np.zeros((7, N, 2), dtype=np.uint8)
    for i, move in zip(range(N), moves):
        act[0, i, ] = [ord(move[0]) - base, int(move[1:])]
        act[1, i, ] = [16 - (ord(move[0]) - base), int(move[1:])]
        act[2, i, ] = [ord(move[0]) - base, 16 - int(move[1:])]
        act[3, i, ] = [16 - (ord(move[0]) - base), 16 - int(move[1:])]
        act[4, i, ] = [int(move[1:]), 16 - (ord(move[0]) - base)]
        act[5, i, ] = [16 - (ord(move[0]) - base), 16 - int(move[1:])]
        act[6, i, ] = [16 - int(move[1:]), ord(move[0]) - base]

    for i in range(act.shape[0]):
        with open(path + "record" + str(num + i) + ".csv", "w") as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            for j in range(N):
                writer.writerow({"x": str(act[i, j, 0]), "y": str(act[i, j, 1])})


def main():
    pattern = re.compile(r'<game id.*rule=\"1\".*>\r\n<move>(.*)<\/move>')
    # pattern = re.compile(r'<game id.*rule=\"1\".*>\r\n.*<\/move>')
    datafile = open("renju.rif", "r")
    database = datafile.read()
    datafile.close()
    i = 0
    for match in re.findall(pattern, database):
        writecsv(i, match.split())
        i += 7


if __name__ == "__main__":
    main()
