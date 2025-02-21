#!/usr/bin/python3
import unittest
import importlib
libTimeSync = importlib.import_module("libudp_time_sync")
# import libcarma_clock
import time


"""Test Case for testing basic udp-time-sync functionality in python
"""
class TestCarmaClock(unittest.TestCase):

    def testSimClockInitializeException(self):
        timeSync = libTimeSync.TimeSync()
        import socket
        # Define the server's address and port
        SERVER_ADDRESS = ('127.0.0.1', 4567)

        # Create a UDP socket
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Send data to the server
        message = r'{"timestep": 1, "timestamp": 100}'
        timeSync.start()

        client_socket.sendto(message.encode('utf-8'), SERVER_ADDRESS)
        self.assertEqual( timeSync.nowInMilliseconds(), 100)
        client_socket.close()



if __name__ == '__main__': 
    unittest.main()
