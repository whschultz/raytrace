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

    Scene objects;

    //Matrix transform;

    int cameraResolution;
    double zoom;

    int width;
    int height;


    public Camera()
    {
        lookat = new laVector(0, -1, -1).unit();
        head = new laVector(0, 1, 0);
        position = new laVector(0, 10, 10);
        cameraResolution = 1;
        setAxes();
        zoom = 1;

    }

    public Color antialias(laVector direction,laVector xDir,laVector yDir,int res)
    {
        laVector antialiasing = new laVector(0,0,0);
        laVector antialiasingReminder = new laVector(0,0,0);
        double r = 0;
        double g = 0;
        double b = 0;
        double count = 0;

        for(int antiX = 0; antiX < res; antiX++)
        {
            for(int antiY = 0; antiY < res; antiY++)
            {
                Color color = objects.followRay(position, direction.add(antialiasing));

                r += color.red;
                g += color.green;
                b += color.blue;

                count += 1;
            }
        }

        return new Color(r/count, g/count, b/count);
    }

    public void setObjects(Scene scene)
    {
        objects = scene;
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

        laVector dY = topLeft.subtract(bottomLeft).multiply(1d/((double)this.height));

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
                .subtract(head.multiply(((double)this.height)/((double)this.width)));

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
