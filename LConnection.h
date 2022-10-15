/* 
 * File:   LConnection.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 25 mai 2014, 10:48
 */

#ifndef LCONNECTION_H
#define LCONNECTION_H

#include <wx/wx.h>
#include "LInformation.h"

class LConnection;
class LConnectionEvent;

/**
 * Abstract class for implementing a database connection.
 */
class LConnection : public wxTrackable
{
public:
    LConnection();
    /**
     * 
     * @param newInfo database connection information.
     */
    LConnection(const wxString& newInfo);
    virtual ~LConnection();
    virtual const char* GetLastLibMessage() const = 0;

    /**
     * 
     * @return the connection information.
     */
    const wxString& GetInfo() const
    {
        return m_connInfo;
    }

    /**
     * 
     * @param newInfo the connection information.
     */
    void SetInfo(const wxString& newInfo)
    {
        m_connInfo = newInfo;
    }
    virtual bool Connect() = 0;
    virtual void * Get() const = 0;
    virtual bool IsValid() const = 0;
    virtual void Close() = 0;
    virtual void * ExecuteSQL(const wxString& newSql) = 0;
    virtual bool ExecuteUpdateSQL(const wxString& newSql) = 0;
    virtual const wxAny GetReturnedValue(const unsigned int col) const = 0;

    /**
     * After a change, the connection must be closed and re-established.
     * @param newReadOnly
     */
    void SetReadOnly(bool newReadOnly)
    {
        m_readOnly = newReadOnly;
    }

    /**
     * 
     * @return
     */
    bool IsReadOnly() const
    {
        return m_readOnly;
    }

    /**
     * Concerns the PostgreSQL backend only.
     * @return the backend user name.
     */
    const wxString& GetUserName() const
    {
        return m_UserName;
    }

    /**
     * Concerns the PostgreSQL backend only.
     * @return the database name.
     */
    const wxString& GetDBName() const
    {
        return m_DBName;
    }

    /**
     * Concerns the PostgreSQL backend only.
     * @return the server address, as specified in the connection string.
     */
    const wxString& GetServerName() const
    {
        return m_ServerName;
    }

    /**
     * Concerns the PostgreSQL backend only.
     * @return the server port.
     */
    const wxString& GetServerPort() const
    {
        return m_Port;
    }

    /**
     * Concerns the PostgreSQL backend only.
     * @return a result set of the returned keys after an insert.
     */
    virtual void * GetReturnedKeys() const
    {
        return NULL;
    };

    /**
     * Concerns the PostgreSQL backend only. Frees memory.
     */
    virtual void ClearReturnedKeys()
    {
    };
    /**
     * Application can register derived classes of LConnectionEvent.
     * @param evh : an LConnectionEvent derived class.
     */
    void RegisterEventHandler(LConnectionEvent * evh);
    /**
     * Application can unregister derived classes of LConnectionEvent.
     * @param evh : an LConnectionEvent derived class.
     */
    void UnRegisterEventHandler(LConnectionEvent * evh);

    /**
     * Returns registered derived classes of LConnectionEvent. 
     * @return A reference to a wxArrayPtrVoid object.
     */
    wxArrayPtrVoid& GetEventHandlers()
    {
        return m_evtHandlers;
    }
protected:
    wxString m_connInfo;
    wxString m_UserName;
    wxString m_DBName;
    wxString m_ServerName;
    wxString m_Port;
    wxArrayPtrVoid m_evtHandlers;
    bool m_readOnly;
private:

};

/**
 This class can be derived by an application to implement its methods.
 * The application should register its derived classes with an LConnection object.
 */
class LConnectionEvent
{
public:
    LConnectionEvent();
    virtual ~LConnectionEvent();
    /**
     * Concerns the PostgreSQL backend only.
     * @param caller the LConnection object having registered the derived class.
     */
    virtual void BeforeExecute(const LConnection * caller);
    /**
     * Concerns the PostgreSQL backend only.
     * @param caller the LConnection object having registered the derived class.
     */
    virtual void AfterExecute(const LConnection * caller);
    /**
     * 
     * @param caller the LConnection object having registered the derived class.
     * @param msg messages from the LConnection object.
     */
    virtual void Inform(const LConnection * caller,
                        const LInformation& msg) const;

private:

};

#endif /* LCONNECTION_H */

