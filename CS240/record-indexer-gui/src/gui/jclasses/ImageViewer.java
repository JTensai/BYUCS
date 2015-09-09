package gui.jclasses;

import gui.communications.BatchState;

import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.io.*;
import java.net.URL;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.*;

import shared.model.Fields;
import listener_interfaces.*;

@SuppressWarnings("serial")
public class ImageViewer extends JComponent  implements BatchStateListener{
  private ArrayList<ImageViewerListener> listeners = new ArrayList<ImageViewerListener>();
  private BatchState batch_state;
  
  private int w_originX;
  private int w_originY;
  private double scale;
  
  private boolean dragging;
  private int w_dragStartX;
  private int w_dragStartY;
  private int w_dragStartOriginX;
  private int w_dragStartOriginY;

  private ArrayList<DrawingShape> shapes;
    
  ImageViewer(BatchState batch_state) {
    batch_state.addListener(this);
    this.addListener(batch_state);
    setBatchState(batch_state);    
    
    Dimension start_size = new Dimension(0, 450);
    this.setMinimumSize(new Dimension(0, 200));
    this.setPreferredSize(start_size);    
  
    w_originX = -50;
    w_originY = -20;
    scale = 1.0;
    
    initDrag();

    shapes = new ArrayList<DrawingShape>();
    
    this.setBackground(new Color(90, 90, 90));
    this.setMinimumSize(new Dimension(100, 100));
    this.setMaximumSize(new Dimension(1000, 1000));
    
    this.addMouseListener(mouseAdapter);
    this.addMouseMotionListener(mouseAdapter);
    this.addMouseWheelListener(mouseAdapter);
  }

  public void addImage(){
    try {
      URL url = new URL(getBatchState().getUrl() + getBatchState().getCurrentBatch().getImageFile());
      getBatchState().setImage(ImageIO.read(url));
    } 
    catch (IOException e) {
      e.printStackTrace();
    }
    if (getBatchState().getSettings().isInverted()){
      BufferedImage bimage = new RescaleOp(-1.0f, 255.0f, null).filter(getBatchState().getImage(), null);
      shapes.add(new DrawingImage(bimage, new Rectangle2D.Double(0, 0, bimage.getWidth(null), bimage.getHeight(null))));
    }
    else{
      Image image = getBatchState().getImage();
      shapes.add(new DrawingImage(image, new Rectangle2D.Double(0, 0, image.getWidth(null), image.getHeight(null))));
    }
    this.repaint();
  }
  
  public void clearImage(){
    this.removeAll();
    this.repaint();
    shapes.removeAll(shapes);
  }
  
  public void invertImage(){
    this.removeAll();
    if (getBatchState().getSettings().isInverted()){
      Image image = getBatchState().getImage();
      shapes.add(new DrawingImage(image, new Rectangle2D.Double(0, 0, image.getWidth(null), image.getHeight(null))));
      getBatchState().getSettings().setInverted(false);
    }
    else{
      BufferedImage bimage = new RescaleOp(-1.0f, 255.0f, null).filter(getBatchState().getImage(), null);
      shapes.add(new DrawingImage(bimage, new Rectangle2D.Double(0, 0, bimage.getWidth(null), bimage.getHeight(null))));  
      getBatchState().getSettings().setInverted(true);  
    }
    getHighlight();
    this.repaint();
  }

  public void getHighlight(){
    if (getBatchState().getSettings().isHighlighted()){
      shapes.add(new DrawingRect(new Rectangle2D.Double(
          getBatchState().getCurrentProject().getFields().get(getBatchState().getSettings().getHighlightedField()).getXcoord(), 
          rowToPixels(getBatchState().getSettings().getHighlightedRow()), 
          getBatchState().getCurrentProject().getFields().get(getBatchState().getSettings().getHighlightedField()).getWidth(), 
          getBatchState().getCurrentProject().getRecordHeight()), 
          new Color(100, 180, 255, 160)));
    }  
    this.repaint();
  }
  
  public void toggleHighlight(){
    if (getBatchState().getSettings().isHighlighted()){
      this.removeAll();
      addImage();
      getBatchState().getSettings().setHighlighted(false);
    }
    else{
      shapes.add(new DrawingRect(new Rectangle2D.Double(
          getBatchState().getCurrentProject().getFields().get(getBatchState().getSettings().getHighlightedField()).getXcoord(), 
          rowToPixels(getBatchState().getSettings().getHighlightedRow()), 
          getBatchState().getCurrentProject().getFields().get(getBatchState().getSettings().getHighlightedField()).getWidth(), 
          getBatchState().getCurrentProject().getRecordHeight()), 
          new Color(100, 180, 255, 160)));
      this.repaint();
      getBatchState().getSettings().setHighlighted(true);
    }
  }

