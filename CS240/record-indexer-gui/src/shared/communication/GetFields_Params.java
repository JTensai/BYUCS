package shared.communication;

import shared.model.*;

public class GetFields_Params {

  private Users user;
  private int project_id;

  public Users getUser() {
    return user;
  }

  public void setUser(Users user) {
    this.user = user;
  }

  public int getProject_id() {
    return project_id;
  }

  public void setProject_id(int project_id) {
    this.project_id = project_id;
  }

}
