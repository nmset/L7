/* 
 * File:   LGridJsonCellRenderer.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 *
 * Created on December 9, 2019, 1:55 PM
 */

#ifndef LGRIDJSONCELLRENDERER_H
#define LGRIDJSONCELLRENDERER_H

#include "wx/grid.h"

/**
 * A grid cell renderer for json stored data.
 * It show the selected Intent value, or INVALID_INTENT.
 */
class LGridJsonCellRenderer : public wxGridCellStringRenderer
{
public:
    LGridJsonCellRenderer();
    virtual ~LGridJsonCellRenderer();

    void Draw(wxGrid & grid,
              wxGridCellAttr & attr,
              wxDC & dc,
              const wxRect & rect,
              int row,
              int col,
              bool isSelected);
private:

    const wxString ProcessJsonValue(const wxString& cellValue);
};

#endif /* LGRIDJSONCELLRENDERER_H */

