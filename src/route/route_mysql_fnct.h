#ifndef ROUTE_MYSQL_FNCT_H
#define ROUTE_MYSQL_FNCT_H
#include <glib.h>
#include <mariadb/mysql.h>
#include "route_struct.h"

MYSQL *connect_to_db();
void close_db_connection(MYSQL *conn);
int query(MYSQL *conn, const char *sql);
int insert(MYSQL *conn, const char *sql);
int update(MYSQL *conn, const char *sql);
int delete(MYSQL *conn, const char *sql);
gboolean updateRoute (Route route);
gboolean insertRoute (Route route);
gboolean deleteRoute (Route *route);
GList *getAllRoutes ();
GList *getAllStations ();
Route getRouteByCode (Route route);
//gboolean insertRoute (Route route);
//gboolean updateRoute (Route route);
//gboolean deleteRoute (Route *route);

#endif
