package gui.jclasses;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.io.IOException;

import gui.communications.BatchState;

import javax.swing.*;

import listener_interfaces.BatchStateListener;

@SuppressWarnings("serial")
public class HelpInfoPanel extends JPanel implements BatchStateListener {
  private BatchState batch_state;
  private JEditorPane html_viewer;
  
  HelpInfoPanel(BatchState batch_state) {
    this.setLayout(new BorderLayout());
    batch_state.addListener(this);
    this.batch_state = batch_state;

    html_viewer = new JEditorPane();
    update();
  }

  private void update(){
    if (batch_state.getCurrentField() != null){
      this.removeAll();
      html_viewer.setOpaque(true);
      html_viewer.setBackground(Color.white);
      html_viewer.setPreferredSize(new Dimension(this.getWidth(), this.getHeight()));
      html_viewer.setEditable(false);
      html_viewer.setContentType("text/html");

      loadPage(batch_state.getUrl() + batch_state.getCurrentField().getHelpHtml());

      JScrollPane htmlScrollPane = new JScrollPane(html_viewer);
      htmlScrollPane.setPreferredSize(new Dimension(this.getWidth(), this.getHeight()));
      this.add(htmlScrollPane, BorderLayout.CENTER);
      this.revalidate();
    }
    else
      this.removeAll();
  }
  

  private void loadPage(String url) {
    try {
      html_viewer.setPage(url);
    }
    catch (IOException ex) {
      ex.printStackTrace();
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
  public void synchronizeTable() {
    update();
  }
  public void synchronizeImage() {
  }
  public void submitButton() {}
  public void saveButton() {}
  
}
