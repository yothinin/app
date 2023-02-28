use tms_db;
drop table if exists province;
CREATE TABLE province (
  prv_id INT PRIMARY KEY AUTO_INCREMENT,
  prv_code VARCHAR (2),
  prv_name VARCHAR (60)
);
alter table province add unique index `idx_prvcode` (prv_code);

insert into province (prv_code, prv_name) values ("10", "กรุงเทพฯ");
insert into province (prv_code, prv_name) values ("40", "ขอนแก่น");
insert into province (prv_code, prv_name) values ("50", "เชียงใหม่");
insert into province (prv_code, prv_name) values ("55", "น่าน");
