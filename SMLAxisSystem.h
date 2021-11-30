#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/epsilon.hpp>

namespace SmartLib
{
template<typename T = double>
class AxisSystem
{
private:
    inline static const glm::tmat4x4<T> MatIdentity{T{1}};
    inline static const glm::tvec3<T> VecAllOne{T{1}, T{1}, T{1}};
    inline static const glm::tvec3<T> VecAllZero{T{0}, T{0}, T{0}};

private:
    glm::tmat4x4<T> _axis{T{1}};             // the vector of x y z axis
    glm::tvec3<T> _unitLen{T{1},T{1},T{1}};  // unit length of the x y z axis
    glm::tvec3<T> _origin{T{0}, T{0}, T{0}}; //in world (absolute) system

public:
//    using mat4 = glm::tmat4x4<T>;
//    using mat3 = glm::tmat3x3<T>;
//    using mat2 = glm::tmat2x2<T>;

//    using vec4 = glm::tvec4<T>;
//    using vec3 = glm::tvec3<T>;
//    using vec2 = glm::tvec2<T>;


public:

    AxisSystem()
    {
    }

    AxisSystem(const AxisSystem& as) :
        _axis{as._axis},
        _unitLen{as._unitLen},
        _origin{as._origin}
    {
    }

    AxisSystem(AxisSystem&& as) :
        _axis{std::move(as._axis)},
        _unitLen{std::move(as._unitLen)},
        _origin{std::move(as._origin)}
    {
    }

    const AxisSystem& operator=(const AxisSystem& as)
    {
        _axis = as._axis;
        _unitLen = as._unitLen;
        _origin = as._origin;
        return *this;
    }

    const AxisSystem& operator=(AxisSystem&& as)
    {
        _axis = std::move(as._axis);
        _unitLen = std::move(as._unitLen);
        _origin = std::move(as._origin);
        return *this;
    }

    AxisSystem& Reset()
    {
        _axis = MatIdentity;
        _unitLen = VecAllOne;
        _origin = VecAllZero;
        return *this;
    }



    //move along the current axis directions and with the current unit length
    AxisSystem& Translate(const glm::tvec3<T>& offset)
    {
        _origin += Mat4xVec3(_axis, offset * _unitLen);
        return *this;
    }

    //move along the absolute (world) axis directions
    AxisSystem& TranslateAbsolutely(const glm::tvec3<T>& offset)
    {
        _origin += offset;
        return *this;
    }

    //rotate in current coordinates system
    AxisSystem& Rotate(T radians, const glm::tvec3<T>& rotAxis)
    {
        auto rotAxisAbsolutely = Mat4xVec3V(_axis, rotAxis);
        return RotateAbsolutely(radians, rotAxisAbsolutely);
    }


    //rotate in absolute (world) coordinates system
    AxisSystem& RotateAbsolutely(T radians, const glm::tvec3<T>& rotAxis)
    {
        _axis = glm::rotate<T>(MatIdentity, radians, rotAxis) * _axis;
        return *this;
    }


    //scale in current coordinates system
    AxisSystem& Scale(const glm::tvec3<T>& scalar)
    {
        _unitLen *= scalar;
        return *this;
    }


    AxisSystem& SetScale(const glm::tvec3<T>& scalar)
    {
        _unitLen = scalar;
        return *this;
    }


    AxisSystem& SetOrigin(const glm::tvec3<T>& origin)
    {
        _origin = origin;
        return *this;
    }

    //axis should be unit and orthogonal matrix
    AxisSystem& SetAxis(const glm::tmat4x4<T>& axis)
    {
        _axis = axis;
        return *this;
    }


    const glm::tvec3<T>& GetScale() const
    {
       return _unitLen;
    }


    const glm::tvec3<T>& GetOrigin() const
    {
        return _origin;
    }

    const glm::tmat4x4<T>& GetAxis() const
    {
        return _axis;
    }


    const glm::tvec3<T> ModelToWorld(const glm::tvec3<T>& model) const
    {
        return _origin + Mat4xVec3(_axis, model * _unitLen) ;
    }

    const glm::tvec3<T> WorldToModel(const glm::tvec3<T>& world) const
    {
        return Mat4xVec3(glm::transpose(_axis), world - _origin)/_unitLen;
    }

    const glm::tmat4x4<T> ModelToWorldMat() const
    {
        auto matScale = glm::scale<T>(MatIdentity, _unitLen);

        auto matTrans = glm::translate<T>(MatIdentity, _origin);

        return matTrans * _axis * matScale;

    }

    const glm::tmat4x4<T> WorldToModelMat() const
    {
        auto matTrans = glm::translate(MatIdentity, -_origin);
        auto matRot = glm::transpose(_axis); // is equal to glm::inverse<T>(_axis)
        auto matScale = glm::scale(MatIdentity, VecAllOne/_unitLen);
        return matScale * matRot * matTrans;
    }

