/* 
 * File:   LConnection.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 25 mai 2014, 10:49
 */

#include "LConnection.h"

LConnection::LConnection()
{
    m_connInfo = wxEmptyString;
    m_UserName = wxEmptyString;
    m_DBName = wxEmptyString;
    m_ServerName = wxEmptyString;
    m_Port = wxEmptyString;
    m_readOnly = false;
}

LConnection::LConnection(const wxString& newInfo)
{
    m_connInfo = newInfo;
    m_UserName = wxEmptyString;
    m_DBName = wxEmptyString;
    m_ServerName = wxEmptyString;
    m_Port = wxEmptyString;
    m_readOnly = false;
}

LConnection::~LConnection()
{
}

void LConnection::RegisterEventHandler(LConnectionEvent * evh)
{
    if (evh == NULL || m_evtHandlers.Index(evh) != wxNOT_FOUND) return;
    m_evtHandlers.Add(evh);
}

void LConnection::UnRegisterEventHandler(LConnectionEvent * evh)
{
    if (evh == NULL || m_evtHandlers.Index(evh) == wxNOT_FOUND) return;
    m_evtHandlers.Remove(evh);
}
///////////////////////////////////////////////////////////////////////////////

LConnectionEvent::LConnectionEvent()
{
}

LConnectionEvent::~LConnectionEvent()
{
}
void LConnectionEvent::BeforeExecute(const LConnection* caller)
{
}
void LConnectionEvent::AfterExecute(const LConnection* caller)
{
}
void LConnectionEvent::Inform(const LConnection* caller, const LInformation& msg) const
{
}