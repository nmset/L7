/* 
 * File:   LGridXmlCellRenderer.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on December 15, 2019, 6:18 PM
 */

#ifndef LGRIDXMLCELLRENDERER_H
#define LGRIDXMLCELLRENDERER_H

#include "wx/grid.h"

/**
 * A grid cell renderer for xml stored data.
 * It show the selected Intent value, or INVALID_INTENT.
 */
class LGridXmlCellRenderer : public wxGridCellStringRenderer
{
public:
    LGridXmlCellRenderer();
    virtual ~LGridXmlCellRenderer();

    void Draw(wxGrid & grid,
              wxGridCellAttr & attr,
              wxDC & dc,
              const wxRect & rect,
              int row,
              int col,
              bool isSelected);
private:

    const wxString ProcessXmlValue(const wxString& cellValue);

};

#endif /* LGRIDXMLCELLRENDERER_H */

