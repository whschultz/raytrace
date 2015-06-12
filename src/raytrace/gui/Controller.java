package raytrace.gui;

import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.stage.FileChooser;

import java.io.File;

public class Controller {
    @FXML private TextField txtWidth;
    @FXML private TextField txtHeight;
    @FXML private TextField txtAntialiasing;

    @FXML private Button btnOpen;
    @FXML private Button btnRender;

    @FXML private Canvas cnvsRender;

    @FXML private BorderPane topPane;

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
    }

    private void OnAntialiasingChanged(ObservableValue<? extends String> observable, String oldValue, String newValue)
    {
    }

    private void OnHeightChanged(ObservableValue<? extends String> observable, String oldValue, String newValue)
    {
    }


    private void OnWidthChanged(ObservableValue<? extends String>  observable, String oldValue, String newValue)
    {

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

        }
    }

    @FXML private void OnRender(ActionEvent event)
    {
    }
}
