package server;

import java.io.*;
import java.net.*;
import java.util.*;

import com.sun.net.httpserver.*;
import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;

import shared.model.*;
import server.database.*;


public class Server {

	private static final int SERVER_PORT_NUMBER = 8081;
	private static final int MAX_WAITING_CONNECTIONS = 10;
	
	private HttpServer server;
	private XStream xmlStream = new XStream(new DomDriver());
	
	private Server() {
		return;
	}
	
	private void run() {
		try {
			Database.initialize();		
		}
		catch (ServerException e) {
			System.out.println("Could not initialize database: " + e.getMessage());
			e.printStackTrace();
			return;
		}
		
		try {
			server = HttpServer.create(new InetSocketAddress(SERVER_PORT_NUMBER),
											MAX_WAITING_CONNECTIONS);
		} 
		catch (IOException e) {
			System.out.println("Could not create HTTP server: " + e.getMessage());
			e.printStackTrace();
			return;
		}

		server.setExecutor(null); // use the default executor
		
		server.createContext("/GetAllContacts", getAllContactsHandler);
		server.createContext("/AddContact", addContactHandler);
		server.createContext("/UpdateContact", updateContactHandler);
		server.createContext("/", emptyHandler);
		
		server.start();
	}
	
	private HttpHandler emptyHandler = new HttpHandler() {

		@Override
		public void handle(HttpExchange exchange) throws IOException {
			System.out.println("GOT HERE");
		}
	};
	

	private HttpHandler getAllContactsHandler = new HttpHandler() {

		@Override
		public void handle(HttpExchange exchange) throws IOException {
			Database db = new Database();
			List<Contact> contacts = null;
			
			try {
				db.startTransaction();
				contacts = db.getContacts().getAll();
				db.endTransaction(true);
			}
			catch (ServerException e) {
				System.out.println(e.getMessage());
				e.printStackTrace();
				db.endTransaction(false);
				exchange.sendResponseHeaders(HttpURLConnection.HTTP_INTERNAL_ERROR, -1);
				return;
			}
			
			exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
			xmlStream.toXML(contacts, exchange.getResponseBody());
			exchange.getResponseBody().close();
		}
	};

	private HttpHandler addContactHandler = new HttpHandler() {

		@Override
		public void handle(HttpExchange exchange) throws IOException {
			Database db = new Database();
			Contact contact = (Contact)xmlStream.fromXML(exchange.getRequestBody());
			
			try {
				db.startTransaction();
				db.getContacts().add(contact);
				db.endTransaction(true);
			}
			catch (ServerException e) {
				System.out.println(e.getMessage());
				e.printStackTrace();
				db.endTransaction(false);
				exchange.sendResponseHeaders(HttpURLConnection.HTTP_INTERNAL_ERROR, -1);
				return;
			}
			
			exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, -1);
		}
	};

	private HttpHandler updateContactHandler = new HttpHandler() {

		@Override
		public void handle(HttpExchange exchange) throws IOException {
			Database db = new Database();
			Contact contact = (Contact)xmlStream.fromXML(exchange.getRequestBody());
			
			try {
				db.startTransaction();
				db.getContacts().update(contact);
				db.endTransaction(true);
			}
			catch (ServerException e) {
				System.out.println(e.getMessage());
				e.printStackTrace();
				db.endTransaction(false);
				exchange.sendResponseHeaders(HttpURLConnection.HTTP_INTERNAL_ERROR, -1);
				return;
			}
			
			exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, -1);
		}
	};

	private HttpHandler deleteContactHandler = new HttpHandler() {

		@Override
		public void handle(HttpExchange exchange) throws IOException {
			Database db = new Database();
			Contact contact = (Contact)xmlStream.fromXML(exchange.getRequestBody());
			
			try {
				db.startTransaction();
				db.getContacts().delete(contact);
				db.endTransaction(true);
			}
			catch (ServerException e) {
				System.out.println(e.getMessage());
				e.printStackTrace();
				db.endTransaction(false);
				exchange.sendResponseHeaders(HttpURLConnection.HTTP_INTERNAL_ERROR, -1);
				return;
			}
			
			exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, -1);
		}
	};
	
	public static void main(String[] args) {
		new Server().run();
	}

}
