package raytrace.engine;

/**
 * Created by whschultz on 6/23/15.
 */
public class CheckeredSurfaceColor extends SurfaceColor
{
    private final Color oddColor;
    private final Color evenColor;

    /**
     * When working with planes that fall precisely on integer values, the offset moves
     * the calculation off a little bit so that errors in the calculation don't produce
     * noise in the image.
     */
    private final laVector offsetHelper;

    public CheckeredSurfaceColor(Color odd, Color even)
    {
        oddColor = odd;
        evenColor = even;
        offsetHelper = new laVector(0,0,0);
    }

    public CheckeredSurfaceColor(Color odd, Color even, laVector offset)
    {
        oddColor = odd;
        evenColor = even;
        offsetHelper = offset;
    }

    @Override
    public Color getColor(laVector intersection)
    {
        if ((Math.floor(intersection.get_x() + offsetHelper.get_x()) +
                Math.floor(intersection.get_y() + offsetHelper.get_y()) +
                Math.floor(intersection.get_z() + offsetHelper.get_z())) % 2 == 0)
        {
            return evenColor;
        }

        return oddColor;
    }
}
