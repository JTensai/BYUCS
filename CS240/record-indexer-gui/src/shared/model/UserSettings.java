package shared.model;

import java.awt.Dimension;
import java.awt.Point;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Scanner;

public class UserSettings {
  private double zoom_level;
  private int scroll_position;
  private boolean highlighted;
  private boolean inverted;
  private int highlighted_field;
  private int highlighted_row;
  private int window_x;
  private int window_y;
  private int window_width;
  private int window_height;
  private int HSplit;
  private int VSplit;

  private Projects project;
  private Batches batch;
  
  public UserSettings(String username) {
    if (new File(".settings" + File.separator + username + "_settings.txt").exists()){     
      project = new Projects();
      batch = new Batches();
      File input_file = new File(".settings" + File.separator + username + "_settings.txt");
      try {
        Scanner scanner = new Scanner(input_file);
        setZoomLevel(scanner.nextDouble());
        setScrollPosition(scanner.nextInt());
        setHighlighted(scanner.nextBoolean());
        setInverted(scanner.nextBoolean());
        setHighlightedField(scanner.nextInt());
        setHighlightedRow(scanner.nextInt());
        setWindowPosition(new Point(scanner.nextInt(), scanner.nextInt()));
        setWindowSize(new Dimension(scanner.nextInt(), scanner.nextInt()));
        setHSplit(scanner.nextInt());
        setVSplit(scanner.nextInt());
        scanner.nextLine();
        // PROJECT INFO
        if (scanner.hasNextLine()){
          getProject().setTitle(scanner.nextLine());
          getProject().setId(scanner.nextInt());
          getProject().setRecordsPerImage(scanner.nextInt());
          getProject().setFirstYCoord(scanner.nextInt());
          getProject().setRecordHeight(scanner.nextInt());
          if(!scanner.next().equals("NoFields")){   
            ArrayList<Fields> fields = new ArrayList<Fields>();
            scanner.nextLine();
            while(scanner.hasNextLine()){
              String title = scanner.nextLine();
              if (title.equals("Batch Info"))
                break;
              int id = scanner.nextInt();
              int project_id = scanner.nextInt();
              int position = scanner.nextInt();
              int xcoord = scanner.nextInt();
              int width = scanner.nextInt();
              scanner.nextLine();
              String help_html = scanner.nextLine();
              String known_data = scanner.nextLine();
              
              fields.add(new Fields(id, project_id, position, title, xcoord, width, help_html, known_data));
            }
            getProject().setFields(fields);
          }
        
          // BATCH INFO
          getBatch().setId(scanner.nextInt());
          getBatch().setProjectId(scanner.nextInt());
          getBatch().setImageFile(scanner.next());
          getBatch().setUserId(scanner.nextInt());
          if(!scanner.next().equals("NoRecords")){  
            ArrayList<Records> records = new ArrayList<Records>();
            while(scanner.hasNextLine()){
              int field_id = scanner.nextInt();
              int line_num = scanner.nextInt();
              scanner.nextLine();
              String answer = scanner.nextLine();            
              records.add(new Records(answer, getBatch().getId(), line_num, field_id));            
            }
            getBatch().setRecords(records);
          }
        }
        scanner.close();
      }
      catch (FileNotFoundException e) {
        e.printStackTrace();
      }
    }
    else{
      setZoomLevel(1);
      setScrollPosition(0);
      setHighlighted(true);
      setInverted(false);
      setHighlightedField(0);
      setHighlightedRow(0);
      setWindowPosition(new Point(100, 100));
      setWindowSize(new Dimension(1200, 800));
      setHSplit(500);
      setVSplit(300);
      setProject(new Projects());
      setBatch(new Batches());
    }
  }

