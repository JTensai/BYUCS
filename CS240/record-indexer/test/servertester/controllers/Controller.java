package servertester.controllers;

import java.util.*;

import client.ClientException;
import client.communication.ClientCommunicator;
import servertester.views.*;
import shared.communication.*;
import shared.model.*;

public class Controller implements IController {

  private IView _view;

  public Controller() {
    return;
  }

  public IView getView() {
    return _view;
  }

  public void setView(IView value) {
    _view = value;
  }

  // IController methods
  //

  @Override
  public void initialize() {
    getView().setHost("localhost");
    getView().setPort("8080");
    operationSelected();
  }

  @Override
  public void operationSelected() {
    ArrayList<String> paramNames = new ArrayList<String>();
    paramNames.add("User");
    paramNames.add("Password");

    switch (getView().getOperation()) {
    case VALIDATE_USER:
      break;
    case GET_PROJECTS:
      break;
    case GET_SAMPLE_IMAGE:
      paramNames.add("Project");
      break;
    case DOWNLOAD_BATCH:
      paramNames.add("Project");
      break;
    case GET_FIELDS:
      paramNames.add("Project");
      break;
    case SUBMIT_BATCH:
      paramNames.add("Batch");
      paramNames.add("Record Values");
      break;
    case SEARCH:
      paramNames.add("Fields");
      paramNames.add("Search Values");
      break;
    default:
      assert false;
      break;
    }

    getView().setRequest("");
    getView().setResponse("");
    getView().setParameterNames(paramNames.toArray(new String[paramNames.size()]));
  }

  @Override
  public void executeOperation() {
    switch (getView().getOperation()) {
    case VALIDATE_USER:
      validateUser();
      break;
    case GET_PROJECTS:
      getProjects();
      break;
    case GET_SAMPLE_IMAGE:
      getSampleImage();
      break;
    case DOWNLOAD_BATCH:
      downloadBatch();
      break;
    case GET_FIELDS:
      getFields();
      break;
    case SUBMIT_BATCH:
      submitBatch();
      break;
    case SEARCH:
      search();
      break;
    default:
      assert false;
      break;
    }
  }

  private void validateUser() {    
    String[] params = _view.getParameterValues();
    ClientCommunicator communicator = new ClientCommunicator(_view.getHost(), Integer.parseInt(_view.getPort()));
    ValidateUser_Params user_params = new ValidateUser_Params();

    ValidateUser_Result results;
    Users user = new Users();
    
    String username = params[0];
    String password = params[1];  
    user.setUsername(username);
    user.setPassword(password);    
    user_params.setUser(user);
    
    try {
      results = communicator.ValidateUser(user_params);
      user = results.getUser();
      if (user != null && user.getFirstName() != null){
        _view.setResponse(user.toString());   
      }
      else
        _view.setResponse("FALSE");         
    } 
    catch (ClientException e) {
      e.printStackTrace();
      _view.setResponse("FAILED");
    }
  }

  private void getProjects() {
    String[] params = _view.getParameterValues();
    ClientCommunicator communicator = new ClientCommunicator(_view.getHost(), Integer.parseInt(_view.getPort()));
    GetProjects_Params user_params = new GetProjects_Params();
    
    GetProjects_Result results;
    Users user = new Users();
    
    String username = params[0];
    String password = params[1];  
    user.setUsername(username);
    user.setPassword(password);    
    user_params.setUser(user);
    
    try {
      results = communicator.getProjects(user_params);
      if (results != null && results.getProjects() != null){
        ArrayList<Projects> projects = results.getProjects();
        StringBuilder builder = new StringBuilder();
        for (Projects project : projects){
          builder.append(project.projectInfo());
        }
        _view.setResponse(builder.toString());
      }
      else
        _view.setResponse("FAILED");
    } 
    catch (ClientException e) {
      _view.setResponse("FAILED");
      e.printStackTrace();
    }
  }

