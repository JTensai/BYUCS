#include "client.h"

Client::Client(string host, int port, bool debug) {
    // setup variables
    host_ = host;
    port_ = port;
    buflen_ = 1;
    buf_ = new char[buflen_+1];
    this->debug = debug;
}

Client::~Client() {
}

void Client::run() {
    // connect to the server and run echo program
    //create();
    echo();
}

void
Client::create() {
    struct sockaddr_in server_addr;

    // use DNS to get IP address
    struct hostent *hostEntry;
    hostEntry = gethostbyname(host_.c_str());
    if (!hostEntry) {
		
		if (debug) {
			cout << "No such host name: " << host_ << endl;
		}
        exit(-1);
    }

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        exit(-1);
    }

    // connect to server
    if (connect(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("connect");
        exit(-1);
    }
}

void
Client::close_socket() {
    close(server_);
}

vector<string> 
Client::split(string s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> elements;
	while(getline(ss, item, delim)) {
		elements.push_back(item);
	}
	return elements;
}

void
Client::handle_send(string user, string subject) {
	if (debug) {
		cout << "Sending to user " + user + " subject: " + subject  << endl;
	}
	cout << "- Type your message. End with a blank line -" << endl;
	string line;
	string message = "";
	while (getline(cin, line)) {
		if (line == "") {
			stringstream out;
			out << message.length() -1;
			send_request("put " + user + " " + subject + " " + out.str() + "\n");
			send_request(message);
			cout << get_response();
			break;
		}
		else {
			message += line + "\n";
		}
	}
}

void
Client::handle_list(string user) {
	if (debug) {
		cout << "Handle list on user: " + user << endl;
	}
	send_request("list " + user + "\n");
	string response = get_response(); 
	vector<string> elements = split(response, ' ');
	if (elements[0] == "error") {
		cout << "error in the response" << endl;
	}
	
	int number_of_messages = atoi(elements.at(1).substr(0, elements.at(1).size() -1).c_str());
	cout << response;
	for (unsigned int i = 0; i < number_of_messages; i++) {
		cout << get_response();
	}
}

void
Client::handle_read(string user, int index) {
	if (debug) {
		cout << "handle read on user " + user << endl;
	}
	stringstream out;
	out << index;
	send_request("get " + user + " " + out.str() + "\n");
	cout << get_response();
}

void
Client::echo() {
    string line;
    // loop to handle user interface
    cout << "% "; 
    while (getline(cin,line)) {
		create();
		if (debug) {
			cout << "Received command " + line << endl;
		}
		try {
			vector<string> elements = split(line, ' ');
			if (elements[0] == "send") {
				handle_send(elements[1], elements[2]);
			}
			else if (elements[0] == "list") {
				handle_list(elements[1]);
			}
			else if (elements[0] == "read") {
				handle_read(elements.at(1), atoi(elements.at(2).c_str()));
			}
			else if (elements[0] == "quit") {
				break;
			}
			else {
				cout << "error Invalid Command" << endl;
			}
		}
		catch (...) {
			cout << "error Invalid command" << endl;	
		}
		cout << "% ";
		close_socket();
    }
    close_socket();
}

bool
Client::send_request(string request) {
	if (debug) {
		cout << "Sending Request " << request << endl;
	}
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    if (debug) {
		cout << "Request sent" << endl;
	}
    return true;
}

string
Client::get_response() {
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {
        int nread = recv(server_,buf_, buflen_, 0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
    }
    // a better client would cut off anything after the newline and
    // save it in a cache
    return response;
}
