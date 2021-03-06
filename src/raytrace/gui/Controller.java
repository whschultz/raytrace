package raytrace.gui;

import javafx.application.Platform;
import javafx.beans.value.ObservableValue;
import javafx.embed.swing.SwingFXUtils;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.SnapshotParameters;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TextField;
import javafx.scene.image.WritableImage;
import javafx.scene.layout.BorderPane;
import javafx.scene.transform.Transform;
import javafx.stage.FileChooser;
import raytrace.engine.*;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.atomic.AtomicInteger;

public class Controller {
    @FXML private TextField txtWidth;
    @FXML private TextField txtHeight;
    @FXML private TextField txtAntialiasing;

    @FXML private Button btnOpen;
    @FXML private Button btnRender;
    @FXML private Button btnSave;

    @FXML private Canvas cnvsRender;

    @FXML private BorderPane topPane;

    @FXML private ProgressIndicator prgRenderProgress;

    final Camera camera = new Camera();
    final Scene scene = new Scene();

    public void init()
    {
        txtWidth.textProperty().addListener((observable, oldValue, newValue) -> {
            OnWidthChanged(observable, oldValue, newValue);
        });

        txtHeight.textProperty().addListener((observable, oldValue, newValue) -> {
            OnHeightChanged(observable, oldValue, newValue);
        });

        txtAntialiasing.textProperty().addListener((observable, oldValue, newValue) -> {
            OnAntialiasingChanged(observable, oldValue, newValue);
        });

        cnvsRender.setWidth(1280);
        cnvsRender.setHeight(800);

        txtWidth.setText("2560");
        txtHeight.setText("1440");
        txtAntialiasing.setText("4");

        camera.setWidth((int) cnvsRender.getWidth());
        camera.setHeight((int) cnvsRender.getHeight());
        camera.setAntialiasResolution(4);

        setDefaultScene();
    }

    private void setDefaultScene()
    {
        Color white = new Color(1,1,1);
        Color grey = new Color(.5,.5,.5);

        scene.deleteAllObjects();

        camera.setPosition(new laVector(0,14,35));
        camera.setLookat(new laVector(0,-4,-10));
        camera.setTop(new laVector(0,1,0));
        camera.setZoom(4d);

        scene.setBackground(15d/255d, 10d/255d, 25.5d/255d);
        scene.setAttenuation(1d/10d);

        Plane plane = new Plane();
        plane.setCenter(new laVector(0,-3,0));
        plane.setNormal(new laVector(0,1,0));

        plane.setAmbient(0);
        plane.setDiffuse(.6);
        plane.setSpecular(.4);
        plane.setPhong(30);
        plane.setColor(new CheckeredSurfaceColor(white, grey, new laVector(0,.5,0)));

        scene.addObject(plane);


        double x = 0.0f, y = 0.0f;

        final double dx = 6.0f;
        final double dy = Math.sqrt(3d) * 3.0f;
        final double dz = dy;

        float x0 = 0.0f;

        final double dim = 5;


        Sphere sparesphere = new Sphere();
        sparesphere.setCenter( new laVector(-3, 0, 0 ));
        sparesphere.setRadius(3);
        sparesphere.setAmbient(.0);
        sparesphere.setDiffuse( .6 );
        sparesphere.setSpecular( .4 );
        sparesphere.setPhong(30);
        sparesphere.setColor(white);
        scene.addObject(sparesphere);

        sparesphere = new Sphere();
        sparesphere.setCenter( new laVector(3, 0, 0 ));
        sparesphere.setRadius(3);
        sparesphere.setAmbient(.0);
        sparesphere.setDiffuse( .6 );
        sparesphere.setSpecular( .4 );
        sparesphere.setPhong(30);
        sparesphere.setColor(white);
        scene.addObject(sparesphere);

        {
            LightSource sparelight = new LightSource();
            sparelight.setColor(new Color(255, 0, 0));
            sparelight.setPosition(new laVector( -32, 16, 32 ));
            sparelight.setRadius( .3 );
            scene.addLight(sparelight);

            sparelight = new LightSource();
            sparelight.setColor(new Color(0, 0, 255));
            sparelight.setPosition(new laVector( 32, 16, 32 ));
            sparelight.setRadius( .3 );
            scene.addLight(sparelight);

            sparelight = new LightSource();
            sparelight.setColor(new Color(0, 255, 0));
            sparelight.setPosition(new laVector( -32, 16, -32 ));
            sparelight.setRadius( .3 );
            scene.addLight(sparelight);

            sparelight = new LightSource();
            sparelight.setColor(new Color(128, 128, 0));
            sparelight.setPosition(new laVector( 32, 16,-32 ));
            sparelight.setRadius( .3 );
            scene.addLight(sparelight);
        }
    }

    private void OnAntialiasingChanged(ObservableValue<? extends String> observable, String oldValue, String newValue)
    {
        camera.setAntialiasResolution(Integer.parseInt(newValue));
    }

