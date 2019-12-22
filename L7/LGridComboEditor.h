/* 
 * File:   LGridComboEditor.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 12 juin 2014, 18:33
 */

#ifndef LGRIDCOMBOEDITOR_H
#define LGRIDCOMBOEDITOR_H

#include "LGridColEditor.h"
#include "LBoundComboBox.h"

/**
 * Edits table data using an LBoundComboBox.
 */
class LGridComboEditor : public wxGridCellEditor, public LGridColEditor
{
public:
    /**
     * 
     * @param newColName database column name
     */
    LGridComboEditor(const wxString& newColName);
    /**
     * The editor gets deleted.
     */
    virtual ~LGridComboEditor();
    /**
     * Creates m_control as LBoundComboBox.
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
     * Applies the editor value as returned by GetStringSelection() to the grid cell.
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
     * Creates a wxComboBox to be used as editor in form view.
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
    wxComboBox * m_formEditor;
    /**
     * Fully typed alias to m_control.
     */
    LBoundComboBox * m_BoundComboBox;
};

#endif /* LGRIDCOMBOEDITOR_H */