  private void getSampleImage() {
    String[] params = _view.getParameterValues();
    ClientCommunicator communicator = new ClientCommunicator(_view.getHost(), Integer.parseInt(_view.getPort()));
    GetSampleImage_Params sample_params = new GetSampleImage_Params();
    
    GetSampleImage_Result results;
    Users user = new Users();
    
    String username = params[0];
    String password = params[1];
    
    int project_id = 0;
    if (params[2].isEmpty()){
      _view.setResponse("FAILED");
      return;
    }
    else
      project_id = Integer.parseInt(params[2]);
    
    user.setUsername(username);
    user.setPassword(password);    
    sample_params.setUser(user);
    sample_params.setProject_id(project_id);
    
    try {
      results = communicator.getSampleImage(sample_params);
      
      if (results != null && results.getImage_url() != null){
        String image_url = results.getImage_url();
        _view.setResponse("http://" + _view.getHost() + ":" + _view.getPort() + "/" + image_url);
      }
      else
        _view.setResponse("FAILED");
    } 
    catch (ClientException e) {
      _view.setResponse("FAILED");
      e.printStackTrace();
    }
  }

  private void downloadBatch() {
    String[] params = _view.getParameterValues();
    ClientCommunicator communicator = new ClientCommunicator(_view.getHost(), Integer.parseInt(_view.getPort()));
    DownloadBatch_Params batch_params = new DownloadBatch_Params();
    
    DownloadBatch_Result results;
    Users user = new Users();
    
    String username = params[0];
    String password = params[1];
    int project_id = 0;
    if (params[2].isEmpty()){
      _view.setResponse("FAILED");
      return;
    }
    else
      project_id = Integer.parseInt(params[2]);
    user.setUsername(username);
    user.setPassword(password);    
    batch_params.setUser(user);
    batch_params.setProject_id(project_id);
    
    try {
      results = communicator.downloadBatch(batch_params);
      if (results != null && results.getBatch() != null && results.getProject() != null && results.getFields() != null){
        Batches batch = results.getBatch();
        Projects project = results.getProject();
        ArrayList<Fields> fields = results.getFields();
        
        StringBuilder builder = new StringBuilder();
        builder.append(batch.getId() + "\n");
        builder.append(batch.getProjectId() + "\n");
        builder.append("http://" + _view.getHost() + ":" + _view.getPort() + "/" + batch.getImageFile() + "\n");
        builder.append(project.getFirstYCoord() + "\n");
        builder.append(project.getRecordHeight() + "\n");
        builder.append(project.getRecordsPerImage() + "\n");
        builder.append(fields.size() + "\n");
        
        for (Fields field : fields){
          builder.append(field.getId() + "\n");
          builder.append(field.getPosition() + "\n");
          builder.append(field.getTitle() + "\n");
          if (!field.getHelpHtml().equals("null"));
            builder.append("http://" + _view.getHost() + ":" + _view.getPort() + "/" + field.getHelpHtml() + "\n");
          builder.append(field.getXcoord() + "\n");
          builder.append(field.getWidth() + "\n");
          if (!field.getKnownData().equals("null"))
            builder.append("http://" + _view.getHost() + ":" + _view.getPort() + "/" + field.getKnownData() + "\n");
        }
        
        _view.setResponse(builder.toString());
      }
      else
        _view.setResponse("FAILED");
    } 
    catch (ClientException e) {
      _view.setResponse("FAILED");
      e.printStackTrace();
    }
  }