    void MakeFromOHV(const glm::tvec3<T>& originPos, const glm::tvec3<T>& horizontalV, const glm::tvec3<T>& verticalV)
    {
        _origin = originPos;
        auto zV = glm::cross<T>(horizontalV, verticalV);
        auto yV = glm::cross<T>(zV, horizontalV);

        _axis[0] = glm::tvec4<T>{glm::normalize(horizontalV), T{0}};
        _axis[1] = glm::tvec4<T>{glm::normalize(yV), T{0}};
        _axis[2] = glm::tvec4<T>{glm::normalize(zV), T{0}};
    }


    void MakeFromOHVPos(const glm::tvec3<T>& originPos, const glm::tvec3<T>& horizontalPos, const glm::tvec3<T>& verticalPos)
    {
        MakeFromOHV(originPos, horizontalPos - originPos, verticalPos - originPos);
    }


    //note: horizontalV verticalV and zV should be orthogonal to each other
    void MakeFromOHVZ(
            const glm::tvec3<T>& originPos,
            const glm::tvec3<T>& horizontalV,
            const glm::tvec3<T>& verticalV,
            const glm::tvec3<T>& zV)
    {
        _origin = originPos;
        _axis[0] = glm::tvec4<T>{glm::normalize(horizontalV), T{0}};
        _axis[1] = glm::tvec4<T>{glm::normalize(verticalV), T{0}};
        _axis[2] = glm::tvec4<T>{glm::normalize(zV), T{0}};
    }


public:

    static glm::tvec3<T> V4ToV3(const glm::tvec4<T>& v4)
    {
        if(glm::epsilonNotEqual(v4[3], T{0}, glm::epsilon<T>()*T{1000}))
        {
            T inverse = T{1}/v4[3];
            return glm::tvec3<T>{v4 * inverse};
        }
        else
        {
            return glm::tvec3<T>{v4};
        }
    }

    static glm::tvec4<T> V3ToV4(const glm::tvec3<T>& v3)
    {
        return glm::tvec4<T>{v3, T{1}}; //positional point
    }

    static glm::tvec4<T> V3ToV4V(const glm::tvec3<T>& v3)
    {
        return glm::tvec4<T>{v3, T{0}}; //directional vector
    }

    static glm::tvec3<T> Mat4xVec3(const glm::tmat4x4<T>& m4, const glm::tvec3<T>& v3)
    {
        return glm::tvec3<T>{m4 * glm::tvec4<T>{v3, T{1}}} ; //positional point
    }

    static glm::tvec3<T> Mat4xVec3V(const glm::tmat4x4<T>& m4, const glm::tvec3<T>& v3)
    {
        return glm::tvec3<T>{m4 * glm::tvec4<T>{v3, T{0}}} ; //directional vector
    }

    static glm::tvec3<T> Mat4xVec4(const glm::tmat4x4<T>& m4, const glm::tvec4<T>& v4)
    {
        auto vec = m4 * v4;
        if(glm::epsilonNotEqual(vec[3], T{0}, glm::epsilon<T>()*T{1000}))
        {
            T inverse = T{1}/vec[3];
            vec *= inverse;
        }
        return glm::tvec3<T>(vec);
    }


public:
    //equivilent to glm::lookAt
    static glm::tmat4x4<T> LookAt(const glm::tvec3<T> &eye, const glm::tvec3<T> &center, const glm::tvec3<T> &up)
    {
        //////////////////////////////////////
        auto zAxis = eye - center; //posive z direction pointing into eye
        auto xAxis = glm::cross(up, zAxis);
        auto yAxis = glm::cross(zAxis, xAxis);

        AxisSystem<T> axisSys;
        axisSys.MakeFromOHVZ(eye, xAxis, yAxis, zAxis);

        return axisSys.WorldToModelMat();
    }

    //equivilent to glm::ortho
    static glm::tmat4x4<T> Ortho(
            T const &left, T const &right,
            T const &bottom, T const &top,
            T const &zNear, T const &zFar
            )
    {
        //////////////////////////////////////
        AxisSystem<T> axisSys;
        axisSys.Translate(glm::tvec3<T>(-(left + right)/T{2}, -(bottom + top)/T{2}, -(zNear + zFar)/T{2}))
            .Scale(glm::tvec3<T>((right - left)/T{2}, (top - bottom)/T{2}, (zFar - zNear)/T{2}));

        auto mat = axisSys.WorldToModelMat();

        //adjust sign to satisfy glm rule
        mat[2][2] = -mat[2][2];
        for(int ii = 0; ii < 3; ++ ii)
        {
            mat[3][ii] = -mat[3][ii];
        }

        return mat;

    }

    static glm::tmat4x4<T> Frustum(
            T const &left, T const &right,
            T const &bottom, T const &top,
            T const &zNear, T const &zFar
            )
    {
        //////////////////////////////////////
        ///TO DO...
        assert(false);
    }
};
}
