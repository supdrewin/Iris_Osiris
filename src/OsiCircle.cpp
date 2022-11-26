/*******************************************************
 * Open Source for Iris : OSIRIS
 * Version : 4.0
 * Date : 2011
 * Author : Guillaume Sutra, Telecom SudParis, France
 * License : BSD
 ********************************************************/

#include <stdexcept>
#include <string>

#include "OsiCircle.h"

// CONSTRUCTORS & DESTRUCTORS
/////////////////////////////

OsiCircle::OsiCircle()
{
    // Do nothing
}

OsiCircle::~OsiCircle()
{
    // Do nothing
}

OsiCircle::OsiCircle(const CvPoint &rCenter, int rRadius)
{
    setCenter(rCenter);
    setRadius(rRadius);
}

// ACCESSORS
////////////

CvPoint OsiCircle::getCenter() const
{
    return mCenter;
}

int OsiCircle::getRadius() const
{
    return mRadius;
}

void OsiCircle::setCenter(const CvPoint &rCenter)
{
    mCenter = rCenter;
}

void OsiCircle::setRadius(int rRadius)
{
    if (rRadius < 0)
    {
        throw std::runtime_error("Circle with negative radius : " + std::to_string(rRadius));
    }
    mRadius = rRadius;
}

void OsiCircle::setCircle(const CvPoint &rCenter, int rRadius)
{
    setCenter(rCenter);
    setRadius(rRadius);
}

void OsiCircle::setCircle(int rCenterX, int rCenterY, int rRadius)
{
    setCircle(cvPoint(rCenterX, rCenterY), rRadius);
}

// OPERATORS
////////////

void OsiCircle::drawCircle(IplImage *pImage, const CvScalar &rColor, int thickness)
{
    cvCircle(pImage, mCenter, mRadius, rColor, thickness);
}

void OsiCircle::computeCircleFitting(const std::vector<CvPoint> &rPoints)
{
    // Compute the averages mx and my
    float mx = 0, my = 0;
    for (int p = 0; p < rPoints.size(); p++)
    {
        mx += rPoints[p].x;
        my += rPoints[p].y;
    }
    mx = mx / rPoints.size();
    my = my / rPoints.size();

    // Work in (u,v) space, with u = x-mx and v = y-my
    float u = 0, v = 0, suu = 0, svv = 0, suv = 0, suuu = 0, svvv = 0, suuv = 0, suvv = 0;

    // Build some sums
    for (int p = 0; p < rPoints.size(); p++)
    {
        u = rPoints[p].x - mx;
        v = rPoints[p].y - my;
        suu += u * u;
        svv += v * v;
        suv += u * v;
        suuu += u * u * u;
        svvv += v * v * v;
        suuv += u * u * v;
        suvv += u * v * v;
    }

    // These equations are demonstrated in paper from R.Bullock (2006)
    float uc = 0.5 * (suv * (svvv + suuv) - svv * (suuu + suvv)) / (suv * suv - suu * svv);
    float vc = 0.5 * (suv * (suuu + suvv) - suu * (svvv + suuv)) / (suv * suv - suu * svv);

    std::cout << "uc: " << uc << std::endl;
    std::cout << "vc: " << vc << std::endl;

    std::cout << "rPoints.size(): " << rPoints.size() << std::endl;
    std::cout << "rPoints[0].x: " << rPoints[0].x << std::endl;
    std::cout << "rPoints[0].y: " << rPoints[0].y << std::endl;

    // Circle parameters
    setCenter(cvPoint(uc + mx, vc + my));
    setRadius(std::sqrt(uc * uc + vc * vc + (suu + svv) / rPoints.size()));
}
