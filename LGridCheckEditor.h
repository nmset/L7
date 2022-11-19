/* 
 * File:   LGridCheckEditor.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 20:04
 */

#ifndef LGRIDCHECKEDITOR_H
#define LGRIDCHECKEDITOR_H

#include "LBoundCheckBox.h"
#include "LGridColEditor.h"

/**
 * Edits table data using an LBoundCheckBox.
 */
class LGridCheckEditor : public wxGridCellEditor, public LGridColEditor
{
public:
    /**
     * N.B. newNullLabel must be the same in LGridCheckEditor and LGridCheckRenderer
     * @param newColName database column name
     * @param isDualState use a dual or triple state checkbox.
     * @param newNullLabel cell value to interprete as undetermined for a tristate checkbox..
     */
    LGridCheckEditor(const wxString& newColName,
                     bool isDualState = false,
                     const wxString& newNullLabel = _T("?"));
    /**
     * The editor gets deleted.
     */
    virtual ~LGridCheckEditor();
    /**
     * Creates m_control as LBoundCheckBox.
     * @param parent
     * @param id
     * @param evtHandler
     */
    void Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler);
    /**
     * Creates m_control if necessary. Registers the editor in the grid's resultset.
     * 
     * @param row
     * @param col
     * @param grid
     */
    void BeginEdit(int row, int col, wxGrid *grid);
    wxGridCellEditor* Clone() const;
    bool EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval);
    /**
     * Applies the editor value as returned by GetData() to the grid cell.
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
     * Creates a wxCheckBox to be used as editor in form view.
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
    bool m_triState;
    unsigned long m_style;
    wxCheckBox * m_formEditor;
    wxString m_nullLabel;
    /**
     * Fully typed alias to m_control.
     */
    LBoundCheckBox * m_BoundCheckBox;
};

#endif /* LGRIDCHECKEDITOR_H */

