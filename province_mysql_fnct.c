#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>
#include "province_mysql_fnct.h"
#include "functions.h"
#include "mysql_fnct.h"

Province getProvinceNameByCode (Province province){
  MYSQL *conn = connect_to_db ();
  MYSQL_RES *result;
  MYSQL_ROW row;
  // Build the query using the station code
  gchar sql[1000] = {0};
  sprintf(sql, "SELECT prv_name FROM province WHERE prv_code='%s'", province.prvCode);

  if (query(conn, sql)) {
    fprintf(stderr, "Error: failed to execute query\n");
    close_db_connection(conn);
    exit (1);
  }

  result = mysql_use_result (conn);
 
  if ((row = mysql_fetch_row (result)) != NULL)
    province.prvName = g_strdup (row[0]);
  else
    province.prvName = NULL;
  
  mysql_free_result(result);
  close_db_connection(conn);

  return province;
}

gboolean updateProvinceName (Province province){
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf ("UPDATE province SET prv_name = '%s' WHERE prv_code = '%s'", province.prvName, province.prvCode);
  
  if (query (conn, sql) == 0){
    result = TRUE;
  }else {
    fprintf(stderr, "Error: failed to execute query\n");
    g_free (sql);
    close_db_connection(conn);
    exit (1);
  }
  
  g_free (sql);
  mysql_close (conn);

  return result;
}

gboolean insertProvince (Province province) {
  MYSQL *conn = connect_to_db ();
  gboolean result = FALSE;
  
  gchar *sql = g_strdup_printf ("INSERT INTO province (prv_code, prv_name) VALUE ('%s', '%s')", province.prvCode, province.prvName);
  
  if (query (conn, sql) == 0){
    result = TRUE;
  }else{
    fprintf(stderr, "Error: failed to execute query\n");
    g_free (sql);
    close_db_connection(conn);
    exit (1);
  }
  
  g_free (sql);
  close_db_connection (conn);

  return result;
}

gboolean deleteProvince (const gchar *prvCode) {
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf("DELETE FROM province WHERE prv_code = '%s'", prvCode);

  if (query(conn, sql) == 0) {
    result = TRUE;
  } else {
    gchar *warning_msg = g_strdup_printf("Cannot delete province '%s' because it is used in the employee table.", prvCode);
    display_warning_message(warning_msg);  // display the warning message
    g_free(warning_msg);
  }

  g_free(sql);
  mysql_close(conn);

  return result;
}


