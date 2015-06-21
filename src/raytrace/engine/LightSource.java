package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class LightSource
{
    laVector position;
    Color color;
    double attenuation;
    double radius;


    public LightSource()
    {

    }

    public Color specular(laVector dir, laVector norm, laVector intersect, double spec, double phong)
    {
        laVector reflection_vector = dir.reflect(norm);
        laVector dirToLight = position.subtract(intersect);
        double distance = dirToLight.norm();
        dirToLight = dirToLight.multiply(1d / distance);

        Color output = color.multiply(spec * Math.pow(reflection_vector.dot(dirToLight), phong)/distance);

        return output;
    }

    public Color diffuse(laVector point, laVector norm, laVector direction, double diffuse, double phong)
    {
        assert Math.abs(norm.norm() - 1) < .000001d;

        double adder1, adder2, dotp;

        final laVector reflectedDir = direction.reflect(norm).unit();

        laVector dirToLight = position.subtract(point);
        double distance = dirToLight.norm();
        dirToLight = dirToLight.multiply(1d / distance);

        adder1 = diffuse * norm.dot(dirToLight);
        dotp = reflectedDir.dot(dirToLight);
        if (dotp >= 0)
        {
            adder2 = Math.pow(dotp, phong);
        }
        else
        {
            adder2 = 0;
        }

        final double product = attenuation * (adder1 + adder2)/distance;

        return color.multiply(product);
    }

    public void setColor(Color c)
    {
        color = c;
    }

    public void setColor(int r, int g, int b)
    {
        color = new Color(((double)r)/255d, ((double)g)/255d, ((double)b)/255d);
    }

    public void setPosition(laVector position)
    {
        this.position = position;
    }

    public void setAttenuation(double newAtt)
    {
        attenuation = newAtt;
    }

    public void setRadius(double newRadius)
    {
        radius = newRadius;
    }

    public double getRadius()
    {
        return radius;
    }

    public laVector getPos()
    {
        return position;
    }
}
