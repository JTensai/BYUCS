package server.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

import shared.model.*;

public class BatchesDAO {
  private Database db;

  BatchesDAO(Database db) {
    this.db = db;
  }

  /**
   * Adds a new batch
   * 
   * @param batch
   * @throws DatabaseException
   */
  public void add(Batches batch) throws DatabaseException {
    ArrayList<Fields> fields = getFields2(batch.getProjectId());
    int first_field_id = 1;
    if (!fields.isEmpty())
      first_field_id = fields.get(0).getId();
    
//     db.startTransaction();
    Connection connect = db.getConnection();
    Statement statement = null;

    String insert_batch_sql = "INSERT INTO Batches (project_id, image_file, recorded, user_id)" + " VALUES (" + batch.getProjectId() + ", '" + batch.getImageFile() + "', 0, -1);\n";

    try {
      statement = connect.createStatement();
      statement.executeUpdate(insert_batch_sql);
      // Gets project_id
      Statement keyStmt = connect.createStatement();
      ResultSet keyRS = keyStmt.executeQuery("select last_insert_rowid()");
      keyRS.next();
      int batch_id = keyRS.getInt(1);
      
      int current_field_id = first_field_id;
      // Add fields for project
      for (Records record : batch.getRecords()) {
        record.setBatchId(batch_id);
        record.setFieldId(current_field_id);
        record.setImageUrl(batch.getImageFile());
        db.getRecordsDAO().create(record);
        if (current_field_id - first_field_id == fields.size()-1)
          current_field_id = first_field_id;
        else
          current_field_id++;
      }
//       db.endTransaction(true);
    } catch (SQLException e) {
      e.printStackTrace();
//       db.endTransaction(false);
    }
  }

  public Batches getBatch(int project_id, int user_id) {
    ResultSet results;
    Batches batch = new Batches();
    int batch_id = -1;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_batch = "SELECT * FROM Batches WHERE project_id = '" + project_id + "' AND recorded = 0 AND user_id = '-1' LIMIT 1;";

      results = statement.executeQuery(select_batch);
      if (results.next()) {
        batch.setId(results.getInt("id"));
        batch.setImageFile(results.getString("image_file"));
        batch.setProjectId(project_id);
        batch.setRecorded(true);
        batch.setUserId(user_id);
      }

      db.endTransaction(true);

      if (batch.getId() != 0)
        batch_id = batch.getId();

      CheckoutBatch(batch_id, user_id);
      return batch;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }

  public ArrayList<Fields> getFields(int project_id) {
    ResultSet results;
    ArrayList<Fields> fields = new ArrayList<Fields>();
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_batch = null;
      if (project_id > 0)
        select_batch = "SELECT * FROM Fields WHERE project_id = '" + project_id + "';";
      else
        select_batch = "SELECT * FROM Fields;";

      results = statement.executeQuery(select_batch);
      while (results.next()) {
        int id = results.getInt("id");
        int new_project_id = results.getInt("project_id");
        int position = results.getInt("position");
        String title = results.getString("title");
        int xcoord = results.getInt("xcoord");
        int width = results.getInt("width");
        String help_html = results.getString("help_html");
        String known_data = results.getString("known_data");

        Fields current_field = new Fields(id, new_project_id, position, title, xcoord, width, help_html, known_data);
        fields.add(current_field);
      }
      db.endTransaction(true);
      return fields;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }
  

  public ArrayList<Fields> getFields2(int project_id) {
    ResultSet results;
    ArrayList<Fields> fields = new ArrayList<Fields>();
    try {
//      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_batch = null;
      if (project_id > 0)
        select_batch = "SELECT * FROM Fields WHERE project_id = '" + project_id + "';";
      else
        select_batch = "SELECT * FROM Fields;";

      results = statement.executeQuery(select_batch);
      while (results.next()) {
        int id = results.getInt("id");
        int new_project_id = results.getInt("project_id");
        int position = results.getInt("position");
        String title = results.getString("title");
        int xcoord = results.getInt("xcoord");
        int width = results.getInt("width");
        String help_html = results.getString("help_html");
        String known_data = results.getString("known_data");

        Fields current_field = new Fields(id, new_project_id, position, title, xcoord, width, help_html, known_data);
        fields.add(current_field);
      }
//      db.endTransaction(true);
      return fields;
    } catch (SQLException e) {
      e.printStackTrace();
//      db.endTransaction(false);
      return null;
    }
  }

  /**
   * @param batch_id
   * @throws DatabaseException
   * 
   * @returns batch object with given batch_id
   */
  public Batches read(int batch_id) throws DatabaseException {
    return null;
  }

  /**
   * update batch
   * 
   * @param batch
   * @throws DatabaseException
   */
  public void update(Batches batch) throws DatabaseException {
  }

  public void CheckoutBatch(int batch_id, int user_id) {
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String update_batch = "UPDATE Batches SET user_id = " + user_id + " WHERE id = " + batch_id + ";"; //
      String update_user = "UPDATE Users SET current_batch_id = " + batch_id + " WHERE id = " + user_id + ";";

      statement.executeUpdate(update_batch);
      statement.executeUpdate(update_user);

      db.endTransaction(true);
    } catch (DatabaseException | SQLException e) {
      // TODO Auto-generated catch block
      db.endTransaction(false);
      e.printStackTrace();
    }
  }

  public boolean isRecorded(int batch_id) {
    ResultSet results;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String check_batch = "SELECT recorded FROM Batches WHERE id = " + batch_id + ";";
      boolean recorded = false;

      results = statement.executeQuery(check_batch);
      if (results.next()) {
        recorded = results.getBoolean("recorded");
      }

      db.endTransaction(true);
      return recorded;
    } catch (DatabaseException | SQLException e) {
      db.endTransaction(false);
      e.printStackTrace();
      return false;
    }
  }
}
