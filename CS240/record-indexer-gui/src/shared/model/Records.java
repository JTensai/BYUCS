package shared.model;

public class Records {

  private String answer;
  private int batch_id;
  private int line_num;
  private int field_id;
  private String image_url;

  public Records(String answer, int line_num) {
    setAnswer(answer);
    setLineNum(line_num);
  }
  
  public Records(String answer, int batch_id, int line_num, int field_id) {
    setAnswer(answer);
    setBatchId(batch_id);
    setLineNum(line_num);
    setFieldId(field_id);
  }

  public Records() {
  }

  public String getAnswer() {
    return answer;
  }

  public void setAnswer(String answer) {
    this.answer = answer;
  }

  public int getBatchId() {
    return batch_id;
  }

  public void setBatchId(int batch_id) {
    this.batch_id = batch_id;
  }

  public int getLineNum() {
    return line_num;
  }

  public void setLineNum(int line_num) {
    this.line_num = line_num;
  }

  public int getFieldId() {
    return field_id;
  }

  public void setFieldId(int field_id) {
    this.field_id = field_id;
  }

  public String getImageUrl() {
    return image_url;
  }

  public void setImageUrl(String image_url) {
    this.image_url = image_url;
  }
}
