/* 
 * File:   L7Test.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * Copyright Saleem Edah-Tally - © 2014
 *
 * Created on 4 juin 2014, 20:42
 */

#ifndef L7TEST_H
#define	L7TEST_H

#include <wx/wx.h>
#include <LConnection.h>
#include <LResultSet.h>
#include <LBoundTextCtrl.h>
#include <LBoundCheckBox.h>
#include <LBoundComboBox.h>
#include <LBoundDatePickerCtrl.h>
#include <LBoundSpinCtrl.h>
#include <LNavigator.h>
#include <LBoundGrid.h>
#include <special/LBoundJsonGridPicker.h>
#include <special/LBoundXmlGridPicker.h>

class L7Test;
class L7Frame;

class L7Test : public wxApp
{
public:
    L7Test();
    virtual ~L7Test();
    
    bool OnInit();
    int OnExit();
    void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg) override;


private:
    wxLocale m_locale;
};

class L7Frame : public wxFrame
{
    friend class ConnectionEVH;
    friend class ResultSetTbl1EVH;
    friend class ResultSetTbl2EVH;
    friend class GridEVH;
public:
    L7Frame(wxWindow * parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString);
    virtual ~L7Frame();
private:
    LConnection * m_conn;
    wxTextCtrl * m_txtConnInfo;
    LResultSet * m_rsTbl1;
    LResultSet * rsTbl2;
    wxRadioBox * rdBE;
    wxTextCtrl * txtMessages;
    LBoundTextCtrl * Pk1;
    LBoundTextCtrl * Text1;
    LBoundCheckBox * Check1d; // Dual state
    LBoundCheckBox * Check1t; // Tri state
    LBoundComboBox * Combo1t; // Translated
    LBoundComboBox * Combo1x; // Text
    LBoundDatePickerCtrl * Date1;
    LBoundSpinCtrl * Spin1;
    LNavigator * navTbl1;
    LBoundGrid * grid;
    LBoundJsonGridPicker * Json1;
    LBoundXmlGridPicker * Xml1;
    bool colsCreated;
    
    void AppendMsg(const wxString& msg);
    void MsgBox(const wxString& msg);
    void OnRadioConnClick(wxCommandEvent& evt);
    void OnButtonConnectClick(wxCommandEvent& evt);
    void OnButtonNewFrameClick(wxCommandEvent& evt);
    void FillComboBox(LBoundComboBox * cmb, const wxString& sql);
    void FilterGrid();
};

class ConnectionEVH : public LConnectionEvent {
public:
    ConnectionEVH(L7Frame * newOwner);
    virtual ~ConnectionEVH();
    
private:
    L7Frame * owner;
    void BeforeExecute(const LConnection * caller);
    void AfterExecute(const LConnection * caller);
    void Inform(const LConnection * caller,
                        const LInformation& msg) const;
};

class ResultSetTbl1EVH : public LResultSetEvent {
public:
    ResultSetTbl1EVH(L7Frame * newOwner);
    virtual ~ResultSetTbl1EVH();
private:
    L7Frame * owner;
    void Inform(const LResultSet* caller, const LInformation& msg) const;
    void AfterAction(LResultSet* caller, ACTIONS action);
    void AfterChangeRow(LResultSet* caller);
    void AfterDelete(LResultSet* caller, const wxAny& oldPK, const wxString& sql);
    void AfterInsert(LResultSet* caller, const wxAny& newPK, const wxString& sql);
    void AfterSave(LResultSet* caller);
    void AfterUpdate(LResultSet* caller, const wxAny& curPK, const wxString& sql);
    void BeforeChangeRow(LResultSet* caller);
    void BeforeDelete(LResultSet* caller);
    void BeforeInsert(LResultSet* caller);
    void BeforeSave(LResultSet* caller);
    void BeforeUpdate(LResultSet* caller);
};

class ResultSetTbl2EVH : public LResultSetEvent {
public:
    ResultSetTbl2EVH(L7Frame * newOwner);
    virtual ~ResultSetTbl2EVH();
private:
    L7Frame * owner;
    void Inform(const LResultSet* caller, const LInformation& msg) const;
    void AfterAction(LResultSet* caller, ACTIONS action);
    void AfterChangeRow(LResultSet* caller);
    void AfterDelete(LResultSet* caller, const wxAny& oldPK, const wxString& sql);
    void AfterInsert(LResultSet* caller, const wxAny& newPK, const wxString& sql);
    void AfterSave(LResultSet* caller);
    void AfterUpdate(LResultSet* caller, const wxAny& curPK, const wxString& sql);
    void BeforeChangeRow(LResultSet* caller);
    void BeforeDelete(LResultSet* caller);
    void BeforeInsert(LResultSet* caller);
    void BeforeSave(LResultSet* caller);
    void BeforeUpdate(LResultSet* caller);
};

class GridEVH : public LBoundGridFormEvent {
public:
    GridEVH(L7Frame * newOwner);
    virtual ~GridEVH();
    
private:
    L7Frame * owner;

    void BeforeFormShown(const LBoundGrid* origin, wxBoxSizer* clientSizer);
    void OnClientButtonClicked(wxCommandEvent& evt);

};
#endif	/* L7TEST_H */