  private void getRecordBox(int x, int y){
    int starting_y = getBatchState().getCurrentProject().getFirstYCoord();
    int record_height = getBatchState().getCurrentProject().getRecordHeight();
    ArrayList<Fields> fields = getBatchState().getCurrentProject().getFields();
    
    Fields prev_field = null;
    int i = 0;
    for (Fields field : fields){
      if (prev_field == null)
        prev_field = field;
      else{
        if (x > prev_field.getXcoord() && x < field.getXcoord()){            
          getBatchState().getSettings().setHighlightedField(i);
          getBatchState().setCurrentField(prev_field);
          break;
        } 
        prev_field = field;
        i++;
        if (i == fields.size()-1 && x > field.getXcoord()){
          getBatchState().getSettings().setHighlightedField(i);
          getBatchState().setCurrentField(prev_field);
        }    
      }      
    }
    int j = 0;
    y -= starting_y;
    while (y > record_height){
      y -= record_height;
      j++;
    }
      getBatchState().getSettings().setHighlightedRow(j);
      getHighlight();
  }
  
  private boolean clickedValidField(int x, int y){
    this.removeAll();
    addImage();
    int starting_y = getBatchState().getCurrentProject().getFirstYCoord();
    int record_height = getBatchState().getCurrentProject().getRecordHeight();
    int records_per_image = getBatchState().getCurrentProject().getRecordsPerImage();
    ArrayList<Fields> fields = getBatchState().getCurrentProject().getFields();
    
    Fields prev_field = null;
    int i = 0;
    for (Fields field : fields){
      if (prev_field == null)
        prev_field = field;
      else{
        if (x > prev_field.getXcoord() && x < field.getXcoord()){
          if (y > starting_y && y < starting_y + (record_height * records_per_image)){
            return true;
          }
        } 
        prev_field = field;
        i++;
        if (i == fields.size()-1 && x > field.getXcoord()){
          if (y > starting_y && y < starting_y + (record_height * records_per_image)){
            return true;
          }
        }    
      }      
    }
    System.out.println("invalid location to click");
    return false;
  }
  
  private int rowToPixels(int row){
    int record_height = getBatchState().getCurrentProject().getRecordHeight();
    int y_coor = getBatchState().getCurrentProject().getFirstYCoord();
    for (int i = 0; i < row; i++){
      y_coor += record_height;
    }
    return y_coor;
  }
  
  private void initDrag() {
    dragging = false;
    w_dragStartX = 0;
    w_dragStartY = 0;
    w_dragStartOriginX = 0;
    w_dragStartOriginY = 0;
  }
    
  public void setScale(double newScale) {
    scale = newScale;
    this.repaint();
    getBatchState().getSettings().setZoomLevel(scale);
  }
  
  public void setOrigin(int w_newOriginX, int w_newOriginY) {
    w_originX = w_newOriginX;
    w_originY = w_newOriginY;
    this.repaint();
  }

  @Override
  protected void paintComponent(Graphics g) {

    super.paintComponent(g);

    Graphics2D g2 = (Graphics2D)g;
    drawBackground(g2);

    g2.scale(scale, scale);
    g2.translate(-w_originX, -w_originY);

    drawShapes(g2);
  }
  
  private void drawBackground(Graphics2D g2) {
    g2.setColor(getBackground());
    g2.fillRect(0,  0, getWidth(), getHeight());
  }

  private void drawShapes(Graphics2D g2) {
    for (DrawingShape shape : shapes) {
      shape.draw(g2);
    }
  }
  
