DROP TABLE IF EXISTS Users;
DROP TABLE IF EXISTS Projects;
DROP TABLE IF EXISTS Batches;
DROP TABLE IF EXISTS Fields;
DROP TABLE IF EXISTS Records;


CREATE TABLE Users
(
  id integer not null primary key autoincrement,
  first_name varchar(225),
  last_name varchar(225),
  username varchar(225),
  password varchar(225),
  email varchar(225),
  indexed_records integer,
  current_batch_id integer
);

CREATE TABLE Projects
(
  id integer not null primary key autoincrement,
  title varchar(225),  
  records_per_image integer,
  first_y_coord integer,
  record_height integer
);

CREATE TABLE Batches
(
  id integer not null primary key autoincrement,
  project_id integer,
  image_file varchar(225)
);

CREATE TABLE Fields
(
  id integer not null primary key autoincrement,
  project_id integer,
  position integer,
  title varchar(225),
  xcoord integer,
  width integer,
  help_html varchar(225),
  known_data varchar(225)
);

CREATE TABLE Records
(
  id integer not null primary key autoincrement,
  answer varchar(225),
  batch_id integer,
  line_num integer,
  field_id integer
);