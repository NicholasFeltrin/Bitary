#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "database.h"


Database *addDatabase(char *path){
  Database *temp;
  temp = (Database*)malloc(sizeof(Database)+(sizeof(char)*(strlen(path)+1)));
  CHECK(temp == NULL, "Unable to allocate memory", return NULL);

  strcpy((char*)&temp->databasePath, path);

  return temp;
}

int removeDatabase(Database *database){
  free(database);
  return 0;
}

int connectDatabase(Database *database){
  int retval = sqlite3_open(&(database->databasePath), &(database->database));
  CHECK(retval != SQLITE_OK, "Unable to connecting database", return -1);
  return 0;
}

int disconnectDatabase(Database *database){
  int retval = sqlite3_close(database->database);
  database->database = NULL;
  CHECK(retval != SQLITE_OK, "Problem disconnecting database", return -1);
     
  return 0;
}
