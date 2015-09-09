package gui.jclasses;

import gui.communications.BatchState;

import java.awt.Dimension;
import java.awt.Image;
import java.awt.event.*;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.*;

import shared.communication.*;
import shared.model.*;
import listener_interfaces.*;

@SuppressWarnings("serial")
public class DownloadBatchWindow extends JDialog implements ActionListener, BatchStateListener {
  private ArrayList<DownloadBatchWindowListener> listeners = new ArrayList<DownloadBatchWindowListener>();
  private BatchState batch_state;
  JPanel window_panel = new JPanel();    
  
  JPanel select_panel = new JPanel();  
  JPanel button_panel = new JPanel();  
  
  JLabel project_select_label = new JLabel("Project: ");
  JComboBox<Object> project_select;
  JButton view_sample_button = new JButton("View Sample");
  JButton cancel_button = new JButton("Cancel");
  JButton download_button = new JButton("Download");
  
  
  public DownloadBatchWindow(BatchState batch_state){
    batch_state.addListener(this);
    this.addListener(batch_state);
    setBatchState(batch_state);

    this.setTitle("Download Batch");
    this.setResizable(false);

    window_panel.setLayout(new BoxLayout(window_panel, BoxLayout.Y_AXIS));
    select_panel.setLayout(new BoxLayout(select_panel, BoxLayout.X_AXIS));
    button_panel.setLayout(new BoxLayout(button_panel, BoxLayout.X_AXIS));

    window_panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
    
    GetProjects_Result projects_result = getBatchState().getFacade().getProjects(getBatchState().getCurrentUser());
    ArrayList<Projects> projects = projects_result.getProjects();
    Projects[] projects_list = new Projects[projects.size()];
    for (Projects project : projects){
      projects_list[project.getId()-1] = project;
    }
    project_select = new JComboBox<Object>(projects_list);
    project_select.addActionListener(this);

    view_sample_button.setActionCommand("get sample image");
    view_sample_button.addActionListener(this);

    download_button.setActionCommand("download batch download");
    download_button.addActionListener(this);
    
    cancel_button.setActionCommand("download batch cancel");
    cancel_button.addActionListener(this);
    
    select_panel.add(project_select_label);
    select_panel.add(Box.createRigidArea(new Dimension(4, 0)));
    select_panel.add(project_select);
    select_panel.add(Box.createRigidArea(new Dimension(6, 0)));
    select_panel.add(view_sample_button);
    
    button_panel.add(cancel_button);
    button_panel.add(Box.createRigidArea(new Dimension(6, 0)));
    button_panel.add(download_button);
    
    window_panel.add(select_panel);
    window_panel.add(Box.createRigidArea(new Dimension(0, 10)));
    window_panel.add(button_panel);
    
    
    
    
    this.add(window_panel);
    this.pack();
    MyJClass.centerWindow(this);
    this.setModalityType(DEFAULT_MODALITY_TYPE);
    this.setVisible(true);
    
  }


  public BatchState getBatchState() {
    return batch_state;
  }
  public void setBatchState(BatchState batch_state) {
    this.batch_state = batch_state;
  }

  public void addListener(DownloadBatchWindowListener menu_bar_listener) {
    listeners.add(menu_bar_listener);
  }
  
  private void notifyListeners(String s) {
    for (DownloadBatchWindowListener listener : listeners) {
      if (s.equals("download batch download")){
        listener.downloadBatch();
      }
    }
  }
  
  public void actionPerformed(ActionEvent e) {
    if ("get sample image".equals(e.getActionCommand())) {
      Projects project = (Projects)project_select.getSelectedItem();
      GetSampleImage_Result result = getBatchState().getFacade().getSampleImage(project.getId(), getBatchState().getCurrentUser());
      
      /*
       * TODO Need to get the host and port num from parameters, not hardcoded
       * 
       */
      
      Image image = null;
      try {
          URL url = new URL(getBatchState().getUrl() + result.getImage_url());
          image = ImageIO.read(url);
      } 
      catch (IOException i) {
        i.printStackTrace();
      }
      
      JLabel image_icon = new JLabel (new ImageIcon(image));
      System.out.println(project.getTitle() + " : " + project.getId() + " (" + getBatchState().getUrl() + result.getImage_url() + ")");
      JOptionPane.showMessageDialog(this, image_icon, "Sample image from " + project.getTitle(), JOptionPane.PLAIN_MESSAGE);
    }
    else if ("download batch download".equals(e.getActionCommand())) {
      Projects project = (Projects)project_select.getSelectedItem();
      DownloadBatch_Result result = getBatchState().getFacade().downloadBatch(project.getId(), getBatchState().getCurrentUser());
      if (result != null){
        getBatchState().setCurrentBatch(result.getBatch());
        getBatchState().setCurrentProject(result.getProject());
        getBatchState().getCurrentProject().setFields(result.getFields());
        getBatchState().setCurrentField(result.getFields().get(0));
  //      getBatchState().setCurrentBatchId
        this.dispose();
        notifyListeners("download batch download");
      }
      else{
        JOptionPane.showMessageDialog(this, "There was a problem downloading this batch.", "Login Error", JOptionPane.ERROR_MESSAGE);
      }
    }
    else if ("download batch cancel".equals(e.getActionCommand())) {
      this.dispose();
    }
  }
  
  public void loginSuccess() {}
  public void loginExit() {}
  public void downloadBatch() {}
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
