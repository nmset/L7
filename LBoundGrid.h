/* 
 * File:   LBoundGrid.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 16:40
 */

#ifndef LBOUNDGRID_H
#define LBOUNDGRID_H

#include <wx/grid.h>
#include "LResultSet.h"
#include "LGridColEditor.h"

class LBoundGridFormEvent;

/**
 * Displays and edits database tables with custom renderers and editors.
 * A custom menu controls data editing.
 * Can edit a row in form view; application can add widgets on the form dialog.
 * Display on phones or tablets with high DPI is optimized.
 */
class LBoundGrid : public wxGrid
{
public:
    /**
     * The constructor forces selection mode to wxGridSelectRows,
     * this is a requirement, don't change it in your code.
     * 
     * @param parent
     * @param id
     */
    LBoundGrid(wxWindow* parent, wxWindowID id = wxID_ANY);
    /**
     * Deletes the menu, the form client sizer and calls ClearGrid().
     */
    virtual ~LBoundGrid();

    void SetResultSet(LResultSet * newResultSet);

    LResultSet* GetResultSet() const
    {
        return m_rs;
    }
    /**
     * Is destructive. All rows and columns are deleted. Column editors get destroyed.
     */
    void ClearGrid();
    /**
     * Deletes all rows, preserves columns, fills the underlying wxStringTable with raw database data.
     */
    void FillGrid();
    /**
     * The specified column will be edited with LGridCheckEditor and rendered with LGridCheckRenderer.
     * @param newColName database table column name
     * @param newLabel grid header label
     * @param width column width
     * @param readOnly if true, editing is not allowed
     * @param isDualstate edit/render as dual or tristate
     * @param newNullLabel if the underlying check box is tristate with undetermined state, the cell displays this value
     */
    void CreateCheckBoxColumn(const wxString& newColName,
                              const wxString& newLabel,
                              unsigned int width,
                              bool readOnly = false,
                              bool isDualstate = false,
                              wxString newNullLabel = _T("?"));
    /**
     * The specified column will be edited with LGridComboEditor and rendered with LGridComboRenderer.
     * @param newColName database table column name
     * @param newLabel grid header label
     * @param width column width
     * @param readOnly if true, editing is not allowed
     */
    void CreateComboBoxColumn(const wxString& newColName,
                              const wxString& newLabel,
                              unsigned int width,
                              bool readOnly = false);
    /**
     * The specified column will be edited with LGridDateEditor and rendered with LGridDateRenderer.
     * @param newColName database table column name
     * @param newLabel grid header label
     * @param width column width
     * @param readOnly if true, editing is not allowed
     */
    void CreateDateColumn(const wxString& newColName,
                          const wxString& newLabel,
                          unsigned int width,
                          bool readOnly = false);
    /**
     * The specified column will be edited with LGridTextEditor and rendered with LGridTextRenderer.
     * @param newColName database table column name
     * @param newLabel grid header label
     * @param width column width
     * @param newMultiline use a single or multiline text editor
     * @param readOnly if true, editing is not allowed
     */
    void CreateTextColumn(const wxString& newColName,
                          const wxString& newLabel,
                          unsigned int width,
                          bool newMultiline = false,
                          bool readOnly = false);
    /**
     * The specified column will be edited with LGridSpinEditor and rendered with LGridSpinRenderer.
     * @param newColName database table column name
     * @param newLabel grid header label
     * @param width column width
     * @param newMin minimal value of the editor
     * @param newMax maximal value of the editor
     * @param newInitial initial value of the editor
     * @param readOnly if true, editing is not allowed
     */
    void CreateSpinColumn(const wxString& newColName,
                          const wxString& newLabel,
                          unsigned int width,
                          int newMin = 0,
                          int newMax = 100,
                          int newInitial = 0,
                          bool readOnly = false);
    void CreateJsonGridColumn(const wxString& newColName,
                              const wxString& newLabel,
                              unsigned int width,
                              const wxString& intentLabel,
                              const wxArrayString& types,
                              wxSize popupSize = wxDefaultSize,
                              bool readOnly = false);
    void CreateXmlGridColumn(const wxString& newColName,
                             const wxString& newLabel,
                             unsigned int width,
                             const wxString& intentLabel,
                             const wxArrayString& types,
                             wxSize popupSize = wxDefaultSize,
                             bool readOnly = false);
    /**
     * 
     * @param col the grid column index
     * @return the database table column name if above Create???Column has been called on that column, wxEmpyString otherwise.
     */
    const wxString GetColName(const unsigned int col);
    /**
     * 
     * @param colName database table column name
     * @return the grid column index of the first column defined with above Create???Column and whose
     * columnName property equals colName. If no column is found, -1 is returned.
     */
    int GetColIndex(const wxString& colName);
    /**
     * Has to call BeginEdit on the cell at row and col. This creates the editor if necessary,
     * which in turn registers it in the underlying resultset.
     * @param row
     * @param col
     * @param keepRegistered if true, does not unregister the editor in the underlying resultset
     * @return The editor typed as LBoundControl* if found, NULL otherwise.
     */
    LBoundControl* GetBoundControl(const unsigned int row,
                                   const unsigned int col,
                                   bool keepRegistered = true);
    /**
     * Has to call BeginEdit on the cell at row and colName. This creates the editor if necessary,
     * which in turn registers it in the underlying resultset.
     * @param row
     * @param colName database table column name
     * @param keepRegistered if true, does not unregister the editor in the underlying resultset
     * @return The editor typed as LBoundControl* if found, NULL otherwise.
     */
    LBoundControl* GetBoundControl(const unsigned int row,
                                   const wxString& colName,
                                   bool keepRegistered = true);
    /**
     * 
     * @param row
     * @param col
     * @return the cell editor at row and col as LGridColEditor* .
     */
    LGridColEditor * GetColEditor(const unsigned int row, const unsigned int col) const;
    /**
     * 
     * @param col grid column index
     * @return the form editor if in form view, NULL otherwise.
     */
    wxControl* GetFormEditor(const unsigned int col) const;
    /**
     * 
     * @param colName database table column name
     * @return the form editor if in form view, NULL otherwise.
     */
    wxControl* GetFormEditor(const wxString& colName);
    /**
     * Calls BeginEdit on the cell at row and colName. This creates the editor if necessary,
     * which in turn registers it in the underlying resultset.
     * @param row
     * @param col
     * @param keepRegistered if true, does not unregister the editor in the underlying resultset
     * @return 
     */
    bool InitEditor(const unsigned int row,
                    const unsigned int col,
                    bool keepRegistered = true);
    void InitAllEditors(const unsigned int row, bool keepRegistered = true);

