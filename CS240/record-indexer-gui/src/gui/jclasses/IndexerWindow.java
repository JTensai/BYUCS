package gui.jclasses;

import gui.communications.*;

import java.awt.*;

import javax.swing.*;

import listener_interfaces.BatchStateListener;

@SuppressWarnings("serial")
public class IndexerWindow extends JFrame implements BatchStateListener {
  private BatchState batch_state;

  public IndexerWindow(BatchState batch_state) {
    setBatchState(batch_state);
    
    this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    JPanel content_pane = new JPanel(new BorderLayout());
        
    JPanel menu = new MenuPanel(batch_state);
    JSplitPane main = new MainSplitPane(batch_state);

    content_pane.add(menu, BorderLayout.PAGE_START);
    content_pane.add(main, BorderLayout.CENTER);

    this.add(content_pane);
    this.pack();
    
    if (getBatchState().getSettings() != null){
      System.out.println("Setting exist, using them.");
      this.setLocation(getBatchState().getSettings().getWindowPosition());
      this.setPreferredSize(getBatchState().getSettings().getWindowSize());      
    }
    else{
      System.out.println("No Settings, using defaults.");
      Dimension start_size = new Dimension(1200, 800);
      this.setPreferredSize(start_size);

//      this.setLocation(100, 100);
      MyJClass.centerWindow(this);
      
    }
    this.setVisible(false);
  }

  public void update(){
    StringBuilder builder = new StringBuilder();
    builder.append("Record Indexer - ");
    builder.append(getBatchState().getCurrentUser().getFirstName() + " " + getBatchState().getCurrentUser().getLastName());
    if (getBatchState().getCurrentUser().getCurrentBatchId() > 0)
      builder.append(" (Batch ID: " + getBatchState().getCurrentUser().getCurrentBatchId() + ")");
    
    this.setTitle(builder.toString());

//    System.out.println(getBatchState().getSettings().getWindowSize().width + " x " + getBatchState().getSettings().getWindowSize().height);
    this.setLocation(getBatchState().getSettings().getWindowPosition());
    this.setSize(getBatchState().getSettings().getWindowSize());
  }

  public BatchState getBatchState() {
    return batch_state;
  }
  public void setBatchState(BatchState batch_state) {
    this.batch_state = batch_state;
  }

  public void loginSuccess() {
    this.update();
    this.setVisible(true);    
  }
  
  public void loginExit() {
    this.dispose();
  }

  public void downloadBatch() {
    update();
  }

  public void logout() {
    this.dispose();
    getBatchState().getSettings().setWindowPosition(this.getLocation());
    getBatchState().getSettings().setWindowSize(this.getSize());
    new LoginWindow(batch_state);
  }

  public void exit() {
//    System.out.println("Exiting called by MenuBar");
    getBatchState().getSettings().setWindowPosition(this.getLocation());
    getBatchState().getSettings().setWindowSize(this.getSize());
    // Saves current state
    this.dispose();
  }
  public void zoomInButton() {}
  public void zoomOutButton() {}
  public void invertImageButton() {}
  public void toggleHighlightsButton() {}
  public void synchronizeTable() {}
  public void synchronizeImage() {}
  public void submitButton() {
    this.dispose();
  }
  public void saveButton() {}
}
