package server.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

import shared.model.*;

public class ProjectsDAO {
  private Database db;

  ProjectsDAO(Database db) {
    this.db = db;
  }

  /**
   * create new project
   * 
   * @param project
   * @throws DatabaseException
   */
  public void create(Projects project) throws DatabaseException {
    db.startTransaction();
    Connection connect = db.getConnection();
    Statement statement = null;

    String insert_project_sql = "INSERT INTO Projects (title, records_per_image, first_y_coord, record_height)" + " VALUES ('" + project.getTitle() + "', '" + project.getRecordsPerImage() + "', '"
        + project.getFirstYCoord() + "', '" + project.getRecordHeight() + "');\n";

    try {
      statement = connect.createStatement();
      statement.executeUpdate(insert_project_sql);
      // Gets project_id
      Statement keyStmt = connect.createStatement();
      ResultSet keyRS = keyStmt.executeQuery("select last_insert_rowid()");
      keyRS.next();
      int project_id = keyRS.getInt(1);

      int position = 1;
      // Add fields for project
      for (Fields field : project.getFields()) {
        field.setProjectId(project_id);
        field.setPosition(position);
        db.getFieldsDAO().add(field);
        position++;
      }
      // Add batches for project      
      for (Batches batch : project.getBatches()) {
        batch.setProjectId(project_id);
        db.getBatchesDAO().add(batch);
      }
      // All updates were successful

      db.endTransaction(true);
    } catch (SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
    }
  }

  public ArrayList<Projects> getAllProjects() throws DatabaseException {
    ResultSet results;
    ArrayList<Projects> projects = new ArrayList<Projects>();
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_all_projects = "SELECT * FROM Projects;";

      results = statement.executeQuery(select_all_projects);
      while (results.next()) {
        int id = results.getInt("id");
        String title = results.getString("title");
        int records_per_image = results.getInt("records_per_image");
        int first_y_coord = results.getInt("first_y_coord");
        int record_height = results.getInt("record_height");

        Projects current_project = new Projects(id, title, records_per_image, first_y_coord, record_height);
        projects.add(current_project);
      }
      db.endTransaction(true);
      return projects;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }

  public String getSampleImage(int project_id) {
    ResultSet results;
    String image_url = null;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_sample_image = "SELECT * FROM Batches WHERE project_id = '" + project_id + "' LIMIT 1;";

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

  public Projects getProject(int project_id) {
    ResultSet results;
    Projects project = null;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_all_projects = "SELECT * FROM Projects WHERE id = '" + project_id + "';";

      results = statement.executeQuery(select_all_projects);
      if (results.next()) {
        int id = results.getInt("id");
        String title = results.getString("title");
        int records_per_image = results.getInt("records_per_image");
        int first_y_coord = results.getInt("first_y_coord");
        int record_height = results.getInt("record_height");

        project = new Projects(id, title, records_per_image, first_y_coord, record_height);
      }
      db.endTransaction(true);
      return project;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return null;
    }
  }
  
  public int remainingBatchCount(int project_id){
    ResultSet results;
    try {
      db.startTransaction();
      Connection connect = db.getConnection();
      Statement statement = connect.createStatement();
      String select_all_projects = "SELECT * FROM Batches WHERE project_id = '" + project_id + "' AND recorded = 0;";

      results = statement.executeQuery(select_all_projects);
      int count = 0;
      while (results.next()) {
        count++;
      }
      db.endTransaction(true);
      return count;
    } catch (DatabaseException | SQLException e) {
      e.printStackTrace();
      db.endTransaction(false);
      return 0;
    }
    
    
  }
  
  //
  // /**
  // * @param project_id
  // * @throws DatabaseException
  // *
  // * @return returns project object with id equal to project_id
  // */
  // public Projects read(int project_id) throws DatabaseException {
  // return null;
  // }
  //
  // /**
  // * update project
  // *
  // * @param project
  // * @throws DatabaseException
  // */
  // public void update(Projects project) throws DatabaseException {
  // }

}
