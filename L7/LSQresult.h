/* 
 * File:   LSQresult.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 31 mai 2014, 18:56
 */

#ifdef USE_LIBSQ
#ifndef LSQRESULT_H
#define LSQRESULT_H
/**
 * SQLite namespace.
 * 
 * Please note USE_LIBSQ pre-processor directive must be defined to include the SQLite backend.
 */
namespace SQ
{

class SQresult
{
    /**
     * This class is used  to reference the data obtained by LSQConnection::ExecuteSQL.
     * 
     * If no rows are returned, m_data is NULL and does not even provide the column names.
     */
public:
    SQresult();
    virtual ~SQresult();
    /**
     * Pointer to the array of column names and data.
     */
    char ** m_data;
    /**
     * Number of rows.
     */
    int m_nbRows;
    /**
     * Number of columns
     */
    int m_nbCols;
private:

};

}
#endif /* LSQRESULT_H */
#endif
