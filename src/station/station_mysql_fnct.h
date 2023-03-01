#ifndef STATION_MYSQL_FNCT_H
#define STATION_MYSQL_FNCT_H
#include <glib.h>
#include <mariadb/mysql.h>
#include "station_struct.h"

MYSQL *connect_to_db();
void close_db_connection(MYSQL *conn);
int query(MYSQL *conn, const char *sql);
int insert(MYSQL *conn, const char *sql);
int update(MYSQL *conn, const char *sql);
int delete(MYSQL *conn, const char *sql);
Station getStationNameByCode (Station station);
gboolean updateStationName (Station station);
gboolean insertStation (Station station);
gboolean deleteStation (const gchar *staCode);

#endif
