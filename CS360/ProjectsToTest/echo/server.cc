#include "server.h"

Server::Server(int port, bool debug) {
    // setup variables
    port_ = port;
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
    this->debug = debug;
}

Server::~Server() {
    delete buf_;
}

void
Server::run() {
    // create and run the server
    create();
    serve();
}

void
Server::create() {
    struct sockaddr_in server_addr;

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

    // call bind to associate the socket with our local address and
    // port
    if (bind(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

    // convert the socket to listen for incoming connections
    if (listen(server_,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }
}

void
Server::close_socket() {
	if (debug) {
		cout << "Closing Server" << endl;
	}
    close(server_);
}

void
Server::serve() {
    // setup client
    while (true) 
    {
		int client;
		struct sockaddr_in client_addr;
		socklen_t clientlen = sizeof(client_addr);

		  // accept clients
		while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {

			handle(client);
		}
		if (debug) {
			cout << "No longer accepting clients..." << endl;
		}
		close_socket();
	}
}

void
Server::handle(int client) {
    while (true) {
		// get a request
		string request = get_request(client);
		if (request.size() == 0) {
			if (debug) {
				cout << "Client Closed Connection" << endl;
			}
			break;
		}
		bool success = send_response(client,request);
		// break if an error occurred
		if (not success && debug) {
			cout << "Failure in transmission" << endl;
		}

		if (debug) {
			cout << "Closing connection to Client" << endl;
		}
	}
	close(client);
}

vector<string> 
Server::split(string s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> elements;
	while(getline(ss, item, delim)) {
		elements.push_back(item);
	}
	return elements;
}

bool
Server::check_put(string request) {

	vector<string> elements = split(request, ' ');
	
	if (elements.size() >= 4 && elements[0] == "put") {
	
		int length = atoi(elements.at(3).c_str());
		vector<string> elements2 = split(request, '\n');
		if (elements.size() < 4) {
			return false;
		}
		else {
			string lengthString = "";
			for (int i = 1; i < elements2.size(); i++) {
				lengthString += elements2[i];
				if (i != elements2.size() -1) {
					lengthString += "\n";
				}
			}

			if (lengthString.size() < length) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	return true;
}

string
Server::get_request(int client) {
	if (debug) {
		cout << "Getting Request from Client" << endl;
	}
    string request = "";
    // read until we have all the data
    while (true) {
		if (debug) {
			cout << "Beginning Read from Client" << endl;
		}
        int nread = recv(client,buf_, buflen_, 0);

        if (nread < 0) {
			if (debug) {
				cout << "There was an error in reading" << endl;
			}
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
			}
            else {
                // an error occurred, so break out
                return "error an error occured\n";
			}
        } 
        else if (nread == 0) {
			if (debug) {
				cout << "No Data Read, closing connection" << endl;
			}
			return "";
		}

        // be sure to use append in case we have binary data
        request.append(buf_, nread);
		vector<string> elementsSoFar = split(request, ' ');
		if (nread < buflen_ && check_put(request)) {
			if (debug) {
				cout << "Finished Message from Client" << endl;
			}
			break;
		}
    }
    if (debug) {
		cout << "Got Request " << request << endl;
	}
    // a better server would cut off anything after the newline and
    // save it in a cache
    vector<string> elements = split(request, ' ');
    try {
		if (elements.at(0) == "put") {
		
			string name = elements.at(1);
			string subject = elements.at(2);
			int length = atoi(elements.at(3).c_str());

			if (length <= 0) {
				return "error invalid number of bytes\n";
			}
			vector <string> elements2 = split(request, '\n');
			string data = "";
			for (int i = 1; i < elements2.size(); i++) {
				data += elements2[i];
				if (i != elements2.size() -1) {
					data += "\n";
				}
			}

			if (data == "" || data.length() != length ) {
				return "error expected more data\n";
			}
			Message msg = Message(subject, data);
			
			cache[name].push_back(msg);

			return "OK\n";
		}
		else if (elements.at(0) == "list") {
			string name = elements.at(1); // strip of the \n
			name = name.substr(0, name.size() -1);
			stringstream out1;
			
			out1 << cache[name].size();
			string response = "list " + out1.str() + "\n";
			for (unsigned int i = 0; i < cache[name].size(); i++) {
				stringstream out;
				out << i + 1;
				response += out.str() + " ";
				response += cache[name].at(i).getSubject() + "\n";
			}
			return response;
		}
		else if (elements.at(0) == "get") {
			string name = elements.at(1);
			int index = atoi(elements.at(2).substr(0, elements.at(2).size() -1).c_str()) - 1;
			if (cache[name].size() <= index || index < 0) {
				return "error index invalid for get\n";
			}
			Message msg = cache[name].at(index);
			stringstream out;
			out << msg.getMessage().size();
			return "message " + msg.getSubject() + " " + out.str() + "\n" + msg.getMessage();
		}
		else if (elements.at(0) == "reset\n") {
			cache.clear();
			return "OK\n";
		}
		else {
			return "error UNKNOWN COMMAND\n";
		}
	}
	catch (exception e) {
		if (debug) {
			cout << "Exception: " << e.what() << endl;
		}
		return "error invalid command\n";
	}
}

string
Server::read_message(int length, int client) {
	string response = "";
	while (response.length() < length) {

		int nread = recv(client, buf_, buflen_ ,0);

        if (nread < 0) {
            if (errno == EINTR) {
				if (debug) {
					cout << "SOMETHING INTERRUPTED ME" << endl;
				}
                // the socket call was interrupted -- try again
                continue;
			}
            else {
				if (debug) {
					cout << "Other error" << endl;
				}
                // an error occurred, so break out
                return "";
			}
        }
        else if (nread == 0) {
            // the socket is closed
            if (debug) {
				cout << "Socket is closed" << endl;
			}
            return "";
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
	}

	return response;
}

bool
Server::send_response(int client, string response) {
	if (debug) {
		cout << "sending response: " + response << endl;
	}
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } 
            else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } 
        else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}
