/* 
 * File:   L7Test.cpp
 * Author: Saleem EDAH-TALLY - nmset@netcourrier.com
 * Copyright Saleem EDAH-TALLY - © 2014
 * 
 * Created on 4 juin 2014, 20:42
 */

#include "L7Test.h"
#include <LPQConnection.h>
#include <LPQResultSet.h>
#include <LSQConnection.h>
#include <LSQResultSet.h>
#include <LLightPQResultSet.h>
#include <LLightSQResultSet.h>
#include <LItemData.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

IMPLEMENT_APP(L7Test)

L7Test::L7Test()
{
}

L7Test::~L7Test()
{
}

bool L7Test::OnInit()
{
    L7Frame * main = new L7Frame(NULL, wxID_ANY, _T("L7 Test"));
    SetAppName(_T("L7Test"));
    SetTopWindow(main);
    main->Show();
    return true;
}

int L7Test::OnExit()
{
    return wxApp::OnExit();
}

///////////////////////////////////////////////////////////////////////////////

L7Frame::L7Frame(wxWindow* parent, wxWindowID id, const wxString& title)
: wxFrame(parent, id, title)
{
    m_conn = NULL;
    m_rsTbl1 = NULL;
    rsTbl2 = NULL;
    colsCreated = false;
    wxBoxSizer* szMain = new wxBoxSizer(wxVERTICAL);
    SetSizer(szMain);

    wxPanel * panConn = new wxPanel(this, wxID_ANY);
    szMain->Add(panConn, 0, wxEXPAND, 0);
    wxBoxSizer * szConn = new wxBoxSizer(wxVERTICAL);
    panConn->SetSizer(szConn);
    const wxString b[3]{_T("Choose"), _T("PostgreSQL"), _T("SQLite")};
    rdBE = new wxRadioBox(panConn, wxID_ANY, _T("Backend"), wxDefaultPosition, wxDefaultSize, 3, b);
    szConn->Add(rdBE, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    wxBoxSizer * szDoConn = new wxBoxSizer(wxHORIZONTAL);
    szConn->Add(szDoConn, 0, wxEXPAND);
    m_txtConnInfo = new wxTextCtrl(panConn, wxID_ANY);
    szDoConn->Add(m_txtConnInfo, 1, wxALIGN_CENTRE_VERTICAL);
    wxButton * btnConn = new wxButton(panConn, wxID_ANY, _("Connect"));
    szDoConn->Add(btnConn, 0, wxALIGN_CENTRE_VERTICAL);
    wxButton * btnNewFrame = new wxButton(panConn, wxID_ANY, _T("+"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    btnNewFrame->SetToolTip(_("New window"));
    szDoConn->Add(btnNewFrame, 0, wxALIGN_CENTER_VERTICAL);
    rdBE->Bind(wxEVT_RADIOBOX, &L7Frame::OnRadioConnClick, this);
    btnConn->Bind(wxEVT_BUTTON, &L7Frame::OnButtonConnectClick, this);
    btnNewFrame->Bind(wxEVT_BUTTON, &L7Frame::OnButtonNewFrameClick, this);

    wxPanel * panTbl1 = new wxPanel(this, wxID_ANY);
    szMain->Add(panTbl1, 0, wxEXPAND, 0);
    wxBoxSizer * szTbl1 = new wxBoxSizer(wxVERTICAL);
    panTbl1->SetSizer(szTbl1);
    wxFlexGridSizer * flxszTbl1 = new wxFlexGridSizer(0, 2, 0, 0);
    flxszTbl1->AddGrowableCol(0, 1);
    flxszTbl1->AddGrowableCol(1, 3);
    szTbl1->Add(flxszTbl1, 0, wxEXPAND, 0);
    wxStaticText * lbl = new wxStaticText(panTbl1, wxID_ANY, _T("pk1"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Pk1 = new LBoundTextCtrl(panTbl1, wxID_ANY);
    Pk1->SetSQLQuote(wxEmptyString);
    flxszTbl1->Add(Pk1, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);
    Pk1->SetEditable(false);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("text1"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Text1 = new LBoundTextCtrl(panTbl1, wxID_ANY);
    flxszTbl1->Add(Text1, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("check1d"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Check1d = new LBoundCheckBox(panTbl1, wxID_ANY, wxCHK_2STATE);
    Check1d->SetLabel(wxEmptyString);
    flxszTbl1->Add(Check1d, 0, wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("check1t"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Check1t = new LBoundCheckBox(panTbl1, wxID_ANY, wxCHK_3STATE | wxCHK_ALLOW_3RD_STATE_FOR_USER);
    Check1t->SetLabel(wxEmptyString);
    flxszTbl1->Add(Check1t, 0, wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("combo1t"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Combo1t = new LBoundComboBox(panTbl1, wxID_ANY);
    flxszTbl1->Add(Combo1t, 0, wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("combo1x"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Combo1x = new LBoundComboBox(panTbl1, wxID_ANY);
    Combo1x->SetSQLQuote(_T("'"));
    flxszTbl1->Add(Combo1x, 0, wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("date1"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Date1 = new LBoundDatePickerCtrl(panTbl1, wxID_ANY);
    flxszTbl1->Add(Date1, 0, wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("spin1"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Spin1 = new LBoundSpinCtrl(panTbl1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -10, 25, 3);
    flxszTbl1->Add(Spin1, 0, wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("json1"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    wxArrayString types;
    types.Add(_("Work"));
    types.Add(_("Home"));
    types.Add(_("Other"));
    Json1 = new LBoundJsonGridPicker(panTbl1, wxID_ANY, types);
    flxszTbl1->Add(Json1, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);
    lbl = new wxStaticText(panTbl1, wxID_ANY, _T("xml1"));
    flxszTbl1->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
    Xml1 = new LBoundXmlGridPicker(panTbl1, wxID_ANY, types);
    flxszTbl1->Add(Xml1, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);
    navTbl1 = new LNavigator(panTbl1, wxID_ANY);
    szTbl1->Add(navTbl1, 0, wxEXPAND, 0);

    Json1->SetIntentLabel(_("Email"));
    Xml1->SetIntentLabel(_("Phone"));

    grid = new LBoundGrid(this, wxID_ANY);
    szMain->Add(grid, 1, wxEXPAND, 0);
    grid->SetGridFormEventHandler(new GridEVH(this));

    txtMessages = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    //szMain->Add(txtMessages, 1, wxEXPAND|wxALIGN_BOTTOM, 0);
    szMain->Add(txtMessages, 1, wxEXPAND, 0);
    SetSize(1200, 800);
}

L7Frame::~L7Frame()
{
    delete m_txtConnInfo;
    delete txtMessages;
    delete rdBE;
    delete Pk1;
    delete Text1;
    delete Check1d;
    delete Check1t;
    delete Combo1t;
    delete Combo1x;
    delete Date1;
    delete Spin1;
    delete Json1;
    delete Xml1;
    delete navTbl1; // Has deleted and unregistered it's own event handler.
    if (m_rsTbl1) {
        for (unsigned int i = 0; i < m_rsTbl1->GetEventHandlers().Count(); i++) {
            delete static_cast<LResultSetEvent*> (m_rsTbl1->GetEventHandlers().Item(i));
        }
        delete m_rsTbl1;
    }
    if (rsTbl2) {
        for (unsigned int i = 0; i < rsTbl2->GetEventHandlers().Count(); i++) {
            delete static_cast<LResultSetEvent*> (rsTbl2->GetEventHandlers().Item(i));
        }
        delete rsTbl2;
    }
    if (m_conn) {
        m_conn->Close();
        for (unsigned int i = 0; i < m_conn->GetEventHandlers().Count(); i++) {
            delete static_cast<ConnectionEVH*> (m_conn->GetEventHandlers().Item(i));
        }
        delete m_conn;
    }
    delete grid->GetGridFormEventHandler();
}

void L7Frame::AppendMsg(const wxString& msg)
{
    static int n = 0;
    txtMessages->AppendText(wxAny(n).As<wxString>() + _T(". ") + msg + _T("\n"));
    n++;
}

void L7Frame::MsgBox(const wxString& msg)
{
    wxMessageBox(msg, wxApp::GetInstance()->GetAppName(), wxOK | wxCENTRE, this);
}

void L7Frame::OnRadioConnClick(wxCommandEvent& evt)
{
    wxString connInfo;
    if (evt.GetInt() == 1) {
        connInfo = _T("host=localhost port=5424 dbname=l7 user=l7 password=L7");
    }
    else if (evt.GetInt() == 2) {
        connInfo = wxStandardPaths::Get().GetUserDataDir()
                + wxFileName::GetPathSeparator()
                + _T("L7.db");
    }
    m_txtConnInfo->SetValue(connInfo);
    rdBE->Enable(false);
}

void L7Frame::OnButtonConnectClick(wxCommandEvent& evt)
{
    if (rdBE->GetSelection() == 0) return;
    if (!m_conn) {
        if (rdBE->GetSelection() == 1) {
            m_conn = new PQ::LPQConnection(m_txtConnInfo->GetValue());
            ConnectionEVH * connEVH = new ConnectionEVH(this);
            m_conn->RegisterEventHandler(connEVH);
            if (m_conn->Connect()) {
                m_rsTbl1 = new PQ::LPQResultSet(m_conn);
                rsTbl2 = new PQ::LPQResultSet(m_conn);
            }
        }
        else if (rdBE->GetSelection() == 2) {
            m_conn = new SQ::LSQConnection(m_txtConnInfo->GetValue());
            ConnectionEVH * connEVH = new ConnectionEVH(this);
            m_conn->RegisterEventHandler(connEVH);
            if (wxFileName::Exists(m_conn->GetInfo())) {
                if (m_conn->Connect()) {
                    m_rsTbl1 = new SQ::LSQResultSet(m_conn);
                    rsTbl2 = new SQ::LSQResultSet(m_conn);
                }
            }
        }
        if (m_conn->IsValid()) {
            Pk1->SetColumnName(_T("pk1"));
            Pk1->SetResultSet(m_rsTbl1);
            Text1->SetColumnName(_T("text1"));
            Text1->SetResultSet(m_rsTbl1);
            Check1d->SetColumnName(_T("check1d"));
            Check1d->SetResultSet(m_rsTbl1);
            Check1t->SetColumnName(_T("check1t"));
            Check1t->SetResultSet(m_rsTbl1);
            Combo1t->SetColumnName(_T("combo1t"));
            Combo1t->SetResultSet(m_rsTbl1);
            Combo1x->SetColumnName(_T("combo1x"));
            Combo1x->SetResultSet(m_rsTbl1);
            Date1->SetColumnName(_T("date1"));
            Date1->SetResultSet(m_rsTbl1);
            Spin1->SetColumnName(_T("spin1"));
            Spin1->SetResultSet(m_rsTbl1);
            Json1->SetColumnName(_T("json1"));
            Json1->SetResultSet(m_rsTbl1);
            Xml1->SetColumnName(_T("xml1"));
            Xml1->SetResultSet(m_rsTbl1);
            Pk1->SetSQLQuote(wxEmptyString);
            FillComboBox(Combo1t, _T("SELECT id_list1, item FROM list1 ORDER BY id_list1"));
            FillComboBox(Combo1x, _T("SELECT item FROM list1 ORDER BY id_list1"));
            navTbl1->SetResultSet(m_rsTbl1);
            m_rsTbl1->SetTableName(_T("tbl1"));
            m_rsTbl1->SetPKColumnName(_T("pk1"));
            m_rsTbl1->RegisterEventHandler(new ResultSetTbl1EVH(this));
            const wxString sql = _T("SELECT * FROM tbl1 ORDER BY text1");
            m_rsTbl1->SetSQL(sql); // Must be last, calls LResultSet::Display()
            rsTbl2->SetTableName(_T("tbl2"));
            rsTbl2->SetPKColumnName(_T("PK2"));
            rsTbl2->SetMasterColumnNames(_T("pk1;rand1"));
            rsTbl2->SetChildColumnNames(_T("pk1;rand1"));
            rsTbl2->SetMasterResultSet(m_rsTbl1);
            grid->SetResultSet(rsTbl2);
            rsTbl2->RegisterEventHandler(new ResultSetTbl2EVH(this));
            FilterGrid();
        }
    }
    const wxAny status = m_conn->IsValid();
    AppendMsg(_("Connection status : ") + status.As<wxString>());
    if (!m_conn->IsValid()) {
        wxDELETE(m_rsTbl1);
        wxDELETE(m_conn);
    }
}

void L7Frame::OnButtonNewFrameClick(wxCommandEvent& evt)
{
    L7Frame * newFrame = new L7Frame(NULL, wxID_ANY, GetTitle());
    newFrame->Show();
}

void L7Frame::FillComboBox(LBoundComboBox* cmb, const wxString& sql)
{
    if (!m_conn || !cmb) return;
    LLightResultSet * rs;
    if (rdBE->GetSelection() == 1) {
        rs = new PQ::LLightPQResultSet(m_conn);
    }
    else if (rdBE->GetSelection() == 2) {
        rs = new SQ::LLightSQResultSet(m_conn);
    }
    else {
        return;
    }
    rs->SetSQL(sql);
    if (rs->HasData()) {
        cmb->Clear();
        if (rs->GetColumnCount() > 1) {
            cmb->Append(_("NULL item"), (LItemData*) NULL);
            const wxString dispCol = rs->GetColumnName(1);
            const wxString idCol = rs->GetColumnName(0);
            do {
                cmb->Append(rs->GetData(dispCol).As<wxString>(), new LItemData(rs->GetData(idCol)));
            }
            while (rs->Next());
        }
        else {
            cmb->Append(wxEmptyString);
            const wxString dispCol = rs->GetColumnName(0);
            do {
                cmb->Append(rs->GetData(dispCol).As<wxString>());
            }
            while (rs->Next());
        }
        delete rs;
    }
}

void L7Frame::FilterGrid()
{
    wxString sql = _T("SELECT rand1, PK2, PK1, text2, check2d, check2t, list1.item AS combo2t,"
                      " combo2x, date2, spin2, json2, xml2 FROM tbl2"
                      " LEFT JOIN list1 ON tbl2.combo2t = list1.id_list1"
                      " WHERE tbl2.pk1");
    sql += Pk1->GetValue().IsEmpty() ? _T(" IS NULL ") : _T(" = ") + Pk1->GetValue();
    sql += " ORDER BY tbl2.text2";
    rsTbl2->SetSQL(sql);
    grid->FillGrid();

    if (!colsCreated) {
        grid->SetDefaultRowSize(40, true);
        grid->CreateTextColumn(_T("Pk2"), _T("PK2"), 40, false, true);
        grid->CreateTextColumn(_T("pK1"), _T("PK1"), 40, false, true);
        grid->CreateTextColumn(_T("text2"), _T("Text 2"), 100, true);
        grid->CreateCheckBoxColumn(_T("check2d"), _T("Case 2"), 50, false, true);
        grid->CreateCheckBoxColumn(_T("check2t"), _T("Case 3"), 50, false, false);
        grid->CreateComboBoxColumn(_T("combo2t"), _T("Combo num"), 80);
        grid->CreateComboBoxColumn(_T("combo2x"), _T("Combo txt"), 80);
        grid->CreateDateColumn(_T("date2"), _T("Date"), 80);
        grid->CreateSpinColumn(_T("spin2"), _T("Spin"), 60, -20, 20, 5);
        wxArrayString types;
        types.Add(_("Work"));
        types.Add(_("Home"));
        types.Add(_("Any"));
        grid->CreateJsonGridColumn(_T("json2"), _T("Json 2"), 80, _("Email"), types, wxSize(450, -1));
        grid->CreateXmlGridColumn(_T("xml2"), _T("XML 2"), 80, _("Phone"), types, wxSize(450, -1));
        grid->CreateTextColumn(_T("rand1"), _T("Random"), 80, false, true);

        LBoundComboBox * Combo2t = static_cast<LBoundComboBox*> (grid->GetBoundControl(grid->GetGridCursorRow(), _T("combo2t"), false));
        FillComboBox(Combo2t, _T("SELECT id_list1, item FROM list1 ORDER BY id_list1"));
        LBoundComboBox * Combo2x = static_cast<LBoundComboBox*> (grid->GetBoundControl(grid->GetGridCursorRow(), _T("combo2x"), false));
        Combo2x->SetSQLQuote(_T("'"));
        FillComboBox(Combo2x, _T("SELECT item FROM list1 ORDER BY id_list1"));
        LBoundControl * Pk2 = grid->GetBoundControl(grid->GetGridCursorRow(), _T("pk2"), false);
        Pk2->SetSQLQuote(wxEmptyString);
        LBoundControl * Pk1 = grid->GetBoundControl(grid->GetGridCursorRow(), _T("pk1"), false);
        Pk1->SetSQLQuote(wxEmptyString);
        colsCreated = true;
    }
}

///////////////////////////////////////////////////////////////////////////////

ConnectionEVH::ConnectionEVH(L7Frame* newOwner)
{
    owner = newOwner;
}

ConnectionEVH::~ConnectionEVH()
{

}

void ConnectionEVH::Inform(const LConnection* caller, const LInformation& msg) const
{
    owner->MsgBox(msg.GetFullInformation());
}

void ConnectionEVH::BeforeExecute(const LConnection* caller)
{
    owner->AppendMsg(_T("LConnection::BeforeExecute"));
}

void ConnectionEVH::AfterExecute(const LConnection* caller)
{
    owner->AppendMsg(_T("LConnection::AfterExecute"));
}

///////////////////////////////////////////////////////////////////////////////

ResultSetTbl1EVH::ResultSetTbl1EVH(L7Frame* newOwner)
{
    owner = newOwner;
}

ResultSetTbl1EVH::~ResultSetTbl1EVH()
{

}

void ResultSetTbl1EVH::Inform(const LResultSet* caller, const LInformation& msg) const
{
    owner->MsgBox(msg.GetFullInformation());
}

void ResultSetTbl1EVH::AfterAction(LResultSet* caller, ACTIONS action)
{
    owner->AppendMsg(_T("LResultSet::AfterAction - ") + caller->GetTableName() + _T(" - ") + wxAny((int) action).As<wxString>());
}

void ResultSetTbl1EVH::AfterChangeRow(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::AfterChangeRow - ") + caller->GetTableName());
    owner->FilterGrid();
}

void ResultSetTbl1EVH::AfterDelete(LResultSet* caller, const wxAny& oldPK, const wxString& sql)
{
    owner->AppendMsg(_T("LResultSet::AfterDelete - ") + caller->GetTableName() + _T(" - ") + sql);
}

void ResultSetTbl1EVH::AfterInsert(LResultSet* caller, const wxAny& newPK, const wxString& sql)
{
    owner->AppendMsg(_T("LResultSet::AfterInsert - ") + caller->GetTableName() + _T(" - ") + sql);
}

void ResultSetTbl1EVH::AfterSave(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::AfterSave - ") + caller->GetTableName());
}

void ResultSetTbl1EVH::AfterUpdate(LResultSet* caller, const wxAny& curPK, const wxString& sql)
{
    owner->AppendMsg(_T("LResultSet::AfterUpdate - ") + caller->GetTableName() + _T(" - ") + sql);
}

void ResultSetTbl1EVH::BeforeChangeRow(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeChangeRow - ") + caller->GetTableName());
}

void ResultSetTbl1EVH::BeforeDelete(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeDelete - ") + caller->GetTableName());
    wxMessageDialog * md = new wxMessageDialog(owner, _("Delete ?"), wxApp::GetInstance()->GetAppName(), wxYES_NO);
    int reply = md->ShowModal();
    caller->SetCanDelete(reply == wxID_YES ? true : false);
}

void ResultSetTbl1EVH::BeforeInsert(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeInsert - ") + caller->GetTableName());
    bool valid = !(owner->Text1->IsNull());
    if (!valid) {
        owner->MsgBox(_("Text1 is NULL."));
    }
    caller->SetCanInsert(valid);
}

void ResultSetTbl1EVH::BeforeSave(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeSave - ") + caller->GetTableName());
}

void ResultSetTbl1EVH::BeforeUpdate(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeUpdate - ") + caller->GetTableName());
    bool valid = !(owner->Text1->IsNull());
    if (!valid) {
        owner->MsgBox(_("Text1 is NULL."));
    }
    caller->SetCanUpdate(valid);
}

///////////////////////////////////////////////////////////////////////////////

ResultSetTbl2EVH::ResultSetTbl2EVH(L7Frame* newOwner)
{
    owner = newOwner;
}

ResultSetTbl2EVH::~ResultSetTbl2EVH()
{

}

void ResultSetTbl2EVH::Inform(const LResultSet* caller, const LInformation& msg) const
{
    owner->MsgBox(msg.GetFullInformation());
}

void ResultSetTbl2EVH::AfterAction(LResultSet* caller, ACTIONS action)
{
    owner->AppendMsg(_T("LResultSet::AfterAction - ") + caller->GetTableName() + _T(" - ") + wxAny((int) action).As<wxString>());
}

void ResultSetTbl2EVH::AfterChangeRow(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::AfterChangeRow - ") + caller->GetTableName());
}

void ResultSetTbl2EVH::AfterDelete(LResultSet* caller, const wxAny& oldPK, const wxString& sql)
{
    owner->AppendMsg(_T("LResultSet::AfterDelete - ") + caller->GetTableName() + _T(" - ") + sql);
}

void ResultSetTbl2EVH::AfterInsert(LResultSet* caller, const wxAny& newPK, const wxString& sql)
{
    owner->AppendMsg(_T("LResultSet::AfterInsert - ") + caller->GetTableName() + _T(" - ") + sql);
}

void ResultSetTbl2EVH::AfterSave(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::AfterSave - ") + caller->GetTableName());
}

void ResultSetTbl2EVH::AfterUpdate(LResultSet* caller, const wxAny& curPK, const wxString& sql)
{
    owner->AppendMsg(_T("LResultSet::AfterUpdate - ") + caller->GetTableName() + _T(" - ") + sql);
}

void ResultSetTbl2EVH::BeforeChangeRow(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeChangeRow - ") + caller->GetTableName());
}

void ResultSetTbl2EVH::BeforeDelete(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeDelete - ") + caller->GetTableName());
}

void ResultSetTbl2EVH::BeforeInsert(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeInsert - ") + caller->GetTableName());
}

void ResultSetTbl2EVH::BeforeSave(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeSave - ") + caller->GetTableName());
}

void ResultSetTbl2EVH::BeforeUpdate(LResultSet* caller)
{
    owner->AppendMsg(_T("LResultSet::BeforeUpdate - ") + caller->GetTableName());
}

///////////////////////////////////////////////////////////////////////////////

GridEVH::GridEVH(L7Frame* newOwner)
{
    owner = newOwner;
}

GridEVH::~GridEVH()
{

}

void GridEVH::BeforeFormShown(const LBoundGrid* origin, wxBoxSizer* clientSizer)
{
    wxButton * btnClient = new wxButton(clientSizer->GetContainingWindow(), wxID_ANY, _("Click"));
    clientSizer->Add(btnClient, 1, wxEXPAND);
    btnClient->Bind(wxEVT_BUTTON, &GridEVH::OnClientButtonClicked, this);
}

void GridEVH::OnClientButtonClicked(wxCommandEvent& evt)
{
    wxTextCtrl * txtFormCtrl = static_cast<wxTextCtrl*> (owner->grid->GetFormEditor(_T("text2")));
    owner->MsgBox(txtFormCtrl->GetValue());
}
