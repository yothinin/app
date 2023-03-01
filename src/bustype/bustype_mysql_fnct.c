#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>
#include "bustype_mysql_fnct.h"
#include "functions.h"
#include "mysql_fnct.h"

Bustype getBustypeNameByCode (Bustype bustype){
  MYSQL *conn = connect_to_db ();
  MYSQL_RES *result;
  MYSQL_ROW row;
  // Build the query using the station code
  gchar sql[1000] = {0};
  sprintf(sql, "SELECT bus_name FROM bustype WHERE bus_code='%s'", bustype.busCode);

  if (query(conn, sql)) {
    fprintf(stderr, "Error: failed to execute query\n");
    close_db_connection(conn);
    exit (1);
  }

  result = mysql_use_result (conn);
 
  if ((row = mysql_fetch_row (result)) != NULL)
    bustype.busName = g_strdup (row[0]);
  else
    bustype.busName = NULL;
  
  mysql_free_result(result);
  close_db_connection(conn);

  return bustype;
}

gboolean updateBustypeName (Bustype bustype){
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf ("UPDATE bustype SET bus_name = '%s' WHERE bus_code = '%s'", bustype.busName, bustype.busCode);
  
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

gboolean insertBustype (Bustype bustype) {
  MYSQL *conn = connect_to_db ();
  gboolean result = FALSE;
  
  gchar *sql = g_strdup_printf ("INSERT INTO bustype (bus_code, bus_name) VALUE ('%s', '%s')", bustype.busCode, bustype.busName);
  
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

gboolean deleteBustype (const gchar *busCode) {
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf("DELETE FROM bustypeWHERE bus_code = '%s'", busCode);

  if (query(conn, sql) == 0) {
    result = TRUE;
  } else {
    gchar *warning_msg = g_strdup_printf("Cannot delete bustype '%s' because it is used in the schedule table.", busCode);
    display_warning_message(warning_msg);  // display the warning message
    g_free(warning_msg);
  }

  g_free(sql);
  mysql_close(conn);

  return result;
}


