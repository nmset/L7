/* 
 * File:   LSQConnection.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 25 mai 2014, 16:08
 */
#ifdef USE_LIBSQ
#include "LSQConnection.h"
#include "LSQresult.h"
using namespace SQ;

LSQConnection::LSQConnection() : LConnection()
{
    m_conn = NULL;
    m_errMsg = NULL;
}

LSQConnection::LSQConnection(const wxString& newInfo) : LConnection(newInfo)
{
    m_conn = NULL;
    m_errMsg = NULL;
}

LSQConnection::~LSQConnection()
{
    Close();
}

const char *LSQConnection::GetLastLibMessage() const
{
    return m_errMsg;
}

bool LSQConnection::Connect()
{
    if (m_conn != NULL)
    {
        return true;
    }
    const char* cinfo = m_connInfo.c_str();
    int flag = (m_readOnly ? SQLITE_OPEN_READONLY | SQLITE_OPEN_CREATE : SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    int res = sqlite3_open_v2(cinfo, &m_conn, flag, NULL);
    if (res != 0) Close();
    return (res == 0);
}

void * LSQConnection::Get() const
{
    return (void*) m_conn;
}

bool LSQConnection::IsValid() const
{
    /*
     Is there a better way to really check the status of the sqlite3 connection object ?
     * Something like PQstatus(PGconn*) ?
     * Quote from the documentation :
     * 
     * The application must insure that the 1st parameter to sqlite3_exec()
     *      is a valid and open [database connection]
     * 
     * How to ?
     */
    return (m_conn != NULL);
}

void LSQConnection::Close()
{
    if (m_conn != NULL)
    {
        sqlite3_close(m_conn);
        m_conn = NULL;
        m_errMsg = NULL;
    }
}

void * LSQConnection::ExecuteSQL(const wxString& newSql)
{
    wxCHECK_MSG(IsValid() == true, NULL, _("Invalid connection"));
    SQresult * res = new SQresult();
    const char* cSQL = newSql.c_str();
    m_errMsg = NULL;
    int rc = sqlite3_get_table(m_conn, cSQL, &res->m_data, &res->m_nbRows, &res->m_nbCols, &m_errMsg);
    if (m_errMsg)
    {
        InformSQMessage(m_errMsg);
        sqlite3_free(m_errMsg);
        sqlite3_free_table(res->m_data);
        res = NULL;
    }
    return (void*) res;
}

bool LSQConnection::ExecuteUpdateSQL(const wxString& newSql)
{
    wxCHECK_MSG(IsValid() == true, false, _("Invalid connection"));
    const char* cSQL = newSql.c_str();
    m_errMsg = NULL;
    int rc = sqlite3_exec(m_conn, cSQL, NULL, NULL, &m_errMsg);
    if (m_errMsg)
    {
        InformSQMessage(m_errMsg);
        sqlite3_free(m_errMsg);
    }
    return (rc == SQLITE_OK);
}

const wxAny LSQConnection::GetReturnedValue(const unsigned int unused) const
{
    sqlite3_int64 r = sqlite3_last_insert_rowid(m_conn);
    return wxAny((long) r);
}

void LSQConnection::InformSQMessage(const char * SQmsg) const
{
    if (!SQmsg) return;
    wxString msg(SQmsg);
    if (msg.IsEmpty()) return;
    const LInformation inf(CNSQMC, msg);
    InformLibMessage(inf);
}

void LSQConnection::InformLibMessage(const LInformation& msg) const
{
    for (int i = 0; i < evtHandlers.GetCount(); i++)
    {
        void * p = evtHandlers.Item(i);
        LConnectionEvent * evh = static_cast<LConnectionEvent*> (p);
        if (evh != NULL) evh->Inform(this, msg);
    }
}

#endif