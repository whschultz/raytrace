package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public abstract class RaytraceObject
{
    protected laVector center;
    private SurfaceColor surfaceColor;
    protected Color background;

    protected float ambient;
    protected float diffuse;
    protected float specular;
    protected float phong;

    protected RaytraceObject()
    {
    }

    public abstract float intersect(laVector start, laVector direction);
    public laVector intersect(laVector start, laVector direction, float t)
    {
        return start.add(direction.multiply(t));
    }

    public Color intersectColor(laVector start, laVector direction, float t, laVector intersection, LightSource light)
    {
        assert t > 0;

        Color output = light.diffuse(intersection, getNorm(intersection), direction, getDiffuse(), getPhong());

        return output;

    }

    public abstract laVector reflect(laVector start, laVector direction, float t, laVector intersection);
    public abstract laVector getNorm(laVector atPoint);

    public void setCenter(laVector center)
    {
        this.center = center;
    }

    public void setColor(Color color)
    {
        surfaceColor = new ConstantSurfaceColor(color);
    }

    public void setColor(SurfaceColor color)
    {
        this.surfaceColor = color;
    }

    public void setBackground(Color color)
    {
        this.background = color;
    }

    public void setAmbient(float ambient)
    {
        this.ambient = ambient;

        if (ambient + diffuse + specular > 1)
        {
            diffuse = diffuse/(diffuse + specular);
            specular = 1 - (ambient + diffuse);
        }
    }

    public void setDiffuse(float diffuse)
    {
        this.diffuse = diffuse;

        if (ambient + diffuse + specular > 1)
        {
            ambient = ambient/(ambient + diffuse);
            this.diffuse = 1 - (ambient + specular);
        }
    }

    public void setSpecular(float specular)
    {
        this.specular = specular;

        if (ambient + diffuse + specular > 1)
        {
            ambient = ambient/(ambient + diffuse);
            diffuse = 1 - (ambient + specular);
        }
    }

    public void setPhong(float phong)
    {
        this.phong = phong;
    }

    public Color getColor(laVector atPoint)
    {
        return surfaceColor.getColor(atPoint);
    }

    public float getPhong()
    {
        return phong;
    }

    public float getSpecular()
    {
        return specular;
    }

    public float getDiffuse()
    {
        return diffuse;
    }

    public float getAmbient()
    {
        return ambient;
    }
}
