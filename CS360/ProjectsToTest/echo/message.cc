#include "message.h"

Message::Message(string subject, string message) {
	this->message = message;
	this->subject = subject;
}

string
Message::getMessage() {
	return message;
}

void
Message::setMessage(string message) {
	this->message = message;
}

string
Message::getSubject() {
	return subject;
}

void
Message::setSubject(string subject) {
	this->subject = subject;
}

Message::~Message() {
}
