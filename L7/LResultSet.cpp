/* 
 * File:   LResultSet.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 28 mai 2014, 21:13
 */

#include "LResultSet.h"
#include <wx/tokenzr.h>

LResultSet::LResultSet() : LLightResultSet()
{
    m_tblName = wxEmptyString;
    m_pkColName = wxEmptyString;
    m_inserting = false;
    m_canDelete = true;
    m_canSave = true;
    m_canInsert = true;
    m_canUpdate = true;
    m_readOnly = false;
    m_canMove = true;
    m_masterResultSet = NULL;
    m_masterColumnNames = wxEmptyString;
    m_childColumnNames = wxEmptyString;
}

LResultSet::LResultSet(LConnection* newConnection) : LLightResultSet(newConnection)
{
    m_tblName = wxEmptyString;
    m_pkColName = wxEmptyString;
    m_inserting = false;
    m_canDelete = true;
    m_canSave = true;
    m_canInsert = true;
    m_canUpdate = true;
    m_readOnly = false;
    m_canMove = true;
    m_masterResultSet = NULL;
    m_masterColumnNames = wxEmptyString;
    m_childColumnNames = wxEmptyString;
}

LResultSet::~LResultSet()
{
    UnRegisterAllControls();
    m_evtHandlers.Clear();
}

void LResultSet::RegisterEventHandler(LResultSetEvent * evh)
{
    if (evh == NULL || m_evtHandlers.Index(evh) != wxNOT_FOUND) return;
    m_evtHandlers.Add(evh);
}

void LResultSet::UnRegisterEventHandler(LResultSetEvent * evh)
{
    if (evh == NULL || m_evtHandlers.Index(evh) == wxNOT_FOUND) return;
    m_evtHandlers.Remove(evh);
}

void LResultSet::RegisterControl(LBoundControl * comp)
{
    if (comp == NULL || m_BoundControls.Index(comp) != wxNOT_FOUND) return;
    m_BoundControls.Add(comp);
}

void LResultSet::UnRegisterControl(LBoundControl * comp)
{
    if (comp == NULL || m_BoundControls.Index(comp) == wxNOT_FOUND) return;
    m_BoundControls.Remove(comp);
    comp->SetResultSet(NULL);
}

void LResultSet::UnRegisterAllControls()
{
    while (m_BoundControls.Count() > 0)
    {
        LBoundControl * ctrl = static_cast<LBoundControl*> (m_BoundControls.Item(0));
        if (ctrl) UnRegisterControl(ctrl);
    }
}

bool LResultSet::IsDirty() const
{
    for (unsigned int i = 0; i < m_BoundControls.GetCount(); i++)
    {
        LBoundControl * control = static_cast<LBoundControl*> (m_BoundControls.Item(i));
        if (!control) continue;
        if (control->IsDirty()) return true;
    }
    return false;
}

bool LResultSet::Refresh()
{
    if (!m_readOnly)
    {
        if (m_inserting)
        {
            InformInserting();
            return false;
        }
        if (IsDirty())
        {
            InformDirty();
            return false;
        }
    }
    bool res = RunSQL();
    if (res) InformAfterAction(LResultSetEvent::ACTION_REFRESH);
    return res;
}

bool LResultSet::Cancel()
{
    if (!m_inserting && !IsDirty()) return false;
    bool wasInserting = m_inserting;
    m_inserting = false;
    Display();
    if (wasInserting)
    {
        if (m_cursor > -1) InformAfterChangeRow();
    }
    InformAfterAction(LResultSetEvent::ACTION_CANCEL);
    return true;
}

bool LResultSet::AddNew()
{
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    if (m_readOnly || m_conn->IsReadOnly()) return false;
    if (m_inserting)
    {
        InformInserting();
        return false;
    }
    if (IsDirty())
    {
        InformDirty();
        return false;
    }
    m_inserting = true;
    Display();
    if (m_cursor > -1) InformAfterChangeRow();
    InformAfterAction(LResultSetEvent::ACTION_ADDNEW);
    return true;
}

bool LResultSet::Delete()
{
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    if (m_readOnly || m_conn->IsReadOnly()) return false;
    if (!HasData()) return false;
    if (m_inserting)
    {
        InformInserting();
        return false;
    }
    if (IsDirty())
    {
        InformDirty();
        return false;
    }
    InformBeforeDelete();
    if (!m_canDelete) return false;
    const wxString sql = MakeDeleteSQL();
    if (sql.IsEmpty()) return false;
    wxVariant oldPkValue = GetData(m_pkColName);
    bool res = m_conn->ExecuteUpdateSQL(sql);
    if (res)
    {
        const int oldCursor = m_cursor;
        RunSQL(); // m_cursor gets 0 or -1
        m_cursor = oldCursor - 1;
        Display();
        InformAfterDelete(oldPkValue, sql);
        InformAfterChangeRow();
        InformAfterAction(LResultSetEvent::ACTION_DELETE);
    }
    return res;
}

LBoundControl* LResultSet::GetControl(const wxString& colName) const
{
    for (unsigned int i = 0; i < m_BoundControls.GetCount(); i++)
    {
        LBoundControl * control = static_cast<LBoundControl*> (m_BoundControls.Item(i));
        if (!control) continue;
        if (control->GetColumnName().Lower() == colName.Lower()) return control;
    }
    return NULL;
}

