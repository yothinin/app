use tms_db;
drop table if exists employee;

CREATE TABLE employee (
  emp_id INT AUTO_INCREMENT,
  emp_code VARCHAR (6),
  emp_cid VARCHAR (13),
  emp_name VARCHAR (100),
  emp_phone VARCHAR (20),
  emp_email VARCHAR (100),
  emp_addr1 VARCHAR (60),
  emp_addr2 VARCHAR (60),
  emp_zip VARCHAR (5),
  prv_code varchar (2),
  primary key (emp_id)
);

alter table employee modify emp_id int not null auto_increment;
alter table employee add unique index `idx_empcode` (emp_code);

ALTER TABLE employee ADD CONSTRAINT fk_prvcode FOREIGN KEY (prv_code) REFERENCES province(prv_code) ON DELETE RESTRICT ON UPDATE CASCADE;

insert into employee 
  (emp_code, emp_cid, emp_name, emp_phone, emp_email, emp_addr1, emp_addr2, emp_zip, prv_code)
values
  ("460034", "3550600002721", "โยธิน อินบรรเลง", "0851624545", "yothinin@hotmail.com", "222/109 บ้านสวนคอนโด ตึก F ชั้น 7", "วิภาวดี-รังสิต 17 แยก 10-3 แขวง/เขตจตุจักร", "10900", "10");
  
