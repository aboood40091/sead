#ifndef SEAD_BOUNDBOX_H_
#define SEAD_BOUNDBOX_H_

#include <math/seadVector.h>

#include <limits>

namespace sead
{

template <typename T>
class BoundBox2
{
public:
    typedef sead::Vector2<T> Vector2;

public:
    BoundBox2()
    {
        setUndef();
    }

    BoundBox2(T x0, T y0, T x1, T y1)
    {
        set(x0, y0, x1, y1);
    }

    T getSizeX() const
    {
        return mMax.x - mMin.x;
    }

    T getSizeY() const
    {
        return mMax.y - mMin.y;
    }

    T getHalfSizeX() const
    {
        return (mMax.x - mMin.x) / 2.0f;
    }

    T getHalfSizeY() const
    {
        return (mMax.y - mMin.y) / 2.0f;
    }

    void getCenter(Vector2* center) const
    {
        center->set((mMin.x + mMax.x) / 2.0f,
                    (mMin.y + mMax.y) / 2.0f);
    }

    bool isUndef() const
    {
        return mMin.x > mMax.x || mMin.y > mMax.y;
    }

    bool isInside(const Vector2& p) const
    {
        return mMin.x <= p.x && p.x <= mMax.x && mMin.y <= p.y && p.y <= mMax.y;
    }

    void setUndef()
    {
        const T max = std::numeric_limits<T>::max();
        mMin.set(max, max);
        const T min = std::numeric_limits<T>::min();
        mMax.set(min, min);
    }

    void set(T x0, T y0, T x1, T y1)
    {
        if (x0 < x1)
        {
            mMin.x = x0;
            mMax.x = x1;
        }
        else
        {
            mMin.x = x1;
            mMax.x = x0;
        }

        if (y0 < y1)
        {
            mMin.y = y0;
            mMax.y = y1;
        }
        else
        {
            mMin.y = y1;
            mMax.y = y0;
        }
    }

    void set(const Vector2& min, const Vector2& max)
    {
        mMin = min;
        mMax = max;
    }

    void setMin(const Vector2& min)
    {
        mMin = min;
    }

    void setMax(const Vector2& max)
    {
        mMax = max;
    }

    void setFromCornerAndXY(T cornerX, T cornerY, T sizeX, T sizeY)
    {
        mMin.set(cornerX, cornerY);
        mMax.set(cornerX + sizeX, cornerY + sizeY);
    }

    void scaleX(T sx)
    {
        T sizeX = (mMax.x - mMin.x) * (sx / 2.0f);
        T centerX = (mMin.x + mMax.x) / 2.0f;

        mMin.x = centerX - sizeX;
        mMax.x = centerX + sizeX;
    }

    void scaleY(T sy)
    {
        T sizeY = (mMax.y - mMin.y) * (sy / 2.0f);
        T centerY = (mMin.y + mMax.y) / 2.0f;

        mMin.y = centerY - sizeY;
        mMax.y = centerY + sizeY;
    }

private:
    Vector2 mMin;
    Vector2 mMax;
};

template <>
inline void BoundBox2<float>::setUndef()
{
    const float max =  std::numeric_limits<float>::max();
    mMin.set(max, max);
    const float min = -std::numeric_limits<float>::max();
    mMax.set(min, min);
}

template <>
inline void BoundBox2<double>::setUndef()
{
    const double max =  std::numeric_limits<double>::max();
    mMin.set(max, max);
    const double min = -std::numeric_limits<double>::max();
    mMax.set(min, min);
}

template <>
inline void BoundBox2<long double>::setUndef()
{
    const long double max =  std::numeric_limits<long double>::max();
    mMin.set(max, max);
    const long double min = -std::numeric_limits<long double>::max();
    mMax.set(min, min);
}

template <typename T>
class BoundBox3
{
public:
    typedef sead::Vector3<T> Vector3;

public:
    BoundBox3()
    {
        setUndef();
    }

    BoundBox3(T x0, T y0, T z0, T x1, T y1, T z1)
    {
        set(x0, y0, z0, x1, y1, z1);
    }

    T getSizeX() const
    {
        return mMax.x - mMin.x;
    }

    T getSizeY() const
    {
        return mMax.y - mMin.y;
    }