void LResultSet::InformInitialised()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->Initialised(this);
    }
}

void LResultSet::InformDataChanged()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->DataChanged(this);
    }
}

void LResultSet::InformBeforeSave()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->BeforeSave(this);
    }
}

void LResultSet::InformBeforeInsert()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->BeforeInsert(this);
    }
}

void LResultSet::InformBeforeUpdate()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->BeforeUpdate(this);
    }
}

void LResultSet::InformBeforeDelete()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->BeforeDelete(this);
    }
}

void LResultSet::InformBeforeChangeRow()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->BeforeChangeRow(this);
    }
}

void LResultSet::InformAfterSave()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->AfterSave(this);
    }
}

void LResultSet::InformAfterInsert(const wxAny& newPK, const wxString& sql)
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->AfterInsert(this, newPK, sql);
    }
}

void LResultSet::InformAfterUpdate(const wxAny& curPK, const wxString& sql)
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->AfterUpdate(this, curPK, sql);
    }
}

void LResultSet::InformAfterDelete(const wxAny& oldPK, const wxString& sql)
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->AfterDelete(this, oldPK, sql);
    }
}

void LResultSet::InformAfterChangeRow()
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->AfterChangeRow(this);
    }
}

void LResultSet::InformLibMessage(const LInformation& msg) const
{
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->Inform(this, msg);
    }
}

void LResultSet::InformAfterAction(const int action)
{
    LResultSetEvent::ACTIONS a = static_cast<LResultSetEvent::ACTIONS> (action);
    for (int i = 0; i < m_evtHandlers.GetCount(); i++)
    {
        void * p = m_evtHandlers.Item(i);
        LResultSetEvent * evh = static_cast<LResultSetEvent*> (p);
        if (evh != NULL) evh->AfterAction(this, a);
    }
}

bool LResultSet::IsChild() const
{
    if (m_masterResultSet == NULL) return false;
    if ((m_masterColumnNames.IsEmpty()) || (m_childColumnNames.IsEmpty()))
        return false;
    wxStringTokenizer zMaster(m_masterColumnNames, _T(";"));
    wxStringTokenizer zChild(m_childColumnNames, _T(";"));
    return ((zMaster.CountTokens()) == (zChild.CountTokens()));
}

wxString LResultSet::MakeChildInsertColList() const
{
    wxStringTokenizer zChild(m_childColumnNames, _T(";"));
    wxString token = wxEmptyString;
    wxString colList = wxEmptyString;
    while (zChild.HasMoreTokens())
    {
        token = zChild.GetNextToken();
        colList += token + _T(", ");
    }
    return colList; // with a trailing ", " if not empty.
}

wxString LResultSet::MakeMasterValueList() const
{
    wxStringTokenizer zMaster(m_masterColumnNames, _T(";"));
    wxString token = wxEmptyString;
    wxString val = wxEmptyString;
    wxString valList = wxEmptyString;
    LBoundControl * control = NULL;
    while (zMaster.HasMoreTokens())
    {
        wxASSERT_MSG(m_masterResultSet != NULL, "m_masterResultSet est NULL.");
        token = zMaster.GetNextToken();
        control = m_masterResultSet->GetControl(token);
        wxString sData = wxEmptyString;
        if (control)
        {
            m_masterResultSet->GetData(token).GetAs<wxString>(&sData);
            val = control->GetSQLQuote() + sData + control->GetSQLQuote();
        }
        else
        {
            // The master resultset may not have GUI controls.
            m_masterResultSet->GetData(m_masterResultSet->GetRow(), m_masterResultSet->GetColumnIndex(token)).GetAs<wxString>(&sData);
            val = _T("'") + sData + _T("'");
        }
        valList += val + _T(", ");
    }
    return valList; // with a trailing ", " if not empty.
}

/////////////////////////////////////////////////////////////////////////////

LResultSetEvent::LResultSetEvent()
{
}

LResultSetEvent::~LResultSetEvent()
{
}

void LResultSetEvent::Initialised(LResultSet* caller)
{
}

void LResultSetEvent::DataChanged(LResultSet* caller)
{
}

void LResultSetEvent::BeforeSave(LResultSet * caller)
{
}

void LResultSetEvent::BeforeInsert(LResultSet * caller)
{
}

void LResultSetEvent::BeforeUpdate(LResultSet * caller)
{
}

void LResultSetEvent::BeforeDelete(LResultSet * caller)
{
}

void LResultSetEvent::BeforeChangeRow(LResultSet * caller)
{
}

void LResultSetEvent::AfterSave(LResultSet * caller)
{
}

void LResultSetEvent::AfterInsert(LResultSet * caller, const wxAny& newPK, const wxString& sql)
{
}

void LResultSetEvent::AfterUpdate(LResultSet * caller, const wxAny& curPK, const wxString& sql)
{
}

void LResultSetEvent::AfterDelete(LResultSet * caller, const wxAny& oldPK, const wxString& sql)
{
}

void LResultSetEvent::AfterChangeRow(LResultSet * caller)
{
}

void LResultSetEvent::AfterAction(LResultSet * caller, ACTIONS action)
{
}

void LResultSetEvent::Inform(const LResultSet * caller, const LInformation& msg) const
{
}