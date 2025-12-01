# Drew Schlesener, Kevin Cunningham and Denton Jarvis
# Slurm Parser for scheduling Libpressio and OptZConfig

from multiprocessing import Process, Queue
import os

def worker(q):
    os.system(q)


if __name__ == '__main__':
    q = './parser'
    p = Process(target=worker, args=(q,))
    p.start()
    # print(q.get())
    p.join()