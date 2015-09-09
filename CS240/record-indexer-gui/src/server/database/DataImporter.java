package server.database;

import java.io.*;
import java.sql.*;
import java.util.ArrayList;

import javax.xml.parsers.*;

import org.w3c.dom.*;
import org.xml.sax.SAXException;

import shared.model.*;

public class DataImporter {

  static ArrayList<Users> users = new ArrayList<Users>();
  static ArrayList<Projects> projects = new ArrayList<Projects>();

  public static void main(String[] args) throws ParserConfigurationException, SAXException, IOException, DatabaseException {
    File xmlFile = new File(args[0]);
    DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
    DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
    Document doc = dBuilder.parse(xmlFile);

    doc.getDocumentElement().normalize();
    Element root = doc.getDocumentElement();

    ArrayList<Element> rootElements = DataImporter.getChildElements(root);

    ArrayList<Element> userElements = DataImporter.getChildElements(rootElements.get(0));
    for (Element userElement : userElements) {
      users.add(new Users(userElement));
    }

    ArrayList<Element> projectElements = DataImporter.getChildElements(rootElements.get(1));
    for (Element projectElement : projectElements) {
      projects.add(new Projects(projectElement));
    }

    clearThenCreateTables();
    addToDatabase();
    copyFilesToServer(args[0]); // Needs to copy all images and known_data to
                                // the server directory
  }

  public static ArrayList<Element> getChildElements(Node node) {
    ArrayList<Element> result = new ArrayList<Element>();
    NodeList children = node.getChildNodes();

    for (int i = 0; i < children.getLength(); i++) {
      Node child = children.item(i);
      if (child.getNodeType() == Node.ELEMENT_NODE) {
        result.add((Element) child);
      }
    }
    return result;
  }

  public static String getValue(Element element) {
    String result = "";
    Node child = element.getFirstChild();
    result = child.getNodeValue();
    return result;
  }

  public static void clearThenCreateTables() throws DatabaseException {
    String drop_tables_sql = "DROP TABLE IF EXISTS Users;" + "DROP TABLE IF EXISTS Projects;" + "DROP TABLE IF EXISTS Batches;" + "DROP TABLE IF EXISTS Fields;" + "DROP TABLE IF EXISTS Records";

    String create_users_sql = "CREATE TABLE Users(" + "id integer not null primary key autoincrement," + "first_name varchar(225)," + "last_name varchar(225)," + "username varchar(225),"
        + "password varchar(225)," + "email varchar(225)," + "indexed_records integer," + "current_batch_id integer)";
    String create_projects_sql = "CREATE TABLE Projects(" + "id integer not null primary key autoincrement," + "title varchar(225)," + "records_per_image integer," + "first_y_coord integer,"
        + "record_height integer)";
    String create_batches_sql = "CREATE TABLE Batches(" + "id integer not null primary key autoincrement," + "project_id integer," + "image_file varchar(225)," + "recorded bool," + "user_id integer)";
    String create_fields_sql = "CREATE TABLE Fields(" + "id integer not null primary key autoincrement," + "project_id integer," + "position integer," + "title varchar(225)," + "xcoord integer,"
        + "width integer," + "help_html varchar(225)," + "known_data varchar(225))";
    String create_records_sql = "CREATE TABLE Records(" + "id integer not null primary key autoincrement," + "answer varchar(225)," + "batch_id integer," + "line_num integer," + "field_id integer,"
        + "image_url varchar(225))";

    Database database = new Database();
    database.startTransaction();
    Connection connect = database.getConnection();
    Statement statement = null;

    try {
      statement = connect.createStatement();

      statement.executeUpdate(drop_tables_sql);
      System.out.println("Tables Cleared");

      statement.executeUpdate(create_users_sql);
      statement.executeUpdate(create_projects_sql);
      statement.executeUpdate(create_batches_sql);
      statement.executeUpdate(create_fields_sql);
      statement.executeUpdate(create_records_sql);
      System.out.println("Tables Created");
      database.endTransaction(true);
    } catch (SQLException e) {
      database.endTransaction(false);
      e.printStackTrace();
    }

  }

  public static void addToDatabase() throws DatabaseException {
    Database database = new Database();

    for (Users user : users) {
      database.getUsersDAO().create(user);
    }
    for (Projects project : projects) {
      database.getProjectsDAO().create(project);
    }
  }

  public static void copyFilesToServer(String xml_path) throws IOException {
    File xml_file = new File(xml_path);
    File source = new File(xml_file.getParent());
    File destination = new File("assets");
    copyDirectory(source, destination);

  }

  public static void copyDirectory(File sourceLocation, File targetLocation) throws IOException {
    if (sourceLocation.isDirectory()) {
      if (!targetLocation.exists()) {
        targetLocation.mkdir();
      }
      String[] children = sourceLocation.list();
      for (int i = 0; i < children.length; i++) {
        copyDirectory(new File(sourceLocation, children[i]), new File(targetLocation, children[i]));
      }
    } else {
      InputStream in = new FileInputStream(sourceLocation);
      OutputStream out = new FileOutputStream(targetLocation);

      byte[] buf = new byte[1024];
      int len;
      while ((len = in.read(buf)) > 0) {
        out.write(buf, 0, len);
      }
      in.close();
      out.close();
    }
  }

}
