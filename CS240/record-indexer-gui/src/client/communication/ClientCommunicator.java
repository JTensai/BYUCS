package client.communication;

import java.io.*;
import java.net.*;

import client.ClientException;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;

import shared.communication.*;

public class ClientCommunicator {

  // Auxiliary Constants, Attributes, and Methods
  private static String SERVER_HOST = "localhost";
  private static String SERVER_PORT = "8080";
  private static String URL_PREFIX = "http://" + SERVER_HOST + ":" + SERVER_PORT;
  // private static final String HTTP_GET = "GET";
  private static final String HTTP_POST = "POST";

  private XStream xmlStream;

  public ClientCommunicator() {
    xmlStream = new XStream(new DomDriver());
    // needs to have port and host passed in so we can locate tbe server
  }

  public ClientCommunicator(String host, String port) {
    xmlStream = new XStream(new DomDriver());

    SERVER_HOST = host;
    SERVER_PORT = port;
    URL_PREFIX = "http://" + SERVER_HOST + ":" + SERVER_PORT;

    // needs to have port and host passed in so we can locate tbe server
  }

  /**
   * Validates if the given username and password match a user in the database
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public ValidateUser_Result ValidateUser(ValidateUser_Params params) throws ClientException {
    return (ValidateUser_Result) doPost("/ValidateUser", params);
  }

  /**
   * Gets an array of all the projects that are in the database
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public GetProjects_Result getProjects(GetProjects_Params params) throws ClientException {
    return (GetProjects_Result) doPost("/GetProjects", params);
  }

  /**
   * Gets a sample image from the database
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public GetSampleImage_Result getSampleImage(GetSampleImage_Params params) throws ClientException {
    return (GetSampleImage_Result) doPost("/GetSampleImage", params);
  }

  /**
   * Loads a batch to be evaluated by the user and locks it from other users
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public DownloadBatch_Result downloadBatch(DownloadBatch_Params params) throws ClientException {
    return (DownloadBatch_Result) doPost("/DownloadBatch", params);
  }

  /**
   * Submits user input back to the database to be stored for the batch
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public SubmitBatch_Result submitBatch(SubmitBatch_Params params) throws ClientException {
    return (SubmitBatch_Result) doPost("/SubmitBatch", params);
  }

  /**
   * Gets all the fields associated with a specific project
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public GetFields_Result getFields(GetFields_Params params) throws ClientException {
    return (GetFields_Result) doPost("/GetFields", params);
  }

  /**
   * Returns results of the search query
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public Search_Result search(Search_Params params) throws ClientException {
    return (Search_Result) doPost("/Search", params);
  }

  /**
   * Downloads the file
   * 
   * @param params
   * @return
   * @throws ClientException
   */
  public DownloadFile_Result downloadFile(DownloadFile_Params params) throws ClientException {
    return (DownloadFile_Result) doPost("/DownloadFile", params);
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
  // public HttpURLResponse doGet(String commandName) throws ClientException {
  // assert commandName != null && commandName.length() > 0;
  //
  // HttpURLResponse result = new HttpURLResponse();
  // try {
  // URL url = new URL(URL_PREFIX + "/" + commandName);
  // HttpURLConnection connection = (HttpURLConnection) url.openConnection();
  // connection.setRequestMethod(HTTP_GET);
  // connection.connect();
  //
  // result.setResponseCode(connection.getResponseCode());
  // result.setResponseLength(connection.getContentLength());
  // if (connection.getResponseCode() == HttpURLConnection.HTTP_OK) {
  // if (connection.getContentLength() == 0) {
  // System.out.println("test");
  // result.setResponseBody(xmlStream.fromXML(connection.getInputStream()));
  // }
  // }
  // else {
  // throw new ClientException(String.format("doGet failed: %s (http code %d)",
  // commandName, connection.getResponseCode()));
  // }
  // }
  // catch (IOException e) {
  // throw new ClientException(String.format("doGet failed: %s",
  // e.getMessage()), e);
  // }
  // return result;
  // }

  public Object doPost(String commandName, Object postData) throws ClientException {
    try {
      URL url = new URL(URL_PREFIX + commandName);
      HttpURLConnection connection = (HttpURLConnection) url.openConnection();
      connection.setRequestMethod(HTTP_POST);
      connection.setDoOutput(true);
      connection.connect();
      xmlStream.toXML(postData, connection.getOutputStream());
      connection.getOutputStream().close();

      if (connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
        throw new ClientException(String.format("doPost failed: %s (http code %d)", commandName, connection.getResponseCode()));
      }
      return xmlStream.fromXML(connection.getInputStream());
    } catch (IOException e) {
      throw new ClientException(String.format("doPost failed: %s", e.getMessage()), e);
    }
  }
}