    /**
     * An application can register a single form event handler here.
     * @param newEVH
     */
    void SetGridFormEventHandler(LBoundGridFormEvent * newEVH)
    {
        m_gridFormEVH = newEVH;
    }

    LBoundGridFormEvent* GetGridFormEventHandler()
    {
        return m_gridFormEVH;
    }

    void AddMenuSeparator();
    void AddMenuItem(wxMenuItem * newItem);
    void AddSubMenu(wxMenu * newSubMenu, const wxString &label);
    void RemoveMenuItem(const wxString& newItemLabelText);
    void RemoveAllExternalMenuItems();
    void EnableMenuItem(int id, bool enable = true);

    wxMenu* GetMenu() const
    {
        return m_menu;
    }
    // Overrides to optimize display on phones and tablets with high DPI. 
    void SetDefaultRowSize(int height, bool resizeExistingRows = false);
    void SetColMinimalAcceptableWidth(int width);
    void SetColMinimalWidth(int col, int width);
    void SetColSize(int col, int width);
    void SetDefaultColSize(int width, bool resizeExistingCols = false);
    void SetRowLabelSize(int width);
    void SetRowMinimalAcceptableHeight(int height);
    void SetRowMinimalHeight(int row, int height);
    void SetRowSize(int row, int height);
private:

    enum mnuIDs
    {
        ID_MNU_SAVE = -50000, ID_MNU_DELETE, ID_MNU_CANCEL, ID_MNU_REFRESH, ID_MNU_FORMVIEW
    };
    wxWeakRef<LResultSet> m_rs;
    /**
     * For phones and tablets with high DPI.
     */
    wxDouble m_scaleFactor;
    /**
     * All database table date is stored as string.
     */
    wxGridStringTable * m_stringTable;
    wxMenu * m_menu;
    /**
     * This wxBoxSizer is created on the wxDialog when the grid is in form view,
     * and is passed to the application via an instance of LBoundGridFormEvent if set.
     */
    wxBoxSizer * m_formClientSizer;
    /**
     * Client application can set a single instance of LBoundGridFormEvent to receive 
     * events in form view.
     */
    LBoundGridFormEvent * m_gridFormEVH;
    /**
     * Only one single line must be selected.
     * @param evt
     */
    void ForceSingleLineRange(wxGridRangeSelectEvent& evt);
    /**
     * Synchronizes the grid and the resultset row positions.
     * @param evt
     */
    void CellSelected(wxGridEvent& evt);
    /**
     * Called in constructor.
     */
    void CreateMenu();
    /**
     * On right click.
     * @param evt
     */
    void ShowMenu(wxGridEvent& evt);
    /**
     * Saves, cancels edits or deletes current row, or shows form view.
     * @param evt
     */
    void MenuAction(wxCommandEvent& evt);
    /**
     * Helper function to Save() and Delete(). Concerns the column declared as primary key.
     * @param row
     * @return 
     */
    bool InitPKEditor(const int row);
    /**
     * Edits the current row in a wxDialog.
     */
    void ShowFormView();
    /**
     * Unregisters all controls in the resultset and set them back to null.
     */
    void RestoreEditorControls();
    /**
     * Calls Reset() on the LGridColEditor at col, i.e., deletes the editor control.
     * @param col grid column index
     */
    void ResetColEditor(const int col);

};

class LBoundGridFormEvent
{
public:
    LBoundGridFormEvent();
    virtual ~LBoundGridFormEvent();
    /**
     * 
     * @param origin
     * @param clientSizer a wxBoxSizer at the bottom of the form window.
     */
    virtual void BeforeFormShown(const LBoundGrid * origin, wxBoxSizer * clientSizer);
    /**
     * 
     * @param origin
     * @param clientSizer a wxBoxSizer at the bottom of the form window.
     */
    virtual void AfterFormHidden(const LBoundGrid * origin, wxBoxSizer * clientSizer);
    virtual void AfterFormDestroyed(const LBoundGrid * origin);

private:

};

#endif /* LBOUNDGRID_H */

