
#include "route_mysql_fnct.h"
#include "station_struct.h"
#include "functions.h"
#include "mysql_fnct.h"

// This functin add data from mysql to GList and return to caller
// Call from insertDataToTreeListStore() in widget_route_fnct.c
GList *getAllRoutes () {
  MYSQL *conn = connect_to_db();
  gchar *query = "SELECT r.rou_code, s1.sta_name, s2.sta_name, rou_direction, r.sta_from, r.sta_to FROM route r LEFT JOIN station s1 ON r.sta_from = s1.sta_code LEFT JOIN station s2 ON r.sta_to = s2.sta_code ORDER BY r.rou_code, r.rou_direction;";

  if (mysql_query(conn, query) != 0) {
    g_error("mysql_query() failed: %s", mysql_error(conn));
    return NULL;
  }

  MYSQL_RES *res = mysql_use_result(conn);
  if (res == NULL) {
    g_error("mysql_use_result() failed: %s", mysql_error(conn));
    return NULL;
  }

  GList *routeList = NULL;
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(res))) {
    //Route *route = g_new(Route, 1);
    Route *route = malloc (sizeof (Route)); 
    //struct MyStruct* myStruct = malloc(sizeof(struct MyStruct));

    route->rouCode = g_strdup(row[0]);
    route->rouNameFrom = g_strdup(row[1]);
    route->rouNameTo = g_strdup(row[2]);
    route->rouDirection = g_strdup(row[3]);
    route->staFrom = g_strdup(row[4]);
    route->staTo = g_strdup(row[5]);

    routeList = g_list_append(routeList, route);
  }
  mysql_free_result(res);
  
  return routeList;
}

GList *getAllStations () {
  MYSQL *conn = connect_to_db();
  gchar *query = "SELECT sta_code, sta_name FROM station ORDER BY sta_code;";

  if (mysql_query(conn, query) != 0) {
    g_error("mysql_query() failed: %s", mysql_error(conn));
    return NULL;
  }

  MYSQL_RES *res = mysql_use_result(conn);
  if (res == NULL) {
    g_error("mysql_use_result() failed: %s", mysql_error(conn));
    return NULL;
  }

  GList *stationList = NULL;
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(res))) {
    //Route *route = g_new(Route, 1);
    Station *station = malloc (sizeof (Station)); 

    station->staCode = g_strdup(row[0]);
    station->staName = g_strdup(row[1]);

    stationList = g_list_append(stationList, station);
  }
  mysql_free_result(res);
  
  return stationList;
}

Route getRouteByCode (Route route){
  MYSQL *conn = connect_to_db ();
  MYSQL_RES *result;
  MYSQL_ROW row;
  // Build the query using the station code and rouDirection.
  gchar sql[1000];
  sprintf(sql, "SELECT s1.sta_name, s2.sta_name, r.sta_from, r.sta_to FROM route r LEFT JOIN station s1 ON r.sta_from = s1.sta_code LEFT JOIN station s2 ON r.sta_to = s2.sta_code WHERE rou_code = '%s' and rou_direction = '%s' ORDER BY r.rou_code, r.rou_direction;", route.rouCode, route.rouDirection);

  //g_print ("%s\n", sql);

  if (query(conn, sql)) {
    fprintf(stderr, "Error: failed to execute query\n");
    close_db_connection(conn);
    exit (1);
  }

  result = mysql_use_result (conn);
 
  if ((row = mysql_fetch_row (result)) != NULL){
    route.rouNameFrom = g_strdup (row[0]);
    route.rouNameTo = g_strdup (row[1]);
    route.staFrom = g_strdup (row[2]);
    route.staTo = g_strdup (row[3]);
  } else {
    route.rouNameFrom = NULL;
  }
  
  mysql_free_result(result);
  close_db_connection(conn);

  return route;
}

gboolean insertRoute (Route route) {
  MYSQL *conn = connect_to_db ();
  gboolean result = FALSE;
  
  gchar *sql = g_strdup_printf ("INSERT INTO route (rou_code, rou_direction, sta_from, sta_to) VALUE ('%s', '%s', '%s', '%s')", route.rouCode, route.rouDirection, route.staFrom, route.staTo);
  
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

gboolean updateRoute (Route route){
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf ("UPDATE route SET sta_from = '%s', sta_to = '%s' WHERE rou_code = '%s' and rou_direction = '%s';", route.staFrom, route.staTo, route.rouCode, route.rouDirection);
  
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

gboolean deleteRoute(Route *route) {
  MYSQL *conn = connect_to_db();
  gboolean result = FALSE;
  gchar *sql = g_strdup_printf("DELETE FROM route WHERE rou_code = '%s' and rou_direction = '%s';", route->rouCode, route->rouDirection);

  if (query(conn, sql) == 0) {
    result = TRUE;
  } else {
    gchar *warning_msg = g_strdup_printf("Cannot delete route '%s'-'%s' because it is used in the schedule table.", route->rouCode, route->rouDirection);
    display_warning_message(warning_msg);  // display the warning message
    g_free(warning_msg);
  }

  g_free(sql);
  mysql_close(conn);

  return result;
}
