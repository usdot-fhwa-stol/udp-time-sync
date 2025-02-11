#!/usr/bin/python3
import unittest
import importlib
libTimeSync = importlib.import_module("libcarma_streets_time_sync")
# import libcarma_clock
import time

"""Test Case for testing basic CarmaClock functionality in python
"""
class TestCarmaClock(unittest.TestCase):

    def testSimClockInitializeException(self):
        timeSync = libTimeSync.TimeSync()
        timeSync.start()


if __name__ == '__main__': 
    unittest.main()
