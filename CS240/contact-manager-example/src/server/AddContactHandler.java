package server;

import java.io.IOException;
import java.util.logging.*;

import com.sun.net.httpserver.*;

public class AddContactHandler implements HttpHandler {

  private Logger logger = Logger.getLogger("contactmanager");

  @Override
  public void handle(HttpExchange exchange) throws IOException {

    // Process AddContact request
    // 1. Deserialize the request object from the request body
    // 2. Extract the new contact object from the request object
    // 3. Call the model to add the new contact

  }
}
