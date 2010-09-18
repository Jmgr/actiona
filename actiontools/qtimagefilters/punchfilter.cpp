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

#include <QtGui>
#include <QtCore>
#include "punchfilter.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880168872420969808
#endif
/*
    In case we need more constants.... Keep them here just to rememeber them.

    #define M_E         2.71828182845904523536028747135266250
    #define M_LOG2E     1.44269504088896340735992468100189214
    #define M_LOG10E    0.434294481903251827651128918916605082
    #define M_LN2       0.693147180559945309417232121458176568
    #define M_LN10      2.30258509299404568401799145468436421
    #define M_PI        3.14159265358979323846264338327950288
    #define M_PI_2      1.57079632679489661923132169163975144
    #define M_PI_4      0.785398163397448309615660845819875721
    #define M_1_PI      0.318309886183790671537767526745028724
    #define M_2_PI      0.636619772367581343075535053490057448
    #define M_2_SQRTPI  1.12837916709551257389615890312154517
    #define M_SQRT2     1.41421356237309504880168872420969808
    #define M_SQRT1_2   0.707106781186547524400844362104849039
*/

/**
 * This function transforms x in order to get the punch effect.
 * The function should be continous and should return 0 for x=0 and return 1 for x = 1
 * If not, abrupt changes might appear in the image.
 * The function only needs to be valid for the closed interval [0..1]
 * \a amplitude should be in the interval [-0.3125,0.3125]
 * If it is outside the interval, punch_xform might return negative for some values
 * (which might give interesting effects, but not a pinch/punch effect)
 * A positive \a amplitude will give a "punch", negative a "pinch"
 *
 * Keeping the function linear (return x;) will in theory not change the image, but due to rounding errors
 * one might expect minor distortion.
 */
static double punch_xform(double x, double amplitude)
{
    return x - sin(M_PI*x)*amplitude;
}

static double fraction(double n)
{
    return n - floor(n);
}

#define FIXEDPOINT_FRACTIONBITS 8
#define FIXEDPOINT_MULTIPLIER (1 << FIXEDPOINT_FRACTIONBITS)
static int fixedfraction(double n)
{
    return (int)(fraction(n) * FIXEDPOINT_MULTIPLIER);
}

static QRgb weighpixel(QRgb rgb, int percentage)
{
    return qRgb((qRed(rgb) * percentage) >> (FIXEDPOINT_FRACTIONBITS * 2),
                (qGreen(rgb) * percentage) >> (FIXEDPOINT_FRACTIONBITS * 2),
                (qBlue(rgb) * percentage) >> (FIXEDPOINT_FRACTIONBITS * 2));
}

/*
 * Gets a pixel at a floating-point coordinate, thus causing it to take into accountance
 * neighbour pixels if one of the coordinates have a fractional part.
 * This should be used if you need to fetch pixels from fractional positions and want to
 * minimize the rounding errors and aliasing effects that gives.
 *
 * The function will calculate the pixel value by calculating all areas the pixel
 * occupies in each neighbouring pixel. The function also assumes that the pixel to get
 * have the same dimensions as a regular pixel. (Thus, a "subpixel" can cover at most 4 pixels)
 */

static QRgb getSubpixel(const QImage &img, double x, double y)
{
    int fx = fixedfraction(x);
    int fy = fixedfraction(y);

    int px = (int)x;
    int py = (int)y;

    QRgb p1;
    QRgb p2;
    QRgb p3;
    QRgb p4;

    int right = 1;
    int below = 1;
    if (py >= img.height() - 1 ) {
        // We are partially outside the image, do our best here.
        below = 0;
    }
    if (px >= img.width() - 1) {
        // We are partially outside the image, do our best here.
        right = 0;
    }

    p1 = weighpixel(img.pixel(px,       py),            (FIXEDPOINT_MULTIPLIER-fx)*(FIXEDPOINT_MULTIPLIER-fy));
    p2 = weighpixel(img.pixel(px+right, py),            fx*(FIXEDPOINT_MULTIPLIER-fy));
    p3 = weighpixel(img.pixel(px,       py+below),      (FIXEDPOINT_MULTIPLIER-fx)*fy);
    p4 = weighpixel(img.pixel(px+right, py+below),      fx*fy);
    
    return qRgb((qRed(p1) + qRed(p2) + qRed(p3) + qRed(p4)),
                (qGreen(p1) + qGreen(p2) + qGreen(p3) + qGreen(p4)),
                (qBlue(p1) + qBlue(p2) + qBlue(p3) + qBlue(p4)));

}