    private void OnHeightChanged(ObservableValue<? extends String> observable, String oldValue, String newValue)
    {
        int newHeight = Integer.parseInt(newValue);

        camera.setHeight(newHeight);

        cnvsRender.setHeight(newHeight);
    }


    private void OnWidthChanged(ObservableValue<? extends String>  observable, String oldValue, String newValue)
    {
        int newWidth = Integer.parseInt(newValue);

        camera.setWidth(newWidth);

        cnvsRender.setWidth(newWidth);
    }

    @FXML private void OnWidthEnter(ActionEvent event)
    {

    }

    @FXML private void OnOpen(ActionEvent event)
    {
        FileChooser chooser = new FileChooser();

        chooser.setSelectedExtensionFilter(new FileChooser.ExtensionFilter("Raytrace Data File", ".dat"));
        File selected = chooser.showOpenDialog(topPane.getScene().getWindow());

        if (selected != null && selected.exists() && selected.isFile())
        {
            scene.deleteAllObjects();
            scene.openFile(selected);
            scene.setAttenuation(5);

            camera.setLookat(new laVector(0, 0, -1));
            camera.setTop(new laVector(0, 1, 0));
            camera.setPosition(new laVector(0, 0, 3.5));
            camera.setZoom(2);
        }
    }

    @FXML private void OnSave(ActionEvent event)
    {
        FileChooser chooser = new FileChooser();

        chooser.setSelectedExtensionFilter(new FileChooser.ExtensionFilter("PNG Image File", "png"));
        File selected = chooser.showSaveDialog(topPane.getScene().getWindow());

        if (selected != null) {
            int dpp = getDpp();

            WritableImage image = new WritableImage((int)(cnvsRender.getWidth() * dpp),(int)( cnvsRender.getHeight() * dpp));

            SnapshotParameters params = new SnapshotParameters();
            params.setTransform(Transform.scale(dpp, dpp));
            cnvsRender.snapshot(params, image);

            BufferedImage bImage = SwingFXUtils.fromFXImage(image, null);
            try {
                ImageIO.write(bImage, "png", selected);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Get Dots per Pixel
     *
     * Note that if antialiasing is turned on at all, this will be set to two.
     *
     * @return
     */
    private int getDpp()
    {
        return camera.getAntialiasResolution() > 1 ? 2 : 1;
    }

    @FXML private void OnRender(ActionEvent event)
    {
        Executors.newCachedThreadPool();

        ExecutorService threadPool = Executors.newWorkStealingPool(Runtime.getRuntime().availableProcessors());

        laVector topLeft = camera.getTopLeft();
        laVector position = camera.getPosition();


        GraphicsContext context = cnvsRender.getGraphicsContext2D();
        AtomicInteger count = new AtomicInteger(0);
        final int originalAntialiasResolution = camera.getAntialiasResolution();
        final boolean enableAntialiasing = originalAntialiasResolution > 1;

        final double dpp = getDpp();
        final laVector dX;
        final laVector dY;


        try {
            camera.setAntialiasResolution(originalAntialiasResolution / (int)dpp);
            dX = camera.getDx().multiply(1d / dpp);
            dY = camera.getDy().multiply(1d / dpp);

            final double total = camera.getWidth() * camera.getHeight() * dpp * dpp;

            for (double x = 0; x < camera.getWidth() * dpp; x++) {
                laVector currentTop = topLeft.add(dX.multiply(x));

                final double finalX = x;
                threadPool.execute(() -> {

                    final List<Color> column = new ArrayList<Color>((int)(camera.getHeight() * dpp));

                    for (double y = 0; y < camera.getHeight() * dpp; y++) {
                        laVector currentXY = currentTop.add(dY.multiply(y));
                        laVector currentDir = currentXY.unit();

                        final Color color;

                        if (enableAntialiasing) {
                            color = camera.antialias(scene, currentDir, dX, dY);
                        } else {
                            color = scene.followRay(position, currentDir);
                        }

                        column.add(color);

                        count.incrementAndGet();
                    }

                    Platform.runLater(() -> {
                        double y = 0;
                        context.setLineWidth(1 / dpp);
                        for(Color color : column)
                        {
                            context.setStroke(javafx.scene.paint.Color.color(color.getRed(), color.getGreen(), color.getBlue()));
                            context.strokeLine(finalX / dpp + .5d / dpp, y / dpp - .5d / dpp, finalX / dpp + .5d / dpp, y / dpp + .5d / dpp);

                            y++;
                        }

                        prgRenderProgress.setProgress((double) count.get() / total);

                        if (count.get() >= total)
                        {
                            btnSave.setDisable(false);
                        }
                    });
                });
            }

        }
        finally
        {
            camera.setAntialiasResolution(originalAntialiasResolution);
        }
    }
}
