/*#include "Qsql.h"

#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <iostream>

static SQLHENV henv;
static SQLHDBC hdbc;
static int8 Readable=0; 

void ConectToDataBase
(
    const char* serverName,
    const char* dataBaseName,
    const char* userName,
    const char* userPassword
)
{
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

    std::string help1=
    (
        "DRIVER={SQL Server};SERVER="+std::string(serverName)+
        ";DATABASE="+std::string(dataBaseName)+
        ";UID="+std::string(userName)+
        ";PWD="+std::string(userPassword)+";"
    );
    SQLCHAR* dsn = (SQLCHAR*)help1.c_str();
    SQLRETURN retcode = SQLDriverConnectA(hdbc, NULL, dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
    if (SQL_SUCCEEDED(retcode)){Readable=1;}
    else{std::cout<<"NO conection\n";Readable=0;}
    return;
}

void ReadTableFromDataBase(Table &table,const char* tableName)
{
    if(Readable)
    {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &stmt);
        std::string help2 = "SELECT * FROM " + std::string(tableName);
        SQLCHAR* sqlQuery = (SQLCHAR*)help2.c_str();
        SQLExecDirectA(stmt, sqlQuery, SQL_NTS);
        SQLSMALLINT numCols;
        SQLNumResultCols(stmt, &numCols);
        SQLCHAR tempData[255];
        for (SQLSMALLINT i = 0; i < numCols; i++) {
            SQLDescribeColA(stmt, i+1, tempData, sizeof(tempData), NULL, NULL, NULL, NULL, NULL);
            std::vector<std::string> vec;
            vec.push_back(std::string((const char*)tempData));
            table.ColumnVec.push_back(vec);
        }
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            for (SQLSMALLINT i = 0; i < numCols; i++) {
                SQLGetData(stmt, i+1, SQL_C_CHAR, tempData, sizeof(tempData), NULL);
                table.ColumnVec[i].push_back(std::string((const char*)tempData));
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        SQLDisconnect(hdbc);
    }
    return;
}

void DisconectFromDataBase()
{
    Readable=0;
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
    return;
}

int8 IsReadable()
{
    return Readable;
}*/