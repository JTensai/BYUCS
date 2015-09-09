package gui.jclasses;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;

import gui.communications.BatchState;

import javax.swing.*;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;

import shared.communication.SubmitBatch_Result;
import shared.model.Fields;
import shared.model.Records;
import listener_interfaces.*;

@SuppressWarnings("serial")
public class TableEntryPanel extends JPanel implements BatchStateListener, TableModelListener {
  private ArrayList<TableEntryListener> listeners = new ArrayList<TableEntryListener>();
  private BatchState batch_state;
  MyTableModel table_model = new MyTableModel(); 
  JTable table;
//  private JScrollPane scroll_pane;
  
  public TableEntryPanel(BatchState batch_state) {  
    super(new GridLayout(1,0));
    
    batch_state.addListener(this);
    this.addListener(batch_state);
    setBatchState(batch_state);
  }
  
  public void update(){
    if (getBatchState().getCurrentBatchId() > 0){
      table_model.setColumnNames();
      table_model.setData();
      table = new JTable(table_model);
      table.setPreferredScrollableViewportSize(new Dimension(500, 70));
      table.getModel().addTableModelListener(this);
      table.setCellSelectionEnabled(true);
      table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      
      table.addMouseListener(mouseAdapter);
            
      JScrollPane scrollPane = new JScrollPane(table);  
      this.add(scrollPane);
    }
  }

