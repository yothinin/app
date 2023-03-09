use tms_db;
drop table if exists route;

create table route(
  rou_id int auto_increment,
  rou_code varchar (4) not null,
  rou_direction char(1) not null,
  sta_from varchar (5) not null,
  sta_to varchar (5) not null,
  primary key (rou_id)
);

alter table route modify rou_id int not null auto_increment;
alter table route add index `idx_codedirect` (rou_code, rou_direction);

insert into route (rou_code, rou_direction, sta_from, sta_to) values ("18", "2", "10000", "50000");
insert into route (rou_code, rou_direction, sta_from, sta_to) values ("18", "1", "50000", "10000");
insert into route (rou_code, rou_direction, sta_from, sta_to) values ("20", "2", "10000", "40000");
insert into route (rou_code, rou_direction, sta_from, sta_to) values ("20", "1", "40000", "10000");
insert into route (rou_code, rou_direction, sta_from, sta_to) values ("31", "2", "10000", "34000");
