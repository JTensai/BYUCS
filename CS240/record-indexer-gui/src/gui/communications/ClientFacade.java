package gui.communications;

import client.ClientException;
import client.communication.ClientCommunicator;
import shared.communication.*;
import shared.model.Users;

public class ClientFacade {
  ClientCommunicator communicator;
  
  ClientFacade(String host, String port){
    communicator = new ClientCommunicator(host, port);
  }
//  public ClientCommunicator getCommunicator() {
//    return communicator;
//  }
//
//  public void setCommunicator(ClientCommunicator communicator) {
//    this.communicator = communicator;
//  }

  public ValidateUser_Result validateUser(String username, String password){
    ValidateUser_Params params = new ValidateUser_Params();
    Users user = new Users();
    user.setUsername(username);
    user.setPassword(password);
    params.setUser(user);
    try {
      ValidateUser_Result result = communicator.ValidateUser(params);
      return result;
    }
    catch (ClientException e) {
      e.printStackTrace();
      return null;
    }
  }
  
  public DownloadBatch_Result downloadBatch(int project_id, Users user){
    DownloadBatch_Params params = new DownloadBatch_Params();
    params.setProject_id(project_id);
    params.setUser(user);
    try {
      DownloadBatch_Result result = communicator.downloadBatch(params);
      return result;
    }
    catch (ClientException e) {
      e.printStackTrace();
      return null;
    }    
  }
  
  public GetProjects_Result getProjects(Users user){
    GetProjects_Params params = new GetProjects_Params();
    params.setUser(user);
    
    try {
      GetProjects_Result result = communicator.getProjects(params);
      return result;
    }
    catch (ClientException e) {
      e.printStackTrace();
      return null;
    }
  }
  
  public GetSampleImage_Result getSampleImage(int project_id, Users user){
    GetSampleImage_Params params = new GetSampleImage_Params();
    params.setProject_id(project_id);
    params.setUser(user);
    System.out.println(params.getProject_id());
    try{
      GetSampleImage_Result result = communicator.getSampleImage(params);
      return result;
    }
    catch (ClientException e) {
      e.printStackTrace();
      return null;
    }
  }
  
  public SubmitBatch_Result submitBatch(Users user, int batch_id, String values){
    SubmitBatch_Params params = new SubmitBatch_Params();
    params.setBatchId(batch_id);
    params.setUser(user);
    params.setValues(values);
    try{
      SubmitBatch_Result result = communicator.submitBatch(params);
      return result;
    }
    catch (ClientException e) {
      e.printStackTrace();
      return null;
    }
    
  }
}
