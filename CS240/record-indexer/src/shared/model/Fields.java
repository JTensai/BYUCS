package shared.model;

import org.w3c.dom.Element;

import server.database.DataImporter;

public class Fields {

  private int id;
  private int project_id;
  private int position;
  private String title;
  private int xcoord;
  private int width;
  private String help_html;
  private String known_data;

  public Fields(Element fieldElement) {
    title = DataImporter.getValue((Element) fieldElement.getElementsByTagName("title").item(0));
    xcoord = Integer.parseInt(DataImporter.getValue((Element) fieldElement.getElementsByTagName("xcoord").item(0)));
    width = Integer.parseInt(DataImporter.getValue((Element) fieldElement.getElementsByTagName("width").item(0)));
    help_html = DataImporter.getValue((Element) fieldElement.getElementsByTagName("helphtml").item(0));

    if ((Element) fieldElement.getElementsByTagName("knowndata").item(0) != null)
      known_data = DataImporter.getValue((Element) fieldElement.getElementsByTagName("knowndata").item(0));
  }

  public Fields(int id, int project_id, int position, String title, int xcoord, int width, String help_html, String known_data) {
    setId(id);
    setProjectId(project_id);
    setPosition(position);
    setTitle(title);
    setXcoord(xcoord);
    setWidth(width);
    setHelpHtml(help_html);
    setKnownData(known_data);
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

  public int getPosition() {
    return position;
  }

  public void setPosition(int position) {
    this.position = position;
  }

  public String getTitle() {
    return title;
  }

  public void setTitle(String title) {
    this.title = title;
  }

  public int getXcoord() {
    return xcoord;
  }

  public void setXcoord(int xcoord) {
    this.xcoord = xcoord;
  }

  public int getWidth() {
    return width;
  }

  public void setWidth(int width) {
    this.width = width;
  }

  public String getHelpHtml() {
    return help_html;
  }

  public void setHelpHtml(String help_html) {
    this.help_html = help_html;
  }

  public String getKnownData() {
    return known_data;
  }

  public void setKnownData(String known_data) {
    this.known_data = known_data;
  }
}
