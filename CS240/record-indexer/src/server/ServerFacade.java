package server;

import java.util.ArrayList;

import server.database.Database;
import server.database.DatabaseException;
import shared.communication.*;
import shared.model.*;

public class ServerFacade {

  public Users ValidateUser(Users user) {
    ValidateUser_Params params = new ValidateUser_Params();
    params.setUser(user);
    ValidateUser_Result result = ValidateUser(params);
    return result.getUser();
  }

  public ValidateUser_Result ValidateUser(ValidateUser_Params params) {
    Database db = new Database();
    Users user = params.getUser();
    ValidateUser_Result result = new ValidateUser_Result();

    try {
      db.startTransaction();
      user = db.getUsersDAO().ValidateUser(user);
      result.setUser(user);
      db.endTransaction(true);
      return result;
    } catch (DatabaseException e) {
      System.out.println(e.getMessage());
      e.printStackTrace();
      return null;
    }
  }

  public GetProjects_Result getProjects(GetProjects_Params params) {
    Database db = new Database();
    Users user = params.getUser();

    Users valid_user = ValidateUser(user); // Check to see if user is valid or
                                           // not

    if (valid_user != null && valid_user.getFirstName() != null) {
      // User validated proceed with method
      try {
        db.startTransaction();
        ArrayList<Projects> projects = db.getProjectsDAO().getAllProjects();
        GetProjects_Result result = new GetProjects_Result();
        result.setProjects(projects);
        db.endTransaction(true);
        return result;
      } catch (DatabaseException e) {
        System.out.println(e.getMessage());
        e.printStackTrace();
        db.endTransaction(false);
        return null;
      }
    } else {
      return null;
    }
  }

  public GetSampleImage_Result getSampleImage(GetSampleImage_Params params) {
    Database db = new Database();
    Users user = params.getUser();

    Users valid_user = ValidateUser(user); // Check to see if user is valid or
                                           // not

    if (valid_user != null && valid_user.getFirstName() != null) {
      // User validated proceed with method
      try {
        db.startTransaction();
        String image_url = db.getProjectsDAO().getSampleImage(params.getProject_id());
        GetSampleImage_Result result = new GetSampleImage_Result();
        result.setImage_url(image_url);
        db.endTransaction(true);
        return result;
      } catch (DatabaseException e) {
        System.out.println(e.getMessage());
        e.printStackTrace();
        db.endTransaction(false);
        return null;
      }
    } else {
      return null;
    }
  }

  public DownloadBatch_Result downloadBatch(DownloadBatch_Params params) {
    Database db = new Database();
    Users user = params.getUser();

    Users valid_user = ValidateUser(user); // Check to see if user is valid or not

    if (valid_user != null && valid_user.getFirstName() != null && valid_user.getCurrentBatchId() == -1) {
      // User validated proceed with method
      try {
        db.startTransaction();
        int remaining_batches = db.getProjectsDAO().remainingBatchCount(params.getProject_id());
        System.out.println(remaining_batches);
        if (remaining_batches > 0){
          DownloadBatch_Result result = new DownloadBatch_Result();
          Projects project = db.getProjectsDAO().getProject(params.getProject_id());
          Batches batch = db.getBatchesDAO().getBatch(params.getProject_id(), valid_user.getId());
          ArrayList<Fields> fields = db.getBatchesDAO().getFields(params.getProject_id());
  
          result.setProject(project);
          result.setBatch(batch);
          result.setFields(fields);
  
          db.endTransaction(true);
          return result;
        }
        else
          return null;        
      } catch (DatabaseException e) {
        System.out.println(e.getMessage());
        e.printStackTrace();
        db.endTransaction(false);
        return null;
      }
    } else {
      return null;
    }
  }

  public SubmitBatch_Result submitBatch(SubmitBatch_Params params) {
    Database db = new Database();
    Users user = params.getUser();
    int batch_id = params.getBatchId();
    Users valid_user = ValidateUser(user); // Check to see if user is valid or
                                           // not
    SubmitBatch_Result result = new SubmitBatch_Result();

    if (valid_user != null && valid_user.getFirstName() != null) {
      // User validated proceed with method
      if (valid_user.getCurrentBatchId() == batch_id && !db.getBatchesDAO().isRecorded(batch_id)) {
        // Check to make sure number of given values matches number of values needed
        // If it matches, then the input is valid and we can proceed
        Batches current_batch = db.getUsersDAO().getCurrentBatch(valid_user);
        int num_of_fields = db.getBatchesDAO().getFields(current_batch.getProjectId()).size(); // number of fields there should be

        ArrayList<Fields> fields = db.getBatchesDAO().getFields(current_batch.getProjectId());
        int first_field_id = fields.get(0).getId();

        String[] records = params.getValues().split("\\;");
        String[] field_answers = records[0].split("\\,");
        int num_of_given = field_answers.length;

        if (num_of_fields == num_of_given) {
          try {
            db.startTransaction();
            int line_num = 0;
            for (String record : records) {
              String[] parts = record.split("\\,");
              line_num++;
              int position = 0;
              db.getUsersDAO().incrementIndexedRecord(valid_user.getId());
              for (String part : parts) {
                // if (part.equals(""))
//                   part = " ";
                Records new_record = new Records(part, batch_id, line_num, first_field_id + position);
                db.getRecordsDAO().add(new_record, valid_user.getId());
                position++;
              }
            }
            result.setSuccess(true);
            db.endTransaction(true);
            return result;
          } catch (DatabaseException e) {
            System.out.println(e.getMessage());
            e.printStackTrace();
            result.setSuccess(false);
            db.endTransaction(false);
            return null;
          }
        } else {
          System.out.println("Number of given fields does not match number of fields in batch.");
          return null;
        }
      } else {
        System.out.println("Batch already recorded.");
        return null;
      }
    } else
      return null;
  }

  public GetFields_Result getFields(GetFields_Params params) {
    Database db = new Database();
    Users user = params.getUser();

    Users valid_user = ValidateUser(user); // Check to see if user is valid or not

    if (valid_user != null && valid_user.getFirstName() != null) {
      // User validated proceed with method
      try {
        db.startTransaction();
        GetFields_Result result = new GetFields_Result();
        ArrayList<Fields> fields = db.getBatchesDAO().getFields(params.getProject_id());

        result.setFields(fields);

        db.endTransaction(true);
        return result;
      } catch (DatabaseException e) {
        System.out.println(e.getMessage());
        e.printStackTrace();
        db.endTransaction(false);
        return null;
      }
    } 
    else {
      return null;
    }
  }

  public Search_Result search(Search_Params params) {
    Database db = new Database();
    Users user = params.getUser();

    Users valid_user = ValidateUser(user); // Check to see if user is valid or not

    if (valid_user != null && valid_user.getFirstName() != null) {
      // User validated proceed with method
      try {
        db.startTransaction();
        ArrayList<Records> results = db.getRecordsDAO().search(params.getSearch_values(), params.getFields());
        
            
//        for (Records record : db.getRecordsDAO().getAllRecords()){
//          for (String value : params.getSearch_values()) {
//            for (int i = 0; i < params.getFields().size(); ++i) {
//              int current_field = params.getFields().get(i);
//              if (!results.contains(record))
//                results.add();
//            }
//          }
//        }

        Search_Result result = new Search_Result();
        result.setResults(results);
        db.endTransaction(true);
        return result;
      } catch (DatabaseException e) {
        System.out.println(e.getMessage());
        e.printStackTrace();
        db.endTransaction(false);
        return null;
      }
    } else {
      return null;
    }
  }
}