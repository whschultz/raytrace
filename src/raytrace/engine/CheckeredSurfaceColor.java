package raytrace.engine;

/**
 * Created by whschultz on 6/23/15.
 */
public class CheckeredSurfaceColor extends SurfaceColor
{
    private final Color oddColor;
    private final Color evenColor;

    public CheckeredSurfaceColor(Color odd, Color even)
    {
        oddColor = odd;
        evenColor = even;
    }

    @Override
    public Color getColor(laVector intersection)
    {
        if (Math.round(intersection.get_x() + intersection.get_y() + intersection.get_z()) % 2 == 0)
        {
            return evenColor;
        }

        return oddColor;
    }
}
