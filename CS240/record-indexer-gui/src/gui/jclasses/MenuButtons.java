package gui.jclasses;

import gui.communications.BatchState;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.*;

import listener_interfaces.*;

@SuppressWarnings("serial")
public class MenuButtons extends JPanel implements ActionListener, BatchStateListener{
  private ArrayList<MenuButtonsListener> listeners = new ArrayList<MenuButtonsListener>();
  private BatchState batch_state;


  JButton zoom_in_button = new JButton("Zoom In");
  JButton zoom_out_button = new JButton("Zoom Out");
  JButton invert_image_button = new JButton("Invert Image");
  JButton toggle_highlights_button = new JButton("Toggle Highlights");
  JButton save_button = new JButton("Save");
  JButton submit_button = new JButton("Submit");
  
  public MenuButtons(BatchState batch_state) {
    batch_state.addListener(this);
    this.addListener(batch_state);
    setBatchState(batch_state);
    
    this.setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));
    this.setBorder(BorderFactory.createEmptyBorder(5, 0, 5, 0));
    // button_panel.setAlignmentY(button_panel.BOTTOM_ALIGNMENT);
    // button_panel.setAlignmentX(this.LEFT_ALIGNMENT);

    this.add(Box.createRigidArea(new Dimension(5, 0)));
    this.add(zoom_in_button);
    this.add(Box.createRigidArea(new Dimension(5, 0)));
    this.add(zoom_out_button);
    this.add(Box.createRigidArea(new Dimension(5, 0)));
    this.add(invert_image_button);
    this.add(Box.createRigidArea(new Dimension(5, 0)));
    this.add(toggle_highlights_button);
    this.add(Box.createRigidArea(new Dimension(5, 0)));
    this.add(save_button);
    this.add(Box.createRigidArea(new Dimension(5, 0)));
    this.add(submit_button);
    this.add(Box.createRigidArea(new Dimension(5, 0)));
    this.add(Box.createHorizontalGlue());

    zoom_in_button.setEnabled(false);
    zoom_out_button.setEnabled(false);
    invert_image_button.setEnabled(false);
    toggle_highlights_button.setEnabled(false);
    save_button.setEnabled(false);
    submit_button.setEnabled(false);

    zoom_in_button.setActionCommand("zoom in");
    zoom_in_button.addActionListener(this);
    zoom_out_button.setActionCommand("zoom out");
    zoom_out_button.addActionListener(this);
    invert_image_button.setActionCommand("invert image");
    invert_image_button.addActionListener(this);
    toggle_highlights_button.setActionCommand("toggle highlights");
    toggle_highlights_button.addActionListener(this);
    save_button.setActionCommand("save");
    save_button.addActionListener(this);
    submit_button.setActionCommand("submit");
    submit_button.addActionListener(this);
  }
  
  public void update(){
//    System.out.println("MenuButtons update() : " + getBatchState().getCurrentBatchId());
    if (getBatchState().getCurrentBatchId() > 0){
      zoom_in_button.setEnabled(true);
      zoom_out_button.setEnabled(true);
      invert_image_button.setEnabled(true);
      toggle_highlights_button.setEnabled(true);
      save_button.setEnabled(true);
      submit_button.setEnabled(true);
    }
    else{
      zoom_in_button.setEnabled(false);
      zoom_out_button.setEnabled(false);
      invert_image_button.setEnabled(false);
      toggle_highlights_button.setEnabled(false);
      save_button.setEnabled(false);
      submit_button.setEnabled(false);      
    }
  }

  public BatchState getBatchState() {
    return batch_state;
  }
  public void setBatchState(BatchState batch_state) {
    this.batch_state = batch_state;
  }

  public void addListener(MenuButtonsListener menu_bar_listener) {
    listeners.add(menu_bar_listener);
  }
  
  private void notifyListeners(String s) {
    for (MenuButtonsListener listener : listeners) {
      if (s.equals("zoom in"))
        listener.zoomInButton();
      else if (s.equals("zoom out"))
        listener.zoomOutButton();
      else if (s.equals("invert image")) 
        listener.invertImageButton();
      else if (s.equals("toggle highlights")) 
        listener.toggleHighlightsButton();
      else if (s.equals("save")) 
        listener.saveButton();
      else if (s.equals("submit")) 
        listener.submitButton();
    }
  }
  
  public void actionPerformed(ActionEvent e) {
    if ("zoom in".equals(e.getActionCommand())) {
      notifyListeners("zoom in");
    }
    else if ("zoom out".equals(e.getActionCommand())) {
      notifyListeners("zoom out");
    }
    else if ("invert image".equals(e.getActionCommand())) {
      notifyListeners("invert image");
    }
    else if ("toggle highlights".equals(e.getActionCommand())) {
      notifyListeners("toggle highlights");
    }
    else if ("save".equals(e.getActionCommand())) {
      notifyListeners("save");
    }
    else if ("submit".equals(e.getActionCommand())) {
      notifyListeners("submit");
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
