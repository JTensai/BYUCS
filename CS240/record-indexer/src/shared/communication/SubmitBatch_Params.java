package shared.communication;

import shared.model.*;

public class SubmitBatch_Params {

  private Users user;
  private int batch_id;
  // private int field_id;
  private String values;

  public Users getUser() {
    return user;
  }

  public void setUser(Users user) {
    this.user = user;
  }

  public int getBatchId() {
    return batch_id;
  }

  public void setBatchId(int batch_id) {
    this.batch_id = batch_id;
  }

  public String getValues() {
    return values;
  }

  public void setValues(String values) {
    this.values = values;
  }

  // public int getFieldId() {
  // return field_id;
  // }
  //
  // public void setFieldId(int field_id) {
  // this.field_id = field_id;
  // }

}
