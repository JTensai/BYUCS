package shared.model;

@SuppressWarnings("serial")
public class ModelException extends Exception {

  public ModelException() {
    return;
  }

  public ModelException(String message) {
    super(message);
  }

  public ModelException(Throwable cause) {
    super(cause);
  }

  public ModelException(String message, Throwable cause) {
    super(message, cause);
  }

}
