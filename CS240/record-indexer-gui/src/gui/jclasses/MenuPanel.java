package gui.jclasses;

import gui.communications.BatchState;

import javax.swing.*;

@SuppressWarnings("serial")
public class MenuPanel extends JPanel {
  public MenuPanel(BatchState batch_state) {
    this.setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));

    this.add(new MenuBar(batch_state));
    this.add(new MenuButtons(batch_state));

  }
}
