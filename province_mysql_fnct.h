#ifndef PROVINCE_MYSQL_FNCT_H
#define PROVINCE_MYSQL_FNCT_H
#include <glib.h>
#include <mariadb/mysql.h>
#include "province_struct.h"

MYSQL *connect_to_db();
void close_db_connection(MYSQL *conn);
int query(MYSQL *conn, const char *sql);
int insert(MYSQL *conn, const char *sql);
int update(MYSQL *conn, const char *sql);
int delete(MYSQL *conn, const char *sql);
Province getProvinceNameByCode (Province province);
gboolean updateProvinceName (Province province);
gboolean insertProvince (Province province);
gboolean deleteProvince (const gchar *staCode);

#endif
