package shared.communication;

import java.util.ArrayList;

import shared.model.*;

public class Search_Result {

  private Batches batch;
  private int field_id;
  private ArrayList<Records> results;

  public Batches getBatch() {
    return batch;
  }

  public void setBatch(Batches batch) {
    this.batch = batch;
  }

  public int getField_id() {
    return field_id;
  }

  public void setField_id(int field_id) {
    this.field_id = field_id;
  }

  public ArrayList<Records> getResults() {
    return results;
  }

  public void setResults(ArrayList<Records> results) {
    this.results = results;
  }

}
