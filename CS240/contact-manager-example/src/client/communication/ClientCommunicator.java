package client.communication;

import shared.communication.*;
import client.*;

public class ClientCommunicator {

  public ClientCommunicator() {
  }

  public GetAllContacts_Result getAllContacts() throws ClientException {
    return (GetAllContacts_Result) doGet("/GetAllContacts");
  }

  public void addContact(AddContact_Params params) throws ClientException {
    doPost("/AddContact", params);
  }

  public void updateContact(UpdateContact_Params params) throws ClientException {
    doPost("/UpdateContact", params);
  }

  public void deleteContact(DeleteContact_Params params) throws ClientException {
    doPost("/DeleteContact", params);
  }

  private Object doGet(String urlPath) throws ClientException {
    // Make HTTP GET request to the specified URL,
    // and return the object returned by the server
    return null;
  }

  private void doPost(String urlPath, Object postData) throws ClientException {
    // Make HTTP POST request to the specified URL,
    // passing in the specified postData object
  }

}
