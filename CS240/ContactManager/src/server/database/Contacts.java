package server.database;

import java.util.*;
import java.sql.*;

import server.*;
import shared.model.*;

public class Contacts {

  private Database db;

  Contacts(Database db) {
    this.db = db;
  }

  public List<Contact> getAll() throws ServerException {
    ArrayList<Contact> result = new ArrayList<Contact>();
    PreparedStatement stmt = null;
    ResultSet rs = null;
    try {
      String query = "select id, name, phone, address, email, url from contact";
      stmt = db.getConnection().prepareStatement(query);

      rs = stmt.executeQuery();
      while (rs.next()) {
        int id = rs.getInt(1);
        String name = rs.getString(2);
        String phone = rs.getString(3);
        String address = rs.getString(4);
        String email = rs.getString(5);
        String url = rs.getString(6);

        result.add(new Contact(id, name, phone, address, email, url));
      }
    } catch (SQLException e) {
      throw new ServerException(e.getMessage(), e);
    } finally {
      Database.safeClose(rs);
      Database.safeClose(stmt);
    }

    return result;
  }

  public void add(Contact contact) throws ServerException {
    PreparedStatement stmt = null;
    ResultSet keyRS = null;
    try {
      String query = "insert into contact (name, phone, address, email, url) values (?, ?, ?, ?, ?)";
      stmt = db.getConnection().prepareStatement(query);
      stmt.setString(1, contact.getName());
      stmt.setString(2, contact.getPhone());
      stmt.setString(3, contact.getAddress());
      stmt.setString(4, contact.getEmail());
      stmt.setString(5, contact.getUrl());
      if (stmt.executeUpdate() == 1) {
        Statement keyStmt = db.getConnection().createStatement();
        keyRS = keyStmt.executeQuery("select last_insert_rowid()");
        keyRS.next();
        int id = keyRS.getInt(1);
        contact.setId(id);
      } else {
        throw new ServerException("Could not insert contact");
      }
    } catch (SQLException e) {
      throw new ServerException("Could not insert contact", e);
    } finally {
      Database.safeClose(stmt);
      Database.safeClose(keyRS);
    }
  }

  public void update(Contact contact) throws ServerException {
    PreparedStatement stmt = null;
    try {
      String query = "update contact set name = ?, phone = ?, address = ?, email = ?, url = ? where id = ?";
      stmt = db.getConnection().prepareStatement(query);
      stmt.setString(1, contact.getName());
      stmt.setString(2, contact.getPhone());
      stmt.setString(3, contact.getAddress());
      stmt.setString(4, contact.getEmail());
      stmt.setString(5, contact.getUrl());
      stmt.setInt(6, contact.getId());
      if (stmt.executeUpdate() != 1) {
        throw new ServerException("Could not update contact");
      }
    } catch (SQLException e) {
      throw new ServerException("Could not update contact", e);
    } finally {
      Database.safeClose(stmt);
    }
  }

  public void delete(Contact contact) throws ServerException {
    PreparedStatement stmt = null;
    try {
      String query = "delete from contact where id = ?";
      stmt = db.getConnection().prepareStatement(query);
      stmt.setInt(1, contact.getId());
      if (stmt.executeUpdate() != 1) {
        throw new ServerException("Could not delete contact");
      }
    } catch (SQLException e) {
      throw new ServerException("Could not delete contact", e);
    } finally {
      Database.safeClose(stmt);
    }
  }

}
