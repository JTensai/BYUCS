package gui.jclasses;

import gui.communications.BatchState;

import javax.swing.*;

@SuppressWarnings("serial")
public class InfoTabbedPane extends JTabbedPane {
  InfoTabbedPane(BatchState batch_state) {

    JPanel help_info = new HelpInfoPanel(batch_state);
    JPanel image_nav = new ImageNavPanel(batch_state);

    this.addTab("Help Info", help_info);
    this.addTab("Image Navigation", image_nav);

  }
}
