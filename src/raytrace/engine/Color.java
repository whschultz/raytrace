package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class Color
{
    private final double red;

    private final double green;

    private final double blue;

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

    public double getRed()
    {
        return Math.max(Math.min(red,1), 0);
    }

    public double getGreen()
    {
        return Math.max(Math.min(green, 1), 0);
    }

    public double getBlue()
    {
        return Math.max(Math.min(blue, 1), 0);
    }
}
