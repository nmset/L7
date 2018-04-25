/* 
 * File:   LNavigator.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 4 juin 2014, 14:56
 */

#ifndef LNAVIGATOR_H
#define	LNAVIGATOR_H

#include <wx/wx.h>
#include "LResultSet.h"

class NavigatorRsEVH;

/**
 * This combined control displays an action bar and a navigation bar.
 * These contains standard buttons to interact with a resultset.
 * 
 * The action bar can 
 * move the resultset to the insert row,
 * save any edits,
 * cancel any edits,
 * refresh the resultset,
 * delete the current record.
 * 
 * The navigation bar is used to move the resultset's cursor
 * and displays the current position.
 */
class LNavigator : public wxPanel
{
public:
    LNavigator(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~LNavigator();

    void SetResultSet(LResultSet * newResultSet);
    LResultSet* GetResultSet() const;
    void ShowActionBar(bool show) const;
    void ShowNavigationBar(bool show) const;
    void ShowButtonAddNew(bool show) const;
    void ShowButtonSave(bool show) const;
    void ShowButtonCancel(bool show) const;
    void ShowButtonRefresh(bool show) const;
    void ShowButtonDelete(bool show) const;

private:
    wxBoxSizer* m_vsz;
    wxBoxSizer* m_hsz1;
    wxButton* m_btnNew;
    wxButton* m_btnSave;
    wxButton* m_btnCancel;
    wxButton* m_btnRefresh;
    wxButton* m_btnDelete;
    wxBoxSizer* m_hsz2;
    wxButton* m_btnFirst;
    wxButton* m_btnPrevious;
    wxStaticText* m_lbl;
    wxButton* m_btnNext;
    wxButton* btnLast;
    wxWeakRef<LResultSet> m_rs;
    NavigatorRsEVH * m_evh;

    void OnButtonNew(wxCommandEvent& evt);
    void OnButtonSave(wxCommandEvent& evt);
    void OnButtonCancel(wxCommandEvent& evt);
    void OnButtonRefresh(wxCommandEvent& evt);
    void OnButtonDelete(wxCommandEvent& evt);
    void OnBtnFirst(wxCommandEvent& evt);
    void OnBtnPrevious(wxCommandEvent& evt);
    void OnBtnNext(wxCommandEvent& evt);
    void OnBtnLast(wxCommandEvent& evt);
};

//////////////////////////////////////////////////////////////////////////////
/**
 * A private resultset event handler for LNavigator. All functions are declared private.
 * Don't use it.
 */
class NavigatorRsEVH : public LResultSetEvent
{
    friend class LNavigator;
private:
    NavigatorRsEVH(wxStaticText * newLbl);
    virtual ~NavigatorRsEVH();
    void Initialised(LResultSet* caller);
    void AfterChangeRow(LResultSet* caller);
    void AfterAction(LResultSet * caller, ACTIONS action);
    wxStaticText * m_lbl;
    void UpdateLocation(const LResultSet* caller);
};

#endif	/* LNAVIGATOR_H */

