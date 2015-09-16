import socket
import sys

class Client:
    """ Echo client """
    def __init__(self,host,port):
        self.host = host
        self.port = port
        self.size = 1024
        self.open_socket()
        print "Enter a blank line to stop."
        sys.stdout.write('> ')

    def open_socket(self):
        """ Connect to the server """
        try:
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server.connect((self.host,self.port))
        except socket.error, (value,message):
            if self.server:
                self.server.close()
            print "Could not open socket: " + message
            sys.exit(1)

    def run(self):
        """ Read from the keyboard and send this line to the server """
        message = ""
        keep_going = 1 
        while keep_going:
            # read from keyboard
            line = sys.stdin.readline()
            first = 0
            if line == '\n':
                first = 1
                sys.stdout.write('> ')
                line = sys.stdin.readline()
                if first:
                    if line == '\n':
                        keep_going = 0
                        self.server.send(line)
                        data = self.server.recv(self.size)
                        sys.stdout.write(data)
            message += line
            sys.stdout.write('> ')
        self.server.close()
