package gui.jclasses;

import gui.communications.BatchState;

import javax.swing.*;

import listener_interfaces.BatchStateListener;

@SuppressWarnings("serial")
public class SubSplitPane extends JSplitPane implements BatchStateListener {
  BatchState batch_state;
  SubSplitPane(BatchState batch_state) {
    batch_state.addListener(this);    

    this.batch_state = batch_state;
    this.setOrientation(HORIZONTAL_SPLIT);
    this.setLeftComponent(new DataInputTabbedPane(batch_state));
    this.setRightComponent(new InfoTabbedPane(batch_state));
  }
  

  public void loginSuccess() {
    this.setDividerLocation(batch_state.getSettings().getVSplit());
  }
  public void loginExit() {}
  public void downloadBatch() {}
  public void logout() {
    System.out.println(this.getDividerLocation());
    batch_state.getSettings().setVSplit(this.getDividerLocation());
  }
  public void exit() {
    batch_state.getSettings().setVSplit(this.getDividerLocation());
  }
  public void zoomInButton() {}
  public void zoomOutButton() {}
  public void invertImageButton() {}
  public void toggleHighlightsButton() {}
  public void synchronizeTable() {}
  public void synchronizeImage() {}
  public void submitButton() {
    batch_state.getSettings().setVSplit(this.getDividerLocation());
  }
  public void saveButton() {
    batch_state.getSettings().setVSplit(this.getDividerLocation());
  }
}
