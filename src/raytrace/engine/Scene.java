package raytrace.engine;

import java.io.*;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.*;

/**
 * Created by whschultz on 6/20/15.
 */
public class Scene
{
    public int maxSpecDepth = 10;
    public final boolean softShadows = true;
    public int softShadowLevel = 4;
    public int softShadowSteps = 4;

    Color background;
    Collection<RaytraceObject> objects = new ArrayList<RaytraceObject>();
    Collection<LightSource> lights = new ArrayList<LightSource>();

    double attenuation = 1;


    public boolean addObject(RaytraceObject object)
    {
        boolean output = !objects.contains(object);

        if (output)
        {
            objects.add(object);
        }

        return output;
    }

    public boolean addLight(LightSource light)
    {
        boolean output = !lights.contains(light);

        if (output)
        {
            light.setAttenuation(attenuation);
            lights.add(light);
        }

        return output;
    }

    public void setBackground(double r, double g, double b)
    {
        background = new Color(r,g,b);
    }

    public void deleteAllObjects()
    {
        lights.clear();
        objects.clear();
    }

    public void setAttenuation(double atten)
    {
        this.attenuation = atten;

        for(LightSource light : this.lights)
        {
            light.setAttenuation(atten);
        }
    }

    public Color followRay(laVector start, laVector direction)
    {
        return followRay(start, direction, 0, null);
    }

    private Color followRay(laVector start, laVector direction, int currentDepth, RaytraceObject ignore)
    {
        Color outputColor = background;

        IntersectResults results = intersect(start, direction, ignore);


        if (results.t > 0)
        {
            RaytraceObject intersected = results.intersected;

            assert intersected != null;

            outputColor = followRayDiffuse(start, direction, results.t, results.intersection, intersected);

            if (currentDepth < maxSpecDepth && intersected.getSpecular() > 0)
            {
                laVector intersectNorm = intersected.getNorm(results.intersection);
                laVector newDirection = direction.reflect(intersectNorm);

                Color reflectedColor = followRay(results.intersection, newDirection,currentDepth + 1,  intersected);

                outputColor = reflectedColor.multiply(intersected.getSpecular()).add(outputColor);
            }

            outputColor = outputColor.multiply(intersected.getColor(results.intersection));
        }

        return outputColor;
    }

    public Color followRayDiffuse(laVector start,laVector direction, double t, laVector intersection, RaytraceObject object)
    {
        Color c = new Color(0,0,0);

        if (object != null) {
            for (LightSource light : this.lights) {
                c = c.add(intersect(start, direction, t, intersection, light, object));
            }
        }
        else
        {
            c = background;
        }

        return c;
    }

//    public Color followRaySpecular(laVector, laVector, double, int, int ignore)
//    {
//
//    }

    public Color intersect(laVector start, laVector direction, double t, laVector intersection, LightSource light, RaytraceObject intersected)
    {
        Color intersectColor = intersected.intersectColor(start, direction, t, intersection, light);

        final laVector point = light.getPos();

        double success = 0;
        double total = 0;

        if (softShadows)
        {
            boolean done = false;
            double previous_success = -1d;

            for(double layer = this.softShadowLevel; layer > 0; layer--)
            {
                if (total > .5d && success < .5d)
                {
                    // Then we've been around the outer shell and nothing created
                    // a shadow.  The inner shell definitely won't either.
                    break;
                }

                double currentRadius = (layer/((double)softShadowLevel))*light.getRadius();

                double dTheta = 2.000001d*Math.PI/((double)(softShadowSteps*layer + 1));
                double dPhi = Math.PI/(softShadowSteps*layer + 2);

                for(double theta = 0; theta < 2*Math.PI; theta += dTheta)
                {
                    for(double phi = dPhi; phi < Math.PI; phi += dPhi)
                    {
                        if (!done)
                        {
                            laVector dir = laVector.fromSpherical(currentRadius, theta, phi);

                            laVector lightPos = point.add(dir);
                            laVector newDirection = lightPos.subtract(intersection);
                            double distance = newDirection.norm();
                            newDirection = newDirection.unit();

                            // dot product is related to the cosine of the angle between the two.
                            // If the dot product is positive, the angle is acute.  Otherwise, it's
                            // right or obtuse.  In which case, the light is irrelevant.
                            if ( intersected.getNorm(intersection).dot(newDirection) > 0 )
                            {
                                IntersectResults results = this.intersect(intersection, newDirection, intersected);

                                final double shadowPoint = results.t;
                                if ((shadowPoint < 0) || (shadowPoint > distance))
                                {
                                    success++;
                                }
                            }
                        }

                        total++;
                    }
                }

                if (success <= previous_success +.5d)
                {
                    done = true;
                }

                previous_success = success;
            }
        }
        else
        {
            laVector newDirection = point.subtract(intersection);
            double distance = newDirection.norm();
            newDirection = newDirection.unit();

            if (intersected.getNorm(intersection).dot(newDirection) > 0)
            {
                IntersectResults results = this.intersect(intersection, newDirection, intersected);

                if (results.t < 0 || results.t > t)
                {
                    success++;
                }
            }

            total++;
        }

        return intersectColor.multiply(success/total);
    }