  public void writeToFile(String username, Projects project, Batches batch){
    PrintWriter writer;
    try {
      new File(".settings").mkdirs();
      writer = new PrintWriter(".settings" + File.separator + username + "_settings.txt", "UTF-8");
      writer.print(getZoomLevel() + " ");
      writer.print(getScrollPosition() + " ");
      writer.print(isHighlighted() + " ");
      writer.print(isInverted() + " ");
      writer.print(getHighlightedField() + " ");
      writer.print(getHighlightedRow() + " ");
      writer.print(getWindowPosition().x + " ");
      writer.print(getWindowPosition().y + " ");
      writer.print(getWindowSize().width + " ");
      writer.print(getWindowSize().height + " ");
      writer.print(getHSplit() + " ");
      writer.println(getVSplit());
      
      // PROJECT INFO
      if (project.getTitle() != null){
        writer.println(project.getTitle());
        writer.print(project.getId() + " ");
        writer.print(project.getRecordsPerImage() + " ");
        writer.print(project.getFirstYCoord() + " ");
        writer.println(project.getRecordHeight());
        if (project.getFields().size() == 0){
          writer.println("NoFields");
        }
        else{
          writer.println("Fields");
          for (Fields field : project.getFields()){
            writer.println(field.getTitle());
            writer.print(field.getId() + " ");
            writer.print(field.getProjectId() + " ");
            writer.print(field.getPosition() + " ");
            writer.print(field.getXcoord() + " ");
            writer.println(field.getWidth());
            writer.println(field.getHelpHtml());
            writer.println(field.getKnownData());
          }
        }
      }
        
      // BATCH INFO
      if (batch.getId() != 0){
        writer.println("Batch Info");
        writer.print(batch.getId() + " ");
        writer.print(batch.getProjectId() + " ");
        writer.print(batch.getImageFile() + " ");
        writer.println(batch.getUserId());
        if (batch.getRecords().size() == 0){
          writer.println("NoRecords");
        }
        else{
          writer.println("Records");
          for (Records record : batch.getRecords()){
            writer.print(record.getFieldId() + " ");
            writer.println(record.getLineNum());
            writer.println(record.getAnswer());
          }        
        }
      }
      writer.close();
    }
    catch (FileNotFoundException | UnsupportedEncodingException e) {
      e.printStackTrace();
    }
  }
  
  public double getZoomLevel() {
    return zoom_level;
  }

  public void setZoomLevel(double scale) {
    this.zoom_level = scale;
  }

  public int getScrollPosition() {
    return scroll_position;
  }

  public void setScrollPosition(int scroll_position) {
    this.scroll_position = scroll_position;
  }

  public boolean isHighlighted() {
    return highlighted;
  }

  public void setHighlighted(boolean highlights) {
    this.highlighted = highlights;
  }

  public boolean isInverted() {
    return inverted;
  }

  public void setInverted(boolean inverted) {
    this.inverted = inverted;
  }

  public Projects getProject() {
    return project;
  }

  public void setProject(Projects project) {
    this.project = project;
  }

  public Batches getBatch() {
    return batch;
  }

  public void setBatch(Batches batch) {
    this.batch = batch;
  }

  public int getHighlightedField() {
    return highlighted_field;
  }

  public void setHighlightedField(int highlighted_field) {
    this.highlighted_field = highlighted_field;
  }

  public int getHighlightedRow() {
    return highlighted_row;
  }

  public void setHighlightedRow(int highlighted_row) {
    this.highlighted_row = highlighted_row;
  }

  public Point getWindowPosition(){    
    return new Point(window_x, window_y);
  }

  public void setWindowPosition(Point location) {
    this.window_x = location.x;
    this.window_y = location.y;
  }

  public Dimension getWindowSize(){    
    return new Dimension(window_width, window_height);
  }

  public void setWindowSize(Dimension size) {
    this.window_width = size.width;
    this.window_height = size.height;
  }

  public int getHSplit() {
    return HSplit;
  }

  public void setHSplit(int hSplit) {
    HSplit = hSplit;
  }

  public int getVSplit() {
    return VSplit;
  }

  public void setVSplit(int vSplit) {
    VSplit = vSplit;
  }
}
