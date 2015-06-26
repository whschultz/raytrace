package raytrace.engine;

/**
 * Created by whschultz on 6/25/15.
 */
public class Cone extends RaytraceObject
{
    /**
     * Direction of cone opening
     */
    final laVector dir;

    /**
     * Angle between dir and surface
     */
    final double theta;

    public Cone(laVector vertex, laVector opening, double theta)
    {
        super(vertex);

        this.dir = opening;
        this.theta = theta;
    }


    @Override
    public double intersect(laVector start, laVector direction)
    {
        assert false : "not implemented";
        return 0;
    }

    @Override
    public laVector getNorm(laVector X)
    {
        final laVector dX = X.subtract(center);
        final laVector output = dX.cross(dX.cross(dir));
        return output;
    }

    @Override
    public boolean intersects(Cone lightCone)
    {
        assert false : "not implemented";
        return false;
    }
}
