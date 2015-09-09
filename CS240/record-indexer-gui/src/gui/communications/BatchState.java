package gui.communications;

import gui.jclasses.*;

import java.awt.image.BufferedImage;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import listener_interfaces.*;
import shared.communication.SubmitBatch_Result;
import shared.communication.ValidateUser_Result;
import shared.model.*;

public class BatchState implements LoginWindowListener, MenuBarListener, MenuButtonsListener, DownloadBatchWindowListener, ImageViewerListener, TableEntryListener{
  private ArrayList<BatchStateListener> listeners = new ArrayList<BatchStateListener>();
  
  private ClientFacade facade;
  private Users current_user = new Users();
  private Projects current_project = new Projects();
  private Batches current_batch = new Batches();
  private Fields current_field = new Fields();
  private BufferedImage image = null;
  private UserSettings settings = null;
  private String host;
  private String port;
  
  public BatchState(String host, String port) {

    setHost(host);
    setPort(port);
    facade = new ClientFacade(getHost(), getPort());
    addListener(new LoginWindow(this));
    addListener(new IndexerWindow(this));  
  }
  
  
  public Users getCurrentUser(){
    return current_user;
  }  
  public void setCurrentUser(Users user){
    current_user = user;
  }
  public ClientFacade getFacade() {
    return facade;
  }
  public void setFacade(ClientFacade facade) {
    this.facade = facade;
  }
  public int getCurrentBatchId(){
    return getCurrentUser().getCurrentBatchId();
  }
  public void setCurrentBatchId(int batch_id) {
    getCurrentUser().setCurrentBatchId(batch_id);
  }
  public Batches getCurrentBatch(){
    return current_batch;
  }
  public void setCurrentBatch(Batches batch){
    current_batch = batch;
    if (batch != null)
      setCurrentBatchId(batch.getId());
    else
      setCurrentBatchId(-1);
  }
  public UserSettings getSettings() {
    return settings;
  }
  public void setSettings(UserSettings settings) {
    this.settings = settings;
    if (settings == null){
      setCurrentBatch(null);
      setCurrentProject(null);
      setCurrentUser(null);
      setCurrentField(null);
    }
    else{
      setCurrentBatch(settings.getBatch());
      setCurrentProject(settings.getProject());
    }
  }
  public Projects getCurrentProject() {
    return current_project;
  }
  public void setCurrentProject(Projects current_project) {
    this.current_project = current_project;
  }
  public BufferedImage getImage(){
    return image;
  }
  public void setImage(BufferedImage image){
    this.image = image;
  }
  public Fields getCurrentField() {
    return current_field;
  }
  public void setCurrentField(Fields current_field) {
    this.current_field = current_field;
  }
  public void setCurrentField(int column){
    for (Fields field : getCurrentProject().getFields()){
      if (field.getPosition() == column)
        current_field = field;
    }
    
  }
  public String getHost() {
    return host;
  }
  public void setHost(String host) {
    this.host = host;
  }
  public String getPort() {
    return port;
  }
  public void setPort(String port) {
    this.port = port;
  }
  public String getUrl(){
    return "http://" + getHost() + ":" + getPort() + "/";
  }
  
  
  public String getBatchValues(){
    StringBuilder builder = new StringBuilder();
    //Jones,Fred,13;Rogers,Susan,42;,,;,,;Van Fleet,Bill,23
    int field_count = 1;
    for (Records record : getCurrentBatch().getRecords()){
      if (field_count == getCurrentProject().getFields().size()){
        builder.append(record.getAnswer() + ";");
        field_count = 1;
      }
      else{
        builder.append(record.getAnswer() + ",");
        field_count++;
      }
    }
    System.out.println(builder.toString());
    return builder.toString();
  }
  
  
  public void addListener(BatchStateListener listener) {
    listeners.add(listener);
  }
  
  private void notifyListeners(String s) {
    for (BatchStateListener listener : listeners) {
      if (s.equals("login success"))
        listener.loginSuccess();
      else if (s.equals("login exit"))
        listener.loginExit();
      else if (s.equals("download batch download"))
        listener.downloadBatch();
      else if (s.equals("logout"))
        listener.logout();
      else if (s.equals("exit"))
        listener.exit();
      else if (s.equals("zoom in"))
        listener.zoomInButton();
      else if (s.equals("zoom out"))
        listener.zoomOutButton();
      else if (s.equals("invert image"))
        listener.invertImageButton();
      else if (s.equals("toggle highlights"))
        listener.toggleHighlightsButton();
      else if (s.equals("submit batch"))
        listener.submitButton();
      else if (s.equals("synchronize")){
        listener.synchronizeImage();
        listener.synchronizeTable();
      }
    }
  }
  

  public void loginSuccess() {
    setSettings(new UserSettings(getCurrentUser().getUsername()));
    for(Fields field : getCurrentProject().getFields()){
      
      if (field.getPosition() == getSettings().getHighlightedField()+1){
        setCurrentField(field);
      }
    }
    notifyListeners("login success");
  }
  public void loginExit() {
    notifyListeners("login exit");
  }
  public void downloadBatch() {
    getSettings().writeToFile(getCurrentUser().getUsername(), getCurrentProject(), getCurrentBatch());
    for(Fields field : getCurrentProject().getFields()){
      if (field.getPosition() == getSettings().getHighlightedField()+1){
        setCurrentField(field);
      }
    }
    notifyListeners("download batch download");    
  }
  public void logout() {
    notifyListeners("logout");   
    getSettings().writeToFile(getCurrentUser().getUsername(), getCurrentProject(), getCurrentBatch());
    setSettings(null);
  }
  public void exit() {
    notifyListeners("exit"); 
    settings.writeToFile(getCurrentUser().getUsername(), getCurrentProject(), getCurrentBatch());  
  }
  public void zoomInButton() {
    notifyListeners("zoom in");
  }
  public void zoomOutButton() {
    notifyListeners("zoom out");
  }
  public void invertImageButton() {
    notifyListeners("invert image");
  }
  public void toggleHighlightsButton() {
    notifyListeners("toggle highlights");
  }
  public void saveButton() {
    settings.writeToFile(getCurrentUser().getUsername(), getCurrentProject(), getCurrentBatch());
  }
  public void submitButton() {
    notifyListeners("submit batch");
    setCurrentProject(new Projects());
    setCurrentBatch(new Batches());
    setCurrentField(new Fields());
    settings.writeToFile(getCurrentUser().getUsername(), getCurrentProject(), getCurrentBatch());
    IndexerWindow indexer = new IndexerWindow(this);
    indexer.setVisible(true);
    addListener(indexer);
  }
  public void tableChanged(){

    notifyListeners("synchronize");
  }
  public void imageFieldClicked() {

    notifyListeners("synchronize");
  }






}
