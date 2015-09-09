package client.communication;

import java.util.*;
import java.io.*;
import java.net.*;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;

import shared.model.*;
import client.*;

/**
 * An example of a client communicator we might use for CS 240. It is not the
 * most general of ClientCommunicators. For instance, it does not allow for the
 * passing of parameters in the URL of a GET method. It also expects the content
 * length of a response to be 0 or -1.
 * 
 * @author Scott Woodfield
 */
public class ClientCommunicator {
  // Class Methods
  /**
   * Used to implement the getSingleton method of the singleton pattern.
   * 
   * @return an ClientCommunicator
   */
  public static ClientCommunicator getSingleton() {
    if (singleton == null) {
      singleton = new ClientCommunicator();
    }
    return singleton;
  }

  // Constructors
  /**
   * Default Constructor. Used only to create the singleton.
   */
  private ClientCommunicator() {
    xmlStream = new XStream(new DomDriver());
  }

  // Queries
  /**
   * The Http Get Method. It sends a request to a server and gets a response.
   * 
   * @param the
   *          name mapped to the corresponding handler in the server on the
   *          other side.
   * @return the response generated during an http communication. The response
   *         object is user defined and not part the java api.
   */
  public HttpURLResponse doGet(String commandName) throws ClientException {
    assert commandName != null && commandName.length() > 0;

    HttpURLResponse result = new HttpURLResponse();
    try {
      URL url = new URL(URL_PREFIX + "/" + commandName);
      HttpURLConnection connection = (HttpURLConnection) url.openConnection();
      connection.setRequestMethod(HTTP_GET);
      connection.connect();

      result.setResponseCode(connection.getResponseCode());
      result.setResponseLength(connection.getContentLength());
      if (connection.getResponseCode() == HttpURLConnection.HTTP_OK) {
        if (connection.getContentLength() == 0) {
          result.setResponseBody(xmlStream.fromXML(connection.getInputStream()));
        }
      } else {
        throw new ClientException(String.format("doGet failed: %s (http code %d)", commandName, connection.getResponseCode()));
      }
    } catch (IOException e) {
      throw new ClientException(String.format("doGet failed: %s", e.getMessage()), e);
    }
    return result;
  }

  public HttpURLResponse doPost(String commandName, Object postData) throws ClientException {
    assert commandName != null;
    assert postData != null;

    HttpURLResponse result = new HttpURLResponse();

    try {
      URL url = new URL(URL_PREFIX + commandName);
      HttpURLConnection connection = (HttpURLConnection) url.openConnection();
      connection.setRequestMethod(HTTP_POST);
      connection.setDoOutput(true);
      connection.connect();
      xmlStream.toXML(postData, connection.getOutputStream());
      connection.getOutputStream().close();

      result.setResponseCode(connection.getResponseCode());
      result.setResponseLength(connection.getContentLength());
      if (connection.getResponseCode() == HttpURLConnection.HTTP_OK) {
        if (connection.getContentLength() == 0) {
          result.setResponseBody(xmlStream.fromXML(connection.getInputStream()));
        }
      } else {
        throw new ClientException(String.format("doPost failed: %s (http code %d)", commandName, connection.getResponseCode()));
      }
    } catch (IOException e) {
      throw new ClientException(String.format("doPost failed: %s", e.getMessage()), e);
    }
    return result;
  }

  // Auxiliary Constants, Attributes, and Methods
  private static final String SERVER_HOST = "localhost";
  private static final int SERVER_PORT = 8080;
  private static final String URL_PREFIX = "http://" + SERVER_HOST + ":" + SERVER_PORT;
  private static final String HTTP_GET = "GET";
  private static final String HTTP_POST = "POST";

  private XStream xmlStream;

  // Singleton Instance
  private static ClientCommunicator singleton = null;
}
