package gui.jclasses;

import gui.communications.BatchState;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.*;

import listener_interfaces.BatchStateListener;
import listener_interfaces.MenuBarListener;

@SuppressWarnings("serial")
public class MenuBar extends JPanel implements ActionListener, BatchStateListener {
  private ArrayList<MenuBarListener> listeners = new ArrayList<MenuBarListener>();
  private BatchState batch_state;

  JMenuBar menu_bar;
  JMenu menu;
  JMenuItem downloadBatch;
  JMenuItem logout;
  JMenuItem exit;
  
  public MenuBar(BatchState batch_state) {
    batch_state.addListener(this);
    this.addListener(batch_state);
    setBatchState(batch_state);
    
    this.setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));

    menu_bar = new JMenuBar();    
    menu = new JMenu("File");
    
    downloadBatch = new JMenuItem("Download Batch");
    downloadBatch.setActionCommand("downloadBatch");
    downloadBatch.addActionListener(this);
    downloadBatch.setEnabled(true);
    menu.add(downloadBatch);
    
    logout = new JMenuItem("Logout");
    logout.setActionCommand("logout");
    logout.addActionListener(this);
    menu.add(logout);
    
    exit = new JMenuItem("Exit");
    exit.setActionCommand("exit");
    exit.addActionListener(this);
    menu.add(exit);
    
    menu_bar.add(menu);

    this.add(menu_bar);
    this.add(Box.createHorizontalGlue());
  }
  
  public void update(){
    if (getBatchState().getCurrentBatchId() > 0){
      downloadBatch.setEnabled(false);
    }
    else{
      downloadBatch.setEnabled(true);
    }
  }
  
  public BatchState getBatchState() {
    return batch_state;
  }
  public void setBatchState(BatchState batch_state) {
    this.batch_state = batch_state;
  }

  public void addListener(MenuBarListener menu_bar_listener) {
    listeners.add(menu_bar_listener);
  }
  
  private void notifyListeners(String s) {
    for (MenuBarListener listener : listeners) {
      if (s.equals("logout"))
        listener.logout();
      else if (s.equals("exit")) 
        listener.exit();
    }
  }
  
  @Override
  public void actionPerformed(ActionEvent e) {
    if ("downloadBatch".equals(e.getActionCommand())) {
      new DownloadBatchWindow(getBatchState());
    }
    else if ("logout".equals(e.getActionCommand())) {
      notifyListeners("logout");
    }
    else if ("exit".equals(e.getActionCommand())) {
      notifyListeners("exit");
    }
  }
  
  public void loginSuccess() {
    update();
  }
  public void loginExit() {}
  
  public void downloadBatch() {
    update();
  }
  
  public void logout() {}
  public void exit() {}
  public void zoomInButton() {}
  public void zoomOutButton() {}
  public void invertImageButton() {}
  public void toggleHighlightsButton() {}
  public void synchronizeTable() {}
  public void synchronizeImage() {}
  public void submitButton() {}
  public void saveButton() {}
}
