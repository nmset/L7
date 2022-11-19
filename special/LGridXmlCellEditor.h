/* 
 * File:   LGridXmlCellEditor.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 *
 * Created on December 15, 2019, 6:32 PM
 */

#ifndef LGRIDXMLCELLEDITOR_H
#define LGRIDXMLCELLEDITOR_H

#include "../LGridColEditor.h"
#include "LBoundXmlGridPicker.h"

class XmlGridEditorRsEVH;
class LGridXmlCellEditor;

/**
 * A grid cell editor using LBoundXmlGridPicker
 * @param newColName
 * @param intentLabel
 * @param types
 * @param popupSize
 */
class LGridXmlCellEditor : public wxGridCellEditor, public LGridColEditor
{
    friend class XmlGridEditorRsEVH;
public:
    LGridXmlCellEditor(const wxString& newColName,
                       const wxString& intentLabel,
                       const wxArrayString& types,
                       wxSize popupSize = wxDefaultSize);
    virtual ~LGridXmlCellEditor();
    /**
     * Creates m_control as LBoundJsonGridPicker.
     * @param parent
     * @param id
     * @param evtHandler
     */
    void Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler);
    /**
     * Creates m_control if necessary. Registers the editor in the grid's resultset.
     * @param row
     * @param col
     * @param grid
     */
    void BeginEdit(int row, int col, wxGrid *grid);
    wxGridCellEditor* Clone() const;
    bool EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval);
    /**
     * Applies the editor value as returned by GetValue() to the grid cell.
     * @param row
     * @param col
     * @param grid
     */
    void ApplyEdit(int row, int col, wxGrid *grid);
    /**
     * Deletes the editor, all pointers to the editor are set to NULL.
     */
    void Reset();
    /**
     * 
     * @return GetData(), or wxEmptyString if the editor control has not been created.
     */
    wxString GetValue() const;
    /**
     * Creates a wxTextCtrl to be used as editor in form view.
     */
    wxControl* ProvideFormEditor(wxWindow * parent);

    wxControl* GetFormEditor() const
    {
        return m_formEditor;
    }
    /**
     * Updates the grid cell and the editor. m_formEditor is deleted and set to NULL.
     * @param row
     * @param col
     * @param grid
     */
    void SyncBack(const int row, const int col, wxGrid * grid);

private:
    wxString m_intentLabel;
    wxArrayString m_choices;
    wxSize m_popupSize;
    XmlGridPickerCtrl * m_formEditor;
    wxAny m_xmlBuffer;
    int m_editRow;
    /**
     * Fully typed alias to m_control.
     */
    LBoundXmlGridPicker * m_BoundXmlGridPicker;
    wxWeakRef<XmlGridEditorRsEVH> m_rsEVH;
};

/**
 * Clears LGridXmlGridEditor::m_jsonBuffer on LBoundGrid::Cancel.
 * Must be trackable. If it is deleted by an application, avoid crash in 
 * destructor of LGridXmlGridEditor.
 */
class XmlGridEditorRsEVH : public LResultSetEvent, public wxTrackable
{
    friend class LGridXmlCellEditor;
public:

private:
    XmlGridEditorRsEVH(LGridXmlCellEditor * owner);
    virtual ~XmlGridEditorRsEVH();
    virtual void AfterAction(LResultSet * caller, ACTIONS action);
    virtual void DataChanged(LResultSet* caller);

    void SetResultSet(LResultSet * caller)
    {
        m_rs = caller;
    }

    LResultSet * m_rs;
    LGridXmlCellEditor * m_owner;
};

#endif /* LGRIDXMLCELLEDITOR_H */

