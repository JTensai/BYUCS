package client.communication;

import java.util.List;

import shared.model.Contact;
import client.ClientException;

public class ClientCommunicatorFascadeForContactManager {
  private static ClientCommunicatorFascadeForContactManager fascade = null;

  public static ClientCommunicatorFascadeForContactManager singleton() {
    if (fascade == null) {
      fascade = new ClientCommunicatorFascadeForContactManager();
    }
    return fascade;
  }

  private ClientCommunicatorFascadeForContactManager() {
  }

  @SuppressWarnings("unchecked")
  public List<Contact> getAllContacts() throws ClientException {
    return (List<Contact>) ClientCommunicator.getSingleton().doGet(GET_ALL_CONTACTS).getResponseBody();
  }

  public void addContact(Contact contact) throws ClientException {
    ClientCommunicator.getSingleton().doPost(ADD_CONTACT, contact);
  }

  public void updateContact(Contact contact) throws ClientException {
    ClientCommunicator.getSingleton().doPost(UPDATE_CONTACT, contact);
  }

  public void deleteContact(Contact contact) throws ClientException {
    ClientCommunicator.getSingleton().doPost(DELETE_CONTACT, contact);
  }

  // Local Methods and Constants
  private static final String GET_ALL_CONTACTS = "GetAllContacts";
  private static final String ADD_CONTACT = "AddContact";
  private static final String UPDATE_CONTACT = "UpdateContact";
  private static final String DELETE_CONTACT = "DeleteContact";
}
