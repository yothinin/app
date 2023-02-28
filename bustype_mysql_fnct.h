#ifndef BUSTYPE_MYSQL_FNCT_H
#define BUSTYPE_MYSQL_FNCT_H
#include <glib.h>
#include <mariadb/mysql.h>
#include "bustype_struct.h"

MYSQL *connect_to_db();
void close_db_connection(MYSQL *conn);
int query(MYSQL *conn, const char *sql);
int insert(MYSQL *conn, const char *sql);
int update(MYSQL *conn, const char *sql);
int delete(MYSQL *conn, const char *sql);
Bustype getBustypeNameByCode (Bustype bustype);
gboolean updateBustypeName (Bustype bustype);
gboolean insertBustype (Bustype bustype);
gboolean deleteBustype (const gchar *busCode);

#endif
