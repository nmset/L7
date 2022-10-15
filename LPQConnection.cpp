/* 
 * File:   LPQConnection.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 25 mai 2014, 11:44
 */
#ifdef USE_LIBPQ
#include "LPQConnection.h"
using namespace PQ;

LPQConnection::LPQConnection() : LConnection()
{
    m_conn = NULL;
    m_retKeys = NULL;
}

LPQConnection::LPQConnection(const wxString& newInfo) : LConnection(newInfo)
{
    m_conn = NULL;
    m_retKeys = NULL;
}

LPQConnection::~LPQConnection()
{
    PQclear(m_retKeys);
    Close();
}

const char* LPQConnection::GetLastLibMessage() const
{
    return PQerrorMessage(m_conn);
}

bool LPQConnection::Connect()
{
    wxCHECK_MSG(!m_connInfo.IsEmpty(), false, _T("Connection string is undefined"));
    if (m_conn != NULL)
    {
        if (PQstatus(m_conn) == CONNECTION_OK) return true;
    }
    const char* cinfo = m_connInfo.utf8_str();
    m_conn = PQconnectdb(cinfo);
    bool res = IsValid();
    if (res)
    {
        ApplyReadWriteStatus();
        m_UserName = wxString(PQuser(m_conn));
        m_DBName = wxString(PQdb(m_conn));
        m_ServerName = wxString(PQhost(m_conn));
        m_Port = wxString(PQport(m_conn));
    }
    else
    {
        Close();
    }
    return res;
}

void* LPQConnection::Get() const
{
    return (void*) m_conn;
}

bool LPQConnection::IsValid() const
{
    if (PQstatus(m_conn) != CONNECTION_OK)
    {
        InformPQMessage(PQerrorMessage(m_conn));
        return false;
    }
    return true;
}

void LPQConnection::Close()
{
    if (m_conn != NULL)
    {
        PQfinish(m_conn);
        m_conn = NULL;
    }
}

void* LPQConnection::ExecuteSQL(const wxString& newSql)
{
    InformBeforeExecute();
    wxCHECK_MSG(IsValid() == true, NULL, _T("Invalid connection"));
    const char* cSQL = newSql.utf8_str();
    PGresult * res = PQexec(m_conn, cSQL);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        InformPQMessage(PQerrorMessage(m_conn));
        InformAfterExecute();
        return NULL;
    }
    InformAfterExecute();
    return (void*) res;
}

bool LPQConnection::ExecuteUpdateSQL(const wxString& newSql)
{
    InformBeforeExecute();
    wxCHECK_MSG(IsValid() == true, false, _T("Invalid connection"));
    const char* cSQL = newSql.utf8_str();
    m_retKeys = PQexec(m_conn, cSQL);
    if ((PQresultStatus(m_retKeys) != PGRES_TUPLES_OK) && (PQresultStatus(m_retKeys) != PGRES_COMMAND_OK))
    {
        PQclear(m_retKeys);
        m_retKeys = NULL;
        InformPQMessage(PQerrorMessage(m_conn));
        InformAfterExecute();
        return false;
    }
    InformAfterExecute();
    return true;
}

const wxAny LPQConnection::GetReturnedValue(const unsigned int col) const
{
    if (!m_retKeys)
    {
        const wxAny k;
        return k;
    }
    const char * data = PQgetvalue(m_retKeys, 0, col);
    const wxAny k(data);
    return k;
}

void* LPQConnection::GetReturnedKeys() const
{
    return (void*) m_retKeys;
}

void LPQConnection::ClearReturnedKeys()
{
    if (m_retKeys != NULL)
    {
        PQclear(m_retKeys);
        m_retKeys = NULL;
    }
}

bool LPQConnection::ApplyReadWriteStatus()
{
    wxString sql = (m_readOnly)
            ? _T("SET SESSION CHARACTERISTICS AS TRANSACTION READ ONLY")
            : _T("SET SESSION CHARACTERISTICS AS TRANSACTION READ WRITE");
    return ExecuteUpdateSQL(sql);

}

void LPQConnection::InformPQMessage(const char* PQmsg) const
{
    if (!PQmsg) return;
    wxString msg(PQmsg);
    if (msg.IsEmpty()) return;
    const LInformation inf(CNPQMC, msg);
    InformLibMessage(inf);
}

void LPQConnection::InformBeforeExecute()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LConnectionEvent * evh = static_cast<LConnectionEvent*> (p);
        if (evh != NULL) evh->BeforeExecute(this);
    }
}

void LPQConnection::InformAfterExecute()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LConnectionEvent * evh = static_cast<LConnectionEvent*> (p);
        if (evh != NULL) evh->AfterExecute(this);
    }
}

void LPQConnection::InformLibMessage(const LInformation& msg) const
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LConnectionEvent * evh = static_cast<LConnectionEvent*> (p);
        if (evh != NULL) evh->Inform(this, msg);
    }
}
#endif