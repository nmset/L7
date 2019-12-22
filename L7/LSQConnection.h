/* 
 * File:   LSQConnection.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 25 mai 2014, 16:07
 */

#ifdef USE_LIBSQ
#ifndef LSQCONNECTION_H
#define LSQCONNECTION_H

#include <sqlite3.h>
#include <vector>
#include <wx/wx.h>
#include "LConnection.h"

/**
 * SQLite namespace.
 * 
 * Please note USE_LIBSQ pre-processor directive must be defined to include the SQLite backend.
 */
namespace SQ
{
#define CNSQMC wxString(_T("CNSQM"))

class SQresult;

/**
 Minimalist class managing an SQLite3 connection; derives from LConnection.
 */
class LSQConnection : public LConnection
{
public:
    LSQConnection();
    /**
     * 
     * @param newInfo : connection information. A file path, or memory/temporary database string instructions.
     */
    LSQConnection(const wxString& newInfo);
    /**
     * The connection is closed in the destructor.
     */
    virtual ~LSQConnection();
    /**
     * 
     * @return the last backend error message, may be NULL.
     */
    const char* GetLastLibMessage() const;
    /**
     * Establishes a new connection, creating the SQLIte database if necessary.
     * Does nothing if a connection already exists.
     * @return true if a new connection is established, or if a connection already esists. False on error.
     */
    bool Connect();
    /**
     * 
     * @return a pointer to the sqlite3 real database pointer.
     */
    void * Get() const;
    /**
     * Checks if the sqlite3 pointer is not NULL.
     * @return true if the sqlite3 pointer is not NULL.
     */
    bool IsValid() const;
    /**
     * Closes the connection. Error message pointer is set to NULL.
     */
    void Close();
    /**
     * Calls sqlite3_get_table and returns rows from a SELECT query.
     * @param newSql a valid SQL statement.
     * @return an untyped pointer to an SQresult referencing the data, or NULL if error.
     */
    void * ExecuteSQL(const wxString& newSql);
    /**
     * Intended for database actions that do not fetch table rows. Calls sqlite3_exec.
     * @param newSql : : a valid SQL statement.
     * @return true on success, false otherwise.
     */
    bool ExecuteUpdateSQL(const wxString& newSql);
    /**
     * 
     * @param col : unused
     * @return an insert row identifier is always returned, even if the table does not have an autoincrement column.
     */
    const wxAny GetReturnedValue(const unsigned int unused) const;

private:
    sqlite3 * m_conn;
    char * m_errMsg;
    /**
     * Forwards SQLite messages to InformLibMessage
     * @param
     */
    void InformSQMessage(const char * SQmsg) const;
    wxArrayPtrVoid evtHandlers;
    /**
     * Informs application that registers LConnectionEvent handlers here.
     * @param msg
     */
    void InformLibMessage(const LInformation& msg) const;
};

}
#endif /* LSQCONNECTION_H */
#endif
