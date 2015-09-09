package server.database;

import java.util.*;
import java.util.logging.*;

import shared.model.*;

public class ContactsDAO {

  private static Logger logger;

  static {
    logger = Logger.getLogger("contactmanager");
  }

  private Database db;

  ContactsDAO(Database db) {
    this.db = db;
  }

  public List<Contact> getAll() throws DatabaseException {

    logger.entering("server.database.ContactsDAO", "getAll");

    // TODO: Use db's connection to query all contacts from the database and
    // return them

    logger.exiting("server.database.ContactsDAO", "getAll");

    return null;
  }

  public void add(Contact contact) throws DatabaseException {

    logger.entering("server.database.ContactsDAO", "add");

    // TODO: Use db's connection to add a new contact to the database

    logger.exiting("server.database.ContactsDAO", "add");
  }

  public void update(Contact contact) throws DatabaseException {

    logger.entering("server.database.ContactsDAO", "update");

    // TODO: Use db's connection to update contact in the database

    logger.exiting("server.database.ContactsDAO", "update");
  }

  public void delete(Contact contact) throws DatabaseException {

    logger.entering("server.database.ContactsDAO", "delete");

    // TODO: Use db's connection to delete contact from the database

    logger.exiting("server.database.ContactsDAO", "delete");
  }

}
