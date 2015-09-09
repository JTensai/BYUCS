package server;

import java.io.IOException;
import java.util.logging.*;

import com.sun.net.httpserver.*;

public class DeleteContactHandler implements HttpHandler {

  private Logger logger = Logger.getLogger("contactmanager");

  @Override
  public void handle(HttpExchange exchange) throws IOException {

    // Process DeleteContact request
    // 1. Deserialize the request object from the request body
    // 2. Extract the contact to be deleted from the request object
    // 3. Call the model to delete the contact

  }
}
