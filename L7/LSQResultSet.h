/* 
 * File:   LSQResultSet.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 1 juin 2014, 11:42
 */
#ifdef USE_LIBSQ
#ifndef LSQRESULTSET_H
#define	LSQRESULTSET_H

#include "LResultSet.h"
#include <sqlite3.h>
#include "LSQresult.h"
/**
 * SQLite namespace.
 * 
 * Please note USE_LIBSQ pre-processor directive must be defined to include the SQLite backend.
 */
namespace SQ
{
#define RSSQMC wxString(_T("RSSQM"))
#define RSSQ000C wxString(_T("RS000"))
#define RSSQ000M wxString(_("Result set is NULL."))
#define RSSQ001C wxString(_T("RS001"))
#define RSSQ001M wxString(_("Insert started\nPlease SAVE or CANCEL first"))
#define RSSQ002C wxString(_T("RS002"))
#define RSSQ002M wxString(_("Unknown error"))
#define RSSQ003C wxString(_T("RS003"))
#define RSSQ003M wxString(_("Data altered\nPlease SAVE or CANCEL first"))
#define RSSQ004C wxString(_T("RS004"))
#define RSSQ004M wxString(_("Invalid column name :\n"))

    /**
     * Scrollable resultset for the SQLite backend.
     * 
     * This class allows data modification through GUI elements.
     * The underlying SQL query must not contain table aliases.
     * 
     * A column alias must still be a real column name of the table specified in SetTableName.
     * 
     * Ex : SELECT tbl1.pk1, tbl1.text1, list2.text2 AS num1 FROM tbl1 LEFT JOIN list2 ON tbl1.num1 = list2.pk2
     */
    class LSQResultSet : public LResultSet
    {
    public:
        LSQResultSet();
        LSQResultSet(LConnection * newConnection);
        virtual ~LSQResultSet();
        /**
         * Updates and runs the SQL string.
         * @param newSql
         * @return 
         */
        bool SetSQL(const wxString& newSql);
        bool HasData() const;
        bool Absolute(const unsigned int newRowIndex);
        bool IsFirst() const;
        bool IsLast() const;
        bool First();
        bool Next();
        bool Previous();
        bool Last();
        const unsigned int GetRowCount() const;
        const unsigned int GetColumnCount() const;
        const wxString GetColumnName(const unsigned int colIndex) const;
        const int GetColumnIndex(const wxString& colName) const;
        const wxAny GetData(const unsigned int rowIdx, const unsigned int colIdx) const;
        /**
         * At current row.
         * @param colName
         * @return database table value or wxEmptyString.
         */
        const wxAny GetData(const wxString& colName) const;
        /**
         * Updates registered controls on screen.
         */
        void Display();
        bool Save();

        inline void InformInserting() const
        {
            InformLibMessage(LInformation(RSSQ001C, RSSQ001M));
        }

        inline void InformDirty() const
        {
            InformLibMessage(LInformation(RSSQ003C, RSSQ003M));
        };

        /**
         * Replaces a single quote ' by two single quotes '' .
         * @param literal
         * @return the number of single quotes replaced
         */
        static int EscapeChar(wxString& literal)
        {
            return literal.Replace(_T("'"), _T("''"));
        }

    private:
        bool RunSQL();
        const wxString MakeUpdateSQL() const;
        const wxString MakeDeleteSQL() const;
        /**
         * Locates a row by the primary key value and returns its index.
         * @param pkValue
         * @return row index or -1 if not found.
         */
        const int GetRowByPKValue(const wxAny& pkValue) const;

        wxArrayString m_colNames;
        bool RetrieveColNames(SQresult * emptyResult);
    };

}
#endif	/* LSQRESULTSET_H */
#endif
