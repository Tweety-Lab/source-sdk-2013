using System;
using System.Runtime.InteropServices;

namespace SourceEngine
{
    // C# Representation of Source Engine's Vector
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;

        // Constructor
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        // Length or Magnitude of the Vector
        public float Length()
        {
            return (float)Math.Sqrt(x * x + y * y + z * z);
        }

        // Normalize the Vector (make its length = 1)
        public Vector3 Normalize()
        {
            float length = Length();
            if (length > 0)
            {
                return new Vector3(x / length, y / length, z / length);
            }
            return new Vector3(0, 0, 0); // Return zero vector if length is 0
        }

        // Dot Product of Two Vectors
        public static float Dot(Vector3 v1, Vector3 v2)
        {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }

        // Cross Product of Two Vectors
        public static Vector3 Cross(Vector3 v1, Vector3 v2)
        {
            return new Vector3(
                v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x
            );
        }

        // Addition of Two Vectors
        public static Vector3 operator +(Vector3 v1, Vector3 v2)
        {
            return new Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
        }

        // Subtraction of Two Vectors
        public static Vector3 operator -(Vector3 v1, Vector3 v2)
        {
            return new Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
        }

        // Scalar Multiplication
        public static Vector3 operator *(Vector3 v, float scalar)
        {
            return new Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
        }

        // Scalar Division
        public static Vector3 operator /(Vector3 v, float scalar)
        {
            if (scalar != 0)
            {
                return new Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
            }
            return v; // Return the vector unchanged if dividing by 0
        }

        // String Representation of the Vector
        public override string ToString()
        {
            return $"({x}, {y}, {z})";
        }

        // Equality Check
        public override bool Equals(object obj)
        {
            if (obj is Vector3)
            {
                Vector3 v = (Vector3)obj;
                return x == v.x && y == v.y && z == v.z;
            }
            return false;
        }

        // Hash Code for the Vector (needed for collections)
        public override int GetHashCode()
        {
            return Tuple.Create(x, y, z).GetHashCode();
        }

        // Static Zero Vector
        public static Vector3 Zero => new Vector3(0, 0, 0);

        // Static Unit Vector (1, 1, 1)
        public static Vector3 Unit => new Vector3(1, 1, 1);
    }
}
