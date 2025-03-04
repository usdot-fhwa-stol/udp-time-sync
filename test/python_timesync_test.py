#!/usr/bin/python3
import unittest
import importlib
libTimeSync = importlib.import_module("libudp_time_sync")
import time
import socket
import os




"""Test Case for testing basic udp-time-sync functionality in python
"""
class TestCarmaClock(unittest.TestCase):

    def testSimClockInitializeException(self):
        os.environ['SIMULATION_MODE'] = 'TRUE'

        timeSync = libTimeSync.TimeSync()
        timeSync.start()

        # Define the server's address and port
        SERVER_ADDRESS = ('127.0.0.1', 4567)
        # Create a UDP socket
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Send data to the server
        message = r'{ "timestep": 100, "seq": 1}'
        client_socket.sendto(message.encode('utf-8'), SERVER_ADDRESS)


        self.assertEqual( libTimeSync.nowInMilliseconds(), 100)
        client_socket.close()



if __name__ == '__main__': 
    unittest.main()
