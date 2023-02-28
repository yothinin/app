use tms_db;
drop table if exists station;

create table station(
  sta_id int auto_increment,
  sta_code varchar (13) not null,
  sta_name varchar (100) not null,
  primary key (sta_id)
);

#alter table station change column sta_id int(11) not null auto_increment;
alter table station modify sta_id int not null auto_increment;
alter table station add index `idx_stacode` (sta_code);

insert into station (sta_code, sta_name) values ("10000", "กรุงเทพฯ");
insert into station (sta_code, sta_name) values ("10001", "วิภาวดี");
insert into station (sta_code, sta_name) values ("10002", "วิภา (เทศกาล)");
insert into station (sta_code, sta_name) values ("34000", "สุรินทร์");
insert into station (sta_code, sta_name) values ("40000", "ขอนแก่น");
insert into station (sta_code, sta_name) values ("55000", "น่าน");
insert into station (sta_code, sta_name) values ("50000", "เชียงใหม่");
