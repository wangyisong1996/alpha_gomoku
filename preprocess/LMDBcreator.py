#!/usr/bin/python
# create the LMDB file

import numpy as np
import pandas as pd

import lmdb
import caffe
from sklearn.cross_validation import train_test_split

import os
import csv
import itertools

empty = 0
black = 1
white = 2
size = 15


def get_action(action):
    y = action % size
    x = (action - y) / size
    return x, y


def connect_num(board, connect_board, num):
    for x, y in itertools.product(range(size), range(size)):
        if connect_board[x, y] != 0 or board[x, y] == 0:
            continue

        horizon = 1
        for i in range(1, num):
            if x + i < size and board[x + i, y] != 0:
                horizon += 1
            else:
                break
        for i in range(1, num):
            if x - i >= 0 and board[x - i, y] != 0:
                horizon += 1
            else:
                break
        if horizon >= num:
            connect_board[x, y] = 1
            continue

        vertical = 1
        for i in range(1, num):
            if y + i < size and board[x, y + i] != 0:
                vertical += 1
            else:
                break
        for i in range(1, num):
            if y - i >= 0 and board[x, y - i] != 0:
                vertical += 1
            else:
                break
        if vertical >= num:
            connect_board[x, y] = 1
            continue

        diagonal = 1
        for i in range(1, num):
            if x + i < size and y + i < size and board[x + i, y + i] != 0:
                diagonal += 1
            else:
                break
        for i in range(1, num):
            if x - i >= 0 and y - i >= 0 and board[x - i, y - i] != 0:
                diagonal += 1
            else:
                break
        if diagonal >= num:
            connect_board[x, y] = 1
            continue

        diagonal = 1
        for i in range(1, num):
            if x + i < size and y - i >= 0 and board[x + i, y - i] != 0:
                diagonal += 1
            else:
                break
        for i in range(1, num):
            if x - i >= 0 and y + i < size and board[x - i, y + i] != 0:
                diagonal += 1
            else:
                break
        if diagonal >= num:
            connect_board[x, y] = 1
            continue


def handle(csvfile):
    data = pd.read_csv(csvfile)
    N = data.shape[0]
    data.iloc[:, 0] -= 1
    data.iloc[:, 1] -= 1

    board = np.zeros((size, size))
    # (state, action) pair
    state = np.zeros((N, 7, size, size), dtype=np.uint8)
    action = np.zeros((N, 2), dtype=np.uint8)

    # the first move
    state[:, 0, ...] = 1
    action[0] = data.iloc[0, :2]

    for i in range(1, N):
        action[i] = data.iloc[i, :2]
        lastX, lastY = action[i - 1]

        state[i:, 0, lastX, lastY] = 0

        state[i, 1, ...] = state[i - 1, 2, ...]
        state[i, 2, ...] = state[i - 1, 1, ...]

        state[i, 2, lastX, lastY] = 1

        state[i, 3, ...] = state[i - 1, 4, ...]
        state[i, 4, ...] = state[i - 1, 3, ...]

        state[i, 5, ...] = state[i - 1, 6, ...]
        state[i, 6, ...] = state[i - 1, 5, ...]

        board = state[i, 2]
        connect_num(board, state[i, 4], 3)
        connect_num(board, state[i, 6], 4)

        for x, y in itertools.product(range(size), range(size)):
            if state[i, 6, x, y] != 0:
                state[i, 4, x, y] = 0

    label = np.zeros(N, dtype=np.uint8)
    for i in range(N):
        label[i] = action[i, 0] * size + action[i, 1]

    return state[7:, ...], label[7:]


def to_lmdb(X, y, name):
    N = X.shape[0]
    channels = X.shape[1]
    height = size
    width = size
    env = lmdb.open(name, map_size=1099511627776)

    max_key = env.stat()["entries"]

    with env.begin(write=True) as txn:
        for i in range(N):
            datum = caffe.proto.caffe_pb2.Datum()
            datum.channels = channels
            datum.height = height
            datum.width = width
            datum.data = X[i].tobytes()  # or .tostring() if numpy < 1.9
            datum.label = int(y[i])
            str_id = '{:08}'.format(max_key + 1 + i)

            # The encode is only essential in Python 3
            txn.put(str_id.encode('ascii'), datum.SerializeToString())
    env.close()


def main():
    path = "./renju_csv/"
    X = np.zeros((0, 7, size, size))
    y = np.zeros(0, dtype=np.uint8)
    print y
    i = 0
    for file in os.listdir(path):
        i += 1
        if file.endswith(".csv") and not file.startswith("._"):
            print "Processing " + file + "..."
            state, label = handle(path + file)
            X = np.vstack([X, state])
            y = np.hstack([y, label])
            if i == 50:
                print "Writing to LMDB..."
                X_train, X_test, y_train, y_test = train_test_split(
                    X, y, test_size=0.1, random_state=np.random.randint(23))
                to_lmdb(X_train, y_train, "train")
                to_lmdb(X_test, y_test, "test")
                X = np.zeros((0, 7, size, size))
                y = np.zeros(0, dtype=np.uint8)
                i = 0

    X, Xt, y, yt = train_test_split(
        X, y, test_size=0.1, random_state=np.random.randint(42))

    to_lmdb(X, y, "train")
    to_lmdb(Xt, yt, "test")

if __name__ == "__main__":
    main()
