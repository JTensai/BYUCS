package shared.model;

import org.w3c.dom.Element;

import server.database.DataImporter;

public class Users {

  private int id;
  private String first_name;
  private String last_name;
  private String username;
  private String password;
  private String email;
  private int indexed_records;
  private int current_batch_id;

  public Users() {

  }

  public Users(int id, String first_name, String last_name, String username, String password, String email, int indexed_records) {
    setId(id);
    setFirstName(first_name);
    setLastName(last_name);
    setUsername(username);
    setPassword(password);
    setEmail(email);
    setIndexedRecords(indexed_records);
    setCurrentBatchId(-1);
  }

  public Users(Element userElement) {
    username = DataImporter.getValue((Element) userElement.getElementsByTagName("username").item(0));
    password = DataImporter.getValue((Element) userElement.getElementsByTagName("password").item(0));
    first_name = DataImporter.getValue((Element) userElement.getElementsByTagName("firstname").item(0));
    last_name = DataImporter.getValue((Element) userElement.getElementsByTagName("lastname").item(0));
    email = DataImporter.getValue((Element) userElement.getElementsByTagName("email").item(0));
    indexed_records = Integer.parseInt(DataImporter.getValue((Element) userElement.getElementsByTagName("indexedrecords").item(0)));
    current_batch_id = -1;
    // System.out.println(toString());
  }

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public String getName() {
    return first_name + " " + last_name;
  }

  public String getFirstName() {
    return first_name;
  }

  public void setFirstName(String first_name) {
    this.first_name = first_name;
  }

  public String getLastName() {
    return last_name;
  }

  public void setLastName(String last_name) {
    this.last_name = last_name;
  }

  public String getUsername() {
    return username;
  }

  public void setUsername(String username) {
    this.username = username;
  }

  public String getPassword() {
    return password;
  }

  public void setPassword(String password) {
    this.password = password;
  }

  public String getEmail() {
    return email;
  }

  public void setEmail(String email) {
    this.email = email;
  }

  public int getIndexedRecords() {
    return indexed_records;
  }

  public void setIndexedRecords(int indexed_records) {
    this.indexed_records = indexed_records;
  }

  public int getCurrentBatchId() {
    return current_batch_id;
  }

  public void setCurrentBatchId(int current_batch_id) {
    this.current_batch_id = current_batch_id;
  }

  public String toString() {
    return "TRUE\n" + first_name + "\n" + last_name + "\n" + indexed_records + "\n";
  }
}
