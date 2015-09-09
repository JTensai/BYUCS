package server;

import java.io.*;
import java.net.*;
import java.nio.file.Files;

import com.sun.net.httpserver.*;
import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;

import shared.communication.*;
import server.database.*;

public class Server {

  private static int port_number = 8085;
  private static final int MAX_WAITING_CONNECTIONS = 10;

  private HttpServer server;
  private XStream xmlStream = new XStream(new DomDriver());
  private ServerFacade facade = new ServerFacade();

  private Server() {
  }

  private Server(int port_num) {
    port_number = port_num;
    System.out.println("Port set to: " + port_num);
  }

  private void run() {
    System.out.println("Server started: " + port_number);
    try {
      Database.initialize();
    } catch (ServerException e) {
      System.out.println("Could not initialize database: " + e.getMessage());
      e.printStackTrace();
      return;
    }

    try {
      server = HttpServer.create(new InetSocketAddress(port_number), MAX_WAITING_CONNECTIONS);
    } catch (IOException e) {
      System.out.println("Could not create HTTP server: " + e.getMessage());
      e.printStackTrace();
      return;
    }

    server.setExecutor(null); // use the default executor

    server.createContext("/ValidateUser", validateUserHandler);
    server.createContext("/GetProjects", getProjectsHandler);
    server.createContext("/GetSampleImage", getSampleImageHandler);
    server.createContext("/DownloadBatch", downloadBatchHandler);
    server.createContext("/SubmitBatch", submitBatchHandler);
    server.createContext("/GetFields", getFieldsHandler);
    server.createContext("/Search", searchHandler);
    server.createContext("/", downloadFileHandler);

    server.start();
  }

  private HttpHandler validateUserHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      ValidateUser_Params params = (ValidateUser_Params) xmlStream.fromXML(exchange.getRequestBody());
      ValidateUser_Result result = facade.ValidateUser(params);

      exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
      xmlStream.toXML(result, exchange.getResponseBody());
      exchange.getResponseBody().close();
    }
  };

  private HttpHandler getProjectsHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      GetProjects_Params params = (GetProjects_Params) xmlStream.fromXML(exchange.getRequestBody());
      GetProjects_Result result = facade.getProjects(params);

      exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
      xmlStream.toXML(result, exchange.getResponseBody());
      exchange.getResponseBody().close();
    }
  };

  private HttpHandler getSampleImageHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      GetSampleImage_Params params = (GetSampleImage_Params) xmlStream.fromXML(exchange.getRequestBody());
      GetSampleImage_Result result = facade.getSampleImage(params);

      exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
      xmlStream.toXML(result, exchange.getResponseBody());
      exchange.getResponseBody().close();
    }
  };

  private HttpHandler downloadBatchHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      DownloadBatch_Params params = (DownloadBatch_Params) xmlStream.fromXML(exchange.getRequestBody());
      DownloadBatch_Result result = facade.downloadBatch(params);

      exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
      xmlStream.toXML(result, exchange.getResponseBody());
      exchange.getResponseBody().close();
    }
  };

  private HttpHandler submitBatchHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      SubmitBatch_Params params = (SubmitBatch_Params) xmlStream.fromXML(exchange.getRequestBody());
      SubmitBatch_Result result = facade.submitBatch(params);

      exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
      xmlStream.toXML(result, exchange.getResponseBody());
      exchange.getResponseBody().close();
    }
  };

  private HttpHandler getFieldsHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      GetFields_Params params = (GetFields_Params) xmlStream.fromXML(exchange.getRequestBody());
      GetFields_Result result = facade.getFields(params);

      exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
      xmlStream.toXML(result, exchange.getResponseBody());
      exchange.getResponseBody().close();
    }
  };

  private HttpHandler searchHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      Search_Params params = (Search_Params) xmlStream.fromXML(exchange.getRequestBody());
      Search_Result result = facade.search(params);

      exchange.sendResponseHeaders(HttpURLConnection.HTTP_OK, 0);
      xmlStream.toXML(result, exchange.getResponseBody());
      exchange.getResponseBody().close();
    }
  };

  private HttpHandler downloadFileHandler = new HttpHandler() {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
      File f = new File("assets" + File.separator + exchange.getRequestURI().getPath());
      if (f.exists()) {
        exchange.sendResponseHeaders(200, 0);
        Files.copy(f.toPath(), exchange.getResponseBody());
        exchange.getResponseBody().close();
      }
    }
  };

  public static void main(String[] args) {
    if (args.length > 0)
      new Server(Integer.parseInt(args[0])).run();
    else
      new Server().run();
  }

}
