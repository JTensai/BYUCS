package gui;

import gui.communications.*;
import gui.jclasses.*;



public class Main {
  static BatchState batch_state;
  static String host;
  static String port;

  public static void main(String[] args) {
    if (args.length > 0){
      host = args[0];
      port = args[1];
    }
    
    javax.swing.SwingUtilities.invokeLater(new Runnable() {
      public void run() {

        initComponents();
      }
    });
  }
  
  private static void initComponents() {
//    facade = new ClientFacade();
    batch_state = new BatchState(host, port);
//    login_window = new LoginWindow(batch_state);
//    
//    login_window.addListener(indexer_window);   
  }
}
