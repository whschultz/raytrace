package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class Plane extends RaytraceObject
{
    laVector normal;

    public Plane()
    {
        normal = new laVector(0, 0, 1);
        center = new laVector(0,0,1);
    }

    public void setNormal(laVector normal)
    {
        this.normal = normal;
    }

    public double intersect(laVector start, laVector dir)
    {
        final double dot = dir.dot(normal);

        if (dot == 0)
            return -1;

        final double intersect = (center.dot(normal) - start.dot(normal)) / dir.dot(normal);

        return intersect;
    }

//    @Override
//    public Color intersectColor(laVector start, laVector direction, double t, laVector intersection, LightSource light) {
//        if ( t <= 0)
//        {
//            return background;
//        }
//
//        Color output = light.diffuse(intersection, normal, direction, diffuse, phong);
//
//        return output;
//    }

    @Override
    public laVector reflect(laVector start, laVector direction, double t, laVector intersection) {
        return direction.reflect(normal);
    }


    @Override
    public laVector getNorm(laVector atPoint)
    {
        return normal;
    }

    @Override
    public boolean intersects(Cone lightCone)
    {
        double angle = Math.abs(Math.acos(normal.dot(lightCone.dir)));

        return Math.min(angle, 180d - angle) < lightCone.theta ;
    }
}
