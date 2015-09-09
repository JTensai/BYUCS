package shared.communication;

import shared.model.*;

import java.util.ArrayList;

public class Search_Params {

  private Users user;
  private ArrayList<Integer> field_ids;
  private ArrayList<String> search_values;

  public Users getUser() {
    return user;
  }

  public void setUser(Users user) {
    this.user = user;
  }

  public ArrayList<Integer> getFields() {
    return field_ids;
  }

  public void setFields(ArrayList<Integer> fields) {
    this.field_ids = fields;
  }

  public ArrayList<String> getSearch_values() {
    return search_values;
  }

  public void setSearch_values(ArrayList<String> search_values) {
    this.search_values = search_values;
  }

}
