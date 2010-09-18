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

#ifndef GAUSSBLURFILTER_H
#define GAUSSBLURFILTER_H

#include <math.h>
#include "convolutionfilter.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

class GaussBlurFilter : public ConvolutionFilter
{
    public:
        GaussBlurFilter() : ConvolutionFilter()
        {
            m_radius = 4.0;
        }

        ////
        // INHERITED FROM ConvolutionFilter
        ////
        QVariant option(int option) const;

        bool setOption(int option, const QVariant &value)
        {
            bool ok = true;
            switch (option) {
                case QtImageFilter::Radius:
                {
                    double radius = value.toDouble(&ok);
                    if (ok) m_radius = radius;
                }
                break;

                default:
                    ok = ConvolutionFilter::setOption(option, value);
                break;
            }
            return ok;
        }

        bool supportsOption(int option) const
        {
            if (option == QtImageFilter::Radius) return true;
            return ConvolutionFilter::supportsOption(option);
        }

	QImage apply(const QImage &image, const QRect& clipRect ) const;
        
	QString name() const { return QLatin1String("GaussBlur"); }
        QString description() const { return QObject::tr("A gaussian blur filter", "GaussBlurFilter"); }

    private:
        qreal m_radius;
};


static qreal Gauss2DFunction(int x, int y, qreal deviance)
{
    /**
    * A 2-D gaussian distribution has the shape:
    *
    *               1         -(x*x + y*y)/(2*dev*dev)
    * G(x,y) = ---------- * e^
    *          2*PI*dev^2
    *
    * see http://www.cee.hw.ac.uk/hipr/html/gsmooth.html for a more readable version
    */
    return exp(-(x*x + y*y)/(2*deviance*deviance))/(2*M_PI*deviance*deviance);
}

QVariant GaussBlurFilter::option(int option) const
{
    if (option == QtImageFilter::Radius) return true;
    return ConvolutionFilter::option(option);
}

QImage GaussBlurFilter::apply(const QImage &image, const QRect& clipRect ) const
{
    bool ok = true;
    if (m_radius > 0.0) {
        int uRadius = (int)ceil(m_radius);

        double deviance = sqrt(-m_radius*m_radius/(2*log(1/255.0)));
        QtMatrix<double> matLeft(2 * uRadius + 1, 1);

        for (int x = -uRadius; x <=uRadius; x++) {
            matLeft.setData(uRadius + x, 0, Gauss2DFunction(x, 0, deviance));
        }
        double scalar = matLeft.at(uRadius, 0);
        matLeft*=(255.0/scalar);
        QtMatrix<double> matRight = matLeft.transposed();
        
        QtConvolutionKernelMatrix integerMatrixLeft = convertMatrixBasetype<int,double>(matLeft);
        QtConvolutionKernelMatrix integerMatrixRight = convertMatrixBasetype<int,double>(matRight);
        
        GaussBlurFilter *localThis = const_cast<GaussBlurFilter*>(this);
        localThis->addKernel(integerMatrixLeft, m_channels, m_borderPolicy);
        localThis->addKernel(integerMatrixRight, m_channels, m_borderPolicy);
    } else {
        ok = false;
    }
    return ConvolutionFilter::apply(image, clipRect);
}

#endif //GAUSSBLURFILTER_H

