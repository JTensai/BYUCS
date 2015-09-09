package server.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

import shared.model.*;

public class RecordsDAO {
  private Database db;

  RecordsDAO(Database db) {
    this.db = db;
  }
  

  public void create(Records record) throws DatabaseException {
//    db.startTransaction();
    Connection connect = db.getConnection();
    Statement statement = null;

    String insert_record_sql = "INSERT INTO Records (answer, batch_id, line_num, field_id, image_url)" + " VALUES ('" + record.getAnswer().toLowerCase() + "', '" + record.getBatchId() + "', '"
        + record.getLineNum() + "', '" + record.getFieldId() + "', '" + record.getImageUrl() + "')";
    String update_batch_sql = "UPDATE Batches SET recorded = 1 WHERE id = " + record.getBatchId() + ";";
    try {
      statement = connect.createStatement();
      statement.executeUpdate(insert_record_sql);
      statement.executeUpdate(update_batch_sql);

//      db.endTransaction(true);
    } catch (SQLException e) {
//      db.endTransaction(false);
      e.printStackTrace();
    }
  }

  /**
   * add new record
   * 
   * @param record
   * @throws DatabaseException
   */
  public void add(Records record, int user_id) throws DatabaseException {
    String image_url = null;
    db.startTransaction();
    image_url = getImage(record.getBatchId());
    db.endTransaction(true);

    db.startTransaction();
    Connection connect = db.getConnection();
    Statement statement = null;

    String insert_record_sql = "INSERT INTO Records (answer, batch_id, line_num, field_id, image_url)" + " VALUES ('" + record.getAnswer().toLowerCase() + "', '" + record.getBatchId() + "', '"
        + record.getLineNum() + "', '" + record.getFieldId() + "', '" + image_url + "')";
    String update_batch_sql = "UPDATE Batches SET recorded = 1 WHERE id = " + record.getBatchId() + ";";
    String clear_user_sql = "UPDATE Users SET current_batch_id = -1 WHERE id = " + user_id + ";";
    try {
      statement = connect.createStatement();
      statement.executeUpdate(insert_record_sql);
      statement.executeUpdate(update_batch_sql);
      statement.executeUpdate(clear_user_sql);

      db.endTransaction(true);
    } catch (SQLException e) {
      db.endTransaction(false);
      e.printStackTrace();
    }
  }

  public ArrayList<Records> search(ArrayList<String> values, ArrayList<Integer> field_ids) {
    ResultSet results;
    ArrayList<Records> full_results = new ArrayList<Records>();
    // System.out.println("given: " + value);
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      
      StringBuilder builder1 = new StringBuilder();
      for (int i = 0; i < values.size(); i++){
        if (i == 0){
          builder1.append(values.get(i).toLowerCase());
        }
        else{
          builder1.append("' OR answer = '" + values.get(i).toLowerCase());          
        }
      }
      StringBuilder builder2 = new StringBuilder();
      for (int i = 0; i < field_ids.size(); i++){
        if (i == 0){
          builder2.append(field_ids.get(i));
        }
        else{
          builder2.append("' OR field_id = '" + field_ids.get(i));          
        }
      }
            
      String select_user = "SELECT * FROM Records "
          + "WHERE (answer = '" + builder1.toString() + "') "
          + "AND (field_id = '" + builder2.toString() + "');";     
      
      System.out.println(select_user);
      results = statement.executeQuery(select_user);
      while (results.next()) {
        Records result = new Records();
        result.setAnswer(results.getString("answer"));
        result.setBatchId(results.getInt("batch_id"));
        result.setLineNum(results.getInt("line_num"));
        result.setFieldId(results.getInt("field_id"));
        result.setImageUrl(results.getString("image_url"));
        full_results.add(result);
      }
      results.close();
      db.endTransaction(true);

      // System.out.println("received: " + result.getAnswer());
      return full_results;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }

  public String getImage(int batch_id) {
    ResultSet results;
    String image_url = null;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_sample_image = "SELECT * FROM Batches WHERE id = " + batch_id + ";";

      results = statement.executeQuery(select_sample_image);
      if (results.next()) {
        String image_file = results.getString("image_file");

        image_url = image_file;
      }
      db.endTransaction(true);
      return image_url;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }

  public ArrayList<Records> getAllRecords() {
    ArrayList<Records> all_records = new ArrayList<Records>();
    ResultSet results;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_sample_image = "SELECT * FROM Records;";

      results = statement.executeQuery(select_sample_image);
      Records result = new Records();
      while (results.next()) {
        result.setAnswer(results.getString("answer"));
        result.setBatchId(results.getInt("batch_id"));
        result.setLineNum(results.getInt("line_num"));
        result.setFieldId(results.getInt("field_id"));
        result.setImageUrl(results.getString("image_url"));
        all_records.add(result);
      }
      db.endTransaction(true);
      return all_records;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }
}
