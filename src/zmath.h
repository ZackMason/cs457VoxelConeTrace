#pragma once

#include <array>

template<class T, int _dim> struct Vector;

template<typename T, int _dim> Vector<T, _dim> mul(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
template<typename T, int _dim> Vector<T, _dim> div(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
template<typename T, int _dim> Vector<T, _dim> add(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
template<typename T, int _dim> Vector<T, _dim> sub(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
template<typename T, int _dim> Vector<T, _dim> normalize(const Vector<T, _dim>& v1);


template<class T, int _dim>
struct Vector
{
    
#if 1
    union
    {
        std::array<T, _dim> v{};
        struct{
            T x;
            T y;
            T z;
            T w;
        };
    };
#endif


    T* operator*(){return v.data();}
    T&operator[](size_t i){return v[i];}
    Vector<T, _dim> operator+(const Vector<T, _dim>& o) const { return add(*this, o); }
    Vector<T, _dim> operator-(const Vector<T, _dim>& o) const { return sub(*this, o); }
    Vector<T, _dim> operator*(const Vector<T, _dim>& o) const { return mul(*this, o); }
    Vector<T, _dim> operator/(const Vector<T, _dim>& o) const { return div(*this, o); }
    


    //friend T dot(Vector<T, _dim> const &v1, Vector<T, _dim> const &v2);
    T dot(const Vector<T, _dim>& v1) const
    {
        T p = 0.;
        for(int i = 0; i < _dim; i++)
            p +=  v[i] * v1.v[i];
        return p;
    }
    friend Vector<T, _dim> mul<T, _dim>(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
    friend Vector<T, _dim> div<T, _dim>(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
    friend Vector<T, _dim> add<T, _dim>(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
    friend Vector<T, _dim> sub<T, _dim>(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2);
    friend Vector<T, _dim> normalize<T, _dim>(const Vector<T, _dim>& v1);
};

template<class T, int _dim>
std::ostream& operator<<(std::ostream& os, const Vector<T, _dim>& v1)
{
    os << "[" << v1.v[0];
    for (int i = 1; i < _dim; ++i)
        os << ", " << v1.v[i];
    return os << "]";
}

template<class T, int _dim>
Vector<T, _dim> normalize(const Vector<T, _dim>& v1)
{
    Vector<T,_dim> p = v1;
    auto length = std::sqrt(v1.dot(v1));
    for(int i = 0; i < _dim; i++)
        p.v[i] /= length;
    return p;
}


template<class T, int _dim>
Vector<T, _dim> mul(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2)
{
    Vector<T,_dim> p;
    for(int i = 0; i < _dim; i++)
        p.v[i] = v1.v[i] * v2.v[i];
    return p;
}

template<class T, int _dim>
Vector<T, _dim> div(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2)
{
    Vector<T,_dim> p;
    for(int i = 0; i < _dim; i++)
        p.v[i] = v1.v[i] / v2.v[i];
    return p;
}

template<class T, int _dim>
Vector<T, _dim> sub(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2)
{
    Vector<T,_dim> p;
    for(int i = 0; i < _dim; i++)
        p.v[i] = v1.v[i] - v2.v[i];
    return p;
}

template<class T, int _dim>
Vector<T, _dim> add(const Vector<T, _dim>& v1, const Vector<T, _dim>& v2)
{
    Vector<T,_dim> p;
    for(int i = 0; i < _dim; i++)
        p.v[i] = v1.v[i] + v2.v[i];
    return p;
}



#if 0
template<class T>
class Vector2 : Vector<T, 2>
{
    T& x() const {return v[0];}
    T& s() const {return v[0];}
    T& u() const {return v[0];}
    T& r() const {return v[0];}
    
    T& y() const {return v[1];}
    T& t() const {return v[1];}
    T& v() const {return v[1];}
    T& g() const {return v[1];}
};

template<class T>
class Vector3 : Vector<T, 3>, Vector2<T>
{
    T& z() const {return v[2];}
    T& r() const {return v[2];}
    T& b() const {return v[2];}
};

template<class T>
class Vector4 : Vector<T, 4>, Vector3<T>
{
    T& w() const {return v[3];}
    T& q() const {return v[3];}
    T& a() const {return v[3];}
};
#endif

using v2f = Vector<float,2>;
using v3f = Vector<float,3>;
using v4f = Vector<float,4>;


static v3f cross(const v3f& a, const v3f& b)
{
    auto [ax, ay, az] = a.v;
    auto [bx, by, bz] = b.v;
    return {
        ay * bz - az * by,
        az * bx - ax * bz,
        ax * by - ay * bx,
    };
}


template<typename T, int N, int M>
struct Matrix
{
    using vec = Vector<T,N>;
    std::array<vec, M> m;
    
    Matrix(T s) 
    {
        for (auto i = 0; i < N; i += 1){
            for (auto j = 0; j < M; j += 1){
                m[i][j] = s;
            }
        }
    }
    
    Matrix() 
    {
        for (auto i = 0; i < std::min(M, N); i += 1){
            m[i][i] = 1;
        }
    }
    
    vec& operator[](size_t i){return m[i];}
    vec* operator*() { return m.data(); }
    
    
    Matrix<T, M, N> transpose() 
    {
        Matrix<T, M, N> ret;
        for (auto i = 0; i < N; i += 1){
            for (auto j = 0; j < M; j += 1){
                ret.m[i][j] = m[j][i];
            }
        }
        return ret;
    }
    
    template <int K>
        Matrix<T, N, K> mul(Matrix<T, M, K>& o)
    {
        auto oT = o.transpose();
        Matrix<T, N, K> ret;
        
        for (auto n = 0; n < N; n += 1) {
            for (auto k = 0; k < K; k += 1) {
                ret.m[n][k] = 0;
                for (auto v = 0; v < M; v++)
                {
                    ret.m[n][k] += m[n][k] * o.m[k][v];
                }
                //ret.m[i][j] = m[i].dot(oT.m[j]);
            }
        }
        return ret;
    }
    
    Vector<T,M> mul(const Vector<T, M>& v)
    {
        Vector<T,M> r;
        for(auto i = 0; i < M; ++i)
            r[i] = m[i].dot(v);
        return r;
    }
    
    Vector<T,M> operator*(const Vector<T,M>& v) { return mul(v); }
    template <int K>
        Matrix<T,N,K> operator*(Matrix<T,M,K>& o) { return mul(o); }
    bool operator==(const Matrix<T, N, M>& o) {
        for (auto i = 0; i < N; i += 1){
            for (auto j = 0; j < M; j += 1){
                if (m[i][j] != o.m[i][j])
                    return false;
            }
        }
        return true;
    }
};

using m22 = Matrix<float, 2, 2>;
using m33 = Matrix<float, 3, 3>;
using m34 = Matrix<float, 3, 4>;
using m44 = Matrix<float, 4, 4>;

static void Translate(m44& A, const v3f& v)
{
    A[0][3] += v.v[0];
    A[1][3] += v.v[1];
    A[2][3] += v.v[2];
}

template <typename T, int N, int M>
void RotateZ(Matrix<T,N,M>& A, const float a)
{
    auto c = std::cos(a);
    auto s = std::sin(a);
    Matrix<T,N,M> r;
    r[0] = {c, -s, 0};
    r[1] = {s,  c, 0};
    r[2] = {0,  0, 1};
    
    A = A * r;
}

template<typename T, int N, int M>
static void RotateY(Matrix<T,N,M>& A, const float a)
{
    auto c = std::cos(a);
    auto s = std::sin(a);
    Matrix<T,N,M> r;
    r[0] = {c, 0, s};
    r[1] = {0, 1, 0};
    r[2] = {-s,0, c};
    
    A = A * r;
}

template<typename T, int N, int M>
static void RotateX(Matrix<T,N,M>& A, const float a)
{
    auto c = std::cos(a);
    auto s = std::sin(a);
    Matrix<T,N,M> r;
    r[0] = {1, 0, 0};
    r[1] = {0, c, -s};
    r[2] = {0,  s, c};
    
    A = A * r;
}




static m44 ProjectionMatrix(float b, float t, float l, float r, float n, float f)
{
    m44 A(0);
    A[0][0] = 2.0 * n / (r - l);
    A[1][1] = 2.0 * n / (t - b);

    A[2][0] = (r + l) / (r - l);
    A[2][1] = (t + b) / (t - b);
    A[2][2] = -(f + n) / (f - n);
    A[2][3] = -1;
    
    A[3][2] = -2 * f * n / (f - n);
    
    return A;
}


static m44 ProjectionMatrix(float aspect, float fov, float near, float far)
{
    auto ang = std::tan(fov * .50f * 3.1415 / 180.0);
    return ProjectionMatrix(-ang*near, ang*near, -aspect*ang, aspect*ang, near, far);

    m44 A(0);
    A[0][0] = 1.0f / (aspect * ang);
    A[1][1] = 1.0f / (ang);
    A[2][2] = -(far + near) / (far - near);
    A[3][2] = (2.0f * far * near) / (far - near);
    A[2][3] = -1;
    return A;
}

static m44 LookAt(const v3f& from, const v3f& to, const v3f up_vec = {0,1,0})
{
    v3f forward = normalize(from - to);
    v3f right = cross(normalize(up_vec), forward);
    v3f up = cross(forward, right);
    
    m44 A;
    for (int i = 0; i < 3; i++)
    {
        A[0][i] = right.v[i];
        A[1][i] = up.v[i];
        A[2][i] = forward.v[i];
    }

    //Translate(A, from);

    return A;
}






