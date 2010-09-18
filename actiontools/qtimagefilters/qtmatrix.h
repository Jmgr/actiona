/****************************************************************************
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
** 
** This file is part of a Qt Solutions component.
**
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact Nokia at qt-info@nokia.com.
** 
****************************************************************************/

#ifndef QTMATRIX_H
#define QTMATRIX_H


/**
 * A general, simple matrix class
 */

template <typename T>
class QtMatrix {

public:
    QtMatrix(int rows, int columns)
    {
        m_rows = rows;
        m_cols = columns;
        m_data = (T*)qMalloc(m_rows * m_cols * sizeof(T));
    }
    QtMatrix()
    {
        m_rows = 0;
        m_cols = 0;
        m_data = 0;
    }
    QtMatrix(const T *elements, int rows, int columns)
    {
        m_data = 0;
        set(elements, rows, columns);
    }

    ~QtMatrix()
    {
        if (m_data) qFree(m_data);
    }

    QtMatrix(const QtMatrix &other)
    {
        m_data = 0;
        set(other);
    }

    QtMatrix<T> &operator=(const QtMatrix<T> &other)
    {
        set(other);
        return *this;
    }

    void set(const T* data, int rows, int columns)
    {
        m_rows = rows;
        m_cols = columns;
        m_data = (T*)qRealloc(m_data, m_rows * m_cols * sizeof(T));
        qMemCopy(m_data, data, m_rows * m_cols * sizeof(T));
    }

    void setData(int row, int column, const T &value)
    {
        Q_ASSERT(row < m_rows && column < m_cols);
        m_data[row * m_cols + column] = value;
    }

    void set(const QtMatrix &other)
    {
        set(other.m_data, other.rowCount(), other.columnCount());
    }
    
    void fill(const T &value)
    {
        for (int i = 0; i < m_rows * m_cols; i++) {
            m_data[i]=value;
        }        
    }

    QtMatrix<T> transposed() const
    {
        QtMatrix<T> result(m_cols, m_rows);
        QtMatrix<T>::transpose(data(), result.data(), m_rows, m_cols);
        return result;
    }

    QtMatrix<T> &operator*=(T scalar)
    {
        for (int i = 0; i < m_rows * m_cols; i++) {
            m_data[i]*=scalar;
        }
        return (*this);
    }

    T sum() const
    {
        T sum = 0;
        for (int i = 0; i < m_rows * m_cols; i++) {
            sum+=m_data[i];
        }
        return sum;
    }

    QtMatrix<int> convertBasetype()
    {
        QtMatrix<int> result( rowCount(), columnCount() );
        for (int i = 0; i < rowCount() * columnCount(); ++i) {
            result.data()[i] = qRound(m_data[i]);
        }
        return result;
    }

    T at(int i) const
    {
        Q_ASSERT(i < m_rows * m_cols);
        return m_data[i];
    }

    T &r_at(int i)
    {
        Q_ASSERT(i < m_rows * m_cols);
        return m_data[i];
    }

    T at(int row, int column) const
    {
        Q_ASSERT(row < m_rows && column < m_cols);
        return m_data[row * m_cols + column];
    }

    T &r_at(int row, int column)
    {
        Q_ASSERT(row < m_rows && column < m_cols);
        return m_data[row * m_cols + column];
    }

    inline int rowCount() const { return m_rows; }
    inline int columnCount() const { return m_cols; }

    inline T* data() { return m_data; }
    inline T* data() const { return m_data; }

private:
    void resize(int rows, int columns)
    {
        m_data = (T*)qRealloc(m_data, sizeof(T) * rows * columns);
    }

    /**
     * These inline statics are used by both fixed-size and variable-size matrix classes.
     */
    inline static
    T* multiply( const T *left, const T* right, T* result, int lrows, int lcols, int rcols)
    {
        for (int i = 0; i < lrows; i++) {
            for (int j = 0; j < rcols; j++) {
                T sum = 0;
                for (int r = 0; r < lcols; r++) {
                    sum += left[i * lcols + r] * right[r * rcols + j];
                }
                result[i * rcols + j] = sum;
            }
        }
        return result;
    }

    inline static
    T* negate(T *other, int rows, int cols)
    {
        for (int i = 0; i < rows * cols; i++) {
            other[i] = -other[i];
        }
        return other;
    }

    inline static
    void add(T *left, const T *right, int rows, int cols)
    {
        for (int i = 0; i < cols*rows; i++) {
            left[i] += right[i];
        }
    }

    inline static
    void sub(T *left, const T *right, int rows, int cols)
    {
        for (int i = 0; i < cols*rows; i++) {
            left[i] -= right[i];
        }
    }

    inline static
    bool equals( const T *left, const T *right, int rows, int cols)
    {
        bool eq = true;
        for (int i = 0; i < cols*rows && eq; i++) {
            eq = (left[i] == right[i]);
        }
        return eq;
    }

    inline static
    T* transpose(const T *other, T *result, int rows, int cols)
    {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                result[c * rows + r] = other[r * cols + c];
            }
        }
        return result;
    }

private:
    T*  m_data;
    int m_rows;
    int m_cols;
};

template <typename T>
QtMatrix<T> operator*( const QtMatrix<T> &left,
                        const QtMatrix<T> &right)
{
    Q_ASSERT(left.columnCount() == right.rowCount());
    if (left.columnCount() != right.rowCount()) return QtMatrix<T>();
    QtMatrix<T> result(left.rowCount(), right.columnCount());
    QtMatrix<T>::multiply(left.data(), right.data(), result.data(), left.rowCount(), left.columnCount(), right.columnCount());
    return result;
}

template <typename T>
QtMatrix<T> operator*(const QtMatrix<T> &left, const T &scalar)
{
    QtMatrix<T> m(left);
    return m*=scalar;
}

template <typename T>
QtMatrix<T> operator+(const QtMatrix<T> &left, const T &scalar)
{
    QtMatrix<T> m(left);
    return m+=scalar;
}

template <typename T1, typename T2>
inline QtMatrix<T1> convertMatrixBasetype(const QtMatrix<T2> &other)
{
    QtMatrix<int> result(other.rowCount(), other.columnCount());
    for (int i = 0; i < other.rowCount() * other.columnCount();++i) {
        result.data()[i] = (T1)other.at(i);
    }
    return result;
}
template <> //specialization
inline QtMatrix<int> convertMatrixBasetype<int,double>(const QtMatrix<double> &other)
{
    QtMatrix<int> result(other.rowCount(), other.columnCount());
    for (int i = 0; i < other.rowCount() * other.columnCount();++i) {
        result.data()[i] = qRound((double)other.at(i));
    }
    return result;
}


#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/QDebug>
template<typename T>
QDebug operator<<(QDebug dbg, const QtMatrix<T> &matrix) {
    dbg.nospace() << "QtMatrix(";
        int rows = matrix.rowCount();
        int cols = matrix.columnCount();
        dbg << "[";
        for (int r = 0; r < rows; r++) {
            dbg << "[";
            dbg << matrix.getData(r, 0);
            for (int c = 1; c < cols; c++) {
                dbg << ',' << matrix.getData(r, c);
            }
            dbg << "]" << "\n";
        }
        dbg << "]";

    dbg << ')';
    return dbg.space();
}
#endif

#endif //QTMATRIX_H

