import errno
import select
import socket
import sys
import traceback
import os
import time

class Server:
    """ Polling server """
    def __init__(self,port,conf_path):
        self.host = ""
        self.port = port
        self.conf_path = conf_path
        self.clients = {}
        self.conf_hosts = {}
        self.conf_media = {}
        self.conf_params = {}
        self.size = 1024
        self.parse_web_conf()
        self.open_socket()

    def parse_web_conf(self):
        if self.conf_path[-8:] == 'web.conf':
            # print 'contains file'
            conf_file = open(self.conf_path)
        else:
            # print 'does not contain file'
            conf_file = open(self.conf_path + 'web.conf')

        for line in conf_file:
            line = line.split(' ')
            if line[0] == 'host':
                self.conf_hosts[line[1]] = line[2].split('\n')[0]
                # print 'host found'
            elif line[0] == 'media':
                self.conf_media[line[1]] = line[2].split('\n')[0]
                # print 'media found'
            elif line[0] == 'parameter':
                self.conf_params[line[1]] = line[2].split('\n')[0]
                # print 'parameter found'

        # print self.conf_hosts
        # print self.conf_media
        # print self.conf_params

    def open_socket(self):
        """ Setup the socket for incoming clients """
        try:
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
            self.server.bind((self.host,self.port))
            self.server.listen(5)
            self.server.setblocking(0)
        except socket.error, (value,message):
            if self.server:
                self.server.close()
            print "Could not open socket: " + message
            sys.exit(1)

    def run(self):
        """ Use poll() to handle each incoming client."""
        self.poller = select.epoll()
        self.pollmask = select.EPOLLIN | select.EPOLLHUP | select.EPOLLERR
        self.poller.register(self.server,self.pollmask)
        idle_sockets = {}
        while True:
            # poll sockets
            try:
                # poll checks to see if any sockets have something to be done
                # waits for n number or seconds before continuing on with the code

                start_time = time.time()
                fds = self.poller.poll(timeout=1)
                end_time = time.time()
                duration = end_time-start_time



                for soc in self.clients: # each socket that is open
                    if soc in idle_sockets: # if the socket has already been seen
                        idle_sockets[soc] += duration
                        # print "Socket %d idle for %.2f" % (soc, idle_sockets[soc])
                    else:   # new socket
                        idle_sockets[soc] = 0
                        # print "Socket %d created" % soc

                for soc in [x[0] for x in fds]: # socket that just had an event
                    idle_sockets[soc] = 0

                new_idle_sockets = dict(idle_sockets)
                for soc in idle_sockets:
                    # if soc is not a key in self.clients remove it
                    if self.clients.get(soc) == None:
                        new_idle_sockets.pop(soc)

                    if idle_sockets[soc] > (int(self.conf_params['timeout'])-1):
                        self.clients[soc].close()
                        self.clients.pop(soc)
                        new_idle_sockets.pop(soc)

                idle_sockets = new_idle_sockets



            except:
                # print "Except was hit"
                # print traceback.print_exc()
                return
            for (fd,event) in fds:
                # handle errors
                if event & (select.POLLHUP | select.POLLERR):
                    self.handleError(fd)
                    continue
                # handle the server socket
                if fd == self.server.fileno():
                    self.handleServer()
                    continue
                # handle client socket
                result = self.handleClient(fd)

    def handleError(self,fd):
        self.poller.unregister(fd)
        if fd == self.server.fileno():
            # recreate server socket
            self.server.close()
            self.open_socket()
            self.poller.register(self.server,self.pollmask)
        else:
            # close the socket
            self.clients[fd].close()
            del self.clients[fd]

    def handleServer(self):
        # accept as many clients as possible
        while True:
            try:
                (client,address) = self.server.accept()
            except socket.error, (value,message):
                # if socket blocks because no clients are available,
                # then return
                if value == errno.EAGAIN or errno.EWOULDBLOCK:
                    return
                print traceback.format_exc()
                sys.exit()
            # set client socket to be non blocking
            client.setblocking(0)
            self.clients[client.fileno()] = client
            self.poller.register(client.fileno(),self.pollmask)

    def handleClient(self,fd):
        try:
            data = self.clients[fd].recv(self.size)
        except socket.error, (value,message):
            # if no data is available, move on to another client
            if value == errno.EAGAIN or errno.EWOULDBLOCK:
                return
            print traceback.format_exc()
            sys.exit()

        if data:
            self.createResponse(data,fd)
        else:
            self.poller.unregister(fd)
            self.clients[fd].close()
            del self.clients[fd]

    def createResponse(self,data,fd):
        request = HTTPRequest(data)
        response_status = ''
        response_body = ''
        date_format = '%a, %d %b %Y %H:%M:%S GMT'
        response_headers = {
            'Server': 'localhost',
            'Content-Length': 0,
            'Content-Type': 'text/html; encoding=utf8',
            'Date': time.strftime(date_format, time.gmtime(time.time())),
            'Last-Modified': '',
            'Connection': 'Keep-Alive'
        }

        # print 'Error: ' + str(request.error_code)
        if str(request.error_code) == 'None':
            if request.command == 'GET':
                request.path = request.path[1:]
                if request.path == '':
                    request.path = 'index.html'
                # print request.path
                try:
                    open_file = open(request.path)
                    response_status = 'HTTP/1.1 200 OK\r\n'

                    response_body = open_file.read()

                    size = os.stat(request.path).st_size
                    response_headers['Content-Length'] = size

                    extension = request.path.split('.')[-1]
                    content_type = self.conf_media[extension]
                    response_headers['Content-Type'] = content_type + '; encoding=utf8'

                    mod_time = os.stat(request.path).st_mtime
                    response_headers['Last-Modified'] = time.strftime(date_format, time.gmtime(mod_time))
                except IOError as (errno,strerror):
                    if errno == 13:
                        # print 'Forbidden'
                        response_status = 'HTTP/1.1 403 Forbidden\r\n'
                        response_body = '<html><body><h1>Error 403 Forbidden</h1></body></html>'
                        size = len(response_body)
                        response_headers['Content-Length'] = size
                    elif errno == 2:
                        # print 'Not Found'
                        response_status = 'HTTP/1.1 404 Not Found\r\n'
                        response_body = '<html><body><h1>Error 404 File Not Found</h1></body></html>'
                        size = len(response_body)
                        response_headers['Content-Length'] = size
                    else:
                        # print 'Internal Server Error'
                        response_status = 'HTTP/1.1 500 Internal Server Error\r\n'
                        size = len(response_body)
                        response_body = '<html><body><h1>Error 500 - Internal Server Error</h1></body></html>'
                        response_headers['Content-Length'] = size
            
            elif request.command == "HEAD":
                # print request.command
                response_status = 'HTTP/1.1 501 Not Implemented\r\n'
                response_body = '<html><body><h1>Error 501 Not Implemented</h1></body></html>'
                response_headers['Content-Length'] = len(response_body)
            elif (request.command == 'DELETE') or (request.command == 'PUT') or (request.command == 'POST') or (request.command == 'OPTIONS') or (request.command == 'TRACE') or (request.command == 'CONNECT'):
                # print request.command
                response_status = 'HTTP/1.1 501 Not Implemented\r\n'
                response_body = '<html><body><h1>Error 501 Not Implemented</h1></body></html>'
                response_headers['Content-Length'] = len(response_body)
            else:
                # print request.command
                response_status = 'HTTP/1.1 400 Bad Request\r\n'
                response_body = '<html><body><h1>Error 400 - Bad Request</h1></body></html>'
                response_headers['Content-Length'] = len(response_body)
        elif request.error_code == 400:
            response_status = 'HTTP/1.1 400 Bad Request\r\n'
            response_body = '<html><body><h1>Error 400 Bad Request</h1></body></html>'
            response_headers['Content-Length'] = len(response_body)
        elif request.error_code == 501:
            response_status = 'HTTP/1.1 501 Not Implemented\r\n'
            response_body = '<html><body><h1>Error 501 Not Implemented</h1></body></html>'
            response_headers['Content-Length'] = len(response_body)

        response_headers_raw = ''.join('%s: %s\r\n' % (k, v) for k, v in response_headers.iteritems())
        response = response_status + response_headers_raw + '\r\n' + response_body

        self.clients[fd].send(response)


from BaseHTTPServer import BaseHTTPRequestHandler
from StringIO import StringIO

class HTTPRequest(BaseHTTPRequestHandler):
    def __init__(self, request_text):
        self.rfile = StringIO(request_text)
        self.raw_requestline = self.rfile.readline()
        self.error_code = self.error_message = None
        self.parse_request()

    def send_error(self, code, message):
        self.error_code = code
        self.error_message = message