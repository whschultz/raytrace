package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public abstract class RaytraceObject
{
    protected laVector center;
    protected Color color;
    protected Color background;

    protected double ambient;
    protected double diffuse;
    protected double specular;
    protected double phong;

    protected RaytraceObject()
    {
    }

    public abstract double intersect(laVector start, laVector direction);
    public laVector intersect(laVector start, laVector direction, double t)
    {
        return start.add(direction.multiply(t));
    }

    public Color intersectColor(laVector start, laVector direction, double t, laVector intersection, LightSource light)
    {
        assert t > 0;

        Color output = light.diffuse(intersection, getNorm(intersection), direction, getDiffuse(), getPhong());

        return output;

    }

    public abstract laVector reflect(laVector start, laVector direction, double t, laVector intersection);
    public abstract laVector getNorm(laVector atPoint);

    public void setCenter(laVector center)
    {
        this.center = center;
    }

    public void setColor(Color color)
    {
        this.color = color;
    }

    public void setBackground(Color color)
    {
        this.background = color;
    }

    public void setAmbient(double ambient)
    {
        this.ambient = ambient;

        if (ambient + diffuse + specular > 1)
        {
            diffuse = diffuse/(diffuse + specular);
            specular = 1 - (ambient + diffuse);
        }
    }

    public void setDiffuse(double diffuse)
    {
        this.diffuse = diffuse;

        if (ambient + diffuse + specular > 1)
        {
            ambient = ambient/(ambient + diffuse);
            this.diffuse = 1 - (ambient + specular);
        }
    }

    public void setSpecular(double specular)
    {
        this.specular = specular;

        if (ambient + diffuse + specular > 1)
        {
            ambient = ambient/(ambient + diffuse);
            diffuse = 1 - (ambient + specular);
        }
    }

    public void setPhong(double phong)
    {
        this.phong = phong;
    }

    public Color getColor(laVector atPoint)
    {
        return color;
    }

    public double getPhong()
    {
        return phong;
    }

    public double getSpecular()
    {
        return specular;
    }

    public double getDiffuse()
    {
        return diffuse;
    }

    public double getAmbient()
    {
        return ambient;
    }
}
