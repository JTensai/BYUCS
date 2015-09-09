package server.database;

import java.sql.*;
import java.util.logging.*;

public class Database {

  private static Logger logger;

  static {
    logger = Logger.getLogger("contactmanager");
  }

  public static void initialize() throws DatabaseException {

    logger.entering("server.database.Database", "initialize");

    // TODO: Load the SQLite database driver

    logger.exiting("server.database.Database", "initialize");
  }

  private ContactsDAO contactsDAO;
  private Connection connection;

  public Database() {
    contactsDAO = new ContactsDAO(this);
    connection = null;
  }

  public ContactsDAO getContactsDAO() {
    return contactsDAO;
  }

  public Connection getConnection() {
    return connection;
  }

  public void startTransaction() throws DatabaseException {

    logger.entering("server.database.Database", "startTransaction");

    // TODO: Open a connection to the database and start a transaction

    logger.exiting("server.database.Database", "startTransaction");
  }

  public void endTransaction(boolean commit) {

    logger.entering("server.database.Database", "endTransaction");

    // TODO: Commit or rollback the transaction and close the connection

    logger.exiting("server.database.Database", "endTransaction");
  }

}
