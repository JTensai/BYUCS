package listener_interfaces;

public interface BatchStateListener {

  public void loginSuccess();
  public void loginExit();
  public void downloadBatch();
  public void logout();
  public void exit();
  
  void zoomInButton();
  void zoomOutButton();
  void invertImageButton();
  void toggleHighlightsButton();
  void saveButton();
  void submitButton();
  public void synchronizeTable();
  public void synchronizeImage();

}
