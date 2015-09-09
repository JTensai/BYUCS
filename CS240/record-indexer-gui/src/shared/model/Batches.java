package shared.model;

import java.util.ArrayList;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import server.database.DataImporter;

public class Batches {

  private int id;
  private int project_id;
  private String image_file;
  private boolean recorded;
  private int user_id;
  private ArrayList<Records> records = new ArrayList<Records>();

  public Batches(Element batchElement) {
    image_file = DataImporter.getValue((Element) batchElement.getElementsByTagName("file").item(0));
    if (batchElement.getElementsByTagName("records").getLength() > 0){
      Element recordsElement = (Element) batchElement.getElementsByTagName("records").item(0);  
      NodeList recordElements = recordsElement.getElementsByTagName("record");
      
      int line_num = 1;
      for (int i = 0; i < recordElements.getLength(); i++) {  
        Element recordElement = (Element) recordElements.item(i);
        
        Element valueElement = (Element) recordElement.getElementsByTagName("values").item(0);
        NodeList valueElements = valueElement.getElementsByTagName("value");
        for (int j = 0; j < valueElements.getLength(); j++) {
          String answer = DataImporter.getValue((Element) valueElement.getElementsByTagName("value").item(j));
          records.add(new Records(answer, line_num));
        }
        line_num++;
        
      }
    }
      
    setRecorded(false);
  }

  public Batches(int id, int project_id, String image_file) {
    setId(id);
    setProjectId(project_id);
    setImageFile(image_file);
    setRecorded(false);
    setUserId(-1);
    setRecords(new ArrayList<Records>());
  }
  
  public Batches(int id, int project_id, String image_file, int user_id, ArrayList<Records> records) {
    setId(id);
    setProjectId(project_id);
    setImageFile(image_file);
    setRecorded(false);
    setUserId(user_id);
    setRecords(records);
  }

  public Batches() {
  }

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public int getProjectId() {
    return project_id;
  }

  public void setProjectId(int project_id) {
    this.project_id = project_id;
  }

  public String getImageFile() {
    return image_file;
  }

  public void setImageFile(String image_file) {
    this.image_file = image_file;
  }

  public boolean isRecorded() {
    return recorded;
  }

  public void setRecorded(boolean recorded) {
    this.recorded = recorded;
  }

  public int getUserId() {
    return user_id;
  }

  public void setUserId(int user_id) {
    this.user_id = user_id;
  }

  public ArrayList<Records> getRecords() {
    return records;
  }

  public void setRecords(ArrayList<Records> records) {
    this.records = records;
  }
}