  private MouseAdapter mouseAdapter = new MouseAdapter() {

    @Override
    public void mousePressed(MouseEvent e) {
      int d_X = e.getX();
      int d_Y = e.getY();      
      
      AffineTransform transform = new AffineTransform();
      transform.scale(scale, scale);
      transform.translate(-w_originX, -w_originY);
      
      Point2D d_Pt = new Point2D.Double(d_X, d_Y);
      Point2D w_Pt = new Point2D.Double();
      try
      {
        transform.inverseTransform(d_Pt, w_Pt);
      }
      catch (NoninvertibleTransformException ex) {
        return;
      }
      int w_X = (int)w_Pt.getX();
      int w_Y = (int)w_Pt.getY();
      
      boolean hitShape = false;
      
      Graphics2D g2 = (Graphics2D)getGraphics();
      for (DrawingShape shape : shapes) {
        if (shape.contains(g2, w_X, w_Y)) {
          hitShape = true;
          break;
        }
      }
      
      if (hitShape) {
        dragging = true;    
        w_dragStartX = w_X;
        w_dragStartY = w_Y;   
        w_dragStartOriginX = w_originX;
        w_dragStartOriginY = w_originY;
      }
    }

    @Override
    public void mouseDragged(MouseEvent e) {
      if (dragging) {
        int d_X = e.getX();
        int d_Y = e.getY();
        
        AffineTransform transform = new AffineTransform();
        transform.scale(scale, scale);
        transform.translate(-w_dragStartOriginX, -w_dragStartOriginY);
        
        Point2D d_Pt = new Point2D.Double(d_X, d_Y);
        Point2D w_Pt = new Point2D.Double();
        try
        {
          transform.inverseTransform(d_Pt, w_Pt);
        }
        catch (NoninvertibleTransformException ex) {
          return;
        }
        int w_X = (int)w_Pt.getX();
        int w_Y = (int)w_Pt.getY();
        
        int w_deltaX = w_X - w_dragStartX;
        int w_deltaY = w_Y - w_dragStartY;
        
        w_originX = w_dragStartOriginX - w_deltaX;
        w_originY = w_dragStartOriginY - w_deltaY;
        
        repaint();
      }
    }

    public void mouseClicked(MouseEvent e){
      int x = e.getX() + w_originX;
      int y = e.getY() + w_originY;

      if (clickedValidField(x, y)){
        getRecordBox(x, y);
//        System.out.println(getBatchState().getSettings().getHighlightedField() + " : " + getBatchState().getSettings().getHighlightedRow());
        notifyListeners("image field clicked");
      }
      
     
    }
    
    @Override
    public void mouseReleased(MouseEvent e) {
      initDrag();
    }

    @Override
    public void mouseWheelMoved(MouseWheelEvent e) {
      scale += -e.getPreciseWheelRotation()/15;
      setScale(scale);
    } 
  };
  
  
  /////////////////
  // Drawing Shape
  /////////////////
  
  
  interface DrawingShape {
    boolean contains(Graphics2D g2, double x, double y);
    void draw(Graphics2D g2);
    Rectangle2D getBounds(Graphics2D g2);
  }

  class DrawingRect implements DrawingShape {

    private Rectangle2D rect;
    private Color color;
    
    public DrawingRect(Rectangle2D rect, Color color) {
      this.rect = rect;
      this.color = color;
    }

    @Override
    public boolean contains(Graphics2D g2, double x, double y) {
      return rect.contains(x, y);
    }

    @Override
    public void draw(Graphics2D g2) {
      g2.setColor(color);
      g2.fill(rect);
    }
    
    @Override
    public Rectangle2D getBounds(Graphics2D g2) {
      return rect.getBounds2D();
    }
  }

  class DrawingImage implements DrawingShape {

    private Image image;
    private Rectangle2D rect;
    
    public DrawingImage(Image image, Rectangle2D rect) {
      this.image = image;
      this.rect = rect;
    }

    @Override
    public boolean contains(Graphics2D g2, double x, double y) {
      return rect.contains(x, y);
    }

    @Override
    public void draw(Graphics2D g2) {
      g2.drawImage(image, (int)rect.getMinX(), (int)rect.getMinY(), (int)rect.getMaxX(), (int)rect.getMaxY(),
              0, 0, image.getWidth(null), image.getHeight(null), null);
    } 
    
    @Override
    public Rectangle2D getBounds(Graphics2D g2) {
      return rect.getBounds2D();
    }
  }


  
  
  public BatchState getBatchState() {
    return batch_state;
  }
  public void setBatchState(BatchState batch_state) {
    this.batch_state = batch_state;
  }

  public void addListener(ImageViewerListener image_viewer_listener) {
    listeners.add(image_viewer_listener);
  }
  
  private void notifyListeners(String s) {
    for (ImageViewerListener listener : listeners) {
      if (s.equals("image field clicked"))
        listener.imageFieldClicked();
    }
  }
   
  public void loginSuccess() {
    if (getBatchState().getCurrentBatchId() > 0){
//      notifyListeners("image field clicked");
      addImage(); 
      getHighlight();
    }
    this.revalidate();
  }
  public void loginExit() {}
  public void downloadBatch() {
    if (getBatchState().getCurrentBatchId() > 0){
//      notifyListeners("image field clicked");
      addImage();
      getHighlight();
    }
    this.revalidate();
  }
  public void logout() {
    clearImage();
  }
  public void exit() {}
  public void zoomInButton() {
    scale += 1.0/15.0;
    setScale(scale);    
  }
  public void zoomOutButton() {
    scale -= 1.0/15.0;
    setScale(scale);   
    }
  public void invertImageButton() {
    invertImage();
  }
  public void toggleHighlightsButton(){
    toggleHighlight();
  }
  public void synchronizeImage() {
    addImage();
    getHighlight();
    
  }
  public void synchronizeTable() {}
  public void submitButton() {}
  public void saveButton() {}

}
