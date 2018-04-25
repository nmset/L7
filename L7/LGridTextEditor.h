/* 
 * File:   LGridTextEditor.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 16:08
 */

#ifndef LGRIDTEXTEDITOR_H
#define	LGRIDTEXTEDITOR_H

#include "LGridColEditor.h"
#include "LBoundTextCtrl.h"

/**
 * Edits table data using an LBoundTextCtrl.
 */
class LGridTextEditor : public wxGridCellAutoWrapStringEditor, public LGridColEditor
{
public:
    /**
     * 
     * @param newColName database column name
     * @param newMultiline if true, use wxTE_MULTILINE
     */
    LGridTextEditor(const wxString& newColName, bool newMultiline = false);
    /**
     * The editor gets deleted.
     */
    virtual ~LGridTextEditor();
    /**
     * Creates m_control as LBoundTextCtrl.
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
    bool m_multiline;
    unsigned long m_style;
    wxTextCtrl * m_formEditor;
    /**
     * Fully typed alias to m_control.
     */
    LBoundTextCtrl * m_BoundTextCtrl;
};

#endif	/* LGRIDTEXTEDITOR_H */

