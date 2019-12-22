/* 
 * File:   LPQConnection.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 25 mai 2014, 11:43
 */

#ifdef USE_LIBPQ
#ifndef LPQCONNECTION_H
#define LPQCONNECTION_H

#include <wx/wx.h>
#include <libpq-fe.h>
#include "LConnection.h"
/**
 * PostgreSQL namespace.
 * 
 * Please note USE_LIBPQ pre-processor directive must be defined to include the PostgreSQL backend.
 */
namespace PQ
{
#define CNPQMC wxString(_T("CNPQM"))

/**
 Minimalist class managing a PostgreSQL connection.
 */
class LPQConnection : public LConnection
{
public:
    LPQConnection();
    /**
     * Ex : host=localhost port=5432 dbname=postgres user=postgres password=secret
     * @param newInfo connection information.
     */
    LPQConnection(const wxString& newInfo);
    /**
     * The connection is closed in the destructor. Clears any returned keys.
     * But any fetched resultset persists. The caller is responsible to clear this.
     */
    virtual ~LPQConnection();
    /**
     * 
     * @return the last backend error message, may be NULL.
     */
    const char* GetLastLibMessage() const;
    /**
     * Establishes a new connection calling PQconnectdb
     * 
     * Does nothing if a connection already exists.
     * Updates the username, dbname, servername and port variables.
     * @return true if a new connection is established, or if a connection already esists. False on error.
     */
    bool Connect();
    /**
     * 
     * @return a pointer to the PGconn real database pointer.
     */
    void* Get() const;
    /**
     * Evaluates PQstatus(PGconn*).
     * @return true if the database connection is valid, false otherwise.
     */
    bool IsValid() const;
    /**
     * Closes the connection.
     * 
     * The data referenced in a PGresult object is not cleared.
     * It is the responsibility of the caller to free this object.
     */
    void Close();
    /**
     * Performs synchronous PQexec query and returns rows from a SELECT query.
     * 
     * @param newSql : a valid single SELECT SQL statement.
     * @return an untyped pointer to a PGresult containing the data, or NULL on error.
     */
    void* ExecuteSQL(const wxString& newSql);
    /**
     * Intended for database actions that do not fetch table rows. Performs synchronous PQexec query.
     * 
     * @param newSql a valid single SQL statement.
     * @return true on success, false otherwise.
     */
    bool ExecuteUpdateSQL(const wxString& newSql);
    /**
     * The value of an auto-generated primary key after executing an INSERT SQL statement.
     * 
     * The SQL statement should have a RETURNING clause that includes a serial column, whose
     * index is specified by the col parameter. The column value of the first row is returned.
     * @param col index of an auto-increment serial column in a RETURNING SQL clause.
     * @return the value of the first row for that column.
     */
    const wxAny GetReturnedValue(const unsigned int col) const;
    /**
     * PGresult object containing a primary key value
     * resulting from a successful execution of an INSERT SQL statement.
     * @return an untyped pointer to the PGresult object containing the key.
     */
    void* GetReturnedKeys() const;
    /**
     * Clears the PGresult object to free memory.
     */
    void ClearReturnedKeys();

private:
    PGconn * m_conn;
    PGresult * m_retKeys;
    bool ApplyReadWriteStatus();
    /**
     * Forwards libpq messages to InformLibMessage
     * @param
     */
    void InformPQMessage(const char * PQmsg) const;
    /**
     * Informs application an SQL statement is about to be processed regardless of connection status..
     */
    void InformBeforeExecute();
    /**
     * Informs application an SQL statement has been processed, regardless of errors.
     */
    void InformAfterExecute();
    /**
     * Informs application that registers LConnectionEvent handlers here.
     * @param msg
     */
    void InformLibMessage(const LInformation& msg) const;

};

}
#endif /* LPQCONNECTION_H */
#endif
