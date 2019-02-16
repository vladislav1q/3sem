#ifndef INC_15_02_VECTOR_H
#define INC_15_02_VECTOR_H

#include <iostream>
#include <cmath>

using namespace std;

class Vector2{
public:
    Vector2(float x1, float y1){
        x = x1;
        y = y1;
    }
    Vector2(){
        x = 0;
        y = 0;
    }
    float length(){                                     //tested
        return sqrt(x*x + y*y);
    }
    float squareLength(){                               //tested
        return x*x + y*y;
    }
    float distance(const Vector2 &a){                   //tested
        return Vector2(a.x - x, a.y - y).length();
    }
    Vector2 ortho(){                                    //tested
        return Vector2(y, -x);
    }
    Vector2& normalize(){                               //tested
        if(this->length() == 0)
            return *this;
        *this *= (1 / this->length());
        return *this;
    }
    Vector2& rotate(float degree) {                         //tested
        degree = degree / 180.0 * M_PI;
        double cosFi = cos(degree);
        double sinFi = sin(degree);
        double x2 = x * cosFi - y * sinFi;
        double y2 = x * sinFi + y * cosFi;
        x = x2;
        y = y2;
        return *this;
    }
    Vector2 getRotated(float degree) {                      //tested
        degree = degree / 180.0 * M_PI;
        double cosFi = cos(degree);
        double sinFi = sin(degree);
        return Vector2(x * cosFi - y * sinFi, x * sinFi + y * cosFi);
    }
    Vector2 operator +(const Vector2 &a) const{             //tested
        return Vector2(a.x + x, a.y + y);
    }
    Vector2 operator +(float a) const{                      //tested
        return Vector2(a + x, a + y);
    }
    Vector2& operator +=(const Vector2 &a){                 //tested
        *this = *this + a;
        return *this;
    }
    Vector2 operator -(const Vector2 &a) const{             //tested
        return Vector2(x - a.x, y - a.y);
    }
    Vector2 operator -(float a) const{                      //tested
        return Vector2(x - a, y - a);
    }
    Vector2& operator -=(const Vector2 &a){                 //tested
        *this = *this - a;
        return *this;
    }
    Vector2 operator *(float number) const{                 //tested
        return Vector2(x * number, y * number);
    }
    Vector2& operator *=(float number){                     //tested
        *this = *this * number;
        return *this;
    }
    Vector2 operator /(float number) const{                 //tested
        return Vector2(x / number, y / number);
    }
    Vector2& operator /=(float number){                     //tested
        *this = *this / number;
        return *this;
    }
    float operator *(const Vector2 &a) const{               //tested
        return (x * a.x + y * a.y);
    }
    float operator ^(const Vector2 &a) const{               //tested
        return (x * a.y - y * a.x);
    }
    Vector2 operator -()const{                              //tested
        return Vector2(-x, -y);
    }
    Vector2 print(){                                        //tested
        cout << "x: " << x << "\n" << "y: " << y << endl;
    }

    float x, y;
};

Vector2 operator *(float number, const Vector2 &a){         //tested
    return Vector2(number * a.x, number * a.y);
}
istream& operator >>(istream& stream, Vector2 &a){          //tested
    stream >> a.x >>a.y;
    return stream;
}
ostream& operator <<(ostream& stream, const Vector2 &a){    //tested
    stream << a.x << ' '<< a.y << endl;
    return stream;
}
#endif //INC_15_02_VECTOR_H
