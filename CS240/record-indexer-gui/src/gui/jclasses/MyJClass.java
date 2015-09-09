package gui.jclasses;

import java.awt.*;

import javax.swing.*;

public class MyJClass {
  public static void centerWindow(JFrame frame) {
    Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
    int x = (int) ((dimension.getWidth() - frame.getWidth()) / 2);
    int y = (int) ((dimension.getHeight() - frame.getHeight()) / 2);
    frame.setLocation(x, y);
  }

  public static void centerWindow(JDialog dialog) {
    Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
    int x = (int) ((dimension.getWidth() - dialog.getWidth()) / 2);
    int y = (int) ((dimension.getHeight() - dialog.getHeight()) / 2);
    dialog.setLocation(x, y);    
  }
}
