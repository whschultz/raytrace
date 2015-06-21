package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class Color
{
    public final double red;

    public final double green;

    public final double blue;

    public Color(double r, double g, double b)
    {
        red = r;
        green = g;
        blue = b;
    }

    public Color multiply(double product)
    {
        return new Color(red * product, green * product, blue * product);
    }

    public Color add(Color val)
    {
        return new Color(red + val.red, green + val.green, blue + val.blue);
    }
}
