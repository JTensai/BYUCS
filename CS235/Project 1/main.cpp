#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Student.h"
#include "Grade.h"
#include "Result.h"

using namespace std;

vector<Student> create_students(char* students_file){
  vector<Student> list;

  string s;
  int attr = 1;
  Student new_s ("","","","");
  ifstream in;

  in.open(students_file);

  while(getline(in, s)){
    if (attr == 1){
      //cout << "ID: " << s << endl;
      new_s.setID(s);
    }
    else if (attr == 2){
      //cout << "Name: " << s << endl;
      new_s.setName(s);

    }
    else if (attr == 3){
      //cout << "Address: " << s << endl;
      new_s.setAddress(s);

    }
    else if (attr == 4){
      //cout << "Phone: " << s << endl;
      new_s.setPhone(s);

      list.push_back(new_s);
      Student new_s ("","","","");
      attr = 0;
    }
    attr++;
  }

  in.close();

  return list;
}

vector<Grade> create_grades(char* grades_file){
  vector<Grade> list;

  string g;
  int attr = 1;
  Grade new_g ("","","");
  ifstream in;

  in.open(grades_file);

  while(getline(in, g)){
    if (attr == 1){
      //cout << "Course ID: " << g << endl;
      new_g.setCourse_id(g);
    }
    else if (attr == 2){
      //cout << "Student ID: " << g << endl;
      new_g.setStud_id(g);

    }
    else if (attr == 3){
      //cout << "Grade: " << g << endl;
      new_g.setGrade(g);

      list.push_back(new_g);
      Grade new_g ("","","");
      attr = 0;
    }
    attr++;
  }

  in.close();

  return list;
}

double toPoints2(string grade){
  double points = 0;

  if (grade == "C")
    points = 2;
  else if (grade == "C-")
    points = 1.7;
  else if (grade == "D+")
    points = 1.4;
  else if (grade == "D")
    points = 1;
  else if (grade == "D-")
    points = 0.7;
  else if (grade == "E")
    points = 0;

  return points;

}

double toPoints(string grade){
  double points = 0;

  if (grade == "A")
    points = 4;
  else if (grade == "A-")
    points = 3.7;
  else if (grade == "B+")
    points = 3.4;
  else if (grade == "B")
    points = 3;
  else if (grade == "B-")
    points = 2.7;
  else if (grade == "C+")
    points = 2.4;
  else 
    points = toPoints2(grade);

  return points;
}

double calculate_gpa(vector<Grade> grade_list, string id){
  double gpa = 0.00;
  int class_count = 0;
  double score = 0;
  unsigned int none = 0;


  for (unsigned int i = 0; i < grade_list.size(); i++){
    if (grade_list[i].getStud_id() == id){
      //cout << grade_list[i].getGrade() << endl; // prints out each students grades
      double points = toPoints(grade_list[i].getGrade());
      class_count++;
      score = score + points;
    }
    else{
      none++;
    }
  }

  if (none == grade_list.size())
    gpa = 0.00;
  else
    gpa = score / class_count;

  return gpa;
}

string getStudent_name(vector<Student> student_list, string id){
  string name;
  for (unsigned int i = 0; i < student_list.size(); i++){
    if (student_list[i].getID() == id){
      name = student_list[i].getName();
    }
  }
  return name;
}

vector<Result> query(char* query_file, vector<Student> student_list, vector<Grade> grade_list){

  vector<Result> query_results;
  double gpa;
  string name;
  Result new_result ("",0,"");

  string q;
  ifstream in;

  in.open(query_file);

  while(getline(in, q)){
    for (unsigned int i = 0; i < student_list.size(); i++){
      //check to see if student id exists
      if (student_list[i].getID() == q){
        name = getStudent_name(student_list, q);
        gpa = calculate_gpa(grade_list, q);
        new_result.setID(q);
        new_result.setGpa(gpa);
        new_result.setName(name);
        query_results.push_back(new_result);
        Result new_result ("",0,"");
        break;  
      }
    }
  }
  in.close();

  return query_results;

}


int main(int argc, char* argv[]){

  vector<Student> student_list = create_students(argv[1]); 
  vector<Grade> grade_list = create_grades(argv[2]);
  vector<Result> query_results = query(argv[3], student_list, grade_list);

  sort(student_list.begin(), student_list.end());  
  sort(grade_list.begin(), grade_list.end());

  ofstream out;

  out.open(argv[4]);
    for (unsigned int i = 0; i < student_list.size(); i++){
      out << student_list[i].toString() << endl;
    }
    out << endl;
    for (unsigned int i = 0; i < grade_list.size(); i++){
      out << grade_list[i].toString() << endl;
    }
    out << endl;
    for (unsigned int i = 0; i < query_results.size(); i++){
      out << query_results[i].toString() << endl;
    }
  out.close();


  // for (unsigned int i = 0; i < student_list.size(); i++){
  //   cout << student_list[i].toString() << endl;
  // }
  // cout << endl;
  // for (unsigned int i = 0; i < grade_list.size(); i++){
  //   cout << grade_list[i].toString() << endl;
  // }
  // cout << endl;
  // for (unsigned int i = 0; i < query_results.size(); i++){
  //   cout << query_results[i].toString() << endl ;
  // }

  return 0;

}

