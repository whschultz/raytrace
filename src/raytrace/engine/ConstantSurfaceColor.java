package raytrace.engine;

/**
 * Created by whschultz on 6/23/15.
 */
public class ConstantSurfaceColor extends SurfaceColor
{
    private final Color color;

    public ConstantSurfaceColor(Color color)
    {
        this.color = color;
    }

    @Override
    public Color getColor(laVector intersection)
    {
        return color;
    }
}
