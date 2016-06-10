import lmdb

env = lmdb.open("train", map_size=1099511627776)
print env.info()
print env.stat()
env.close()

env = lmdb.open("test", map_size=1099511627776)
print env.info()
print env.stat()
env.close()