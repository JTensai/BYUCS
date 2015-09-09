package server.database;

import java.io.File;
import java.sql.*;

import server.ServerException;

public class Database {

  public static void initialize() throws ServerException {
    try {
      final String driver = "org.sqlite.JDBC";
      Class.forName(driver);
    } catch (ClassNotFoundException e) {
      throw new ServerException("Could not load database driver", e);
    }
  }

  private UsersDAO usersDAO;
  private ProjectsDAO projectsDAO;
  private FieldsDAO fieldsDAO;
  private BatchesDAO batchesDAO;
  private RecordsDAO recordsDAO;
  private Connection connection;

  public Database() {
    usersDAO = new UsersDAO(this);
    projectsDAO = new ProjectsDAO(this);
    fieldsDAO = new FieldsDAO(this);
    batchesDAO = new BatchesDAO(this);
    recordsDAO = new RecordsDAO(this);

    connection = null;
  }

  public Connection getConnection() {
    return connection;
  }

  public UsersDAO getUsersDAO() {
    return usersDAO;
  }

  public ProjectsDAO getProjectsDAO() {
    return projectsDAO;
  }

  public FieldsDAO getFieldsDAO() {
    return fieldsDAO;
  }

  public BatchesDAO getBatchesDAO() {
    return batchesDAO;
  }

  public RecordsDAO getRecordsDAO() {
    return recordsDAO;
  }

  //
  // public void startTransaction() throws DatabaseException {
  // try {
  // initialize();
  // connection = null;
  //
  // try {
  // // Open a database connection
  // connection = DriverManager.getConnection(connectionURL);
  // // Start a transaction
  // connection.setAutoCommit(false);
  // }
  // catch (SQLException e) {
  // }
  // }
  // catch (ServerException e1) {
  // e1.printStackTrace();
  // }
  // }

  public void startTransaction() throws DatabaseException {
    String dbName = "db" + File.separator + "database.sqlite";
    String connectionURL = "jdbc:sqlite:" + dbName;
    try {
      initialize();
      connection = DriverManager.getConnection(connectionURL);
      connection.setAutoCommit(false);
    } catch (SQLException | ServerException e) {
      // throw new ServerException("Could not connect to database. Make sure " +
      // DATABASE_FILE + " is available in ./" + DATABASE_DIRECTORY, e);
    }
  }

  //
  // public void endTransaction(boolean commit) {
  // try {
  // if (commit) {
  // connection.commit();
  // }
  // else {
  // connection.rollback();
  // }
  // }
  // catch (SQLException e) {
  // System.out.println("connection.commit() failed");
  // e.printStackTrace();
  // }
  // finally {
  // try {
  // connection.close();
  // }
  // catch (SQLException e) {
  // System.out.println("connection.close() failed");
  // e.printStackTrace();
  // }
  // }
  // // connection = null;
  // }

  // public Connection getConnection() {
  // return connection;
  // }

  public void endTransaction(boolean commit) {
    if (connection != null) {
      try {
        if (commit) {
          connection.commit();
        } else {
          connection.rollback();
        }
      } catch (SQLException e) {
        System.out.println("Could not end transaction");
        e.printStackTrace();
      } finally {
        safeClose(connection);
        connection = null;
      }
    }
  }

  public static void safeClose(Connection conn) {
    if (conn != null) {
      try {
        conn.close();
      } catch (SQLException e) {
        // ...
      }
    }
  }

  public static void safeClose(Statement stmt) {
    if (stmt != null) {
      try {
        stmt.close();
      } catch (SQLException e) {
        // ...
      }
    }
  }

  public static void safeClose(PreparedStatement stmt) {
    if (stmt != null) {
      try {
        stmt.close();
      } catch (SQLException e) {
        // ...
      }
    }
  }

  public static void safeClose(ResultSet rs) {
    if (rs != null) {
      try {
        rs.close();
      } catch (SQLException e) {
        // ...
      }
    }
  }
}
