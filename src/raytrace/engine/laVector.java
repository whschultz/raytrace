package raytrace.engine;

/**
 * Created by whschultz on 6/11/15.
 */
public class laVector
{
    final private float x_coor;
    final private float y_coor;
    final private float z_coor;

    public float get_x()
    {
        return x_coor;
    }

    public float get_y()
    {
        return y_coor;
    }

    public float get_z()
    {
        return z_coor;
    }

    public laVector(float x, float y, float z)
    {
        x_coor = x;
        y_coor = y;
        z_coor = z;
    }

    public static laVector fromSpherical(float r, float theta, float phi)
    {
        final float sinPhi = (float)Math.sin(phi);

        float x = r*sinPhi*(float)Math.cos(theta);
        float y = r*sinPhi*(float)Math.sin(theta);
        float z = r*(float)Math.cos(phi);

        return new laVector(x,y,z);
    }

    @Override
    public boolean equals(Object obj)
    {
        if (obj == null || !obj.getClass().equals(laVector.class))
        {
            return false;
        }

        laVector rhs = (laVector)obj;

        return rhs.x_coor == x_coor && rhs.y_coor == y_coor && rhs.z_coor == z_coor;
    }

    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder("< ");
        sb.append(x_coor);
        sb.append(", ");
        sb.append(y_coor);
        sb.append(", ");
        sb.append(z_coor);
        sb.append(" >");

        return sb.toString();
    }

    public laVector add(laVector rhs)
    {
        return new laVector(rhs.x_coor + x_coor, rhs.y_coor + y_coor, rhs.z_coor + z_coor);
    }

    public laVector subtract(laVector rhs)
    {
        return new laVector(x_coor - rhs.x_coor, y_coor - rhs.y_coor, z_coor - rhs.z_coor);
    }

    public float norm()
    {
        return (float)Math.sqrt(x_coor * x_coor + y_coor * y_coor + z_coor * z_coor);
    }

    public laVector cross(laVector rhs)
    {
        return new laVector(y_coor*rhs.z_coor - z_coor*rhs.y_coor,
                z_coor*rhs.x_coor - x_coor*rhs.z_coor,
                x_coor*rhs.y_coor - y_coor*rhs.x_coor);
    }

    public static laVector cross(laVector lhs, laVector rhs)
    {
        return lhs.cross(rhs);
    }

    public float dot(laVector rhs)
    {
        return x_coor*rhs.x_coor + y_coor*rhs.y_coor + z_coor*rhs.z_coor;
    }

    public static float dot(laVector lhs, laVector rhs)
    {
        return lhs.dot(rhs);
    }

    public laVector multiply(float val)
    {
        return new laVector(val * x_coor, val*y_coor, val*z_coor);
    }

    public  laVector unit()
    {
        final float length = this.norm();

        return new laVector(x_coor / length, y_coor / length, z_coor / length);
    }

    public laVector reflect(laVector norm)
    {
        return this.add(norm.multiply(this.dot(norm)).multiply(-2));
    }
}
