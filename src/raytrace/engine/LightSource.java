package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class LightSource
{
    laVector position;
    Color color;
    float attenuation = 1;
    float radius = .3f;


    public LightSource()
    {

    }

    public Color specular(laVector dir, laVector norm, laVector intersect, float spec, float phong)
    {
        laVector reflection_vector = dir.reflect(norm);
        laVector dirToLight = position.subtract(intersect);
        float distance = dirToLight.norm();
        dirToLight = dirToLight.multiply(1f / distance);

        Color output = color.multiply(spec * (float)Math.pow(reflection_vector.dot(dirToLight), phong)/distance);

        return output;
    }

    public Color diffuse(laVector point, laVector norm, laVector direction, float diffuse, float phong)
    {
        assert Math.abs(norm.norm() - 1) < .000001f;

        float adder1, adder2, dotp;

        final laVector reflectedDir = direction.reflect(norm).unit();

        laVector dirToLight = position.subtract(point);
        float distance = dirToLight.norm();
        dirToLight = dirToLight.multiply(1f / distance);

        adder1 = diffuse * norm.dot(dirToLight);
        dotp = reflectedDir.dot(dirToLight);
        if (dotp >= 0)
        {
            adder2 = (float)Math.pow(dotp, phong);
        }
        else
        {
            adder2 = 0;
        }

        final float product = attenuation * (adder1 + adder2)/distance;

        return color.multiply(product);
    }

    public void setColor(Color c)
    {
        color = c;
    }

    public void setColor(int r, int g, int b)
    {
        color = new Color(((float)r)/255f, ((float)g)/255f, ((float)b)/255f);
    }

    public void setPosition(laVector position)
    {
        this.position = position;
    }

    public void setAttenuation(float newAtt)
    {
        attenuation = newAtt;
    }

    public void setRadius(float newRadius)
    {
        radius = newRadius;
    }

    public float getRadius()
    {
        return radius;
    }

    public laVector getPos()
    {
        return position;
    }
}
