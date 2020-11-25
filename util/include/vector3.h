#ifndef UTIL_VECTOR3_H
#define UTIL_VECTOR3_H

namespace rinvid
{

struct Vector3
{
    float x;
    float y;
    float z;

    inline void move(const Vector3 move_vector)
    {
        x += move_vector.x;
        y += move_vector.y;
        z += move_vector.z;
    }

    inline void set(const Vector3 vector)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
    }
};

} // namespace rinvid

#endif // UTIL_VECTOR3_H