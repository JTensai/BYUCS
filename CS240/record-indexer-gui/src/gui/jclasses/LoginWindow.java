package gui.jclasses;

import gui.communications.BatchState;

import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

import javax.swing.*;

import listener_interfaces.BatchStateListener;
import listener_interfaces.LoginWindowListener;
import shared.communication.ValidateUser_Result;
import shared.model.Users;

@SuppressWarnings("serial")
public class LoginWindow extends JFrame implements ActionListener, BatchStateListener {
  private ArrayList<LoginWindowListener> listeners = new ArrayList<LoginWindowListener>();
  private BatchState batch_state;
  JPanel login_panel = new JPanel();

  JPanel username_panel = new JPanel();
  JLabel username_label = new JLabel("Username: ");
  JTextField username_field = new JTextField(20);

  JPanel password_panel = new JPanel();
  JLabel password_label = new JLabel("Password: ");
  JPasswordField password_field = new JPasswordField(20);

  JPanel button_panel = new JPanel();
  JButton login_button = new JButton("Login");
  JButton exit_button = new JButton("Exit");
  
  
  public LoginWindow(BatchState batch_state) {
    this.addListener(batch_state);
    setBatchState(batch_state);

    // Create and set up the window.
    this.setTitle("Record Indexer Login");
    this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    this.setResizable(false);

    login_panel.setLayout(new BoxLayout(login_panel, BoxLayout.Y_AXIS));

    username_panel.setLayout(new BoxLayout(username_panel, BoxLayout.X_AXIS));
    username_panel.add(username_label);
    username_panel.add(Box.createRigidArea(new Dimension(5, 0)));
    username_panel.add(username_field);

    password_panel.setLayout(new BoxLayout(password_panel, BoxLayout.X_AXIS));
    password_panel.add(password_label);
    password_panel.add(Box.createRigidArea(new Dimension(5, 0)));
    password_panel.add(password_field);

    login_button.setActionCommand("login");
    login_button.addActionListener(this);

    exit_button.setActionCommand("exit");
    exit_button.addActionListener(this);

    button_panel.setLayout(new BoxLayout(button_panel, BoxLayout.LINE_AXIS));
    button_panel.add(login_button);
    button_panel.add(Box.createRigidArea(new Dimension(40, 0)));
    button_panel.add(exit_button);

    login_panel.setBorder(BorderFactory.createEmptyBorder(30, 70, 30, 70));
    login_panel.add(Box.createRigidArea(new Dimension(0, 10)));
    login_panel.add(username_panel);
    login_panel.add(Box.createRigidArea(new Dimension(0, 7)));
    login_panel.add(password_panel);
    login_panel.add(Box.createRigidArea(new Dimension(0, 20)));
    login_panel.add(button_panel);
    login_panel.add(Box.createRigidArea(new Dimension(0, 20)));
    login_panel.add(Box.createVerticalGlue());
    
    this.add(login_panel);
    this.pack();
    MyJClass.centerWindow(this);
    this.setVisible(true);
  }
  
  private void setBatchState(BatchState batch_state) {
    this.batch_state = batch_state;    
  }
  
  private BatchState getBatchState(){
    return batch_state;
  }

  public void addListener(LoginWindowListener listener) {
    listeners.add(listener);
  }
  
  private void notifyListeners(String s) {
    for (LoginWindowListener listener : listeners) {
      if (s.equals("login success")){
        listener.loginSuccess();
      }
      else if (s.equals("login exit")) {
        listener.loginExit();
      }
    }
  }

  public void actionPerformed(ActionEvent e) {
    if ("login".equals(e.getActionCommand())) {
      ValidateUser_Result result = getBatchState().getFacade().validateUser(username_field.getText(), new String(password_field.getPassword()));
      Users valid_user = result.getUser();
      
      if (valid_user.getFirstName() != null) {
        // User is valid, show welcome notification
        JOptionPane.showMessageDialog(this, "Welcome " + valid_user.getFirstName() + " " + valid_user.getLastName()
            + ".\nYou have indexed " + valid_user.getIndexedRecords() + " record(s).", "Welcome Message", JOptionPane.PLAIN_MESSAGE);
        
        getBatchState().setCurrentUser(valid_user);
        getBatchState().setCurrentBatchId(valid_user.getCurrentBatchId());
        this.dispose();
        
        notifyListeners("login success");
      }
      else {
        // Show dialog box saying input was invalid
        JOptionPane.showMessageDialog(this, "Username or Password is invalid, please try again.", "Login Error", JOptionPane.ERROR_MESSAGE);
      }
    }
    else if ("exit".equals(e.getActionCommand())) {
      notifyListeners("login exit");
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
