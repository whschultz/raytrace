package raytrace.engine;

/**
 * Created by whschultz on 6/11/15.
 */
public class laVector
{
    final private double x_coor;
    final private double y_coor;
    final private double z_coor;

    public double get_x()
    {
        return x_coor;
    }

    public double get_y()
    {
        return y_coor;
    }

    public double get_z()
    {
        return z_coor;
    }

    public laVector(double x, double y, double z)
    {
        x_coor = x;
        y_coor = y;
        z_coor = z;
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

    public double norm()
    {
        return Math.sqrt(x_coor * x_coor + y_coor * y_coor + z_coor * z_coor);
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

    public double dot(laVector rhs)
    {
        return x_coor*rhs.x_coor + y_coor*rhs.y_coor + z_coor*rhs.z_coor;
    }

    public static double dot(laVector lhs, laVector rhs)
    {
        return lhs.dot(rhs);
    }

    public laVector multiply(double val)
    {
        return new laVector(val * x_coor, val*y_coor, val*z_coor);
    }

    public  laVector unit()
    {
        final double length = this.norm();

        return new laVector(x_coor / length, y_coor / length, z_coor / length);
    }
}
