package gui.jclasses;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;

import gui.communications.BatchState;

import javax.swing.*;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import shared.model.Fields;
import listener_interfaces.BatchStateListener;
import listener_interfaces.TableEntryListener;

@SuppressWarnings("serial")
public class FormEntryPanel extends JPanel implements BatchStateListener, TableModelListener {
  private ArrayList<TableEntryListener> listeners = new ArrayList<TableEntryListener>();
  private BatchState batch_state;
  private JPanel list_panel = new JPanel();
  private JPanel label_panel = new JPanel();
  private JPanel fields_panel = new JPanel();
  private JList<Object> list;
  
  ArrayList<JLabel> labels;
  ArrayList<JTextField> text_fields;
  
  public FormEntryPanel(BatchState batch_state) {
    batch_state.addListener(this);
    this.addListener(batch_state);
    setBatchState(batch_state);

    this.setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));

    JScrollPane scrollPane = new JScrollPane();

    this.add(scrollPane);
    this.revalidate();
    update();
  }
  
  public void update(){
    if (getBatchState().getCurrentBatchId() > 0){
      this.removeAll();
      
      list_panel.setBackground(new Color(180, 180, 180));
      list_panel.setLayout(new BoxLayout(list_panel, BoxLayout.PAGE_AXIS));
      
      label_panel.setBackground(new Color(80, 80, 80));
      label_panel.setLayout(new BoxLayout(label_panel, BoxLayout.PAGE_AXIS));    

      fields_panel.setBackground(new Color(0, 80, 80));
      fields_panel.setLayout(new BoxLayout(fields_panel, BoxLayout.PAGE_AXIS));
            
      list_panel.setLayout(new FlowLayout());
      int records_per_image = getBatchState().getCurrentProject().getRecordsPerImage();
      String[] line_nums = new String[records_per_image];
      for (int i = 0; i < records_per_image; i++){
        line_nums[i] = Integer.toString(i+1);
      }
      list = new JList<Object>(line_nums);
      list.setSelectedIndex(getBatchState().getSettings().getHighlightedRow());
      list.setFixedCellWidth(120);
      list.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
      list_panel.add(new JScrollPane(list));

      list.addMouseListener(mouseAdapter);
      
      JPanel label_panel = new JPanel();
      label_panel.setLayout(new BoxLayout(label_panel, BoxLayout.PAGE_AXIS));
      
      labels = new ArrayList<JLabel>();
      text_fields = new ArrayList<JTextField>();
      
      for (Fields field : getBatchState().getCurrentProject().getFields()){          
        labels.add(new JLabel(field.getTitle()));
        text_fields.add(new JTextField(15));  
      }   

      for (JLabel label : labels){
        label_panel.add(Box.createRigidArea(new Dimension(80, 0))); 
        label_panel.add(label);
        label_panel.add(Box.createRigidArea(new Dimension(80, 0))); 
        label_panel.add(Box.createVerticalGlue());     
      }
      for (JTextField text_field : text_fields){
        fields_panel.add(Box.createRigidArea(new Dimension(80, 0))); 
        text_field.setMaximumSize(new Dimension(150, 20));
        fields_panel.add(text_field);
        fields_panel.add(Box.createVerticalGlue());   
        text_field.addMouseListener(mouseAdapter);
//      fields_panel.add(Box.createHorizontalGlue());     
      }
      
      this.add(list_panel);
      this.add(label_panel);
      this.add(fields_panel);
    }
  }
  

  private MouseAdapter mouseAdapter = new MouseAdapter() {
    public void mouseClicked(MouseEvent e){
      System.out.println("Clicked on form " + e.getSource());
//      getBatchState().getSettings().setHighlightedRow(table.getSelectedRow());
//      getBatchState().getSettings().setHighlightedField(table.getSelectedColumn()-1);
//      notifyListeners("table changed");
    }
  };
  
  public BatchState getBatchState() {
    return batch_state;
  }
  public void setBatchState(BatchState batch_state) {
    this.batch_state = batch_state;
  }

  public void addListener(TableEntryListener menu_bar_listener) {
    listeners.add(menu_bar_listener);
  }
  
  private void notifyListeners(String s) {
    for (TableEntryListener listener : listeners) {
      if (s.equals("table changed"))
        listener.tableChanged();
    }
  }
  
  public void actionPerformed(ActionEvent e) {
    if ("zoom in".equals(e.getActionCommand())) {
      notifyListeners("zoom in");
    }
  }  

  @Override
  public void tableChanged(TableModelEvent e) {}
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
  public void submitButton() {}
  public void synchronizeTable() {
    text_fields.get(getBatchState().getSettings().getHighlightedField()).requestFocus();
    list.setSelectedIndex(getBatchState().getSettings().getHighlightedRow());
  }
  public void synchronizeImage() {}
  public void saveButton() {}
}
