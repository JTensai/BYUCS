package server;

import java.io.IOException;
import java.util.logging.*;

import com.sun.net.httpserver.*;

public class GetAllContactsHandler implements HttpHandler {

  private Logger logger = Logger.getLogger("contactmanager");

  @Override
  public void handle(HttpExchange exchange) throws IOException {

    // Process GetAllContacts request
    // 1. Call model to get a list of all contacts
    // 2. Create a result object
    // 3. Populate it with the list of contacts
    // 4. Serialize the result object
    // 5. Return the serialized result object in the response body

  }
}
