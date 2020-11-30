#ifndef UTIL_VECTOR2_H
#define UTIL_VECTOR2_H

namespace rinvid
{

struct Vector2
{
    float x;
    float y;

    inline void move(const Vector2 move_vector)
    {
        x += move_vector.x;
        y += move_vector.y;
    }

    inline void set(const Vector2 vector)
    {
        x = vector.x;
        y = vector.y;
    }
};

} // namespace rinvid

#endif // UTIL_VECTOR2_H