PunchFilter::PunchFilter() : m_Radius(10.0), m_Center(0.0,0.0), m_Force(0.5)
{
}

QImage PunchFilter::apply(const QImage &image, const QRect& clipRect /*= QRect()*/ ) const
{
    QImage out;
    QImage::Format fmt = image.format();
    Punch(image, &out, clipRect);
    if (out.format() != fmt) {
        out = out.convertToFormat(fmt);
    }
    return out;
}

bool PunchFilter::setOption(int option, const QVariant &value)
{
    bool bOK = true;
    switch (option) {
        case QtImageFilter::Radius:
        {
            double radius = value.toDouble(&bOK);
            if (bOK) m_Radius = radius;
        }
        break;

        case QtImageFilter::Center:
        {
            QPointF center = value.toPointF();
            if (center.isNull()) {
                bOK = false;
            } else {
                m_Center = center;
            }
        }
        break;

        case QtImageFilter::Force:
        {
            double force = value.toDouble(&bOK);
            if (bOK) m_Force = force;
        }
        break;

        default:
            bOK = false;
        break;
    }
    return bOK;
}

QVariant PunchFilter::option(int option) const
{
    QVariant value;
    switch (option) {
        case QtImageFilter::Radius:
            value = m_Radius;
            break;
        case QtImageFilter::Center:
            value = m_Center;
            break;
        case QtImageFilter::Force:
            value = m_Force;
            break;
        default:
            value = QtImageFilter::option(option);
            break;
    }
    return value;
}

bool PunchFilter::supportsOption(int option) const
{
    return (option == QtImageFilter::Radius
        || option == QtImageFilter::Center
        || option == QtImageFilter::Force);
}

bool PunchFilter::Punch(const QImage &img, QImage *outputImage, const QRect &clipRect /*= QRect()*/ ) const
{
    *outputImage = img;
    int top = 0;
    int bottom = img.height();
    int left = 0;
    int right = img.width();

    if (!clipRect.isNull()) {
        // If we have a cliprect, set our coordinates to our cliprect
        // and make sure it is within the boundaries of the image
        // After that, an optimization where we adjust the cliprect to only be the bounding 
        // rect of the circle we are manipulating, (so that we don't have to run the filter on 
        // the whole image if the default cliprect was given).
        top = qMax(top, clipRect.top());
        top = qMax(top, (int)(ceil(m_Center.y() - 1) - m_Radius));
        bottom = qMin(bottom, clipRect.bottom());
        bottom = qMin(bottom, (int)(floor(m_Center.y() + 1) + m_Radius));
        left = qMax(left, clipRect.left());
        left = qMax(left, (int)(ceil(m_Center.x() - 1) - m_Radius));
        right = qMin(right, clipRect.right());
        right = qMin(right, (int)(floor(m_Center.x() + 1) + m_Radius));
    }

    qreal centerx = m_Center.x();
    qreal centery = m_Center.y();

    int x;
    int y;
    double amplitude = m_Force/3.2; // scale down
    //amplitude = qBound(-0.3125, amplitude, 0.3125);

    for (y = top; y < bottom; y++) {
        for (x = left; x < right; x++) {
            qreal dx = x - centerx;
            qreal dy = y - centery;
            double distance = sqrt(dx * dx + dy * dy);
            QRgb rgb;
            if (distance <= m_Radius + M_SQRT2) {   // M_SQRT2 is the maximum "width" of a pixel. (If measured diagonally)
                                                    // we must evaluate this area "outside" the radius also to reduce aliasing effects.
                double distort = distance / m_Radius;
                if (distort > 0.0 && distort < 1.0) {
                    distort = punch_xform(distort, amplitude);
                }
                
                // Normalize the distance vector and find the length after distortion
                if (dx != 0 || dy != 0) {
                    double mag = m_Radius/sqrt(dx * dx + dy * dy);
                    mag *= distort;
                    dx *= mag;
                    dy *= mag;
                }

                double tx = centerx + dx;
                double ty = centery + dy;
                // Crop off any overflows. This happens since we are adding M_SQRT2 to the radius to evaluate a small area outside the radius circle.
                if (tx > img.width()  || tx < 0) tx = x;
                if (ty > img.height() || ty < 0) ty = y;
                
                rgb = getSubpixel(img, tx, ty);
                outputImage->setPixel(x, y, rgb);
            }
        }
    }
    return true;
}
