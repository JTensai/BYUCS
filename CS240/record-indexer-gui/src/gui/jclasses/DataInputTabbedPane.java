package gui.jclasses;

import gui.communications.BatchState;

import java.awt.Dimension;

import javax.swing.*;

@SuppressWarnings("serial")
public class DataInputTabbedPane extends JTabbedPane {
  DataInputTabbedPane(BatchState batch_state) {
    Dimension start_size = new Dimension(500, 0);
    this.setMinimumSize(start_size);

    JPanel table = new TableEntryPanel(batch_state);
    JPanel form = new FormEntryPanel(batch_state);

    this.addTab("Table", table);
    this.addTab("Form", form);
  }
}
