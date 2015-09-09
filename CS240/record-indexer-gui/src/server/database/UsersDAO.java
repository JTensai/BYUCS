package server.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import shared.model.*;

public class UsersDAO {

  private Database db;

  UsersDAO(Database db) {
    this.db = db;
  }

  /**
   * create new user
   * 
   * @param user
   * @throws DatabaseException
   */
  public void create(Users user) throws DatabaseException {
    db.startTransaction();
    Connection connect = db.getConnection();
    Statement statement = null;

    String insert_user_sql = "INSERT INTO Users (first_name, last_name, username, password, email, indexed_records, current_batch_id)" + " VALUES ('" + user.getFirstName() + "', '"
        + user.getLastName() + "', '" + user.getUsername() + "', '" + user.getPassword() + "', '" + user.getEmail() + "', '" + user.getIndexedRecords() + "', " + -1 + ")";

    try {
      statement = connect.createStatement();
      statement.executeUpdate(insert_user_sql);
      db.endTransaction(true);
    } catch (SQLException e) {
      db.endTransaction(false);
      e.printStackTrace();
    }
  }

  /**
   * @param username
   * @throws DatabaseException
   * 
   * @return returns user object with username equal to username
   */
  // public Users read(String username) throws DatabaseException {
  // return null;
  // }

  /**
   * update user
   * 
   * @param user
   * @throws DatabaseException
   */
  // public void update(Users user) throws DatabaseException {
  //
  // }

  /**
   * ValidateUser
   * 
   * @return Returns full user object if given user exists in database,
   *         otherwise returns null
   */
  public Users ValidateUser(Users user) {
    ResultSet results;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_user = "SELECT * FROM Users WHERE username = '" + user.getUsername() + "' AND password = '" + user.getPassword() + "';";

      results = statement.executeQuery(select_user);
      Users return_user = new Users();
      if (results.next()) {
        return_user.setId(results.getInt("id"));
        return_user.setFirstName(results.getString("first_name"));
        return_user.setLastName(results.getString("last_name"));
        return_user.setUsername(results.getString("username"));
        return_user.setPassword(results.getString("password"));
        return_user.setEmail(results.getString("email"));
        return_user.setIndexedRecords(results.getInt("indexed_records"));
        return_user.setCurrentBatchId(results.getInt("current_batch_id"));
      }
      results.close();
      db.endTransaction(true);
      return return_user;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }

  public Batches getCurrentBatch(Users user) {
    Batches batch = new Batches();
    ResultSet results;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_current_batch = "SELECT * FROM Batches WHERE id = '" + user.getCurrentBatchId() + "';";

      results = statement.executeQuery(select_current_batch);

      if (results.next()) {
        batch.setId(results.getInt("id"));
        batch.setImageFile(results.getString("image_file"));
        batch.setProjectId(results.getInt("project_id"));
        batch.setRecorded(true);
        batch.setUserId(results.getInt("user_id"));
      }

      results.close();
      db.endTransaction(true);
      return batch;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }

  }

  public void incrementIndexedRecord(int user_id) throws DatabaseException {
    ResultSet results;
    db.startTransaction();
    Connection connect = db.getConnection();
    Statement statement = null;

    String get_user_sql = "SELECT indexed_records FROM Users WHERE id = " + user_id + ";";
    int indexed_records = 0;

    try {
      statement = connect.createStatement();

      results = statement.executeQuery(get_user_sql);
      if (results.next()) {
        indexed_records = results.getInt("indexed_records") + 1;
      }
      results.close();

      String update_user_sql = "UPDATE Users SET indexed_records = " + indexed_records + " WHERE id = " + user_id + ";";
      statement.executeUpdate(update_user_sql);
      db.endTransaction(true);
    } catch (SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
    }
  }

}
