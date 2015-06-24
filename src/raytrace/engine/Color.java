package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class Color
{
    private final float red;

    private final float green;

    private final float blue;

    public Color(float r, float g, float b)
    {
        red = r;
        green = g;
        blue = b;
    }

    public Color multiply(float product)
    {
        return new Color(red * product, green * product, blue * product);
    }

    public Color multiply(Color other)
    {
        return new Color(red * other.red, green * other.green, blue * other.blue);
    }

    public Color add(Color val)
    {
        return new Color(red + val.red, green + val.green, blue + val.blue);
    }

    public float getRed()
    {
        return Math.max(Math.min(red,1), 0);
    }

    public float getGreen()
    {
        return Math.max(Math.min(green, 1), 0);
    }

    public float getBlue()
    {
        return Math.max(Math.min(blue, 1), 0);
    }

    public Color norm()
    {
        return new Color(getRed(), getGreen(), getBlue());
    }
}
