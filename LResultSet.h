/* 
 * File:   LResultSet.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 28 mai 2014, 21:11
 */

#ifndef LRESULTSET_H
#define LRESULTSET_H

#include "LInformation.h"
#include "LBoundControl.h"
#include "LLightResultSet.h"

class LBoundControl;
class LResultSetEvent;

/**
 * An abstract class that allows data modification using GUI elements.
 * 
 * Don't use aliases for table names.
 * 
 * A column alias must still be a real column name of the table specified in SetTableName.
 * 
 * Ex : SELECT tbl1.pk1, tbl1.text1, list2.text2 AS num1 FROM tbl1 LEFT JOIN list2 ON tbl1.num1 = list2.pk2
 * 
 */
class LResultSet : public LLightResultSet
{
public:
    LResultSet();
    LResultSet(LConnection * newConnection);
    /**
     * All controls are unregistered in the destructor.
     */
    virtual ~LResultSet();

    /**
     * 
     * @param newTableName : the database table to update.
     */
    void SetTableName(const wxString& newTableName)
    {
        m_tblName = newTableName;
    }

    const wxString& GetTableName() const
    {
        return m_tblName;
    }

    /**
     * May not necessarily be the real primary key of the table.
     * 
     * @param newPKColumnName : an auto-increment column.
     */
    void SetPKColumnName(const wxString& newPKColumnName)
    {
        m_pkColName = newPKColumnName;
    }

    const wxString& GetPKColumnName() const
    {
        return m_pkColName;
    }
    void RegisterControl(LBoundControl * comp);
    /**
     * Unregisters a control and set its internal resultset member to NULL.
     * @param comp
     */
    void UnRegisterControl(LBoundControl * comp);
    virtual void UnRegisterAllControls();

    const wxArrayPtrVoid GetRegisteredControls() const
    {
        return m_BoundControls;
    }

    bool IsControlRegistered(LBoundControl * comp) const
    {
        return (m_BoundControls.Index(comp) != wxNOT_FOUND);
    }
    virtual void Display() = 0;
    /**
     * 
     * @return true if any registered control is dirty, false otherwise.
     */
    bool IsDirty() const;
    virtual bool Save() = 0;
    bool Refresh();
    bool Cancel();
    bool AddNew();
    bool Delete();
    LBoundControl* GetControl(const wxString& colName) const;

    void SetCanSave(bool newCanSave)
    {
        m_canSave = newCanSave;
    }

    bool GetCanSave() const
    {
        return m_canSave;
    }

    void SetCanInsert(bool newCanInsert)
    {
        m_canInsert = newCanInsert;
    }

    bool GetCanInsert() const
    {
        return m_canInsert;
    }

    void SetCanUpdate(bool newCanUpdate)
    {
        m_canUpdate = newCanUpdate;
    }

    bool GetCanUpdate() const
    {
        return m_canUpdate;
    }

    void SetCanDelete(bool newCanDelete)
    {
        m_canDelete = newCanDelete;
    }

    bool GetCanDelete() const
    {
        return m_canDelete;
    }

    void SetReadOnly(bool newReadOnly)
    {
        m_readOnly = newReadOnly;
    }

    bool IsReadOnly() const
    {
        return m_readOnly;
    }

    void SetCanMove(bool newCanMove)
    {
        m_canMove = newCanMove;
    }

    bool GetCanMove() const
    {
        return m_canMove;
    }

    bool IsInserting() const
    {
        return m_inserting;
    }

    void SetMasterResultSet(LResultSet * newResultSet)
    {
        m_masterResultSet = newResultSet;
    }

    const LResultSet* GetMasterResultSet() const
    {
        return m_masterResultSet;
    }

    void SetMasterColumnNames(const wxString& newColumnNames)
    {
        m_masterColumnNames = newColumnNames;
    }

    const wxString& GetMasterColumnNames() const
    {
        return m_masterColumnNames;
    }

    void SetChildColumnNames(const wxString& newColumnNames)
    {
        m_childColumnNames = newColumnNames;
    }

    const wxString& GetChildColumnNames() const
    {
        return m_childColumnNames;
    }
    virtual void InformInserting() const = 0;
    virtual void InformDirty() const = 0;

    void RegisterEventHandler(LResultSetEvent * evh);
    void UnRegisterEventHandler(LResultSetEvent * evh);

    wxArrayPtrVoid& GetEventHandlers()
    {
        return m_evtHandlers;
    }

protected:
    wxString m_tblName;
    wxString m_pkColName;
    wxArrayPtrVoid m_BoundControls;
    virtual const wxString MakeUpdateSQL() const = 0;
    virtual const wxString MakeDeleteSQL() const = 0;
    virtual const int GetRowByPKValue(const wxAny& pkValue) const = 0;
    bool m_inserting;
    bool m_canSave;
    bool m_canInsert;
    bool m_canUpdate;
    bool m_canDelete;
    bool m_readOnly;
    bool m_canMove;

    wxArrayPtrVoid m_evtHandlers;
    void InformInitialised();
    void InformDataChanged();
    void InformBeforeSave();
    void InformBeforeInsert();
    void InformBeforeUpdate();
    void InformBeforeDelete();
    void InformBeforeChangeRow();
    void InformAfterSave();
    void InformAfterChangeRow();
    void InformAfterAction(const int action);
    void InformAfterInsert(const wxAny& newPK, const wxString& sql);
    void InformAfterUpdate(const wxAny& curPK, const wxString& sql);
    void InformAfterDelete(const wxAny& oldPK, const wxString& sql);
    void InformLibMessage(const LInformation& msg) const;

    wxWeakRef<LResultSet> m_masterResultSet;
    wxString m_masterColumnNames;
    wxString m_childColumnNames;
    bool IsChild() const;
    wxString MakeChildInsertColList() const;
    wxString MakeMasterValueList() const;
private:

};

////////////////////////////////////////////////////////////////////////

class LResultSetEvent
{
public:
    LResultSetEvent();
    virtual ~LResultSetEvent();

    enum ACTIONS
    {
        ACTION_ADDNEW = 0, ACTION_SAVE, ACTION_CANCEL, ACTION_REFRESH, ACTION_DELETE
    };
    virtual void Initialised(LResultSet * caller);
    virtual void DataChanged(LResultSet * caller);
    virtual void BeforeSave(LResultSet * caller);
    virtual void BeforeInsert(LResultSet * caller);
    virtual void BeforeUpdate(LResultSet * caller);
    virtual void BeforeDelete(LResultSet * caller);
    virtual void BeforeChangeRow(LResultSet * caller);
    virtual void AfterSave(LResultSet * caller);
    virtual void AfterInsert(LResultSet * caller, const wxAny& newPK, const wxString& sql);
    virtual void AfterUpdate(LResultSet * caller, const wxAny& curPK, const wxString& sql);
    virtual void AfterDelete(LResultSet * caller, const wxAny& oldPK, const wxString& sql);
    virtual void AfterChangeRow(LResultSet * caller);
    virtual void AfterAction(LResultSet * caller, ACTIONS action);
    virtual void Inform(const LResultSet * caller, const LInformation& msg) const;

private:

};


#endif /* LRESULTSET_H */

