#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct {
  sqlite3 *database;
  char databasePath;
}Database ;

extern Database *addDatabase(char *path);
extern int removeDatabase(Database *database);
extern int connectDatabase(Database *database);
extern int disconnectDatabase(Database *database);

#endif // !DATABASE_H
