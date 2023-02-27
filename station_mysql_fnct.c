#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>
#include "station_mysql_fnct.h"
#include "functions.h"
#include "mysql_fnct.h"

Station getStationNameByCode (Station station){
  MYSQL *conn = connect_to_db ();
  MYSQL_RES *result;
  MYSQL_ROW row;
  // Build the query using the station code
  gchar sql[1000] = {0};
  sprintf(sql, "SELECT sta_name FROM station WHERE sta_code='%s'", station.staCode);

  if (query(conn, sql)) {
    fprintf(stderr, "Error: failed to execute query\n");
    close_db_connection(conn);
    exit (1);
  }

  result = mysql_use_result (conn);
 
  if ((row = mysql_fetch_row (result)) != NULL)
    station.staName = g_strdup (row[0]);
  else
    station.staName = NULL;
  
  mysql_free_result(result);
  close_db_connection(conn);

  return station;
}

gboolean updateStationName (Station station){
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf ("UPDATE station SET sta_name = '%s' WHERE sta_code = '%s'", station.staName, station.staCode);
  
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

gboolean insertStation (Station station) {
  MYSQL *conn = connect_to_db ();
  gboolean result = FALSE;
  
  gchar *sql = g_strdup_printf ("INSERT INTO station (sta_code, sta_name) VALUE ('%s', '%s')", station.staCode, station.staName);
  
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

//gboolean deleteStation (const gchar *staCode) {
  //MYSQL *conn = connect_to_db();
  //gboolean result = FALSE;
  //gchar *sql = g_strdup_printf ("DELETE FROM station WHERE sta_code = '%s'", staCode);

  //if (query (conn, sql) == 0){
    //result = TRUE;
  //}else {
    //fprintf(stderr, "deleteStation::Error: failed to execute query\n");
    //g_free (sql);
    //close_db_connection(conn);
    //exit (1);
  //}

  //g_free (sql);
  //mysql_close (conn);

  //return result;
//}

gboolean deleteStation(const gchar *staCode) {
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf("DELETE FROM station WHERE sta_code = '%s'", staCode);

  if (query(conn, sql) == 0) {
    result = TRUE;
  } else {
    gchar *warning_msg = g_strdup_printf("Cannot delete station '%s' because it is used in the route table.", staCode);
    display_warning_message(warning_msg);  // display the warning message
    g_free(warning_msg);
    //g_free(sql);
    //close_db_connection(conn);
  }

  g_free(sql);
  mysql_close(conn);

  return result;
}


