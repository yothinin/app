use tms_db;
drop table if exists bustype;
CREATE TABLE bustype (
  bus_id INT PRIMARY KEY AUTO_INCREMENT,
  bus_code VARCHAR (2),
  bus_name VARCHAR (60)
);
alter table bustype add unique index `idx_buscode` (bus_code);

insert into bustype (bus_code, bus_name) values ("11", "ม.1ก");
insert into bustype (bus_code, bus_name) values ("12", "ม.1ข");
insert into bustype (bus_code, bus_name) values ("15", "ม.1พ");
