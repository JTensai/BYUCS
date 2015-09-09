#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include "Student.h"

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
      new_s.setID(s);
    }
    else if (attr == 2){
      new_s.setName(s);

    }
    else if (attr == 3){
      new_s.setAddress(s);

    }
    else if (attr == 4){
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

vector<Student> create_queries(char* query_file){
  vector<Student> q;
  Student new_s ("","","","");

  string s;
  ifstream in;

  in.open(query_file);

  while(getline(in, s)){
    new_s.setID(s);
    q.push_back(new_s);
  }

  in.close();

  return q;
}

void swap(vector<Student>& students, int first, int second){
  //swap items[first] and items[second]
  Student s = students[first];
  students[first] = students[second];
  students[second] = s;
}

void selection_sort(vector<Student>& students, int size){
  Student::compareCount = 0;
  for (unsigned i = 0; i < size; i++){
    int min = i;
    for (unsigned j = i+1; j < size; j++){
      if (students[j] < students[min]){
        min = j;
      }
    }
    if (i != min){
      swap(students, i, min);
    }
  }
}

void insertion_sort(vector<Student>& students, int size){
  Student::compareCount = 0;
  for (unsigned i = 1; i < size; i++){
    Student s = students[i];
    int j = i;
    while (j > 0 and s < students[j-1]){
      students[j] = students[j-1];
      j--;
    }
    students[j] = s;
  }
}

void std_sort(vector<Student>& students, int size){
  Student::compareCount = 0;
  sort(students.begin(), students.begin() + size);
}

void list_sort(vector<Student>& students, int size){
  Student::compareCount = 0;
  list<Student> s;
  for (unsigned i = 0; i < size; i++){
    s.push_back(students[i]);
  }
  s.sort();

  list<Student>::iterator it = s.begin();
  for (unsigned i = 0; i < size; i++){
    students[i] = *it;
    it++;
  }
}

int linear_search(vector<Student>& students, vector<Student>& queries, int size, int q_size){
  int total_compares = 0;
  for (unsigned i = 0; i < q_size; i++){
    // For each query do this
    Student::compareCount = 0;
    for (unsigned j = 0; j < size; j++){
      if (students[j] == queries[i]){
        break;
      }
    }
    total_compares += Student::compareCount;
  }
  int average_compares = total_compares/q_size;
  return average_compares;
}

int std_find(vector<Student>& students, vector<Student>& queries, int size, int q_size){
  int total_compares = 0;
  for (unsigned i = 0; i < q_size; i++){
    // For each query do this
    Student::compareCount = 0;
    std::find(students.begin(), students.begin() + size, queries[i]);
    total_compares += Student::compareCount;
  }
  int average_compares = total_compares/q_size;
  return average_compares;
}

int std_set_find(vector<Student>& students, vector<Student>& queries, int size, int q_size){
  int total_compares = 0;  
  set<Student> student_set;
  for (unsigned i = 0; i < size; i++){
    student_set.insert(students[i]);
  }

  for (unsigned i = 0; i < q_size; i++){
    // For each query do this
    Student::compareCount = 0;
    student_set.find(queries[i]);
    total_compares += Student::compareCount;
  }
  int average_compares = total_compares/q_size;
  return average_compares;
}

int main(int argc, char* argv[]){
  const vector<Student> original = create_students(argv[1]); 
  vector<Student> queries = create_queries(argv[2]);
  vector<Student> students;

  int quarter = original.size()/4;
  int half = original.size()/2;
  int full = original.size();

  int q_quarter = queries.size()/4;
  int q_half = queries.size()/2;
  int q_full = queries.size();

  ofstream out;
  out.open(argv[3]);

  out << "Selection Sort" << endl;
  students = original;
  selection_sort(students, quarter);
  out << "size: " << quarter << "    compares: " << Student::compareCount << endl;
  students = original;
  selection_sort(students, half);
  out << "size: " << half << "    compares: " << Student::compareCount << endl;
  students = original;
  selection_sort(students, full);
  out << "size: " << full << "    compares: " << Student::compareCount << endl;

  out << "Insertion Sort" << endl;
  students = original;
  insertion_sort(students, quarter);
  out << "size: " << quarter << "    compares: " << Student::compareCount << endl;
  students = original;
  insertion_sort(students, half);
  out << "size: " << half << "    compares: " << Student::compareCount << endl;
  students = original;
  insertion_sort(students, full);
  out << "size: " << full << "    compares: " << Student::compareCount << endl;

  out << "std::sort" << endl;
  students = original;
  std_sort(students, quarter);
  out << "size: " << quarter << "    compares: " << Student::compareCount << endl;
  students = original;
  std_sort(students, half);
  out << "size: " << half << "    compares: " << Student::compareCount << endl;
  students = original;
  std_sort(students, full);
  out << "size: " << full << "    compares: " << Student::compareCount << endl;

  out << "std::list.sort" << endl;
  students = original;
  list_sort(students, quarter);
  out << "size: " << quarter << "    compares: " << Student::compareCount << endl;
  students = original;
  list_sort(students, half);
  out << "size: " << half << "    compares: " << Student::compareCount << endl;
  students = original;
  list_sort(students, full);
  out << "size: " << full << "    compares: " << Student::compareCount << endl;

  int average_compares;

  out << "Linear Search" << endl;
  students = original;
  average_compares = linear_search(students, queries, quarter, q_quarter);
  out << "size: " << quarter << "    compares: " << average_compares << endl;
  students = original;
  average_compares = linear_search(students, queries, half, q_half);
  out << "size: " << half << "    compares: " << average_compares << endl;
  students = original;
  average_compares = linear_search(students, queries, full, q_full);
  out << "size: " << full << "    compares: " << average_compares << endl;

  out << "std::find" << endl;
  students = original;
  average_compares = std_find(students, queries, quarter, q_quarter);
  out << "size: " << quarter << "    compares: " << average_compares << endl;
  students = original;
  average_compares = std_find(students, queries, half, q_half);
  out << "size: " << half << "    compares: " << average_compares << endl;
  students = original;
  average_compares = std_find(students, queries, full, q_full);
  out << "size: " << full << "    compares: " << average_compares << endl;

  out << "std::set.find" << endl;
  students = original;
  average_compares = std_set_find(students, queries, quarter, q_quarter);
  out << "size: " << quarter << "    compares: " << average_compares << endl;
  students = original;
  average_compares = std_set_find(students, queries, half, q_half);
  out << "size: " << half << "    compares: " << average_compares << endl;
  students = original;
  average_compares = std_set_find(students, queries, full, q_full);
  out << "size: " << full << "    compares: " << average_compares << endl;
 
  out.close();

  return 0;
}