#include "MySQLMangager.h"

MySQLMangager::MySQLMangager()
{
	wcscpy_s(chr_ds_name, SQL_MAX_DSN_LENGTH, (SQLWCHAR *)L"mysql_unicode"); // odbc name
}


MySQLMangager::~MySQLMangager()
{
}

// Allocate environment handle and connection handle, connect to data source, and allocate statement handle.
void MySQLMangager::sqlconn() {
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv, &hdbc);
	rc = SQLConnect(hdbc, chr_ds_name, SQL_NTS, NULL, 0, NULL, 0);

	// Deallocate handles, display error message, and exit.
	if (!MYSQLSUCCESS(rc)) {
		SQLFreeConnect(henv);
		SQLFreeEnv(henv);
		SQLFreeConnect(hdbc);
		if (hstmt)
			error_out();
		exit(-1);
	}

	rc = SQLAllocStmt(hdbc, &hstmt);
}

// Execute SQL command with SQLExecDirect() ODBC API.
void MySQLMangager::sqlexec(SQLWCHAR * query, SQLWCHAR * result) {
	rc = SQLExecDirect(hstmt, query, SQL_NTS);
	if (!MYSQLSUCCESS(rc)) {  //Error
		error_out();
		// Deallocate handles and disconnect.
		SQLFreeStmt(hstmt, SQL_DROP);
		SQLDisconnect(hdbc);
		SQLFreeConnect(hdbc);
		SQLFreeEnv(henv);
		exit(-1);
	}
	else {
		for (rc = SQLFetch(hstmt); rc == SQL_SUCCESS; rc = SQLFetch(hstmt)) {
			SQLGetData(hstmt, 1, SQL_C_WCHAR, szData, sizeof(szData), &cbData);
			// In this example, the data is sent to the console; SQLBindCol() could be called to bind 
			// individual rows of data and assign for a rowset.
			swprintf_s(result, cbData,  L"%ls", szData);
			wprintf(L"%ls\n",szData);
		}
	}
}

// Free the statement handle, disconnect, free the connection handle, and free the environment handle.
void MySQLMangager::sqldisconn() {
	SQLFreeStmt(hstmt, SQL_DROP);
	SQLDisconnect(hdbc);
	SQLFreeConnect(hdbc);
	SQLFreeEnv(henv);
}

// Display error message in a message box that has an OK button.
void MySQLMangager::error_out() {
	SQLWCHAR szSQLSTATE[10];
	SDWORD nErr;
	SQLWCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1];
	SWORD cbmsg;

	while (SQLError(0, 0, hstmt, szSQLSTATE, &nErr, msg, sizeof(msg), &cbmsg) == SQL_SUCCESS) {
		swprintf_s(szData, sizeof(szData), L"Error:\nSQLSTATE=%ls, Native error=%ld, msg='%ls'", szSQLSTATE, nErr, msg);
		MessageBox(NULL, szData, L"ODBC Error", MB_OK);
	}
}
