package shared.communication;

import shared.model.*;
import java.util.ArrayList;

public class DownloadBatch_Result {

  private Projects project;
  private Batches batch;
  private ArrayList<Fields> fields;

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

  public ArrayList<Fields> getFields() {
    return fields;
  }

  public void setFields(ArrayList<Fields> fields) {
    this.fields = fields;
  }

}
