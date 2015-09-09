package shared.model;

import java.util.ArrayList;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import server.database.*;

public class Projects {

  private int id;
  private String title = null;
  private int records_per_image = -1;
  private int first_y_coord = -1;
  private int record_height = -1;
  ArrayList<Fields> fields = new ArrayList<Fields>();
  ArrayList<Batches> batches = new ArrayList<Batches>();

  public Projects(Element projectElement) {
    title = DataImporter.getValue((Element) projectElement.getElementsByTagName("title").item(0));
    records_per_image = Integer.parseInt(DataImporter.getValue((Element) projectElement.getElementsByTagName("recordsperimage").item(0)));
    first_y_coord = Integer.parseInt(DataImporter.getValue((Element) projectElement.getElementsByTagName("firstycoord").item(0)));
    record_height = Integer.parseInt(DataImporter.getValue((Element) projectElement.getElementsByTagName("recordheight").item(0)));
    
    Element fieldsElement = (Element) projectElement.getElementsByTagName("fields").item(0);
    NodeList fieldElements = fieldsElement.getElementsByTagName("field");

    for (int i = 0; i < fieldElements.getLength(); i++) {
      fields.add(new Fields((Element) fieldElements.item(i)));
    }

    Element imagesElement = (Element) projectElement.getElementsByTagName("images").item(0);
    NodeList imageElements = imagesElement.getElementsByTagName("image");

    for (int i = 0; i < imageElements.getLength(); i++) {
//      System.out.println("Batch Number: " + (i+1));
      batches.add(new Batches((Element) imageElements.item(i)));
    }
  }

  public Projects(int id, String title, int records_per_image, int first_y_coord, int record_height) {
    setId(id);
    setTitle(title);
    setRecordsPerImage(records_per_image);
    setFirstYCoord(first_y_coord);
    setRecordHeight(record_height);
  }

  public Projects() {
  }

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public String getTitle() {
    return title;
  }

  public void setTitle(String title) {
    this.title = title;
  }

  public int getRecordsPerImage() {
    return records_per_image;
  }

  public void setRecordsPerImage(int records_per_image) {
    this.records_per_image = records_per_image;
  }

  public int getFirstYCoord() {
    return first_y_coord;
  }

  public void setFirstYCoord(int first_y_coord) {
    this.first_y_coord = first_y_coord;
  }

  public int getRecordHeight() {
    return record_height;
  }

  public void setRecordHeight(int record_height) {
    this.record_height = record_height;
  }

  public ArrayList<Fields> getFields() {
    return fields;
  }
  
  public void setFields(ArrayList<Fields> fields) {
    this.fields = fields; 
  }

  public ArrayList<Batches> getBatches() {
    return batches;
  }

  public String toString() {
    StringBuilder builder = new StringBuilder();
    builder.append(title + "\n");
    for (Fields field : fields) {
      builder.append("\t" + field.getTitle());
    }

    return builder.toString();
  }

  public String projectInfo() {
    return id + "\n" + title + "\n";
  }

}