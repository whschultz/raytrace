package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class Sphere extends RaytraceObject
{
    double radius = 1;

    public void setRadius(double radius)
    {
        this.radius = radius;
    }

    @Override
    public double intersect(laVector start, laVector direction) {
        laVector v = start.subtract(center);

        double output;

        final double b = 2.0d * v.dot(direction);
        final double a = direction.dot(direction);
        final double c = v.dot(v) - radius * radius;

        final double radical = b*b-4*a*c;

        if (radical <= 0)
        {
            output = -1;
        }
        else
        {
            final double root = Math.sqrt(radical);

            final double denominator = 2.0d * a;
            final double t1 = (-b + root) / denominator;
            final double t2 = (-b - root) / denominator;

            if (t1 > 0d)
            {
                if (t2 > 0d)
                {
                    output = t1 < t2 ? t1 : t2;
                }
                else
                {
                    output = t1;
                }
            }
            else
            {
                if (t2 > 0d)
                {
                    output = t2;
                }
                else
                {
                    output = -1;
                }
            }
        }

        return output;
    }

    @Override
    public Color intersectColor(laVector start, laVector direction, double t, laVector intersection, LightSource light) {
        Color output = light.diffuse(intersection, getNorm(intersection), direction, diffuse, phong);

        return output;
    }

    @Override
    public laVector reflect(laVector start, laVector direction, double t, laVector intersection) {
        final laVector pointNorm = this.getNorm(intersection);
        final laVector output = direction.reflect(pointNorm);
        return output;
    }

    @Override
    public laVector getNorm(laVector atPoint) {
        final laVector norm = atPoint.subtract(center);
        final laVector output = norm.unit();

        return output;
    }
}