  private void getFields() {
    String[] params = _view.getParameterValues();
    ClientCommunicator communicator = new ClientCommunicator(_view.getHost(), Integer.parseInt(_view.getPort()));
    GetFields_Params fields_params = new GetFields_Params();
    
    GetFields_Result results;
    Users user = new Users();
    
    String username = params[0];
    String password = params[1];
    
    int project_id = 0;
    if (params[2].isEmpty())
      project_id = -1;
    else
      project_id = Integer.parseInt(params[2]);
    
    user.setUsername(username);
    user.setPassword(password);    
    fields_params.setUser(user);
    fields_params.setProject_id(project_id);
    
    try {
      results = communicator.getFields(fields_params);
      
      if (results != null && results.getFields() != null){
        ArrayList<Fields> fields = results.getFields();
        if (fields.size() < 1){
          _view.setResponse("FAILED");
          return;
        }
        
        StringBuilder builder = new StringBuilder();
        for (Fields field : fields){
          builder.append(field.getProjectId() + "\n");
          builder.append(field.getId() + "\n");
          builder.append(field.getTitle() + "\n");
        }
        
        _view.setResponse(builder.toString());
      }
      else
        _view.setResponse("FAILED");
    } 
    catch (ClientException e) {
      _view.setResponse("FAILED");
      e.printStackTrace();
    }
  }

  private void submitBatch() {
    String[] params = _view.getParameterValues();
    ClientCommunicator communicator = new ClientCommunicator(_view.getHost(), Integer.parseInt(_view.getPort()));
    SubmitBatch_Params submit_params = new SubmitBatch_Params();
    
    SubmitBatch_Result results;
    Users user = new Users();
    
    String username = params[0];
    String password = params[1];
    int batch_id = 0;
    if (params[2].isEmpty()){
      _view.setResponse("FAILED");
      return;
    }
    else
      batch_id = Integer.parseInt(params[2]);
    String field_values = params[3];
    user.setUsername(username);
    user.setPassword(password);    
    submit_params.setUser(user);
    submit_params.setBatchId(batch_id);
    submit_params.setValues(field_values);
    
    try {
      results = communicator.submitBatch(submit_params);
      
      if (results != null && results.isSuccess()){
        _view.setResponse("TRUE");
      }
      else
        _view.setResponse("FAILED");
    } 
    catch (ClientException e) {
      _view.setResponse("FAILED");
      e.printStackTrace();
    }
  }

  private void search() {
    String[] params = _view.getParameterValues();
    ClientCommunicator communicator = new ClientCommunicator(_view.getHost(), Integer.parseInt(_view.getPort()));
    Search_Params search_params = new Search_Params();
    
    Search_Result results;
    Users user = new Users();
    
    String username = params[0];
    String password = params[1]; 
    String[] given_field_ids = params[2].split("\\,");
    ArrayList<Integer> field_ids = new ArrayList<Integer>();
    for (String id : given_field_ids){
      if (id.isEmpty()){
        _view.setResponse("FAILED");
        return;
      }
      else
        field_ids.add(Integer.parseInt(id));
    }    
    String[] given_search_values = params[3].split("\\,");
    ArrayList<String> search_values = new ArrayList<String>();
    for (String value : given_search_values){
      search_values.add(value.toLowerCase());
    }
    
    if (field_ids.size() == 0 || search_values.size() == 0){
      _view.setResponse("FAILED");
      return;      
    }
    
    user.setUsername(username);
    user.setPassword(password);    
    search_params.setUser(user);
    search_params.setFields(field_ids);
    search_params.setSearch_values(search_values);
    
    try {
      results = communicator.search(search_params);
      if (results != null){
        StringBuilder builder = new StringBuilder();
        for (Records result : results.getResults()){
          if (result != null && result.getBatchId() > 0){
            builder.append(result.getBatchId() + "\n");          
            builder.append("http://" + _view.getHost() + ":" + _view.getPort() + "/" + result.getImageUrl() + "\n");
            builder.append(result.getLineNum() + "\n");
            builder.append(result.getFieldId() + "\n");
          }
        }
        String final_result = builder.toString();
        if (final_result.length() > 0)
          _view.setResponse(builder.toString());
        else{
          _view.setResponse("FAILED");
          System.out.println("No results found");
        }
      }
      else
        _view.setResponse("FAILED");
    } 
    catch (ClientException e) {
      _view.setResponse("FAILED");
      e.printStackTrace();
    }    
  }
}
