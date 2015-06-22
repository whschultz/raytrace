package raytrace.engine;

/**
 * Created by whschultz on 6/20/15.
 */
public class Camera
{
    laVector lookat;
    laVector head;
    laVector right;
    laVector position;

    //Matrix transform;

    int cameraResolution;
    double zoom;

    int width;
    int height;


    public Camera()
    {
        lookat = new laVector(0, 0, -1).unit();
        head = new laVector(0, 1, 0);
        position = new laVector(0, 0, 1.5);
        cameraResolution = 1;
        setAxes();
        zoom = 1;

    }

    public Color antialias(Scene objects, laVector direction, laVector dX, laVector dY)
    {
        laVector antialiasing = new laVector(0,0,0);
        Color sum = new Color(0,0,0);
        double count = 0;

        final double change = 1d/((double)cameraResolution);

        for(int antiX = 0; antiX < cameraResolution; antiX++)
        {
            laVector top = direction.add(dX.multiply(antiX * change));
            for(int antiY = 0; antiY < cameraResolution; antiY++)
            {
                Color color = objects.followRay(position, top.add(dY.multiply(antiY * change)));

                sum = sum.add(color);

                count += 1;
            }
        }

        return sum.multiply(1d/count);
    }

    public void setWidth(int width)
    {
        this.width = width;
    }

    public void setHeight(int height)
    {
        this.height = height;
    }

    public int getWidth()
    {
        return width;
    }

    public int getHeight()
    {
        return height;
    }

    public void setAxes()
    {
        right = lookat.cross(head);
        head = right.cross(lookat);
        right = right.unit();
        head = head.unit();
        lookat = lookat.unit();
    }

    public void setLookat(laVector lookat)
    {
        this.lookat = lookat.unit();
        setAxes();
    }

    public void setTop(laVector top)
    {
        this.head = top.unit();
        setAxes();
    }

    public void setPosition(laVector position)
    {
        this.position = position;
    }

    public laVector getPosition()
    {
        return position;
    }

    public void setZoom(double mag)
    {
        zoom = mag;
    }

    public double getZoom()
    {
        return zoom;
    }

    public laVector getDx()
    {
        laVector topRight = getTopRight();
        laVector topLeft = getTopLeft();

        laVector dX = (topRight.subtract(topLeft)).multiply(1d/((double)this.width));

        return dX;
    }

    public laVector getDy()
    {
        laVector topLeft = getTopLeft();
        laVector bottomLeft = getBottomLeft();

        laVector dY = bottomLeft.subtract(topLeft).multiply(1d/((double)this.height));

        return dY;
    }

    public laVector getBottomLeft()
    {
        laVector bl = lookat.multiply(zoom)
                .subtract(right)
                .subtract(head.multiply(((double) this.height) / ((double) this.width)));

        return bl;
    }

    public laVector getBottomRight()
    {
        laVector br = lookat.multiply(zoom)
                .add(right)
                .subtract(head.multiply(((double) this.height) / ((double)this.width)));

        return br;
    }

    public laVector getTopLeft()
    {
        laVector tl = lookat.multiply(zoom)
                .subtract(right)
                .add(head.multiply(((double) this.height) / ((double) this.width)));

        return tl;
    }

    public laVector getTopRight()
    {
        laVector tr = lookat.multiply(zoom)
                .add(right)
                .add(head.multiply(((double)this.height)/((double)this.width)));

        return tr;
    }

    public void setAntialiasResolution(int newRes)
    {
        cameraResolution = newRes;
    }

    public int getAntialiasResolution()
    {
        return cameraResolution;
    }
}
