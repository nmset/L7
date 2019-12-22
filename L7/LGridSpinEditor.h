/* 
 * File:   LGridSpinEditor.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 23 septembre 2014, 18:01
 */

#ifndef LGRIDSPINEDITOR_H
#define LGRIDSPINEDITOR_H

#include "LGridColEditor.h"
#include "LBoundSpinCtrl.h"

/**
 * Edits table data using an LBoundSpinCtrl.
 */
class LGridSpinEditor : public wxGridCellEditor, public LGridColEditor
{
public:
    /**
     * 
     * @param newColName
     * @param newMin
     * @param newMax
     * @param newInitial
     */
    LGridSpinEditor(const wxString& newColName,
                    int newMin = 0,
                    int newMax = 100,
                    int newInitial = 0);
    /**
     * The editor gets deleted.
     */
    virtual ~LGridSpinEditor();

    /**
     * Creates m_control as LBoundSpinCtrl.
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
     * Applies the editor value to the grid cell.
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
     * @return GetData(), or the initial value if the editor control has not been created.
     */
    wxString GetValue() const;
    /**
     * Creates a wxSpinCtrl to be used as editor in form view.
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
    wxSpinCtrl * m_formEditor;
    /**
     * Fully typed alias to m_control.
     */
    LBoundSpinCtrl * m_BoundSpinCtrl;
    int m_min, m_max, m_initial;
};

#endif /* LGRIDSPINEDITOR_H */

