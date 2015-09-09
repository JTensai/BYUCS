package server.database;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import shared.model.*;

public class FieldsDAO {

  private Database db;

  FieldsDAO(Database db) {
    this.db = db;
  }

  /**
   * add new field
   * 
   * @param field
   * @throws DatabaseException
   */
  public void add(Fields field) throws DatabaseException {
    // db.startTransaction();
    Connection connect = db.getConnection();
    Statement statement = null;

    String insert_field_sql = "INSERT INTO Fields (title, project_id, position, xcoord, width, help_html, known_data)" + " VALUES ('" + field.getTitle() + "', '" + field.getProjectId() + "', '"
        + field.getPosition() + "', '" + field.getXcoord() + "', '" + field.getWidth() + "', '" + field.getHelpHtml() + "', '" + field.getKnownData() + "');\n";

    try {
      statement = connect.createStatement();
      statement.executeUpdate(insert_field_sql);
      // db.endTransaction(true);
    } catch (SQLException e) {
      e.printStackTrace();
      // db.endTransaction(false);
    }
  }
}
