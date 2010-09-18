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

#ifndef CONVOLUTIONFILTER_H
#define CONVOLUTIONFILTER_H
#include <QtCore/QFlags>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include "qtimagefilter.h"

class QImage;

struct KernelMatrixData {
    QtConvolutionKernelMatrix matrix;
    int divisor;
    int bias;
};

class ConvolutionFilter : public QtImageFilter {
public:
    typedef enum {      // This enum specifies in what alternative way pixels should be selected if a pixel 
                        // is outside the image bounds.
        Extend = 1,     // Will pick the closest border pixel, (the border pixels defines what is outside the image)
                        // This is the default.
        Mirror = 2,     // The image is mirrored.
        Wrap            // The image is wrapped. (Good for tiled images.)
    } FilterBorderPolicy;
    
    enum FilterChannelFlag {
        Red   = 0x01,
        Green = 0x02,
        Blue  = 0x04,
        Alpha = 0x08,
        RGB   = Red | Green | Blue,
        RGBA  = RGB | Alpha
    };
    Q_DECLARE_FLAGS(FilterChannels, FilterChannelFlag)

    ConvolutionFilter();
    void addKernel(  const QtConvolutionKernelMatrix &kernelMatrix,
                FilterChannels channels, FilterBorderPolicy borderPolicy,
                int divisor = 0, int bias = 0);

    ////
    // INHERITED FROM QtImageFilter
    ////
    QVariant option(int option) const;
    bool setOption(int option, const QVariant &value);
    bool supportsOption(int option) const;
    QImage apply(const QImage &image, const QRect& clipRect = QRect() ) const;
    QString name() const { return m_name; }
    QString description() const { return m_description; }
    ~ConvolutionFilter();


    void setName(const QString &name){
        m_name = name;
    }

    void setDescription(const QString &desc)
    {
        m_description = desc;
    }

protected:
    FilterChannels m_channels;
    FilterBorderPolicy m_borderPolicy;
    QVector<KernelMatrixData> m_kernels;

private:

    bool setChannels(const QString &rgba);
    QString getChannels() const;
    bool setBorderPolicy(const QString &borderPolicy);
    QString getBorderPolicy() const;

    QImage convolve(const QImage &img, const QRect& clipRect) const;
    QRgb convolvePixelRGBA(   const QImage &img, int x, int y, const int *kernelMatrix, int kernelRows, int kernelColumns, 
                                    int divisor, int bias) const;

private:
    QString  m_name;
    QString     m_description;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ConvolutionFilter::FilterChannels)
#endif //CONVOLUTIONFILTER_H