    public IntersectResults intersect(laVector start, laVector direction, RaytraceObject ignore)
    {
        IntersectResults results = new IntersectResults();


        for(RaytraceObject object : objects)
        {
            if (object == ignore)
                continue;

            double t = object.intersect(start, direction);

            if (t <= 0)
                continue;

            if (results.t < 0 || t < results.t)
            {
                results.t = t;
                results.intersection = start.add(direction.multiply(t));
                results.intersected = object;
            }
        }

        return results;
    }

    public void removeOneLight()
    {

    }

    public void removeOneObject()
    {

    }

    public void openFile(File file)
    {
        if (file.exists())
        {
            try (Scanner scanner = new Scanner(file, StandardCharsets.UTF_8.name())) {

                double r = Double.parseDouble(scanner.next());
                double g = Double.parseDouble(scanner.next());
                double b = Double.parseDouble(scanner.next());

                this.setBackground(r, g, b);

                int numLights = Integer.parseInt(scanner.next());

                for(int i = 0; i < numLights; i++)
                {
                    LightSource light = new LightSource();

                    double x = Double.parseDouble(scanner.next());
                    double y = Double.parseDouble(scanner.next());
                    double z = Double.parseDouble(scanner.next());

                    light.setPosition(new laVector(x, y, z));

                    light.setColor(255, 255, 255);

                    this.addLight(light);
                }

                int numSurfaces = Integer.parseInt(scanner.next());
                ArrayList<Surface> surfaces = new ArrayList<Surface>(numSurfaces);

                for(int i = 0; i < numSurfaces; i++)
                {
                    double[] vals = new double[9];
                    int phong;

                    for (int j =0; j < vals.length; j++)
                    {
                        vals[j] = Double.parseDouble(scanner.next());
                    }

                    phong = Integer.parseInt(scanner.next());

                    Surface s = new Surface();

                    s.r = vals[3];
                    s.g = vals[4];
                    s.b = vals[5];

                    s.ambient = vals[0];
                    s.diffuse = (r + g + b) / 3d;
                    s.specular = vals[6];
                    s.phong = phong;

                    surfaces.add(s);
                }

                int numObjects = Integer.parseInt(scanner.next());

                for(int i = 0; i < numObjects; i++)
                {
                    String line = scanner.nextLine();

                    while ("".equals(line))
                    {
                        line = scanner.nextLine();
                    }

                    Scanner lineScanner = new Scanner(line);

                    String type = lineScanner.next();

                    RaytraceObject object;

                    switch(type)
                    {
                        case "S":
                        default:
                            {
                                double x = Double.parseDouble(lineScanner.next());
                                double y = Double.parseDouble(lineScanner.next());
                                double z = Double.parseDouble(lineScanner.next());
                                double radius = Double.parseDouble(lineScanner.next());

                                Sphere s = new Sphere();

                                s.setCenter(new laVector(x, y, z));
                                s.setRadius(radius);

                                object = s;
                            }
                            break;
                    }

                    int surfaceId = Integer.parseInt(lineScanner.next());

                    Surface s = surfaces.get(surfaceId - 1);

                    object.setColor(new Color(s.r,s.g,s.b));
                    object.setAmbient(s.ambient);
                    object.setDiffuse(s.diffuse);
                    object.setSpecular(s.specular);
                    object.setPhong(s.phong);

                    objects.add(object);
                }

            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            catch(Exception e)
            {
                e.printStackTrace();
            }
        }
    }

    private static class Surface
    {
        public double r;
        public double g;
        public double b;

        public double ambient;
        public double diffuse;
        public double specular;
        public double phong;
    }

    private class IntersectResults
    {
        RaytraceObject intersected = null;
        double t = -1;
        laVector intersection = null;
    }
}