  private MouseAdapter mouseAdapter = new MouseAdapter() {
    public void mouseClicked(MouseEvent e){
      int column = table.getSelectedColumn()-1;
      if (column >= 0){
        getBatchState().getSettings().setHighlightedRow(table.getSelectedRow());
        getBatchState().getSettings().setHighlightedField(column);
        getBatchState().setCurrentField(column+1);
        notifyListeners("table changed");
      }
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

  public void tableChanged(TableModelEvent e) {
//    System.out.println(e.getColumn() + " : " + e.getFirstRow());
//    System.out.println((String) table.getValueAt(e.getFirstRow(), e.getColumn()));
//    System.out.println("Position: " + e.getColumn() + " / Field_id:" + positionToFieldId(e.getColumn()));
    getBatchState().getCurrentBatch().getRecords().add(new Records((String) table.getValueAt(e.getFirstRow(), e.getColumn()), getBatchState().getCurrentBatchId(), e.getFirstRow()+1, positionToFieldId(e.getColumn())));
    notifyListeners("table changed");
    spellCheck(e.getFirstRow(), e.getColumn());
//    e.getSource()
  }

  public void spellCheck(int row, int column){
    table.getValueAt(row, column);
    table.setSelectionBackground(new Color(200, 200, 100));
  }
  
  public int positionToFieldId(int position){
    for(Fields field : getBatchState().getCurrentProject().getFields()){
      if (field.getPosition() == position){
        return field.getId();
      }
    }
    return -1;
  }
  
  public int fieldIdToPosition(int field_id){
    for(Fields field : getBatchState().getCurrentProject().getFields()){
      if (field.getId() == field_id){
        return field.getPosition();
      }
    }
    return -1;
  }

  class MyTableModel extends AbstractTableModel {
  
    private String[] column_names;
    private Object[][] data;
    ArrayList<Integer> id_to_position = new ArrayList<Integer>();
    
    public void setColumnNames(){
      if (getBatchState() != null){
        column_names = new String[getBatchState().getCurrentProject().getFields().size()+1];
        column_names[0] = "Record Number";
        for(Fields field : getBatchState().getCurrentProject().getFields()){
          column_names[field.getPosition()] = field.getTitle();
          id_to_position.add(field.getId());
        }        
      }
    }
    
   
    
    public void setData(){
      data = new Object[getBatchState().getCurrentProject().getRecordsPerImage()][getBatchState().getCurrentProject().getFields().size()+1];
      ArrayList<Records> remaining_records = getBatchState().getCurrentBatch().getRecords();
  
      for (int row = 0; row < getBatchState().getCurrentProject().getRecordsPerImage(); row++){
        // loop through each row of the table
        for (int column = 0; column < getBatchState().getCurrentProject().getFields().size()+1; column++){
          // loop through each column of the row
          if (column == 0){
            // the first column is an incremental counter
            data[row][column] = row + 1;
            column++;
          }
          // for each remaining column we must sort through all remaining records and find one that matches field_id and line_num
          for(Records record : remaining_records){
            if (fieldIdToPosition(record.getFieldId()) == column){
              if (record.getLineNum() == row){
                if (record.getAnswer() != null)
                  data[row][column] = record.getAnswer();
                else
                  data[row][column] = "";
              }
            }
          }
        }
      }
    }    

    public int getColumnCount() {
      if (getBatchState().getCurrentBatchId() > 0)
        return column_names.length;
      else
        return 0;
    }

    public int getRowCount() {
      if (getBatchState().getCurrentBatchId() > 0)
        return data.length;
      else
        return 0;
    }

    public String getColumnName(int col) {
        return column_names[col];
    }

    public Object getValueAt(int row, int col) {
        return data[row][col];
    }

    public boolean isCellEditable(int row, int col) {
        if (col < 1) {
            return false;
        } else {
            return true;
        }
    }

    public void setValueAt(Object value, int row, int col) {
        data[row][col] = value;
        fireTableCellUpdated(row, col);
    }
  }


  
  
  
  public void loginSuccess() {
    update();
    if (getBatchState().getCurrentBatchId() > 0)
      table.changeSelection(getBatchState().getSettings().getHighlightedRow(), getBatchState().getSettings().getHighlightedField()+1, false, false);
  }
  public void loginExit() {}
  public void downloadBatch() {
    update();
    table.changeSelection(getBatchState().getSettings().getHighlightedRow(), getBatchState().getSettings().getHighlightedField()+1, false, false);
  }
  public void logout() {
    ArrayList<Records> records_from_table = new ArrayList<Records>();    
    for (int row = 0; row < table_model.getRowCount(); row++){
      for (int column = 0; column < table_model.getColumnCount(); column++){
        Records new_record = new Records();
        Object value = table.getValueAt(row, column);
        String submit_value = "";
        if (value != null){
          if (value.getClass() == Integer.class)
            submit_value = value.toString();
          if (value.getClass() == String.class)
            submit_value = (String) value;
        }
        new_record.setAnswer(submit_value);
        new_record.setBatchId(getBatchState().getCurrentBatchId());
        new_record.setLineNum(row);
        new_record.setFieldId(positionToFieldId(column));
        records_from_table.add(new_record);        
      }
    }
    getBatchState().getCurrentBatch().setRecords(records_from_table);
    this.removeAll();
  }
  public void exit() {
    this.removeAll();
  }
  public void zoomInButton() {}
  public void zoomOutButton() {}
  public void invertImageButton() {}
  public void toggleHighlightsButton() {}
  public void synchronizeTable() {
//    table.getSurrendersFocusOnKeystroke();
//    table.setValueAt(arg0, arg1, arg2);
    table.requestFocus();
    table.changeSelection(getBatchState().getSettings().getHighlightedRow(), getBatchState().getSettings().getHighlightedField()+1, false, false);
  }
  public void synchronizeImage() {}
  public void submitButton() {
    // loop through each entry in the table starting in top left going left to right top to bottom. if empty, use empty string
    StringBuilder builder = new StringBuilder();
    for (int row = 0; row < getBatchState().getCurrentProject().getRecordsPerImage(); row++){
      for (int column = 1; column <= getBatchState().getCurrentProject().getFields().size(); column++){
        Object value = table_model.getValueAt(row, column);
        if (value == null)
          value = "";
        if (column < getBatchState().getCurrentProject().getFields().size())
          builder.append((String) value + ",");
        else
          builder.append((String) value + ";");
      }
    }
    System.out.println(builder.toString());
    SubmitBatch_Result result = getBatchState().getFacade().submitBatch(getBatchState().getCurrentUser(), getBatchState().getCurrentBatchId(), builder.toString());
      
  }
  public void saveButton() {}

}