    T getSizeZ() const
    {
        return mMax.z - mMin.z;
    }

    T getHalfSizeX() const
    {
        return (mMax.x - mMin.x) / 2.0f;
    }

    T getHalfSizeY() const
    {
        return (mMax.y - mMin.y) / 2.0f;
    }

    T getHalfSizeZ() const
    {
        return (mMax.z - mMin.z) / 2.0f;
    }

    void getCenter(Vector3* center) const
    {
        center->set((mMin.x + mMax.x) / 2.0f,
                    (mMin.y + mMax.y) / 2.0f,
                    (mMin.z + mMax.z) / 2.0f);
    }

    bool isUndef() const
    {
        return mMin.x > mMax.x || mMin.y > mMax.y || mMin.z > mMax.z;
    }

    bool isInside(const Vector3& p) const
    {
        return mMin.x <= p.x && p.x <= mMax.x && mMin.y <= p.y && p.y <= mMax.y && mMin.z <= p.z && p.z <= mMax.z;
    }

    void setUndef()
    {
        const T max = std::numeric_limits<T>::max();
        mMin.set(max, max, max);
        const T min = std::numeric_limits<T>::min();
        mMax.set(min, min, min);
    }

    void set(T x0, T y0, T z0, T x1, T y1, T z1)
    {
        if (x0 < x1)
        {
            mMin.x = x0;
            mMax.x = x1;
        }
        else
        {
            mMin.x = x1;
            mMax.x = x0;
        }

        if (y0 < y1)
        {
            mMin.y = y0;
            mMax.y = y1;
        }
        else
        {
            mMin.y = y1;
            mMax.y = y0;
        }

        if (z0 < z1)
        {
            mMin.z = z0;
            mMax.z = z1;
        }
        else
        {
            mMin.z = z1;
            mMax.z = z0;
        }
    }

    void set(const Vector3& min, const Vector3& max)
    {
        mMin = min;
        mMax = max;
    }

    void setMin(const Vector3& min)
    {
        mMin = min;
    }

    void setMax(const Vector3& max)
    {
        mMax = max;
    }

    void setFromCornerAndXYZ(T cornerX, T cornerY, T cornerZ, T sizeX, T sizeY, T sizeZ)
    {
        mMin.set(cornerX, cornerY, cornerZ);
        mMax.set(cornerX + sizeX, cornerY + sizeY, cornerZ + sizeZ);
    }

    void scaleX(T sx)
    {
        T sizeX = (mMax.x - mMin.x) * (sx / 2.0f);
        T centerX = (mMin.x + mMax.x) / 2.0f;

        mMin.x = centerX - sizeX;
        mMax.x = centerX + sizeX;
    }

    void scaleY(T sy)
    {
        T sizeY = (mMax.y - mMin.y) * (sy / 2.0f);
        T centerY = (mMin.y + mMax.y) / 2.0f;

        mMin.y = centerY - sizeY;
        mMax.y = centerY + sizeY;
    }

    void scaleZ(T sz)
    {
        T sizeZ = (mMax.z - mMin.z) * (sz / 2.0f);
        T centerZ = (mMin.z + mMax.z) / 2.0f;

        mMin.z = centerZ - sizeZ;
        mMax.z = centerZ + sizeZ;
    }

private:
    Vector3 mMin;
    Vector3 mMax;
};

template <>
inline void BoundBox3<float>::setUndef()
{
    const float max =  std::numeric_limits<float>::max();
    mMin.set(max, max, max);
    const float min = -std::numeric_limits<float>::max();
    mMax.set(min, min, min);
}

template <>
inline void BoundBox3<double>::setUndef()
{
    const double max =  std::numeric_limits<double>::max();
    mMin.set(max, max, max);
    const double min = -std::numeric_limits<double>::max();
    mMax.set(min, min, min);
}

template <>
inline void BoundBox3<long double>::setUndef()
{
    const long double max =  std::numeric_limits<long double>::max();
    mMin.set(max, max, max);
    const long double min = -std::numeric_limits<long double>::max();
    mMax.set(min, min, min);
}

typedef BoundBox2<f32> BoundBox2f;
typedef BoundBox3<f32> BoundBox3f;

}  // namespace sead

#endif // SEAD_BOUNDBOX_H_
