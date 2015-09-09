package shared.model;

import java.util.*;

import server.database.*;

public class Model {

  public static void initialize() throws ModelException {
    try {
      Database.initialize();
    } catch (DatabaseException e) {
      throw new ModelException(e.getMessage(), e);
    }
  }

  public static List<Contact> getAllContacts() throws ModelException {

    Database db = new Database();

    try {
      db.startTransaction();
      List<Contact> contacts = db.getContactsDAO().getAll();
      db.endTransaction(true);
      return contacts;
    } catch (DatabaseException e) {
      db.endTransaction(false);
      throw new ModelException(e.getMessage(), e);
    }
  }

  public static void addContact(Contact contact) throws ModelException {

    Database db = new Database();

    try {
      db.startTransaction();
      db.getContactsDAO().add(contact);
      db.endTransaction(true);
    } catch (DatabaseException e) {
      db.endTransaction(false);
      throw new ModelException(e.getMessage(), e);
    }
  }

  public static void updateContact(Contact contact) throws ModelException {

    Database db = new Database();

    try {
      db.startTransaction();
      db.getContactsDAO().update(contact);
      db.endTransaction(true);
    } catch (DatabaseException e) {
      db.endTransaction(false);
      throw new ModelException(e.getMessage(), e);
    }
  }

  public static void deleteContact(Contact contact) throws ModelException {

    Database db = new Database();

    try {
      db.startTransaction();
      db.getContactsDAO().delete(contact);
      db.endTransaction(true);
    } catch (DatabaseException e) {
      db.endTransaction(false);
      throw new ModelException(e.getMessage(), e);
    }
  }

}
