package raytrace.gui;

import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.stage.FileChooser;
import raytrace.engine.Camera;
import raytrace.engine.Color;
import raytrace.engine.Scene;
import raytrace.engine.laVector;

import java.io.File;

public class Controller {
    @FXML private TextField txtWidth;
    @FXML private TextField txtHeight;
    @FXML private TextField txtAntialiasing;

    @FXML private Button btnOpen;
    @FXML private Button btnRender;

    @FXML private Canvas cnvsRender;

    @FXML private BorderPane topPane;

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

        txtWidth.setText("1280");
        txtHeight.setText("800");
        txtAntialiasing.setText("1");

        camera.setWidth((int) cnvsRender.getWidth());
        camera.setHeight((int) cnvsRender.getHeight());
        camera.setAntialiasResolution(1);
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
            scene.openFile(selected);
        }
    }

    @FXML private void OnRender(ActionEvent event)
    {
        laVector topLeft = camera.getTopLeft();
        laVector topRight = camera.getTopRight();
        laVector bottomLeft = camera.getBottomLeft();
        laVector bottomRight = camera.getBottomRight();
        laVector position = camera.getPosition();

        laVector dX = camera.getDx();
        laVector dY = camera.getDy();

        GraphicsContext context = cnvsRender.getGraphicsContext2D();

        final boolean antialias = camera.getAntialiasResolution() > 1;

        for(int x = 0; x < camera.getWidth(); x++)
        {
            laVector currentTop = topLeft.add(dX.multiply(x));

            for(int y = 0; y < camera.getHeight(); y++)
            {
                laVector currentXY = currentTop.add(dY.multiply(y));
                laVector currentDir = currentXY.subtract(position).unit();

                Color color = scene.followRay(position, currentDir);

                context.setFill(javafx.scene.paint.Color.color(color.red, color.green, color.blue));
                context.fillRect(x, y, 1, 1);
            }
        }
    }
}
