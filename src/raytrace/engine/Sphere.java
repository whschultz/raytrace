package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class Sphere extends RaytraceObject
{
    float radius = 1;

    public void setRadius(float radius)
    {
        this.radius = radius;
    }

    @Override
    public float intersect(laVector start, laVector direction) {
        laVector v = start.subtract(center);

        float output;

        final float b = 2.0f * v.dot(direction);
        final float a = direction.dot(direction);
        final float c = v.dot(v) - radius * radius;

        final float radical = b*b-4*a*c;

        if (radical <= 0)
        {
            output = -1;
        }
        else
        {
            final float root = (float)Math.sqrt(radical);

            final float denominator = 2.0f * a;
            final float t1 = (-b + root) / denominator;
            final float t2 = (-b - root) / denominator;

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

//    @Override
//    public Color intersectColor(laVector start, laVector direction, float t, laVector intersection, LightSource light) {
//        assert t > 0;
//
//        Color output = light.diffuse(intersection, getNorm(intersection), direction, getDiffuse(), getPhong());
//
//        return output;
//    }

    @Override
    public laVector reflect(laVector start, laVector direction, float t, laVector intersection) {
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
