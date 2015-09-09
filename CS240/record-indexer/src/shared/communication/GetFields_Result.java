package shared.communication;

import shared.model.*;
import java.util.ArrayList;

public class GetFields_Result {

  private ArrayList<Fields> fields;
  private int project_id;

  public ArrayList<Fields> getFields() {
    return fields;
  }

  public void setFields(ArrayList<Fields> fields) {
    this.fields = fields;
  }

  public int getProject_id() {
    return project_id;
  }

  public void setProject_id(int project_id) {
    this.project_id = project_id;
  }

}
